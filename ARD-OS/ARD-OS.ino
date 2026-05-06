//#define DEBUG

#include "utilities.h"
#include "conversions.h"
#include "file.h"
#include "set.h"
#include "compiler.h"
#include "memory.h"
#include "program.h"

File exe;  //File for programs

void setup() {
  Serial.begin(9600);
  SD.begin();
  Serial.print("Starting timer...\n");
  uint32_t timer = millis();

  File obj = fclear("obj");
  File exe = fclear("exe");
  File tmp = fclear("tmp");

  tmp.print("ADD @u32 x0a00000a, i32 456643654123893999, u8 xAFFAB11B; BOMBAAAA\n.jump\nSUB u32 x000a, f32 f12389.3987, u8 @jump; BOMBOKLAT\n");
  ftype(tmp, 0);
  Serial.print("\n\n");

  Serial.print("[ ASSEMBLER TMP -> OBJ ]\n");
  assembler(tmp, obj);

  Serial.print("ftype(obj, 0):\n");
  ftype(obj, 0);
  Serial.print("\n");

  Serial.print("ftype(obj, 1):\n");
  ftype(obj, 1);
  Serial.print("\n");

  Serial.print("ftype(obj, 2):\n");
  ftype(obj, 2);
  Serial.print("\n");


  Serial.print("[ LINKER OBJ -> EXE ]\n");

  linker(obj, exe);

  Serial.print("ftype(exe, 0):\n");
  ftype(exe, 0);
  Serial.print("\n");

  Serial.print("ftype(exe, 1):\n");
  ftype(exe, 1);
  Serial.print("\n");

  Serial.print("ftype(exe, 2):\n");
  ftype(exe, 2);
  Serial.print("\n");

  Serial.print("ftype(exe, 3):\n");
  ftype(exe, 3);
  Serial.print("\n");

  Serial.print("\n\n");

  load(exe, 0);
  ftype(mem, 2);


  tmp.close();
  obj.close();
  exe.close();

  Serial.print("\n\nSetup completed in " + String(millis() - timer) + " ms\n");
  mem.close();  //Memory closes last
}

void loop() {
}