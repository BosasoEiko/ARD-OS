uint32_t ftoi(const float f) {  //Float bytes to integer
  return *(uint32_t *)&f;       //Gets the bytes stored in the float and saves them as an integer
}

uint32_t htoi(const String s) {                                                         //Hex to uint32_t
  return strtoul(s.substring((s.length() - 8) * (s.length() >= 8)).c_str(), NULL, 16);  //If string is bigger than 8 chars, read only the last 8 chars, else read the entire string
}

uint32_t btoi(const String s) {  //Binary to uint32_t
  return strtoul(s.substring((s.length() - 32) * (s.length() >= 32)).c_str(), NULL, 2);
}

uint32_t stoi(const String s) {  //String to uint32_t (to use for integers)
  return strtoll(s.c_str(), NULL, 10);
}

String itoh(const uint32_t i, const uint8_t size) {  //Integer to 8 chars hexadecimal string
  String base = "";
  for (uint8_t j = size; j > 0; j -= 1) base += "00";
  String hex = String(i, 16);
  uint32_t len = hex.length();
  base.remove((size * 2 - len) * (len <= size * 2));    //Removes everything but the first bytes needed
  hex.remove(0, (len - size * 2) * (size * 2 <= len));  //Removes everything but the last bytes needed
  base += hex;
  base.toUpperCase();
  return base;
}