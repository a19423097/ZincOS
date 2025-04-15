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
        Serial.println("KERNEL_PANIC: UNKOWN DATA MIGHT HAVE BEEN OVERWRITTEN/INVALID MEMORY SIZE");
        Serial.println("THE SYSTEM HAS TO STOP TO AVOID CRASHES");
        Serial.print("reseting in:");
        Serial.print("3");
        delay(1000);
        Serial.print("2");
        delay(1000);
        Serial.println("1");
        dwr(13, 0);
        delay(2000);
        if (ignorePanic) {
          Serial.println("ignorePanic was true. returning to ZincShell");
          return;
        } else {
          Serial.print("it does not look like the system has reset. connect pin 13 to RESET if your using an arduino Uno.");
          while(1);
        }
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
};         
