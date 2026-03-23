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

#define PTR_SIZE 4                       //Size in bytes of the pointers
#define HDR_COUNT 3                      //Number of pointers in the header
#define HDR_SIZE (HDR_COUNT * PTR_SIZE)  //Number of bytes of the header in memory (n° of pointers * 4 bytes)

void load(File &exe, const uint32_t pos) {  //Loads executable in a defined position in memory (TODO: automatic memory sectors)
  mem.seekSet(pos);

  fwrites(mem, pos + HDR_SIZE, 4);               //Program pointer: next instruction to be executed
  fwrites(mem, pos + HDR_SIZE + exe.size(), 4);  //Memory pointer: next free memory byte
  fwrites(mem, pos + HDR_SIZE + exe.size(), 4);  //Memory base pointer: start of the memory
  fcopy(exe, mem);                               //Stores the program in memory
}

void execute(const uint32_t start) {  //Executes an instruction of a program loaded in memory, with its memory space starting at start
  mem.seekSet(start);

  uint32_t progPtr = freads(mem, 4);  //Reads the program pointer
  uint32_t progPtrPos = start;        //Position in memory of the program pointer

  uint32_t progBasePtr = start + HDR_SIZE;         //Calculates the program base pointer
  uint32_t progBasePtrPos = start + PTR_SIZE * 1;  //Position in memory of the program base pointer

  uint32_t memPtr = freads(mem, 4);           //Reads the memory pointer
  uint32_t memPtrPos = start + PTR_SIZE * 2;  //Position in memory of the memory pointer

  uint32_t memBasePtr = freads(mem, 4);           //Reads the memory base pointer
  uint32_t memBasePtrPos = start + PTR_SIZE * 3;  //Position in memory of the memory base pointer

  mem.seekSet(progPtr);
  uint32_t inst[7] = {
    freads(mem, 1),  //op
    freads(mem, 1),  //type 1
    freads(mem, 4),  //data 1
    freads(mem, 1),  //type 2
    freads(mem, 4),  //data 2
    freads(mem, 1),  //type 3
    freads(mem, 4),  //data 3
  }

  switch (inst[0]) {
    default:
      break;
  }
}