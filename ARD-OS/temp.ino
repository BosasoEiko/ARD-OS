/*

mem.rewind();
  fwrites(mem, htoi("AFFAB11B"), 4);
  mem.rewind();
  Serial.print("\n\n");
  Serial.print(freads(mem, 4), 16);
  mem.rewind();
  Serial.print("\n\n");
  ftype(mem, 1);
  Serial.print("\n\n");

  mem.rewind();
  Serial.println(htoi("AFFAB11B"));
  Serial.println(htoi("AFFAB11B"), 16);
  fwrites(mem, htoi("AFFAB11B"), 4);
  mem.rewind();
  Serial.println();
  Serial.print(freads(mem, 4), 16);

  Serial.print("\n\n");
  String s = "IODUBAWEIDUB";
  Serial.print(s.substring((s.length() - 8) * (s.length() >= 8))); //If string is bigger than 8 chars, read only the last 8 chars, else read the entire string




  frag("TEST, u8 15, ptr, , ,  , , , , 0x0000000000000000    ,,,,,,,    str \" B,om\\\"ba  \\n\" AA; as,\"dasd");
  frag("ADD u8 69, u16 902837, str \"\\nGiorno,; \\n \\\" asdsda\"; commento");
  Serial.print("\n\n");
  frag("ADD ,, , , , ,, , , , , ,  u8                                 69 u16 902837 str,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,, \"\\nGiorno,; \\n \\\" asdsda\"; commento");

  
  uint32_t i;
  float y = 1.5;
  i = *(uint32_t *)&y;
  Serial.println(i);
  Serial.println(i, 16);
  
String a = "0100010001";
Serial.println(strtol(a.c_str(), NULL, 2));












*/