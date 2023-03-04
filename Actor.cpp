#include "Actor.h"
#include "StudentWorld.h"
#include <iostream>
using namespace std;

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
			int die_roll = randInt(1, 1); // chnage to (1,10)
			ticks_to_move = die_roll * 8;
			waiting = false;
			isNew = true;
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

bool Player::isValidPos() const {
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

// Used for coin and bank squares
// Bank Square: Returns number of coins taken from player
int Player::updateCoins(int c) {
	int coinsBeforeChange = coins;

	if (coins + c < 0) {
		coins = 0;
		std::cout << coins << "  ";
		return coinsBeforeChange;
	}
	else {
		coins += c;
		std::cout << coins << "  ";
		return -1 * c;
	}
}

void Player::updateStars() {
	if (coins < 20) {
		return;
	}
	coins -= 20;
	stars++;
	getWorld()->playSound(SOUND_GIVE_STAR);
	std::cout << stars << "  ";
}

void Player::changeMoveDir(int dir) {
	currDir = dir;
	changeSpriteDirection();
}

void Player::playerTeleport() {
	getWorld()->playSound(SOUND_PLAYER_TELEPORT);
}

void Player::giveVortex() {
	getWorld()->playSound(SOUND_GIVE_VORTEX); // play sound only if vortex given?
	if (vortex == 0) {
		vortex++;
	}
}

int Player::getPlayerCoins() const {
	return coins;
}

int Player::getPlayerStars() const {
	return stars;
}

bool Player::getWaiting() const {
	return waiting;
}

bool Player::getIsNew() const {
	return isNew;
}

void Player::resetIsNew() {
	isNew = false;
}

//// ACTIVATING OBJECT ////

bool ActivatingObject::onObject(Player* player, int onOrPassing) {
	int playerX = player->getX(), playerY = player->getY();
	int objectX = getX(), objectY = getY();
	bool waiting = player->getWaiting();

	// Conditons for stopping on a object:
	// - Pixel coordinates of player and object must match
	// - The player must be in the waiting mode
	// - The player must be new

	if (onOrPassing == 1 && playerX == objectX && playerY == objectY && waiting && player->getIsNew()) {
		player->resetIsNew();
		return true;
	}

	// Conditons for passing an object:
	// - Pixel coordinates of player and object must match
	if (onOrPassing == 2 && playerX == objectX && playerY == objectY && !waiting) {
		player->resetIsNew();
		return true;
	}

	return false;
}

bool ActivatingObject::passingObject(Actor* player) {
	int movingX = player->getX(), movingY = player->getY();
	int objectX = getX(), objectY = getY();

	

	if (movingX == objectX && movingY == objectY) {
		return true;
	}
	return false;
}



//template<typename func>
//void ActivatingObject::processAction(func action) {
//	Player* tempPeach = getWorld()->getPeach();
//	Player* tempYoshi = getWorld()->getYoshi();
//	action(tempPeach);
//	action(tempYoshi);
//}


//// COIN SQUARE ////

void CoinSquare::doSomething() {
	if (!isAlive()) {
		return;
	}

	Player* tempPeach = getWorld()->getPeach();
	Player* tempYoshi = getWorld()->getYoshi();
	processCoinSquare(tempPeach);
	processCoinSquare(tempYoshi);
}

void CoinSquare::processCoinSquare(Player* player) {
	if (onObject(player, 1)) {
		player->updateCoins(coinsGiven);
		coinSound();
	}
}


void CoinSquare::coinSound() const {
	if (coinsGiven > 0) {
		getWorld()->playSound(SOUND_GIVE_COIN);
	}
	else {
		getWorld()->playSound(SOUND_TAKE_COIN);
	}
}

//// STAR SQUARE ////

void StarSquare::doSomething() {
	Player* tempPeach = getWorld()->getPeach();
	Player* tempYoshi = getWorld()->getYoshi();
	processStarSquare(tempPeach);
	processStarSquare(tempYoshi);
}

void StarSquare::processStarSquare(Player* player) {
	if (onObject(player, 1) || onObject(player, 2)) {
		player->updateStars();
	}
}

//// DIRECTION SQUARE ////

void DirectionSquare::doSomething() {
	Player* tempPeach = getWorld()->getPeach();
	Player* tempYoshi = getWorld()->getYoshi();
	processDirectionSquare(tempPeach);
	processDirectionSquare(tempYoshi);
}

void DirectionSquare::processDirectionSquare(Player* player) {
	if (onObject(player, 1) || onObject(player, 2)) {
		player->changeMoveDir(dir);
	}
}

//// BANK SQUARE ////

void BankSquare::doSomething() {
	Player* tempPeach = getWorld()->getPeach();
	Player* tempYoshi = getWorld()->getYoshi();
	processBankSquare(tempPeach);
	processBankSquare(tempYoshi);
}

void BankSquare::processBankSquare(Player* player) {

	// Player is stopped on a bank square
	if (onObject(player, 1)) {
		player->updateCoins(getWorld()->getBankCoins());
		getWorld()->resetBankCoins();
		cout << "Stopped on bank square" << endl;
	} 
	// Player is passing over bank square
	else if (onObject(player, 2)) {
		int coinsTaken = player->updateCoins(getWorld()->getCoinsTaken());
		getWorld()->depositBankCoins(coinsTaken);
		cout << "Passing on bank square" << endl;
	}
}

//// EVENT SQUARE ////

void EventSquare::doSomething() {
	Player* tempPeach = getWorld()->getPeach();
	Player* tempYoshi = getWorld()->getYoshi();
	processEventSquare(tempPeach, tempYoshi);
	processEventSquare(tempYoshi, tempPeach);
}

void EventSquare::processEventSquare(Player* player1, Player* player2) {
	if (!onObject(player1, 1)) {
		return;
	}
	int event = randInt(1, 3);

	if (event == 1) { // Teleport
		player1->playerTeleport();
	}
	else if (event == 2) { // Position Swap

	}
	else { // Give Vortex
		player1->giveVortex();
	}
}

//// DROPPING SQUARE ////

void DroppingSquare::doSomething() {

}

