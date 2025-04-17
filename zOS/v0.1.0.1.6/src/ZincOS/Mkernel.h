struct Mkern {
  static bool ignorePanic;

  static void setup(bool ignoreKernelPanic) {
    ignorePanic = ignoreKernelPanic;
  }

  static void dwr(int pin, int val) {
    digitalWrite(pin, val);
  }

  static int drd(int pin) {
    return digitalRead(pin);
  }

  static void clrmem(char* mem, int size) {
    const int memSize = 50;
    for (int i = 0; i < size; i++) {
      if (i > memSize) {
        handleKernelPanic();
        return;
      }
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

private:
  static void handleKernelPanic() {
    Serial.println("KERNEL_PANIC: ATTEMPT TO WRITE PAST BUFFER. DATA MIGHT HAVE BEEN OVERWRITTEN.");
    if (ignorePanic) {
      Serial.println("ignorePanic is TRUE. skipping hard stop, but corruption may have occurred!");
      return;
    }
    Serial.println("THE SYSTEM HAS TO STOP TO AVOID CRASHES");
    Serial.print("reseting in:");
    Serial.print("3");
    delay(1000);
    Serial.print("2");
    delay(1000);
    Serial.println("1");
    delay(5);
    pinMode(12, OUTPUT);
    dwr(12, 0);
    delay(2000); 

    Serial.print("the system has not reset. reset manually or connect pin 12 to RESET");
    while(1);  // Halt the system
  }
};
