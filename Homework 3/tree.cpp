#include <iostream>
#include <string>
#include <cassert>
using namespace std;

// Return the number of ways that all n1 elements of a1 appear in
	  // the n2 element array a2 in the same order (though not necessarily
	  // consecutively).  We decree that the empty sequence (i.e. one where
	  // n1 is 0) appears in a sequence of length n2 in 1 way, even if n2
	  // is 0.  For example, if a2 is the 7 element array
	  //	10 50 40 20 50 40 30
	  // then for this value of a1     the function must return
	  //	10 20 40                        1
	  //	10 40 30                        2
	  //	20 10 40                        0
	  //	50 40 30                        3
int countIsIn(const double a1[], int n1, const double a2[], int n2)
{
	if (n1 <= 0) { // If the end of a1 is reached, then subsequence a1 is in a2
		return 1; // Increments the count by 1
	}
	if (n1 > n2 || n2 <= 0) { // no more elements in a2 to search
		return 0;
	}
	int count = countIsIn(a1, n1, a2 + 1, n2 - 1); 

	if (a1[0] == a2[0]) { // If the numbers match, then shift indexes of both arrays forward
		return count + countIsIn(a1 + 1, n1 - 1, a2 + 1, n2 - 1);
	}
	else {
		return count;
	}
}

// Exchange two doubles
void exchange(double& x, double& y)
{
	double t = x;
	x = y;
	y = t;
}

// Rearrange the elements of the array so that all the elements
// whose value is > divider come before all the other elements,
// and all the elements whose value is < divider come after all
// the other elements.  Upon return, firstNotGreater is set to the
// index of the first element in the rearranged array that is
// <= divider, or n if there is no such element, and firstLess is
// set to the index of the first element that is < divider, or n
// if there is no such element.
// In other words, upon return from the function, the array is a
// permutation of its original value such that
//   * for 0 <= i < firstNotGreater, a[i] > divider
//   * for firstNotGreater <= i < firstLess, a[i] == divider
//   * for firstLess <= i < n, a[i] < divider
// All the elements > divider end up in no particular order.
// All the elements < divider end up in no particular order.
void divide(double a[], int n, double divider, int& firstNotGreater, int& firstLess)
{
	if (n < 0)
		n = 0;

	// It will always be the case that just before evaluating the loop
	// condition:
	//  firstNotGreater <= firstUnknown and firstUnknown <= firstLess
	//  Every element earlier than position firstNotGreater is > divider
	//  Every element from position firstNotGreater to firstUnknown-1 is
	//    == divider
	//  Every element from firstUnknown to firstLess-1 is not known yet
	//  Every element at position firstLess or later is < divider

	firstNotGreater = 0;
	firstLess = n;
	int firstUnknown = 0;
	while (firstUnknown < firstLess)
	{
		if (a[firstUnknown] < divider)
		{
			firstLess--;
			exchange(a[firstUnknown], a[firstLess]);
		}
		else
		{
			if (a[firstUnknown] > divider)
			{
				exchange(a[firstNotGreater], a[firstUnknown]);
				firstNotGreater++;
			}
			firstUnknown++;
		}
	}
}

// Rearrange the elements of the array so that
// a[0] >= a[1] >= a[2] >= ... >= a[n-2] >= a[n-1]
// If n <= 1, do nothing.

// Sort array in decreasing order
void order(double a[], int n)
{
	if (n <= 1) {
		return;
	}
	int firstNotGreater;
	int firstLess;
	int mid = n / 2;

	divide(a, n, a[mid], firstNotGreater, firstLess); 

	order(a, firstNotGreater); // order the left side
	order(a + firstLess, n - firstLess); // order the right side
}

void testcountIsIn()
{
	const int SIZE = 7;
	const int SIZE1 = 3;
	const int SIZE2 = 1;
	const int SIZE3 = 0;

	double a1[SIZE] = { 1, 2, 3, 4, 2, 3, 5 };
	double a2[SIZE1] = { 1, 4, 3 };
	double a3[SIZE1] = { 1, 3, 5 };
	double a4[SIZE1] = { 4, 1, 3 };
	double a5[SIZE1] = { 2, 3, 5 };
	double a6[SIZE2] = { 0 };

	assert(countIsIn(a2, SIZE1, a1, SIZE) == 1);
	assert(countIsIn(a3, SIZE1, a1, SIZE) == 2);
	assert(countIsIn(a6, SIZE2, a1, SIZE3) == 0);
	assert(countIsIn(a4, SIZE1, a1, SIZE) == 0);
	assert(countIsIn(a5, SIZE1, a1, SIZE) == 3);
	assert(countIsIn(a1, SIZE2, a6, SIZE3) == 0);
}

void testOrder()
{
    const int SIZE = 4;
    const int SIZE1 = 3;
    const int SIZE2 = 1;

	// Even
	double a[SIZE] = { 1, 2, 3, 4 };
	order(a, SIZE);
	assert(a[0] == 4 && a[1] == 3 && a[2] == 2 && a[3] == 1);
	double a4[SIZE] = { 4, 2, 3, 1 };
	order(a4, SIZE);
	assert(a4[0] == 4 && a4[1] == 3 && a4[2] == 2 && a4[3] == 1);

	// Odd
	double a0[1] = { 1 };
	order(a0, 1);
	assert(a0[0] == 1);
	double a2[SIZE1] = { 2, 3, 4};
	order(a2, SIZE1);
	assert(a2[0] == 4 && a2[1] == 3 && a2[2] == 2);
	double a3[5] = { 1, 2, 3, 4, 5 };
	order(a3, 5);
	assert(a3[0] == 5 && a3[1] == 4 && a3[2] == 3);
}

int main()
{
	testcountIsIn();
	testOrder();
	cout << "Pass all tests!" << endl;
}

