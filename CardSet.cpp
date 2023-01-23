#include "Set.h"
#include "CardSet.h"
#include <iostream>
using namespace std;

CardSet::CardSet() {
}

bool CardSet::add(unsigned long cardNumber) {
	return cards.insert(cardNumber);
}

int CardSet::size() const {
	return cards.size();
}

void CardSet::print() const{
	for (int i = 0; i < cards.size(); i++) {
		ItemType item;
		cards.get(i, item);
		cout << item << endl;
	}
}