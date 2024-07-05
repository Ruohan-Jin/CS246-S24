// Vec interface file (vector.cc)
export module vec; // indicates that this is the module interface file
                   // we can have only one module interface file
                   // vec: name of the module

export struct Vec { // Anything marked export is made avaiable 
                    // to the client of the module to use
  int x, y;
};

export Vec operator+(const Vec& v1, const Vec& v2);
