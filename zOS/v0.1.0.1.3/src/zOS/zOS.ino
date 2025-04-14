#include "Mkernel.h"
#include "Trimmer.h"

const int MAX_LINES = 10;
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

void linesInterpret(String code, char* mem1) {
  Trimmer::splitAndTrimLines(code.c_str(), lines, lineCount, MAX_LINES);
  for (int i = 0; i < lineCount; i++) {
    interpret(lines[i], mem1);
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
    linesInterpret(code, memory);

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
  
} else {
    Serial.println("command " + String(b) + " was not found.");
  }
}


void setup() {
  Serial.begin(9600);
  Serial.println("     == ZincShell v0.1| Zinc v0.1.3 ==");
  Serial.println("commands: pr, sav, clrmem, dwr, drd, delay, run\n");
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
