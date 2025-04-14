struct Trimmer {
  static String trim(const String& s) {
    int start = 0;
    while (start < s.length() && isspace(s[start])) start++;

    int end = s.length() - 1;
    while (end >= 0 && isspace(s[end])) end--;

    if (start > end) return "";

    return s.substring(start, end + 1);
  }

  static void splitAndTrimLines(const char* code, String* outputArray, int& count, int maxCount) {
    String line = "";
    count = 0;

    for (int i = 0; code[i] != '\0'; i++) {
      char c = code[i];
      if (c == '\n') {
        line = trim(line);
        if (line.length() > 0 && count < maxCount) {
          outputArray[count++] = line;
        }
        line = "";
      } else {
        line += c;
      }
    }

    // Handle the last line (if no trailing \n)
    line = trim(line);
    if (line.length() > 0 && count < maxCount) {
      outputArray[count++] = line;
    }
  }
};
