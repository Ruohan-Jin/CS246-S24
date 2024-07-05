// 1. Files(fileInput.cc)
  // * std::ifstream - stream for reading
  // * std::ofstream - stream for writing

// 1.1 C
#include <stdio.h>
int main() {
  char s[256]; // Hold up to 255 characters with trailing \0
  FILE *f = fopen("file.txt", "r");
  while (1) {
    fscanf(f, "%255s", s); // reads up to 255 chars into s
    if (feof(f)) break;
    printf("%s\n", s);
  }
  fclose(f);
}

// 1.2 C++ 
import <iostream>;
import <fstream>;
import <string>;
using namespace std;
int main() {
  ifstream f{"file.txt"}; // declares and initializes the ifstream var f.
                          // opens file and positions read pointer at first char
  string s;
  while (f >> s) { // No concerns about word length
  // 改成while (getline(f, s)) {输出什么
    cout << s << endl;
  } 
} // File is closed when the ifstream goes out of scope


// 2. String Streams(getNum.cc, readIntsSS.cc)
  // * std::istringstream - extracts from a string
  // * std::ostringstream - sends to a string
import <sstream>;
string intToString(int n) {
  ostringstream oss;
  oss << n;
  return oss.str(); // extracts string from string stream
}

int i = 42;
cout << i; // writes int to std output

cout << intToString(42); // writes a string to standard output


// 3. Processing the Command Line(args-sum.cc)
int main(int argc, char*argv[]) { // argc: # of command line arguments
                                  // argv: array of char pointers, 
                                  //       always one more element than argc
  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i]; // converting C-style string into C++ string
    if (arg == "-c") {...}
  }
}
// ./myprogram abc 123
// argc = 3
// argv[0]指向./myprogram\0
// argv[1]指向abc\0
// argv[2]指向123\0
// argv[3]指向\0
// These are C-style strings with '\0'


// 4. Default Function Parameters
int f(int a, int b, int c) {...}
f(1, 2, 3);
f(1);
f(1, 2);
int f(int a, int b = 0, int c = 0) {...}
// 不可以f(1, ,3);
f(1); // f(1, 0, 0); 
      // compiler adds the missing parameters at compile time

// default value only appears once
// 你如果在function declaration里面写了default value
// 就不要在function definition里面写
// 函数声明
int add(int x = 10, int y = 20);
// 函数定义
int add(int x = 10, int y = 20) { // 错误：不应该再次提供默认值
    return x + y;
}
// 函数定义
int add(int x, int y) { // 正确：不再提供默认值
    return x + y;
}

cin.ignore();
cin.ignore(5);
cin.ignore(80, '\n');  //contain two, first is the length and second is the stop value
//default values create more flexible functions
