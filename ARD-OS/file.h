void fwrite(File &file, uint32_t val, uint8_t size) {
  for (int8_t i = size - 1; i >= 0; i--) file.write(val >> i * 8);
}

uint32_t fread(File &file, uint8_t size) {
  uint32_t val = 0;
  for (int8_t i = size - 1; i >= 0; i--) val += file.read() * pow(256, i);
  return val;
}

void fileCopy(File &src, File &dst) {  //Source and destination
  while (dst.available()) src.write(dst.read());
}

void fileLook(File &file, uint8_t mode) {
  file.rewind();
  switch (mode) {
    case 0: //Prints the contents
      while (file.available()) Serial.write(file.read());
      break;

    case 1: //Prints binary
      while (file.available()) Serial.print(String(file.read(), 16) + " ");
      break;
  }
}