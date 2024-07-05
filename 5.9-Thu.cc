int main() {
  // how to read contents from standard input
  int x;
  char ch;

  // >> : input operator
  // reads an integer from standard input, and write the value into x
  std::cin >> x;
  // std::cin ignores whitespace by default
  std::cin >> ch; // equivalent to scanf(" %c", &ch);
  // reads an integer followed by a char from standard input
  std::cin >> x >> ch;

  // << : output operator
  // output the value of x to standard output stream
  std::cout << x;
  // 先把x的值写到standard output，再把ch的值写到standard output
  std::cout << x << ch;
  // output the value of ch to standard error stream
  std::cerr << ch;
}

// How do we know if read fails?
// cin.fail() --> true if it fails
// if EOF, cin.fail() and cin.eof() will be true

// write a function, read integers from input
// print all integers read, one integer per line
// you may assume that only integers will be read

// C:  
void func() {
  int num;
  while (scanf("%d", &num) > 0) {
    printf("%d\n", num);
  }
} 

// cin是一个object而不是一个variable 
// cin has a few its own functionality
// cin.fail() returns true if read fails due to reason, and false otherwise
// cin.eof() returns true only if there is nothing to read, and false otherwise
// C++:
void func() {
  int num;
  while (true) {
    std::cin >> num;
    // break when read fail
    if (std::cin.fail()) {
    // if (std::cin.eof()) {
      break;
    }
    std::cout << num << std::endl;  
  }
  /*
  // 可以这么写但>> does not return a boolean value
  there is an implicit conversion from cin’s type (istream) to bool
  can use cin as a condition: true unless the stream has a read failure
  while (std::cin >> num) {
    std::cout << num << std::endl;
  }
  */
} 


// write a function, read integers from input
// print all integers read, one integer per line
// if non-integer is encountered, skip, and continue reading
// input: 1 2 3 a b 4 5 apple 6
// output: 1 2 3 4 5 6

// if read fails, failbit is set to true
// if the failbit is set to true, nothing will be read, and read will always fail
// clear the failbit: cin.clear()
// ignore the next byte read: cin.ignore()
void func() {
  int num;
  while (true) {
    if (std::cin >> num) {
      std::cout << num << std::endl;
    } else {
      if (std::cin.eof()) {
        break;
      } else {
        std::cin.clear(); // clears failbit
        std::cin.ignore(); // ignore next byte
      }
    }
  }
}

// std::endl : new line and flush the output directly
// std::noskipws: does not ignore whitespace when reading chars
int main() {
  char ch;
  std::cin >> ch; // ignore whitespace
  std::cin >> std::noskipws >> ch; // does not ignore whitespace  
}

/**
 * cin.fail() – something unexpected happened (eof, read "a" into int), 
                returns true if read fails, false otherwise
 * cin.good() – the operation succeeded
 * cin.eof() – end of input, returns true only if there is nothing to read, 
               and false otherwise
 * cin.bad() – suh and it was serious
 * cin.clear() – clear the state of the stream
 * cin.ignore() – throw away one character and read again

 * if we want to read whitespace, use 
 * char c;
 * cin.get(c); <- one char at a time
 
 * string s;
 * getline(cin, s); <- one line at a time (up to but not including “\n”)
 *                  <- reads up to \n, but does not include it in s
*/
#include <iostream>
using std::cin;
using std::cout;
using std::endl;
int main(int argc, char* argv[]) {
  int v;
  while (true) {
    cin >> v;
    cout << "> " << v
         << ", good=" << cin.good()
         << ", eof=" << cin.eof()
         << ", fail=" << cin.fail()
         << ", bad=" << cin.bad()
         << endl;
    if (!cin) break;
  }
}

/**
 * C++ has "manipulators" to format I/O => can be used to:
 * print numbers as hexadecimal(16)/octal(8)/decimal(10), 
 *                  bod alpha, setw, setfill, skipws, noskipws
 * Ex.
 * cout << hex << value; // all subsequent numbers are in hexadecimal, i.e. "sticky"
 * cout << dec; // Best practice: Undo any "sticky" changes
*/


#include <iostream>
#include <string>
// in C: C-string is an array of characters with null terminator \0
// in C++: String is a type, defined under namespace std

int main() {
  std::string str; // string definition, empty string by default
  // implicit constructor
  std::string s2 = "abc";
  // string constructor
  std::string s3("abc");
  // string constructor
  std::string s4(3, 'a'); // "aaa"

  // string assignment
  s2 = s3;

  // string concatenation
  s2 = s3 + s4;

  // string length
  // size_t: non-negative integer
  size_t len = s2.length();

  // string comparison
  s2 == s3;
  s2 < s3;
  s3 > s4;

  // 2-param - substring
  // first param: which index you want to start
  // second param: the length of substring
  std::string sub1 = s4.substr(1, 2);
  // 1-param - substring
  // first param: which index you want to start
  // by default, it cuts the string to the end
  std::string sub2 = s4.substr(1);
}

int main() {
  std::string str;
  // ignores all whitespace
  std::cin >> str; // reads a string from input, separated by whitespace

  // reads a whole line of input
  // by default, it stops at newline character
  std::getline(std::cin, str);
  std::cout << str << std::endl;
  // we can customize the stopping character 
  std::getline(std::cin, str, ',');
}
