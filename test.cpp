#include "iarraystream.h"
#include <iostream>
using namespace std;

int main() {
    char h[13] = "Hello World!";
    ICE::iarraystream ias(h, 13);
    cout << h << endl;
    cout << ias.rdbuf();
};
