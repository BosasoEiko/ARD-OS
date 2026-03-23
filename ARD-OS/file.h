void fwrites(File &file, uint32_t val, uint8_t size) {  //File write with size
  for (int8_t i = size - 1; i >= 0; i--) file.write(val >> i * 8);
}

uint32_t freads(File &file, uint8_t size) {  //File read with size
  uint32_t val = 0;
  for (int8_t i = size - 1; i >= 0; i--) val += file.read() * pow(256, i);
  return val;
}

void fcopy(File &src, File &dst) {  //Source and destination
  src.rewind();
  while (src.available()) dst.write(src.read());
}

void ftype(File &file, uint8_t mode) {  //Types into Serial the entire file
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