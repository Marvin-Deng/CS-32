#include "newSet.h"
#include <iostream>

using namespace std;

Set::Set(int max) {
	if (max < 0) {
		exit;
	}
	m_length = 0;
	m_max = max;
	set = new ItemType[max];
}

Set::Set(const Set& other) {
	m_length = other.m_length;
	m_max = other.m_max;
	set = new ItemType[m_max];
	for (int i = 0; i < m_length; i++) {
		set[i] = other.set[i];
	}
}

Set& Set::operator=(const Set& rhs) {
	if (this != &rhs) {
		Set temp(rhs);
		this->swap(temp);
	}
	return *this;
}

Set::~Set() {
	delete [] set;
}

bool Set::empty() const {
	return m_length == 0;
}

int Set::size() const {
	return m_length;
}

bool Set::insert(const ItemType& value) {
	if (contains(value) || m_length >= m_max) {
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
	if (i < 0 || i >= m_length) {
		return false;
	}
	value = set[i]; // set is ordered, ith greatest element is in the correct position
	return true;
}

void Set::swap(Set& other) {
	// Switch lengths
	int tlen = other.m_length;
	other.m_length = this->m_length;
	this->m_length = tlen;

	// Switch maximum capacity
	int tmax = other.m_max;
	other.m_max = this->m_max;
	this->m_max = tmax;

	// Switch pointer
	ItemType* tpointer = other.set;
	other.set = this->set;
	this->set = tpointer;
}

