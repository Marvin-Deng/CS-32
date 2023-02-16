#include <iostream>
#include <string>
#include <cassert>
using namespace std;

bool somePredicate(double x)
{
	return x == 0;
}


// Return true if the somePredicate function returns true for at
// least one of the array elements; return false otherwise.
bool anyTrue(const double a[], int n)
{
	if (n <= 0) {
		return false;
	}
	else if (somePredicate(a[0])) {
		return true;
	}
	anyTrue(a + 1, n - 1); // Shift index forward, reduce array length 
}

// Return the number of elements in the array for which the
// somePredicate function returns true.
int countTrue(const double a[], int n)
{
	if (n <= 0) {
		return 0;
	}
	int count = 0;
	if (somePredicate(a[0])) {
		count++;
	}
	return count + countTrue(a + 1, n - 1); // Add the current count to the next one, shift index forward, reduce array length 
}

// Return the subscript of the first element in the array for which
// the somePredicate function returns true.  If there is no such
// element, return -1.
int firstTrue(const double a[], int n)
{
	if (n <= 0) { // end of array reached, somePredicate was never true
		return -1;
	}
	if (somePredicate(a[0])) { // Checks first item in the array 
		return 0; 
	}

	int idx = firstTrue(a + 1, n - 1); // shift index forward, reduce array length

	if (idx == -1) { // if no predicate true, return -1;
		return -1;
	}
	else {
		return idx + 1; // increment index and continue
	}
}

// Return the subscript of the smallest element in the array (i.e.,
// return the smallest subscript m such that a[m] <= a[k] for all
// k from 0 to n-1).  If the function is told that no doubles are to
// be considered in the array, return -1.
int indexOfMinimum(const double a[], int n)
{
	if (n <= 0) {
		return -1;
	}
	if (n == 1) { // if there is only one element left in the array, then return 0 (first index)
		return 0;
	}

	int mid = n / 2; // divide array

	// index of left min
	int left = indexOfMinimum(a, mid); 

	// index of right min
	int right = indexOfMinimum(a + mid, n - mid) + mid; 
	// a + mid shifts starting pointer to index mid
	// add mid at the end so the index reflects the index in the original array

	if (a[left] <= a[right]) { // compare values of min of left and right side
		return left;
	}
	else {
		return right;
	}
}

// If all n1 elements of a1 appear in the n2 element array a2, in
	  // the same order (though not necessarily consecutively), then
	  // return true; otherwise (i.e., if the array a1 is not a
	  // not-necessarily-contiguous subsequence of a2), return false.
	  // (Of course, if a1 is empty (i.e., n1 is 0), return true.)
	  // For example, if a2 is the 7 element array
	  //    10 50 40 20 50 40 30
	  // then the function should return true if a1 is
	  //    50 20 30
	  // or
	  //    50 40 40
	  // and it should return false if a1 is
	  //    50 30 20
	  // or
	  //    10 20 20

//a1 is the shorter list
//a2 is the longer list
bool isIn(const double a1[], int n1, const double a2[], int n2)
{
	if (n1 <= 0) { // If the end of a1 is reached, then subsequence a1 is in a2
		return true;
	}
	if (n1 > n2 || n2 <= 0) { // n1 cannot be greater than n2 at any point
		return false;
	}
	if (a1[0] == a2[0]) { // If the numbers match, then shift indexes of both arrays forward
		return isIn(a1 + 1, n1 - 1, a2 + 1, n2 - 1);
	}
	else { // If the numbers don't match, then continue searching a2
		return isIn(a1, n1, a2 + 1, n2 - 1);
	}
}

void testanyTrue()
{
	const int SIZE = 3;

	double a[SIZE] = { 0, 5, 2 };

	assert(!anyTrue(a, 0));
	assert(anyTrue(a, 1));
	assert(anyTrue(a, 2));
	assert(anyTrue(a, 3));

	double b[SIZE] = { 4, 0, 2 };

	assert(!anyTrue(b, 0));
	assert(!anyTrue(b, 1));
	assert(anyTrue(b, 2));
	assert(anyTrue(b, 3));

	double c[SIZE] = { 2, 3, 0};

	assert(!anyTrue(c, 0));
	assert(!anyTrue(c, 1));
	assert(!anyTrue(c, 2));
	assert(anyTrue(c, 3));

	double d[SIZE] = { 0, 3, 0 };

	assert(!anyTrue(d, 0));
	assert(anyTrue(d, 1));
	assert(anyTrue(d, 2));
	assert(anyTrue(d, 3));

	double e[SIZE] = { 2, 2, 2 };

	assert(!anyTrue(e, 0));
	assert(!anyTrue(e, 1));
	assert(!anyTrue(e, 2));
	assert(!anyTrue(e, 3));
}

