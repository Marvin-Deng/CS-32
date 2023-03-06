// Set.h

#ifndef SET_INCLUDED
#define SET_INCLUDED

template<typename ItemType>

class Set
{
public:
    Set();               // Create an empty set (i.e., one whoe size() is 0).
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

    bool get(int i, ItemType& value) const;
    // If 0 <= i < size(), copy into value the item in the set that is
    // strictly greater than exactly i items in the set and return true.
    // Otherwise, leave value unchanged and return false.

    void swap(Set& other);
    // Exchange the contents of this set with the other one.

    // Housekeeping functions
    ~Set();
    Set(const Set& other);
    Set& operator=(const Set& rhs);

private:
    // Representation:
    //   a circular doubly-linked list with a dummy node.
    //   m_head points to the dummy node.
    //   m_head->m_prev->m_next == m_head and m_head->m_next->m_prev == m_head
    //   m_size == 0  iff  m_head->m_next == m_head->m_prev == m_head
    //   If p and p->m_next point to nodes other than the dummy node,
    //      p->m_value > p->m_next->m_value

    struct Node
    {
        ItemType m_value;
        Node* m_next;
        Node* m_prev;
    };

    Node* m_head;
    int   m_size;

    void createEmpty();
    // Create an empty list.  (Will be called only by constructors.)

    void insertBefore(Node* p, const ItemType& value);
    // Insert value in a new Node before Node p, incrementing m_size.

    void doErase(Node* p);
    // Remove the Node p, decrementing m_size.

    Node* findFirstAtLeast(const ItemType& value) const;
    // Return pointer to first Node whose m_value >= value if present,
    // else m_head
};


// Declarations of non-member functions
template<typename ItemType>
void unite(const Set<ItemType>& s1, const Set<ItemType>& s2, Set<ItemType>& result);
// result = { x | (x in s1) OR (x in s2) }

template<typename ItemType>
void butNot(const Set<ItemType>& s1, const Set<ItemType>& s2, Set<ItemType>& result);
// result = { x | (x in s1) AND NOT (x in s2) }

// Inline implementations

template<typename ItemType>
inline
int Set<ItemType>::size() const
{
    return m_size;
}

template<typename ItemType>
inline
bool Set<ItemType>::empty() const
{
    return size() == 0;
}

template<typename ItemType>
inline
bool Set<ItemType>::contains(const ItemType& value) const
{
    Node* p = findFirstAtLeast(value);
    return p != m_head && p->m_value == value;
}

#endif // SET_INCLUDED