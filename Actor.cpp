#include "Actor.h"
#include "StudentWorld.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>
using namespace std;

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//// ACTOR /////
int Actor::validMoveDirection(int currDir) const {
	int newX, newY;

	if (currDir == right || currDir == left) {
		getPositionInThisDirection(up, SPRITE_WIDTH, newX, newY);
		if (getWorld()->isValidPos(newX, newY)) {
			return up;
		}
		else {
			return down;
		}
	}
	else {
		getPositionInThisDirection(right, SPRITE_WIDTH, newX, newY);
		if (getWorld()->isValidPos(newX, newY)) {
			return right;
		}
		else {
			return left;
		}
	}
}

void Actor::changeSpriteDirection(int moveDir) {
	if (moveDir == 180) {
		setDirection(180);
	}
	else {
		setDirection(0);
	}
}

int Actor::getRandValidDir() const {
	int dirs[4] = { 0, 180, 90, 270 }; // right, left, up, down
	int newX, newY;
	bool valid = false;
	while (!valid) {
		int randDir = dirs[randInt(0, 3)];
		getPositionInThisDirection(randDir, SPRITE_WIDTH, newX, newY);
		if (getWorld()->isValidPos(newX, newY)) {
			valid = true;
			return randDir;
		}
	}
	return 0;
}

// If number of available paths > 2, Actor is at a fork
bool Actor::isFork() const {
	int dirs[4] = { right, left, up, down };
	int ret = 0;
	for (int dir : dirs) {
		int newX, newY;
		getPositionInThisDirection(dir, SPRITE_WIDTH, newX, newY);
		if (getWorld()->isValidPos(newX, newY)) {
			ret++;
			//cout << dir << " ";
		}
	}
	return ret > 2;
}


StudentWorld* Actor::getWorld() const {
	return m_world;
}

bool Actor::isAlive() const {
	return alive;
}

void Actor::setInactive() {
	alive = false;
}
//// PLAYER ////

