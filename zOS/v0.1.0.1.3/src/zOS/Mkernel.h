struct Mkern {
  static void dwr(int pin, int val) {
    digitalWrite(pin, val);
  }
  static int drd(int pin) {
    return digitalRead(pin);
  }
};