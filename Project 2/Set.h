#ifndef SET_H
#define SET_H

#include <string>

using ItemType = std::string;
//using ItemType = unsigned long;

class Set
{
public:
    Set();         // Create an empty set.

    Set(const Set& rhs); // Copy constructor

    Set& operator=(const Set& other); // Assignment operator

    ~Set(); // Destructor

    bool empty() const;  // Return true if the set is empty, otherwise false.

    int size() const;    // Return the number of items in the set.

    bool insert(const ItemType& value);
    // Insert value into the set if it is not already present.  Return
    // true if the value is actually inserted.  Leave the set unchanged
    // and return false if value is not inserted (perhaps because it
    // was already in the set or because the set has a fixed capacity and
    // is full).

    bool erase(const ItemType& value);
    // Remove the value from the set if it is present.  Return true if the
    // value was removed; otherwise, leave the set unchanged and
    // return false.

    bool contains(const ItemType& value) const;
    // Return true if the value is in the set, otherwise false.

    bool get(int pos, ItemType& value) const;
    // If 0 <= pos < size(), copy into value the item in the set that is
    // strictly greater than exactly pos items in the set and return true.
    // Otherwise, leave value unchanged and return false.

    void swap(Set& other);
    // Exchange the contents of this set with the other one.

    void dump() const;
    // debugging function (prints linked list)


private:
    struct Node {
        ItemType val;
        Node* next;
        Node* prev;
    };
    Node* head;
    int m_len;
};

void unite(const Set& s1, const Set& s2, Set& result);
//When this function returns, result must contain one copy of each 
//of the values that appear in s1 or s2 or both, and must not contain 
//any other values. Since result is a Set, it must, of course, not have 
//any duplicates

void butNot(const Set& s1, const Set& s2, Set& result);
//When this function returns, result must contain one copy of each of the 
//values that appear in s1 but not s2, and must not contain any other values.

#endif