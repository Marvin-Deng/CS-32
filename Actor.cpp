#include "Actor.h"
#include "StudentWorld.h"
#include <iostream>

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//// ACTOR /////

StudentWorld* Actor::getWorld() const {
	return m_world;
}

bool Actor::isAlive() const {
	return alive;
}

//// PLAYER ////

void Player::doSomething() {

	// WAITING STATE

	if (waiting)
	{
		// Check if player is facing in the correct decision after teleporting
		/*while (!isValidPos()) {
			updateValidMoveDirection();
		}
		changeSpriteDirection();*/

		// Evaluate action key
		int action = getWorld()->getAction(pNum);
		if (action == ACTION_ROLL) {
			int die_roll = randInt(1, 10);
			ticks_to_move = die_roll * 8;
			waiting = false;
		}
		else if (action == ACTION_FIRE) {
			
			getWorld()->playSound(SOUND_PLAYER_FIRE);
			vortex--;
		}
		else {
			return;
		}
	}

	// WALKING STATE 

	if (!waiting) {

		// If new position isn't valid, change walking direction
		if (getX() % SPRITE_WIDTH == 0 && getY() % SPRITE_HEIGHT == 0 && !isValidPos()) { // Only check if the next posiiton is valid at the center of a square
			
			// Change the movement direction
			updateValidMoveDirection();

			// Change sprite direction
			changeSpriteDirection();
		}
		moveAtAngle(currDir, 2); // Move 2 pixels in the current direction
		ticks_to_move--;
		if (ticks_to_move == 0) {
			waiting = true;
		}
	}
}

bool Player::isValidPos() const{
	int newX, newY;
	getPositionInThisDirection(currDir, SPRITE_WIDTH, newX, newY);

	Board::GridEntry ge = getWorld()->getBoard()->getContentsOf(newX / SPRITE_WIDTH, newY / SPRITE_HEIGHT);

	switch (ge) {

	case Board::empty:
		return false;
	default:
		return true;
	}
}

void Player::updateValidMoveDirection() {
	if (currDir == right || currDir == left) {
		currDir = up;
		if (!isValidPos()) {
			currDir = down;
		}
	}
	else {
		currDir = right;
		if (!isValidPos()) {
			currDir = left;
		}
	}
}

void Player::changeSpriteDirection() {
	if (currDir == left) {
		setDirection(left);
	}
	else {
		setDirection(right);
	}
}

void Player::updateCoins(int c) {
	if (c > 0) {
		getWorld()->playSound(SOUND_GIVE_COIN);
	}
	else {
		getWorld()->playSound(SOUND_TAKE_COIN);
	}
	if (coins + c < 0) {
		coins = 0;
	}
	else {
		coins += c;
	}
	std::cout << coins << "  ";
}

bool Player::getWaiting() const {
	return waiting;
}

int Player::getPlayerCoins() const {
	return coins;
}

//// ACTIVATING OBJECT ////

bool ActivatingObject::onSquare(Player* player) {
	int playerX = player->getX(), playerY = player->getY();
	int objectX = getX(), objectY = getY();
	bool waiting = player->getWaiting();
	
	// Conditons for acitvating square:
	// - Pixel coordinates of player and square must match
	// - The player must be in the waiting mode
	// - The square must be active

	if (playerX == objectX && playerY == objectY && waiting && active) {
		active = false; // Sets the square to inactive
		return true;
	} 
	if (!waiting) {
		active = true;
	}
	return false;
}   

void ActivatingObject::resetActive() {
	active = true;
}

//// COIN SQUARE ////

void CoinSquare::doSomething() {
	if (!isAlive()) {
		return;
	}

	// Determine if Peach or Yoshi are on the square
	if (onSquare(getWorld()->getPeach())) {
		getWorld()->getPeach()->updateCoins(coinsGiven);
	}

	if (onSquare(getWorld()->getYoshi())) {
		getWorld()->getYoshi()->updateCoins(coinsGiven);
	}
}

//// STAR SQUARE ////

void StarSquare::doSomething() {

}

//// DIRECTION SQUARE ////

void DirectionSquare::doSomething() {

}

//// BANK SQUARE ////

void BankSquare::doSomething() {

}

//// EVENT SQUARE ////

void EventSquare::doSomething() {

}

//// DROPPING SQUARE ////

void DroppingSquare::doSomething() {

}
