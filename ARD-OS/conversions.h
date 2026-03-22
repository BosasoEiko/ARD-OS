uint32_t ftoi(float f) {   //Float bytes to integer
  return *(uint32_t *)&f;  //Gets the bytes stored in the float and saves them as an integer
}

uint32_t htoi(String s) {                                                               //Hex to uint32_t
  return strtoul(s.substring((s.length() - 8) * (s.length() >= 8)).c_str(), NULL, 16);  //If string is bigger than 8 chars, read only the last 8 chars, else read the entire string
}

uint32_t btoi(String s) {  //Binary to uint32_t
  return strtoul(s.substring((s.length() - 32) * (s.length() >= 32)).c_str(), NULL, 2);
}

uint32_t stoi(String s) {
  return strtoll(s.c_str(), NULL, 10);
}