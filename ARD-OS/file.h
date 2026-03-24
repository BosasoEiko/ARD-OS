#include <SPI.h>
#include <SdFat.h>
SdFat SD;
File mem, tmp, exe;  //Virtual memory //File for programs //TEMP: to test compilers
//File sectors;  //Division of memory (UNUSED)

//---------- fwrite ----------

void fwrites(File &file, const uint32_t val, const uint8_t size) {  //File write with size
  for (int8_t i = size - 1; i >= 0; i--) file.write(val >> i * 8);
}

void fwrites(File &file, const uint32_t val, const uint8_t size, const uint32_t pos) {  //File pos write with size
  file.seekSet(pos);
  fwrites(file, val, size);
}

//---------- fread ----------

uint32_t freads(File &file, const uint8_t size) {  //File read with size
  uint32_t val = 0;
  for (int8_t i = size - 1; i >= 0; i--) val += file.read() * pow(256, i);
  return val;
}

uint32_t freads(File &file, const uint8_t size, const uint32_t pos) {  //File pos read with size
  file.seekSet(pos);
  return freads(file, size);
}

//---------- other ----------

void fcopy(File &src, File &dst) {  //Source and destination
  src.rewind();
  while (src.available()) dst.write(src.read());
}

void ftype(File &file, const uint8_t mode) {  //Types into Serial the entire file
  file.rewind();
  switch (mode) {
    case 0:  //Prints the contents
      while (file.available()) Serial.write(file.read());
      break;

    case 1:  //Prints binary
      while (file.available()) Serial.print(String(file.read(), 16) + " ");
      break;

    case 2:
      while (file.available()) {
        Serial.print("\n 0x" + itoh(file.curPosition(), 4) + ": ");
        for (uint8_t i = 0; i < 4; i += 1) Serial.print(itoh(file.read(), 1) + " ");
      }
      break;
  }
}