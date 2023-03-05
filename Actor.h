#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <string>

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

class Actor : public GraphObject
{
public:
	Actor(StudentWorld* world, int imageID, int startX, int startY, int direction = right, int depth = 0)
		: GraphObject(imageID, startX, startY, direction, depth), m_world(world), alive(true)
	{}

	virtual void doSomething() = 0;

	int validMoveDirection(int currDir) const;

	void changeSpriteDirection(int moveDir);

	int getRandValidDir() const;

	bool isFork() const;

	bool isAlive() const;

	void setInactive();

	virtual bool isSquare() const = 0;

	StudentWorld* getWorld() const;

private:
	StudentWorld* m_world;
	bool alive;
};

class Player : public Actor
{
public:
	Player(StudentWorld* world, int imageID, int startX, int startY, int playerNum)
		: Actor(world, imageID, startX, startY), ticks_to_move(0), currDir(right),
		waiting(true), dice(0), pNum(playerNum), vortex(0), coins(0), stars(0), isNew(true),
		onDirSquare(false), isTeleported(false)
	{}

	virtual void doSomething();

	// Event square functions
	void playerTeleport();
	void giveVortex();

	// Coins 
	int updateCoins(int c);
	int getPlayerCoins() const;
	void replaceCoins(int c);

	// Stars
	void increaseStars();
	void deductStar();
	int getPlayerStars() const;
	void replaceStars(int s);

	// Waiting
	bool getWaiting() const;
	void setWaiting(bool wait);

	// Ticks
	int getTicks() const;
	void setTicks(int ticks);

	// IsNew
	bool getIsNew() const;
	void setIsNew(bool n);

	// Move direction
	int getMoveDir() const;
	void changeMoveDir(int dir); // also changes sprite direction

	void setOnDirSquare(bool isOn);
	bool hasVortex() const;
	virtual bool isSquare() const;

private:
	bool evalDirKey();
	int dice;
	int pNum;
	int ticks_to_move;
	int currDir;
	bool waiting;
	int vortex;
	int coins;
	int stars;
	bool isNew;
	bool onDirSquare;
	bool isTeleported;
};


class ActivatingObject : public Actor {

public:
	ActivatingObject(StudentWorld* world, int imageID, int startX, int startY, int depth, int direction = right)
		: Actor(world, imageID, startX, startY, direction, depth)
	{}

	bool onObject(Player* player, int onOrPassing);
	bool passingObject(Actor* player);
	
	virtual bool isSquare() const = 0;

private:

};

class Vortex : public ActivatingObject
{
public:
	Vortex(StudentWorld* world, int imageID, int startX, int startY, int direction)
		: ActivatingObject(world, imageID, startX, startY, direction)
	{}

	virtual bool isSquare() const;

private:

};

class Enemy : public ActivatingObject {

public:
	Enemy(StudentWorld* world, int imageID, int startX, int startY, int depth = 0)
		: ActivatingObject(world, imageID, startX, startY, depth), paused(true), pauseCounter(180),
		ticks(0), currDir(right), enemyIsNew(true)
	{}

	virtual void doSomething();

	virtual void changeCoinsStars(Player* player) = 0;

	virtual void leaveDropping() = 0;

	virtual bool isSquare() const;

	bool enemyOnPlayer(Player* player);

	// Paused
	bool isPaused() const;
	void setPaused(bool isPaused);

	// Paused Counter
	int getPauseCounter() const;
	void setPauseCounter(int count); 

private:
	bool paused;
	int pauseCounter;
	int ticks;
	int currDir;
	bool enemyIsNew;
};


class Bowser : public Enemy
{
public:
	Bowser(StudentWorld* world, int imageID, int startX, int startY)
		: Enemy(world, imageID, startX, startY)
	{}

	virtual void changeCoinsStars(Player* player);

	virtual void leaveDropping();

private:

};

class Boo : public Enemy
{
public:
	Boo(StudentWorld* world, int imageID, int startX, int startY)
		: Enemy(world, imageID, startX, startY)
	{}

	virtual void changeCoinsStars(Player* player);

	virtual void leaveDropping();

private:
	
};

class Square : public ActivatingObject {

public:
	Square(StudentWorld* world, int imageID, int startX, int startY, int direction = right, int depth = 1)
		: ActivatingObject(world, imageID, startX, startY, depth, direction)
	{}

	virtual void doSomething();

	virtual void processAction(Player* player) = 0;

	virtual bool isSquare() const;

private:

};

class CoinSquare : public Square
{
public:
	CoinSquare(StudentWorld* world, int imageID, int startX, int startY, int coins)
		: Square(world, imageID, startX, startY), coinsGiven(coins)
	{}

	virtual void processAction(Player* player);

private:
	void coinSound() const;
	int coinsGiven;
};

class StarSquare : public Square
{
public:
	StarSquare(StudentWorld* world, int imageID, int startX, int startY)
		: Square(world, imageID, startX, startY)
	{}

	virtual void processAction(Player* player);

private:

};

class DirectionSquare : public Square
{
public:
	DirectionSquare(StudentWorld* world, int imageID, int startX, int startY, int direction)
		: Square(world, imageID, startX, startY, direction), dir(direction)
	{}

	virtual void processAction(Player* player);

private:
	int dir;
};

class BankSquare : public Square
{
public:
	BankSquare(StudentWorld* world, int imageID, int startX, int startY)
		: Square(world, imageID, startX, startY)
	{}

	virtual void processAction(Player* player);

private:
};

class EventSquare : public Square
{
public:
	EventSquare(StudentWorld* world, int imageID, int startX, int startY)
		: Square(world, imageID, startX, startY)
	{}

	virtual void processAction(Player* player);

private:

};

class DroppingSquare : public Square
{
public:
	DroppingSquare(StudentWorld* world, int imageID, int startX, int startY)
		: Square(world, imageID, startX, startY)
	{}

	virtual void processAction(Player* player);

private:

};


#endif // ACTOR_H_