void Player::doSomething() {

	// WAITING STATE
	int newX, newY;
	getPositionInThisDirection(currDir, SPRITE_WIDTH, newX, newY);

	if (waiting)
	{
		// Check if player is facing in the correct direction after teleporting
		if (isTeleported) {
			if (!getWorld()->isValidPos(newX, newY)) {
				currDir = validMoveDirection(currDir);
				changeSpriteDirection(currDir);
			}
			isTeleported = false;
		}

		// Evaluate action key
		int action = getWorld()->getAction(pNum);
		if (action == ACTION_ROLL) {
			dice = randInt(1,1); // chnage to (1,10)
			ticks_to_move = dice * 8;
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

		if (getX() % SPRITE_WIDTH == 0 && getY() % SPRITE_HEIGHT == 0) {
			if (!onDirSquare && isFork()) {
				//cout << "Is fork" << endl;
				//cout << "Curr: " << currDir << " ";
				if (!evalDirKey()) {
					return;
				}
			}

			// Check if Avatar can continue walking forward, change the movement direction if not valid
			else if (!getWorld()->isValidPos(newX, newY)) {
				currDir = validMoveDirection(currDir); 
				changeSpriteDirection(currDir);
			}
		}

		moveAtAngle(currDir, 2); // Move 2 pixels in the current direction
		ticks_to_move--;
		if (ticks_to_move == 0) {
			waiting = true;
		}
		onDirSquare = false;
	}
}

// Returns if valid key is pressed
bool Player::evalDirKey() {
	int newX, newY;
	int action = getWorld()->getAction(pNum);

	if (action == ACTION_RIGHT) {
		getPositionInThisDirection(right, SPRITE_WIDTH, newX, newY);
		if (getWorld()->isValidPos(newX, newY) && currDir != left) {
			changeMoveDir(right);
			return true;
		}
	}
	else if (action == ACTION_LEFT) {
		getPositionInThisDirection(left, SPRITE_WIDTH, newX, newY);
		if (getWorld()->isValidPos(newX, newY) && currDir != right) {
			changeMoveDir(left);
			return true;
		}
	}
	else if (action == ACTION_UP) {
		getPositionInThisDirection(up, SPRITE_WIDTH, newX, newY);
		if (getWorld()->isValidPos(newX, newY) && currDir != down) {
			changeMoveDir(up);
			return true;
		}
	}
	else if (action == ACTION_DOWN) {
		getPositionInThisDirection(down, SPRITE_WIDTH, newX, newY);
		if (getWorld()->isValidPos(newX, newY) && currDir != up) {
			changeMoveDir(down);
			return true;
		}
	}
	return false;
}

// Used for coin and bank squares 
// Bank Square: Returns number of coins taken from player
int Player::updateCoins(int c) {
	int coinsBeforeChange = coins;

	if (coins + c < 0) {
		coins = 0;
		// std::cout << coins << "  ";
		return coinsBeforeChange;
	}
	else {
		coins += c;
		// std::cout << coins << "  ";
		return -1 * c;
	}
}

void Player::increaseStars() {
	if (coins < 20) {
		return;
	}
	coins -= 20;
	stars ++;
	// std::cout << stars << "  ";
}

void Player::deductStar() {
	if (stars != 0) {
		stars--;
	}
}

void Player::playerTeleport() {
	int x = 0, y = 0;
	getWorld()->getRandomPoint(getX(), getY(), x, y);
	moveTo(x, y);
	isNew = true;
}

void Player::giveVortex() {
	if (vortex == 0) {
		vortex++;
	}
}

int Player::getPlayerCoins() const {
	return coins;
}

void Player::replaceCoins(int c) {
	coins = c;
}

int Player::getPlayerStars() const {
	return stars;
}

void Player::replaceStars(int s) {
	stars = s;
}

bool Player::getWaiting() const {
	return waiting;
}

void Player::setWaiting(bool wait) {
	waiting = wait;
}

int Player::getTicks() const {
	return ticks_to_move;
}

void Player::setTicks(int ticks) {
	ticks_to_move = ticks;
}

bool Player::getIsNew() const {
	return isNew;
}

void Player::setIsNew(bool n) {
	isNew = n;
}

int Player::getMoveDir() const {
	return currDir;
}

void Player::changeMoveDir(int dir) {
	currDir = dir;
	changeSpriteDirection(currDir);
}

void Player::setOnDirSquare(bool isOn) {
	onDirSquare = isOn;
}

bool Player::hasVortex() const {
	return vortex == 1;
}

bool Player::isSquare() const {
	return false;
}

//// ACTIVATING OBJECT ////

// 1: Player stopped on object
// 2: Player passes over object
bool ActivatingObject::onObject(Player* player, int onOrPassing) {
	int playerX = player->getX(), playerY = player->getY();
	int objectX = getX(), objectY = getY();
	bool playerWaiting = player->getWaiting();

	// Conditons for stopping on a object:
	// - Pixel coordinates of player and object must match
	// - The player must be in the waiting mode
	// - The player must be new

	if (onOrPassing == 1 && playerX == objectX && playerY == objectY && playerWaiting && player->getIsNew()) {
		player->setIsNew(false);
		return true;
	}

	// Conditons for passing an object:
	// - Pixel coordinates of player and object must match
	// - Player is walking
	if (onOrPassing == 2 && playerX == objectX && playerY == objectY && !playerWaiting) {
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

/// ENEMY ////

void Enemy::doSomething() {

	// WAITING STATE
	if (paused) {
		Player* tempPeach = getWorld()->getPeach();
		Player* tempYoshi = getWorld()->getYoshi();
		changeCoinsStars(tempPeach);
		changeCoinsStars(tempYoshi);

		pauseCounter--;
		if (pauseCounter == 0) {
			int dice = randInt(1, 1); // change to (1,10)
			ticks = dice * 8;
			currDir = getRandValidDir();
			changeSpriteDirection(currDir);
			paused = false;
			enemyIsNew = true;
		}
	}

	// WALLKING STATE
	int newX, newY;
	getPositionInThisDirection(currDir, SPRITE_WIDTH, newX, newY);

	if (!paused) {

		if (getX() % SPRITE_WIDTH == 0 && getY() % SPRITE_HEIGHT == 0) {
			if (isFork()) {
				currDir = getRandValidDir();
				changeSpriteDirection(currDir);
			}
			else if (!getWorld()->isValidPos(newX, newY)) {
				currDir = validMoveDirection(currDir);
				changeSpriteDirection(currDir);
			}
		} 

		moveAtAngle(currDir, 2); 
		ticks--;
		if (ticks == 0) {
			paused = true;
			pauseCounter = 180; // 180
			int dice = randInt(1, 1); // (1, 4)
			if (dice == 1) {
				leaveDropping();
			}
		}
	}
}

bool Enemy::enemyOnPlayer(Player* player) {
	int playerX = player->getX(), playerY = player->getY();
	int enemyX = getX(), enemyY = getY();
	bool playerWaiting = player->getWaiting();

	// Conditions for enemy stopping on player
	// - Player and enemy coordinates match
	// - Player is waiting
	// - Enemy hasn't activated on player in the current move

	if (playerX == enemyX && playerY == enemyY && playerWaiting && enemyIsNew) {
		enemyIsNew = false;
		return true;
	}
	return false;
}

bool Enemy::isSquare() const {
	return false;
}

bool Enemy::isPaused() const {
	return paused;
}

void Enemy::setPaused(bool isPaused) {
	paused = isPaused;
}

int Enemy::getPauseCounter() const {
	return pauseCounter;
}
void Enemy::setPauseCounter(int count) {
	pauseCounter = count;
}

//// VORTEX ////

//// BOWSER ////


void Bowser::changeCoinsStars(Player* player) {
	if (onObject(player, 1) && enemyOnPlayer(player)) {
		int lose = randInt(1, 2);
		if (lose == 1) {
			player->replaceCoins(0); // replace all player coins
			player->replaceCoins(0); // replace all player stars
			getWorld()->playSound(SOUND_BOWSER_ACTIVATE);
		}
	}
}

void Bowser::leaveDropping() {
	getWorld()->addDroppingSquare(getX(), getY());
	getWorld()->playSound(SOUND_DROPPING_SQUARE_CREATED);
}

//// BOO ////

void Boo::changeCoinsStars(Player* player) {
	if (onObject(player, 1) && enemyOnPlayer(player)) {
		getWorld()->swapCoinsStars();
		getWorld()->playSound(SOUND_BOO_ACTIVATE);
	}
}

void Boo::leaveDropping() {
	// Leave empty
}


//// SQUARE ////

void Square::doSomething() {
	Player* tempPeach = getWorld()->getPeach();
	Player* tempYoshi = getWorld()->getYoshi();
	processAction(tempPeach);
	processAction(tempYoshi);
}

bool Square::isSquare() const {
	return true;
}


//// COIN SQUARE ////

void CoinSquare::processAction(Player* player) {
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

void StarSquare::processAction(Player* player) {
	if (onObject(player, 1) || onObject(player, 2)) {
		player->increaseStars(); // Increment by one star
		getWorld()->playSound(SOUND_GIVE_STAR);
	}
}

//// DIRECTION SQUARE ////

void DirectionSquare::processAction(Player* player) {
	if (onObject(player, 1) || onObject(player, 2)) {
		player->changeMoveDir(dir);
		player->setOnDirSquare(true);
	}
}

//// BANK SQUARE ////

void BankSquare::processAction(Player* player) {

	// Player is stopped on a bank square
	if (onObject(player, 1)) {
		player->updateCoins(getWorld()->getBankCoins());
		getWorld()->resetBankCoins();
		//cout << "Stopped on bank square" << endl;
	} 
	// Player is passing over bank square
	else if (onObject(player, 2)) {
		int coinsTaken = player->updateCoins(getWorld()->getCoinsTaken());
		getWorld()->depositBankCoins(coinsTaken);
		//cout << "Passing on bank square" << endl;
	}
}

//// EVENT SQUARE ////

void EventSquare::processAction(Player* player) {
	if (!onObject(player, 1)) {
		return;
	}
	int event = randInt(1, 3); // chnage to (1, 3)

	if (event == 1) { // Teleport
		player->playerTeleport();
		getWorld()->playSound(SOUND_PLAYER_TELEPORT);
	}
	else if (event == 2) { // Position Swap
		getWorld()->swapPlayers();
		player->setIsNew(true); // allow the player to activate the coin it gets swapped to
		getWorld()->playSound(SOUND_PLAYER_TELEPORT);
	}
	else if (event == 3) { // Give Vortex
		player->giveVortex();
		getWorld()->playSound(SOUND_GIVE_VORTEX);
	}
}

//// DROPPING SQUARE ////                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      w

void DroppingSquare::processAction(Player* player) {
	if (onObject(player, 1)) {
		getWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
		int dice = randInt(1, 2); // (1, 2)

		if (dice == 1) { // Take 10 coins
			player->updateCoins(-10);
		}
		else if (dice == 2) { // Take 1 star
			player->deductStar();
		}
	}

}
