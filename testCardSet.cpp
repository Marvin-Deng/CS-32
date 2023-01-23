#include "CardSet.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
    CardSet s;
    s.add(1234);
    assert(s.add(1234) == false); // no duplicates
    s.add(12);
    s.add(53);   
    s.add(12); // won't be added
    s.add(123);
    assert(s.size() == 4);
    cout << "Passed all tests" << endl;
    s.print(); // numbers should be printed in increasing order
}