void testcountTrue()
{
	const int SIZE = 3;

	double a[SIZE] = { 0, 5, 2 };
	double b[SIZE] = { 4, 0, 2 };
	double c[SIZE] = { 2, 3, 0 };
	double d[SIZE] = { 0, 3, 0 };
	double e[SIZE] = { 2, 2, 2 };

	assert(countTrue(a, 0) == 0);
	assert(countTrue(a, 1) == 1);
	assert(countTrue(a, 2) == 1);
	assert(countTrue(a, 3) == 1);

	assert(countTrue(b, 0) == 0);
	assert(countTrue(b, 1) == 0);
	assert(countTrue(b, 2) == 1);
	assert(countTrue(b, 3) == 1);

	assert(countTrue(c, 0) == 0);
	assert(countTrue(c, 1) == 0);
	assert(countTrue(c, 2) == 0);
	assert(countTrue(c, 3) == 1);

	assert(countTrue(d, 0) == 0);
	assert(countTrue(d, 1) == 1);
	assert(countTrue(d, 2) == 1);
	assert(countTrue(d, 3) == 2);

	assert(countTrue(e, 0) == 0);
	assert(countTrue(e, 1) == 0);
	assert(countTrue(e, 2) == 0);
	assert(countTrue(e, 3) == 0);
}

void testfirstTrue()
{
	const int SIZE = 3;

	double a[SIZE] = { 0, 5, 2 };
	double b[SIZE] = { 4, 0, 2 };
	double c[SIZE] = { 2, 3, 0 };
	double d[SIZE] = { 0, 3, 0 };
	double e[SIZE] = { 2, 2, 2 };

	assert(firstTrue(a, 0) == -1);
	assert(firstTrue(a, 1) == 0);
	assert(firstTrue(a, 2) == 0);
	assert(firstTrue(a, 3) == 0);

	assert(firstTrue(b, 0) == -1);
	assert(firstTrue(b, 1) == -1);
	assert(firstTrue(b, 2) == 1);
	assert(firstTrue(b, 3) == 1);

	assert(firstTrue(c, 0) == -1);
	assert(firstTrue(c, 1) == -1);
	assert(firstTrue(c, 2) == -1);
	assert(firstTrue(c, 3) == 2);

	assert(firstTrue(d, 0) == -1);
	assert(firstTrue(d, 1) == 0);
	assert(firstTrue(d, 2) == 0);
	assert(firstTrue(d, 3) == 0);

	assert(firstTrue(e, 0) == -1);
	assert(firstTrue(e, 1) == -1);
	assert(firstTrue(e, 2) == -1);
	assert(firstTrue(e, 3) == -1);
}

void testindexOfMinimum()
{
	const int SIZE = 3;

	double a[SIZE] = { 0, 5, 2 };
	double b[SIZE] = { 4, 0, 2 };
	double c[SIZE] = { 2, 3, 0 };
	double d[SIZE] = { 0, 3, 0 };
	double e[SIZE] = { 2, 2, 2 };

	assert(indexOfMinimum(a, 0) == -1);
	assert(indexOfMinimum(a, 1) == 0);
	assert(indexOfMinimum(a, 2) == 0);
	assert(indexOfMinimum(a, 3) == 0);

	assert(indexOfMinimum(b, 0) == -1);
	assert(indexOfMinimum(b, 1) == 0);
	assert(indexOfMinimum(b, 2) == 1);
	assert(indexOfMinimum(b, 3) == 1);

	assert(indexOfMinimum(c, 0) == -1);
	assert(indexOfMinimum(c, 1) == 0);
	assert(indexOfMinimum(c, 2) == 0);
	assert(indexOfMinimum(c, 3) == 2);

	assert(indexOfMinimum(d, 0) == -1);
	assert(indexOfMinimum(d, 1) == 0);
	assert(indexOfMinimum(d, 2) == 0);
	assert(indexOfMinimum(d, 3) == 0);

	assert(indexOfMinimum(e, 0) == -1);
	assert(indexOfMinimum(e, 1) == 0);
	assert(indexOfMinimum(e, 2) == 0);
	assert(indexOfMinimum(e, 3) == 0);
}

void testisIn()
{
	const int SIZE = 7;
	const int SIZE1 = 3;
	const int SIZE2 = 0;

	double a1[SIZE] = { 1, 2, 3, 4, 2, 3, 6 };
	double a2[SIZE1] = { 2, 4, 6 };
	double a3[SIZE1] = { 2, 3, 3 };
	double a4[SIZE1] = { 2, 6, 4 };
	double a5[SIZE1] = { 1, 4, 4 };
	double a6[SIZE1] = { 0 };

	assert(isIn(a2, SIZE1, a1, SIZE));
	assert(isIn(a3, SIZE1, a1, SIZE));
	assert(isIn(a6, SIZE2, a1, SIZE));

	assert(!isIn(a4, SIZE1, a1, SIZE));
	assert(!isIn(a5, SIZE1, a1, SIZE));
}

int main() {
	testanyTrue();
	testcountTrue();
	testfirstTrue();
	testindexOfMinimum();
	testisIn();
	cout << "Pass all tests!" << endl;
}