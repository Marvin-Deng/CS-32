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

	bool isAlive() const;

	StudentWorld* getWorld() const;


private:
	StudentWorld* m_world;
	bool alive;
};

class Player : public Actor
{
public:
	Player(StudentWorld* world, int imageID, int startX, int startY, int playerNum)
		: Actor(world, imageID, startX, startY), ticks_to_move(0), currDir(right), waiting(true), pNum(playerNum), vortex(0), coins(0), stars(0), isNew(true)
	{}

	virtual void doSomething();

	void updateCoins(int c);

	void updateStars();

	int getPlayerCoins() const;

	bool getWaiting() const;

	bool getIsNew() const;

	void resetIsNew();

	void setDirection(int dir);

private:
	bool isValidPos() const;
	void updateValidMoveDirection();
	void changeSpriteDirection();
	int pNum;
	int ticks_to_move;
	int currDir;
	bool waiting;
	int vortex;
	int coins;
	int stars;
	bool isNew;
};

class ActivatingObject : public Actor {

public:
	ActivatingObject(StudentWorld* world, int imageID, int startX, int startY, int direction = right, int depth = 1)
		: Actor(world, imageID, startX, startY, direction, depth), active(true)
	{}

	bool onObject(Player* player);

	bool passingObject(Player* player);

	void resetActive();

private:
	bool active;
};



class CoinSquare : public ActivatingObject
{
public:
	CoinSquare(StudentWorld* world, int imageID, int startX, int startY, int coins)
		: ActivatingObject(world, imageID, startX, startY), coinsGiven(coins)
	{}
	virtual void doSomething();

private:
	int coinsGiven;
};

class StarSquare : public ActivatingObject
{
public:
	StarSquare(StudentWorld* world, int imageID, int startX, int startY)
		: ActivatingObject(world, imageID, startX, startY)
	{}
	virtual void doSomething();

private:
	
};

class DirectionSquare : public ActivatingObject
{
public:
	DirectionSquare(StudentWorld* world, int imageID, int startX, int startY, int direction)
		: ActivatingObject(world, imageID, startX, startY, direction), dir(direction)
	{}
	virtual void doSomething();

	int getDir();

private:
	int dir;
};

class BankSquare : public ActivatingObject
{
public:
	BankSquare(StudentWorld* world, int imageID, int startX, int startY)
		: ActivatingObject(world, imageID, startX, startY)
	{}
	virtual void doSomething();

private:

};

class EventSquare : public ActivatingObject
{
public:
	EventSquare(StudentWorld* world, int imageID, int startX, int startY)
		: ActivatingObject(world, imageID, startX, startY)
	{}
	virtual void doSomething();

private:

};

class DroppingSquare : public ActivatingObject
{
public:
	DroppingSquare(StudentWorld* world, int imageID, int startX, int startY)
		: ActivatingObject(world, imageID, startX, startY)
	{}
	virtual void doSomething();

private:

};

#endif // ACTOR_H_
