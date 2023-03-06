#include "Actor.h"
#include "StudentWorld.h"
using namespace std;

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

void Actor::setIsHit(bool hit) {
	isHit = hit;
}

bool Actor::getIsHit() const {
	return isHit;
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
			int dice = randInt(1, 10); // (1,10)
			ticks_to_move = dice * 8;
			waiting = false;
			isNew = true;
		}
		else if (action == ACTION_FIRE && hasVortex()) {
			getWorld()->addVortex(getX(), getY(), currDir);
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
			if (!onDirSquare && isFork() && !evalDirKey()) {
				return;
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
		return coinsBeforeChange;
	}
	else {
		coins += c;
		return -1 * c;
	}
}

void Player::increaseStars() {
	if (coins < 20) {
		return;
	}
	coins -= 20;
	stars++;
	getWorld()->playSound(SOUND_GIVE_STAR);
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
	if (s < 0) {
		stars = 0;
	}
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

bool Player::canBeHit() const {
	return beHit;
}

//// ACTIVATING OBJECT ////


bool ActivatingObject::onObject(Player* player) {
	int playerX = player->getX(), playerY = player->getY();
	int objectX = getX(), objectY = getY();
	bool playerWaiting = player->getWaiting();

	// Conditons for stopping on a object:
	// - Pixel coordinates of player and object must match
	// - The player must be in the waiting mode
	// - The player must be new

	if (playerX == objectX && playerY == objectY && playerWaiting && player->getIsNew()) {
		player->setIsNew(false);
		return true;
	}

	return false;
}

bool ActivatingObject::passingObject(Player* player) {
	int movingX = player->getX(), movingY = player->getY();
	int objectX = getX(), objectY = getY();

	if (movingX == objectX && movingY == objectY) {
		return true;
	}
	return false;
}

//// VORTEX ////

void Vortex::doSomething() {
	if (!isAlive()) {
		return;
	}
	moveAtAngle(currDir, 2); // move by 2 pixels
	int x = getX(), y = getY();
	if (x < 0 || x > VIEW_WIDTH || y < 0 || y > VIEW_HEIGHT) {
		setInactive();
	} 
	if (getWorld()->hitByVortex(x, y)) {
		setInactive();
		getWorld()->playSound(SOUND_HIT_BY_VORTEX);
	}
}

bool Vortex::isSquare() const {
	return false;
}

bool Vortex::canBeHit() const {
	return beHit;
}

/// ENEMY ////

void Enemy::doSomething() {

	// Enemy hit by vortex
	if (getIsHit()) {
		int randX, randY;
		getWorld()->getRandomPoint(getX(), getY(), randX, randY); // get random X and Y
		moveTo(randX, randY); // Move to random location
		currDir = right;
		changeSpriteDirection(currDir);
		paused = true;
		ticks = 180;
		setIsHit(false);
	}

	// WAITING STATE
	if (paused) {
		Player* tempPeach = getWorld()->getPeach();
		Player* tempYoshi = getWorld()->getYoshi();
		changeCoinsStars(tempPeach);
		changeCoinsStars(tempYoshi);

		pauseCounter--;
		if (pauseCounter == 0) {
			int dice = randInt(1, 10); // (1,10)
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
			int dice = randInt(1, 4); // (1, 4)
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

bool Enemy::isSquare() const {
	return false;
}

bool Enemy::canBeHit() const {
	return beHit;
}

//// BOWSER ////

void Bowser::changeCoinsStars(Player* player) {
	if (onObject(player) || enemyOnPlayer(player)) {
		int lose = randInt(1, 2);
		if (lose == 1) {
			player->replaceCoins(0); // replace all player coins
			player->replaceStars(0); // replace all player stars
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
	if (onObject(player) || enemyOnPlayer(player)) {
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

bool Square::canBeHit() const {
	return beHit;
}

//// COIN SQUARE ////

void CoinSquare::processAction(Player* player) {
	if (onObject(player)) {
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
	if (onObject(player) || (passingObject(player) && !player->getWaiting())) {
		player->increaseStars(); // Increment by one star
	}
}

//// DIRECTION SQUARE ////

void DirectionSquare::processAction(Player* player) {
	if (onObject(player) || passingObject(player)) {
		player->changeMoveDir(dir);
		player->setOnDirSquare(true);
	}
}

//// BANK SQUARE ////

void BankSquare::processAction(Player* player) {

	// Player is stopped on a bank square
	if (onObject(player)) {
		player->updateCoins(getWorld()->getBankCoins());
		getWorld()->resetBankCoins();
	}
	// Player is passing over bank square
	else if (passingObject(player) && !player->getWaiting()) {
		int coinsTaken = player->updateCoins(getWorld()->getCoinsTaken());
		getWorld()->depositBankCoins(coinsTaken);
	}
}

//// EVENT SQUARE ////

void EventSquare::processAction(Player* player) {
	if (!onObject(player)) {
		return;
	}
	int event = randInt(1, 3); // (1, 3)

	if (event == 1) { // Teleport
		player->playerTeleport();
		getWorld()->playSound(SOUND_PLAYER_TELEPORT);
	}
	else if (event == 2) { // Position Swap
		getWorld()->playSound(SOUND_PLAYER_TELEPORT);
		getWorld()->swapPlayers(player);
	}
	else if (event == 3) { // Give Vortex
		player->giveVortex();
		getWorld()->playSound(SOUND_GIVE_VORTEX);
	}
}

//// DROPPING SQUARE ////                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      

void DroppingSquare::processAction(Player* player) {
	if (onObject(player)) {
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