void assemble(File &src, File &dst) {
  src.rewind();

  while (src.available()) {

    String inst[7];  //[ INST ], [ DATA TYPE ] [ DATA ], [ DATA TYPE ] [ DATA ], [ DATA TYPE ] [ DATA ]
    uint8_t saving = 0;
    char c;
    bool string = false, special = false, start = false;

    while (src.available() && (c = src.read()) != '\n') {
      //Serial.print("i: " + String(i) + "; saving: " + String(saving) + "\n");

      if (saving == 255) continue;  //If end of instruction is reached, skip every char from now on

      switch (c) {
        default:
          special = false;  //Clears special char flag
          start = true;     //Found start of new parameter
          break;

        case '\\':
          special = !special;  //Toggles special char flag (if '\' is found, it will be on, and every other char clears it, but if there is "\\" it needs to also clear the special flag)
          start = true;        //Found start of new parameter
          break;

        case '\"':
          if (!special) string = !string;  //Toggles the string flag if it is not a special char (\")
          special = false;                 //Clears special char flag
          start = true;                    //Found start of new parameter
          break;

        case ',':  //Put here any parameter separator char
        case ' ':
          if (start && !string) {  //If a parameter start has been found and if it is not in a string
            start = false;         //Clear the found parameter flag
            saving += 1;           //Start saving chars to the next parameter
            continue;              //Skip saving this char
          }
          break;  //Else save this char

        case ';':          //Put here any end-of-instruction char
          if (!string) {   //If not in a string then the instruction is completed
            saving = 255;  //Mark end of instruction
            continue;      //Skip saving this char
          }
          break;  //Else save this char
      }

      if (!start) continue;  //If start of parameter has not yet been found, skip saving this char
      inst[saving] += c;     //Save the char to the correct parameter
    }

    fwrites(dst, instCode(inst[0]), 1);  //Write the instruction code

    for (uint8_t i = 1; i < 7; i += 2) {

      if (inst[i].charAt(0) == '@') fwrites(dst, dataCode(inst[i].substring(1)) + 128, 1);  //Write data type
      else fwrites(dst, dataCode(inst[i]), 1);                                              //@U8 -> U8 -> dataCode -> += 128 -> dst ELSE U8 -> dataCode -> dst

      switch (inst[i + 1].charAt(0)) {
        default:  //Decimal
          fwrites(dst, stoi(inst[i + 1]), 4);
          break;
        case 'f':  //Float
          fwrites(dst, ftoi(inst[i + 1].substring(1).toFloat()), 4);
          break;
        case 'x':  //Hexadecimal
          fwrites(dst, htoi(inst[i + 1].substring(1)), 4);
          break;
        case 'b':  //Binary
          fwrites(dst, btoi(inst[i + 1].substring(1)), 4);
          break;
      }
    }
  }
}

void load(File &src, File &dst,  uint32_t pos) {  //Loads executable in a defined position in memory (TODO: automatic memory sectors)
  dst.seekSet(pos);
  fwrites(dst, pos + 4 * 4, 4);               //Program pointer: next instruction to be executed
  fwrites(dst, pos + 4 * 4 + src.size(), 4);  //Memory pointer: next free memory byte
  fwrites(dst, pos + 4 * 4 + src.size(), 4);  //Memory base pointer: start of the memory
  fcopy(src, dst);                            //Stores the program in memory
}