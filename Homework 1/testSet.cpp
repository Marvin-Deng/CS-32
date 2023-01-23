#include "Set.h"
//#include "newSet.h"
#include <iostream>
#include <string>
#include <cassert>
using namespace std;

int main()
{
    Set s;
    assert(s.empty());
    ItemType x = "arepa";
    assert(!s.get(42, x) && x == "arepa"); // x unchanged by get failure
    s.insert("chapati");
    assert(s.size() == 1);
    assert(s.get(0, x) && x == "chapati");
    cout << "Passed all tests" << endl;
}


//int main()
//{
//    Set s;
//    assert(s.empty());
//    ItemType x = 9876543;
//    assert(!s.get(42, x) && x == 9876543); // x unchanged by get failure
//    s.insert(123456789);
//    assert(s.size() == 1);
//    assert(s.get(0, x) && x == 123456789);
//    cout << "Passed all tests" << endl;
//}


