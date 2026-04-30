#define INST_SIZE 16  //Byte size of a full instruction

void assembler(File &src, File &obj) {  //Takes an assembly file and outputs an object file ( [ DEF-POS ] [ BINARY ] [ DEF ] )

  fwrites(obj, 0, 4);        //Allocate space for saving the definition position at the end of the object file
  File def = fclear("def");  //[ DEF ] [ VAL ]

  uint32_t line = 0;  //Line counter
  src.rewind();

  while (src.available()) {
    String inst[7];  //[ INST ], [ TYPE ] [ DATA ], [ TYPE ] [ DATA ], [ TYPE ] [ DATA ]
    uint8_t saving = 0;
    char c;
    bool string = false, special = false, start = false;

    while (src.available() && (c = src.read()) != '\n') {

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

    if (inst[0].charAt(0) == '.') {                                              //Syntax for a jump definition
      def.print(inst[0].substring(1) + " " + itoh(line * INST_SIZE, 4) + "\n");  //[ DEF ] [ VAL ]
      continue;                                                                  //Skip the line counter
    }

    fwrites(obj, instCode(inst[0]), 1);  //Write [ INST ]

    for (uint8_t i = 1; i < 7; i += 2) {
      if (inst[i].charAt(0) == '@') fwrites(obj, dataCode(inst[i].substring(1)) + 128, 1);  //Write [ TYPE ]
      else fwrites(obj, dataCode(inst[i]), 1);                                              //@U8 -> U8 -> dataCode -> += 128 -> obj ELSE U8 -> dataCode -> obj

      switch (inst[i + 1].charAt(0)) {  //Write [ DATA ]

        default:  //Decimal
          fwrites(obj, stoi(inst[i + 1]), 4);
          break;

        case 'f':  //Float
          fwrites(obj, ftoi(inst[i + 1].substring(1).toFloat()), 4);
          break;

        case 'x':  //Hexadecimal
          fwrites(obj, htoi(inst[i + 1].substring(1)), 4);
          break;

        case 'b':  //Binary
          fwrites(obj, btoi(inst[i + 1].substring(1)), 4);
          break;

        case '@':                        //Definition
          obj.print(inst[i + 1] + "@");  //@def@
          break;
      }
    }
    line += 1;
  }

  fwrites(obj, obj.curPosition(), 4, 0);  //Saves the start position of the definitions at the start of the obj file
  fcopy(def, obj);                        //Copies the entire file definition in object

  def.close();
}

void linker(File &obj, File &exe) {
  obj.rewind();
  uint32_t pos = freads(obj, 4);  //Reads at the start of the file the def position

  /* Well damn it already does that (l'ho scritto io porca boia)
  File def = fclear("def");  //Same as in the assembler
  ftake(obj, pos, -1, def);  //WIP: It could be optimized to not use another file,
                             //instead modify ffind to use start and end pos to the search
                             //and use only the obj file
  */

  uint32_t i = 0;  //To search a definition only when there is a value
  while (obj.available() && obj.curPosition() < pos) {
    char c = obj.read();
    if (c == '@' && ((i % 2 == 0) || (i % 7 == 0) || (i % 12 == 0))) {  //Checks only the start of the values
      String str = "";
      while (obj.available() && obj.curPosition() < pos && (c = obj.read()) != '@') str += c;
      fwrites(exe, htoi(fdef(obj, str, pos, -1, true)), 4);
      i += 4;  //4 bytes written
    } else {
      exe.write(c);
      i += 1;  //1 byte written
    }
  }
}