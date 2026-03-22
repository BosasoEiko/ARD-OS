#include <SPI.h>
#include <SdFat.h>

#include "set.h"
#include "conversions.h"
#include "file.h"
#include "program.h"

SdFat SD;
File MEM, EXE, ASM;  //Shared memory //Program to load to memory
//File sectors;  //Division of memory (UNUSED)



void setup() {
  Serial.begin(9600);
  SD.begin();
  Serial.println("\n\nExecuting program...");
  uint32_t timer = millis();

  SD.remove("EXE");
  SD.remove("MEM");
  SD.remove("ASM");
  EXE = SD.open("EXE", (O_CREAT | O_RDWR));
  MEM = SD.open("MEM", (O_CREAT | O_RDWR));
  ASM = SD.open("ASM", (O_CREAT | O_RDWR));

  ASM.print("ADD @u32 x0a00000a, i32 456643654123893999, u8 xAFFAB11B; BOMBAAAA\nSUB u32 x000a, f32 f12389.3987, u8 x-a; BOMBOKLAT\n");
  fileLook(ASM, 0);
  Serial.print("\n\n");

  assemble(ASM, EXE);

  fileLook(EXE, 0);
  Serial.print("\n\n");
  fileLook(EXE, 1);

  MEM.rewind();
  fwrite(MEM, htoi("AFFAB11B"), 4);
  MEM.rewind();
  Serial.print("\n\n");
  Serial.print(fread(MEM, 4), 16);
  MEM.rewind();
  Serial.print("\n\n");
  fileLook(MEM, 1);
  Serial.print("\n\n");

  MEM.rewind();
  Serial.println(htoi("AFFAB11B"));
  Serial.println(htoi("AFFAB11B"), 16);
  fwrite(MEM, htoi("AFFAB11B"), 4);
  MEM.rewind();
  Serial.println();
  Serial.print(fread(MEM, 4), 16);

  Serial.print("\n\n");
  String s = "IODUBAWEIDUB";
  Serial.print(s.substring((s.length() - 8) * (s.length() >= 8))); //If string is bigger than 8 chars, read only the last 8 chars, else read the entire string

  ASM.close();
  MEM.close();
  EXE.close();

  //frag("TEST, u8 15, ptr, , ,  , , , , 0x0000000000000000    ,,,,,,,    str \" B,om\\\"ba  \\n\" AA; as,\"dasd");
  //frag("ADD u8 69, u16 902837, str \"\\nGiorno,; \\n \\\" asdsda\"; commento");
  //Serial.print("\n\n");
  //frag("ADD ,, , , , ,, , , , , ,  u8                                 69 u16 902837 str,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,, \"\\nGiorno,; \\n \\\" asdsda\"; commento");

  /*
  uint32_t i;
  float y = 1.5;
  i = *(uint32_t *)&y;
  Serial.println(i);
  Serial.println(i, 16);
  */
  //String a = "0100010001";
  //Serial.println(strtol(a.c_str(), NULL, 2));

  Serial.println("\n\nProgram executed in " + String(millis() - timer) + " ms");
}

void loop() {
}