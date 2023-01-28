#include "Set.h"
#include<iostream>

using namespace std;

Set::Set() {
	head = new Node; // initalize head as a dummy node pointing to itself

	// make the dummy node point to itself
	head->next = head;
	head->prev = head;
	m_len = 0;
}

Set::Set(const Set& other) {
	// copy length
	m_len = other.m_len; 

	// create dummy pointer of circular doubly linked list
	head = new Node;
	head->next = head;
	head->prev = head;

	Node* curr = head->next; // set iterator pointer to start of linked list (currently pointing to head)
	Node* op = other.head->next; // set iterator pointer to start of other 

	// While the op has not reached its starting position
	while (op != other.head) {
		Node* newP = new Node; // create new Node to add to the list
		newP->val = op->val; // copy op's value to the value of the new node

		// Link the nodes
		curr->next = newP; // set the next node after curr to the new node
		newP->prev = curr; // set curr to the previous of the new node

		// Complete the circle
		newP->next = head; // set the node after the new node to dummy
		head->prev = newP; // set the new pointer to the previous of dummy
		
		// shift pointers forward
		op = op->next;
		curr = curr->next;
	}
	
}

Set& Set::operator=(const Set& rhs) {
	if (this != &rhs) {
		Set temp(rhs); // Create a temporary Set to store the values of rhs
		swap(temp); //Swap lhs and rhs
	}
	return *this;
}

Set::~Set() {
	// Delete the linked list
	while (!empty()) {
		Node* toBeDeleted = head->next; // stores node that shoud be deleted
		head->next = toBeDeleted->next; // shifts pointer to node after the deleted node
		toBeDeleted->next->prev = head; // sets the prev of the node after the deleted node to head 

		// set next and prev to nullptr
		toBeDeleted->next = nullptr;
		toBeDeleted->prev = nullptr;

		delete toBeDeleted;
		m_len--;
	}
	delete head; // delete dummy node
}

bool Set::empty() const {
	return m_len == 0;
}

int Set::size() const {
	return m_len;
}

bool Set::insert(const ItemType& value) {
	if (contains(value)) {
		return false; 
	}
	Node* curr = head->prev; // set curr to last node in the list
	Node* newP = new Node; // create node to be inserted
	newP->val = value;

	while (curr != head) { 
		if (value < curr->val) { // find position to insert new node in order
			break;
		}
		curr = curr->next; // curr points to the node after the insert position
	}

	// link curr's previous node to new node
	(curr->prev)->next = newP;
	newP->prev = curr->prev;

	// link curr and newP
	curr->prev = newP;
	newP->next = curr;

	m_len++;
	return true;
}

bool Set::erase(const ItemType& value) {
	if (!contains(value)) {
		return false;
	}
	Node* curr = head->next;
	while (curr != head) {
		if (curr->val == value) {
			Node* toBeDeleted = curr; // stores node that shoud be deleted
			(curr->prev)->next = curr->next; // the previous node points to the node after the deleted node
			(curr->next)->prev = curr->prev; // the next node points to the node before the deleted node

			// Set the deleted node's pointers to nullptr
			toBeDeleted->next = nullptr;
			toBeDeleted->prev = nullptr;

			delete curr;
			m_len--;
			return true;
		}
	}
	return true;
}

bool Set::contains(const ItemType& value) const {
	Node* p = head->next; // set iterator to node value of the list
	while (p != head) { // continue traversing the list until the iterator reaches the dummy node
		if (p->val == value) { // compare values
			return true;
		}
		p = p->next;
	}
	return false;
}

// use double for loop to find pos greatest value
bool Set::get(int pos, ItemType& value) const {
	if (pos < 0 || pos >= m_len) {
		return false;
	}
	Node* p = head->next; // set iterator to first node in the list
	int i = 0;
	while (p != head) {
		if (i == pos) {
			value = p->val;
			break;
		}
		i++;
		p = p->next;
	}
	return true;
}

void Set::swap(Set& other) {
	// swap lengths
	int tempLen = other.m_len;
	other.m_len = m_len;
	m_len = tempLen;

	// swap pointers
	Node* temp = other.head;
	other.head = head;
	head = temp;
}

void Set::dump() const
{
	cerr << "The size of the Set is: " << m_len << endl;
	cerr << "The items in the Set are: " << endl;

	Node* curr = head->next;
	while (curr != head) {
		cerr << curr->val << " ";
		curr = curr->next;
	}
	cerr << endl;
}

void unite(const Set& s1, const Set& s2, Set& result) {

}

void butNot(const Set& s1, const Set& s2, Set& result) {

}

