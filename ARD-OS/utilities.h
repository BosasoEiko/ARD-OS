void frag(String str) {  //For testing purposes
  String inst[7];
  uint8_t saving = 0;  //7 = end
  char c;
  bool string = false, special = false, start = false;


  for (uint8_t i = 0; i < str.length() && (c = str.charAt(i)) != '\n'; i += 1) {
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

  Serial.println("[" + inst[0] + "]");
  Serial.println("(" + inst[1] + ") [" + inst[2] + "]");
  Serial.println("(" + inst[3] + ") [" + inst[4] + "]");
  Serial.println("(" + inst[5] + ") [" + inst[6] + "]");
}

void debug(const String s) {
#ifdef DEBUG
  Serial.print(s);
#endif
}

// Bit operations

void bitset(uint32_t &val, uint8_t bit) {
  val = val | 1 << bit;
}
void bitclr(uint32_t &val, uint8_t bit) {
  val = val & ~(1 << bit);
}
void bittgl(uint32_t &val, uint8_t bit) {
  val = val ^ 1 << bit;
}
uint32_t bitget(uint32_t val, uint8_t bit) {
  return val & (1 << bit);
}