enum instSet {
  NOI,  //No instruction
  MOV,
  JMP,
  ADD,
  SUB,
  MUL,
  DIV,
  POW,
  ROOT
};

enum dataSet {  //For pointer variants += 0b1000000 or += 128 or += 0x80
  NOD,          //No data
  U8,           //Unsigned Integer
  I8,           //Signed Integer
  F8,           //Float
  U16,
  I16,
  F16,
  U32,
  I32,
  F32,
  CHR,  //Char (U8)
  STR   //String (U32 size + data)
};

instSet instCode(const String &inst) {
  if (inst.equalsIgnoreCase("MOV")) return MOV;
  if (inst.equalsIgnoreCase("JMP")) return JMP;
  if (inst.equalsIgnoreCase("ADD")) return ADD;
  if (inst.equalsIgnoreCase("SUB")) return SUB;
  if (inst.equalsIgnoreCase("MUL")) return MUL;
  if (inst.equalsIgnoreCase("DIV")) return DIV;
  if (inst.equalsIgnoreCase("POW")) return POW;
  if (inst.equalsIgnoreCase("ROOT")) return ROOT;

  return NOI;
}

dataSet dataCode(const String &inst) {
  if (inst.equalsIgnoreCase("U8")) return U8;
  if (inst.equalsIgnoreCase("I8")) return I8;
  if (inst.equalsIgnoreCase("F8")) return F8;
  if (inst.equalsIgnoreCase("U16")) return U16;
  if (inst.equalsIgnoreCase("I16")) return I16;
  if (inst.equalsIgnoreCase("F16")) return F16;
  if (inst.equalsIgnoreCase("U32")) return U32;
  if (inst.equalsIgnoreCase("I32")) return I32;
  if (inst.equalsIgnoreCase("F32")) return F32;
  if (inst.equalsIgnoreCase("CHR")) return CHR;
  if (inst.equalsIgnoreCase("STR")) return STR;

  return NOD;
}