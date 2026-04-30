#include <SPI.h>
#include <SdFat.h>
SdFat SD;
File mem, tmp, exe;  //Virtual memory //File for programs //TEMP: to test compilers
//File sectors;  //Division of memory (UNUSED)

//ret: true = return to previous position (default = false)

//-------------------------------------------------------------------------------------

void fwrites(File &dst, const uint32_t val, const uint8_t size) {  //File write with size
  for (int8_t i = size - 1; i >= 0; i--) dst.write(val >> i * 8);
}

void fwrites(File &dst, const uint32_t val, const uint8_t size, const uint32_t pos) {
  uint32_t cur = dst.curPosition();
  dst.seekSet(pos);
  fwrites(dst, val, size);
  dst.seekSet(cur);
}

//-------------------------------------------------------------------------------------------------

uint32_t freads(File &src, const uint8_t size) {  //File read with size
  uint32_t val = 0;
  for (int8_t i = size - 1; i >= 0; i--) val += src.read() * pow(256, i);
  return val;
}

uint32_t freads(File &src, const uint8_t size, const uint32_t pos) {
  uint32_t cur = src.curPosition();
  src.seekSet(pos);
  uint32_t val = freads(src, size);
  src.seekSet(cur);
  return val;
}

//-------------------------------------------------------------------------

String _fdef(File &src, const String def, const uint32_t start = 0, const uint32_t end = -1) {
  char c;
  String str = "";
  src.seekSet(start);
  while (src.available() && src.curPosition() <= end) {
    while (src.available() && src.curPosition() <= end && (c = src.read()) != '\n') str += c;
    uint32_t i = str.lastIndexOf(" ");                                       //Index of the space between [ DEF ] and [ VALUE ] (lastIndexOf to ignore DEF)
    if (str.substring(0, i) == def && i != -1) return str.substring(i + 1);  //Return [ VAL ] if found
  }
  return "";  //If not found return empty string
}

String fdef(File &src, const String def, const uint32_t start = 0, const uint32_t end = -1, const bool ret = false) {  //File find def and return value ( [ DEF ] [ VALUE ] )
  uint32_t cur;
  if (ret) cur = src.curPosition();
  String str = _fdef(src, def, start, end);
  if (ret) src.seekSet(cur);
  return str;
}

//--------------------------------------------------------------------------

void fcopy(File &src, File &dst, const uint32_t start = 0, const uint32_t end = -1, const bool ret = false) {  //fcopy() but with a start and end positions (both inclusive; start = 0 && end = -1 to copy whole file)
  uint32_t cur;
  if (ret) cur = src.curPosition();
  src.seekSet(start);
  while (src.available() && src.curPosition() <= end) dst.write(src.read());
  if (ret) src.seekSet(cur);
}

//---------- other ----------

File fclear(const String file) {
  SD.remove(file);
  return SD.open(file, (O_CREAT | O_RDWR));
}

void ftype(File &file, const uint8_t mode) {  //Types into Serial the entire file
  file.rewind();
  switch (mode) {
    case 0:  //Prints the contents
      while (file.available()) Serial.write(file.read());
      break;

    case 1:  //Prints hexadecimal
      while (file.available()) Serial.print(String(file.read(), 16) + " ");
      break;

    case 2:  //Prints whole file in hexadecimal
      while (file.available()) {
        Serial.print("\n 0x" + itoh(file.curPosition(), 4) + ": ");
        for (uint8_t i = 0; i < 4; i += 1) Serial.print(itoh(file.read(), 1) + " ");
      }
      break;

    case 3:  //Prints the binary divided in instructions
      while (file.available()) {
        Serial.print("\n 0x" + itoh(file.curPosition(), 4) + ": ");

        Serial.print("[ ");
        Serial.print(itoh(file.read(), 1) + " ");  //OP
        Serial.print("], [ ");
        Serial.print(itoh(file.read(), 1) + " ");  //TYPE1
        Serial.print("] [ ");
        for (uint8_t i = 0; i < 4; i += 1) Serial.print(itoh(file.read(), 1) + " ");  //DATA1
        Serial.print("], [ ");
        Serial.print(itoh(file.read(), 1) + " ");  //TYPE2
        Serial.print("] [ ");
        for (uint8_t i = 0; i < 4; i += 1) Serial.print(itoh(file.read(), 1) + " ");  //DATA2
        Serial.print("], [ ");
        Serial.print(itoh(file.read(), 1) + " ");  //TYPE3
        Serial.print("] [ ");
        for (uint8_t i = 0; i < 4; i += 1) Serial.print(itoh(file.read(), 1) + " ");  //DATA3
        Serial.print("];");
      }
      break;
  }
}