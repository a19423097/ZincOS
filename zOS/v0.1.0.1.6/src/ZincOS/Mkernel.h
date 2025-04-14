struct Mkern {
  static void dwr(int pin, int val) {
    digitalWrite(pin, val);
  }

  static int drd(int pin) {
    return digitalRead(pin);
  }

  static void clrmem(char* mem, int size) {
    for (int i = 0; i < size; i++) {
      mem[i] = '\0';
    }
  }

  static void sav(char* mem, const char* src, int size) {
    int i = 0;
    while (src[i] != '$' && i < size - 1) {
      mem[i] = src[i];
      i++;
    }
    mem[i] = '\0';
  }
};
