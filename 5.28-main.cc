// client program(main.cc)
import vec; // Note no < > because this is a user module, 
            // not a system module like iosteram

int main() {
  Vec v1{1, 2};
  Vec v2(3, 4);
  Vec v3 = v1 + v2;
}
