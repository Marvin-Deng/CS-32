// ItemType unsigned long
#include "newSet.h"
#include <iostream>
#include <cassert>
#include <string>
using namespace std;

int main()
{
	Set a(100); // holds at most 1000 distinct items
	Set b(5);	// holds at most 5 distinct items
	Set c;		 //holds at most DEFAULT_MAX_ITEMS distinct items
	ItemType items[6] = { "a", "b", "c", "d", "e", "f" };

	//Insert 5 distinct items into b
	for (int k = 0; k < 5; k++)
		assert(b.insert(items[k]));

	// Can't insert another item into b
	assert(!b.insert(items[5]));

	// When contents are swapped, their capacities are swapped
	a.swap(b);
	assert(!a.insert(items[5]) && b.insert(items[5])); // a capacity = 5, b capacity = 100, b inserts f
	assert(b.get(0, "f") == true);

	// Equals operator and copy constructor
	b = a;


	cout << "Passed all tests" << endl;
}