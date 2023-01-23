#ifndef NEWSET_H
#define NEWSET_H

#include <string>
using ItemType = std::string;
//using ItemType = unsigned long;
const int DEFAULT_MAX_ITEMS = 160;

class Set
{
public:
    Set(int max = DEFAULT_MAX_ITEMS);

    Set(const Set& other); // copy constructor

    Set& operator=(const Set& rhs);

    ~Set();

    bool empty() const;

    int size() const; 

    bool insert(const ItemType& value);

    bool erase(const ItemType& value);

    bool contains(const ItemType& value) const;

    bool get(int i, ItemType& value) const;

    void swap(Set& other);

private:
    ItemType *set;
    int m_max;
    int m_length;
};

#endif