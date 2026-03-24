#include "file.h"
#include "conversions.h"
#include "set.h"
#include "program.h"

void setup() {
  Serial.begin(9600);
  SD.begin();
  Serial.print("Starting timer...\n");
  uint32_t timer = millis();

  SD.remove("exe");
  SD.remove("mem");
  SD.remove("tmp");
  exe = SD.open("exe", (O_CREAT | O_RDWR));
  mem = SD.open("mem", (O_CREAT | O_RDWR));
  tmp = SD.open("tmp", (O_CREAT | O_RDWR));

  tmp.print("ADD @u32 x0a00000a, i32 456643654123893999, u8 xAFFAB11B; BOMBAAAA\nSUB u32 x000a, f32 f12389.3987, u8 x-a; BOMBOKLAT\n");
  ftype(tmp, 0);
  Serial.print("\n\n");

  assemble(tmp, exe);
  ftype(exe, 1);
  Serial.print("\n");
  ftype(exe, 2);
  Serial.print("\n\n");

  load(exe, 0);
  ftype(mem, 2);


  tmp.close();
  mem.close();
  exe.close();

  Serial.print("\n\nSetup completed in " + String(millis() - timer) + " ms\n");
}

void loop() {
}