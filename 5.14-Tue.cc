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

// 思睿
// C++ Stream
// 1. iostream: standard input & output stream
#include <iostream>
// 2. sstream: string stream, read from string stream, write to string stream
#include <sstream>
// 3. fstream: file stream, read from existing file, write to a file
#include <fstream> // f stands for file

/** file stream
 * ifstream: input file stream
 * ofstream: output file stream
*/

int main() {
  std::string str;
  // read a string from standard input
  std::cin >> str;

  // std::ifstream identifier{path of the file};
  std::ifstream infile{"input.in"};
  // you have to make sure the file exists and can be opened
  if (infile.is_open()) {
    // how to read from existing file
    // read from an existing file created as infile
    infile >> str;
    std::cout << str << std::endl;
  } else {
      std::cerr << "file failed to open" << std::endl;
  }
}

int main() {
  std::string str = "apple";
  std::ofstream outfile{"out.txt"};
  // if file does not exist, a new empty file will be created
  outfile << str << std::endl;
}

// write a function, that reads from existing file named "in.txt", and 
// copys the contents of even lines to another existing file named "out.txt"
void copy_file() {
  std::ifstream infile{"in.txt"};
  std::ofstream outfile{"out.txt"};

  std::string line;
  int count = 1;
  while (std::getline(infile, line)) {
    if (count % 2 == 0) {
      outfile << line << std::endl;
    }
    count += 1;
  }
}


/** string stream
 * istringstream : input string stream
 * ostringstream : output string stream 
*/

void strToInt(const std::string str) {
  int num;
  // convert string str into a input string stream
  std::istringstream iss{str};
  if (iss >> num) {
    std::cout << num << std::endl;
  } else {
    std::cerr << "Cannot be parsed an integer" << std::endl;
  }
}

void intToStr(const int n) {
  std::ostringstream oss; // no initalization
  oss << n;
  // convert ostringstream into string
  std::string str = oss.str();
  std::cout << "string is: " << str << std::endl;
}

int main() {
  strToInt("123");          // 123
  strToInt("abc123");       // Cannot be parsed an integer
  strToInt("123abc123");    // 123
  intToStr(123);            // string is 123
  intToStr(-200);           // string is -200
}

void func() {
  std::string line;
  while (std::getline(std::cin, line)) {
    std::istringstream iss{line};
    int result = 0;
    int num; // place holder for number
    char ch; // place holder for operator
    iss >> result;
    while (iss >> ch >> num) {
      if (ch == '-') {
        result -= num;
      } else {
        result += num;
      }
    }
    std::cout << result << std::endl;
  }
}


// first parameter: length of array
// second parameter: array of C-string
//                   stores all command-line arguments while running the program
int main(int argc, char* argv[]) {
  std::cout << "Number of Arguments" << argc << std::endl;
  // argv[0] is the name of the program you are running
  // the rest of the array is the actual arguments provided
  for (int i = 1; i < argc; ++i) {
    std::cout << "Arg " << i << ": " << argv[i] << std::endl;
  }
}

int main(int argc, char* argv[]) {
  int count = 0;
  for (int i = 1; i < argc; ++i) {
    /*
    if (argv[i] == "apple") {
    // "" in C/C++ is string literal: const char *
    // char* == char*, you are comparing the addresses
    */
    std::string arg = argv[i]; // convert char * into std::string
    if (arg == "apple") {
      count += 1;
    }
  }
  std::cout << count << std::endl;
}
