#include "Mkernel.h"
#include "Trimmer.h"

const int MAX_LINES = 30;
String lines[MAX_LINES];
int lineCount = 0;

char readByte;
String buffer = "";
char memory[50];

const char* code = R"(dwr13$1
delay1000$
dwr13$0
delay1000$
)";
char code2[200] = "";

void linesInterpret(String code, char* mem1) {
  Trimmer::splitAndTrimLines(code.c_str(), lines, lineCount, MAX_LINES);

  int i = 0;
  while (i < lineCount) {
    String current = lines[i];

    if (current.startsWith("jump")) {
      int j = 0;
      char numStr[6];
      while ((j + 4) < current.length() && current[j + 4] != '$' && j < 5) {
        numStr[j] = current[j + 4];
        j++;
      }
      numStr[j] = '\0';

      int target = atoi(numStr);

      if (target >= 0 && target < lineCount) {
        i = target;
        continue;
      } else {
        Serial.println("Invalid jump target: " + String(target));
        break;
      }
    }

    interpret(current, mem1);
    i++;
  }
}



void interpret(String b, char* mem) {
  if (b.startsWith("pr")) {
    char str[51]; // max 50 characters + null terminator
    int i = 0;

    if (b[2] == 'm' && b[3] == '*') {
      Serial.println(mem);
      return;
    }

    while ((i + 2) < b.length() && b[i + 2] != '$' && i < 50) {
      str[i] = b[i + 2];
      i++;
    }
    str[i] = '\0'; // null-terminate the string
    Serial.println(str);

  } else if (b.startsWith("sav")) {
    int i = 0;
    while ((i + 2) < b.length() && b[i + 2] != '$' && i < 50) {
      mem[i] = b[i + 2];
      i++;
    }
    mem[i] = '\0';
    Serial.println("saved string from \"" + b + "\".");

  } else if (b.startsWith("clrmem")) {
    for (int i = 0; i < 50; i++) {
      mem[i] = '\0';
  }
  Serial.println("memory cleared.");

  } else if (b.startsWith("dwr")) {
    int i = 0;
    char wpinStr[4];
    while (b[i + 3] != '$' && i < 3) {
      wpinStr[i] = b[i + 3];
      i++;
  }
    wpinStr[i] = '\0';

    char wvalStr[2];
    wvalStr[0] = b[b.length() - 1];
    wvalStr[1] = '\0';

    int wpin = String(wpinStr).toInt();
    int wval = String(wvalStr).toInt();

    pinMode(wpin, OUTPUT);
    Mkern::dwr(wpin, wval);

    Serial.println("set pin " + String(wpinStr) + " to " + wvalStr);

  } else if (b.startsWith("drd")) {
    int i = 0;
    char rpinStr[4];
    while (b[i + 3] != '$' && i < 3) {
      rpinStr[i] = b[i + 3];
      i++;
    }
    rpinStr[i] = '\0';

    int rpin = String(rpinStr).toInt();

    pinMode(rpin, INPUT);
    int readval = Mkern::drd(rpin);

    Serial.println("read from pin " + String(rpinStr) + " and got value " + readval);

  } else if (b.startsWith("run")) {
    if (b[3] == '1') {
      linesInterpret(code, memory);
    } else if (b[3] == '2') {
      linesInterpret(code2, memory);
    } else {
      Serial.println("program" + String(b[3]) + " was not found");
    }

  } else if (b.startsWith("delay")) {
    char time[10];
    int i = 0;

    while (b[i + 5] != '$' && i < 9) { // also guard against overflow
      time[i] = b[i + 5];
      i++;
    }
    time[i] = '\0';
    int delayms = atoi(time);
    delay(delayms);
  
} else if (b.startsWith("append")) {
  int i = 0, j = 0;
  char str[51];
  while (b[i + 6] != '@' && j < 50) {
    char ch = b[i + 6];
    if (ch == '\\' && b[i + 7] == 'n') {
      str[j++] = '\n';
      i += 2; // skip the '\n' characters
    } else {
      str[j++] = ch;
      i++;
    }
  }
  str[j] = '\0'; // null-terminate the string

  if (b[b.length() - 1] == '1') {
    Serial.println("read only storage.");
  } else if (b[b.length() - 1] == '2') {
    code2[sizeof(code2) - 1] = '\0';
    strcat(code2, str);
    Serial.println("appended \"" + String(str) + "\" into 2");
  }
} else {
    Serial.println("command " + String(b) + " was not found.");
  }
}


void setup() {
  Serial.begin(9600);
  Serial.println("     == ZincShell v0.1| Zinc v0.1.3 ==");
  Serial.println("commands: pr, sav, clrmem, dwr, drd, delay, run, append\n");
  Serial.print("OK|");
}

void loop() {
  while (Serial.available() > 0) {
    readByte = Serial.read();

    if (readByte == '\n') {
      Serial.println(buffer);
      interpret(buffer, memory); // pass buffer and memory to interpret
      Serial.print("OK|");
      buffer = "";
    } else {
      buffer += readByte;
    }
  }
}
