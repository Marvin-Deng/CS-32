#include "Set.h"
#include <iostream>

using namespace std;

Set::Set() {
	m_length = 0;
}

bool Set::empty() const {
	return m_length == 0;
}

int Set::size() const {
	return m_length;
}

bool Set::insert(const ItemType& value) {
	if (contains(value) || m_length >= DEFAULT_MAX_ITEMS) {
		return false;
	}
	// insert in order
	for (int i = 0; i < m_length; i++) {
		if (set[i] > value) {
			for (int j = m_length; j > i; j--) {
				set[j] = set[j - 1];
			}
			set[i] = value;
			m_length++;
			return true;
		}
	}
	// if the value is larger than all other elements
	set[m_length] = value;
	m_length++;
	return true;
}

bool Set::erase(const ItemType& value) {
	for (int i = 0; i < m_length; i++) {
		if (set[i] == value) {
			for (int j = i; j < m_length - 1; j++) {
				set[j] = set[j + 1];
			}
			m_length--;
			return true;
		}
	}
	return false;
}

bool Set::contains(const ItemType& value) const {
	for (int i = 0; i < m_length; i++) {
		if (set[i] == value) {
			return true;
		}
	}
	return false;
}

bool Set::get(int i, ItemType& value) const {
	if (i < 0 || i >= size()) {
		return false;
	}
	value = set[i]; // set is ordered, ith greatest element is in the correct position
	return true;
}

void Set::swap(Set& other) {
	// find greatest length between sets
	int len = size() > other.size() ? size() : other.size();

	// Swap the contents of sets
	for (int i = 0; i < len; i++) {
		ItemType temp = other.set[i];
		other.set[i] = set[i];
		set[i] = temp;
	}

	// swap lengths
	int temp = other.m_length;
	other.m_length = m_length;
	m_length = temp;
}

void Set::dump() const {
	for (int i = 0; i < size(); i++) {
		cerr << set[i] << ", ";
	}
	cerr << endl;
}