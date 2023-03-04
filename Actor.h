#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <string>
#include <set>

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

	int updateCoins(int c);
	void updateStars();
	void changeMoveDir(int dir);
	void playerTeleport();
	void giveVortex();
	int getPlayerCoins() const;
	int getPlayerStars() const;
	bool getWaiting() const;
	void setWaiting(bool wait);
	int getTicks() const;
	void setTicks(int ticks);
	bool getIsNew() const;
	void resetIsNew();
	int getMoveDir() const;
	void setMoveDir(int dir);

private:
	void updateValidMoveDirection();
	void changeSpriteDirection();
	std::set<int> validForkDirs();
	int pNum;
	int ticks_to_move;
	int currDir;
	bool waiting;
	int vortex;
	int coins;
	int stars;
	bool isNew;
};

class Vortex : public Actor
{
public:
	Vortex(StudentWorld* world, int imageID, int startX, int startY, int direction)
		: Actor(world, imageID, startX, startY, direction)
	{}

private:

};

class ActivatingObject : public Actor {

public:
	ActivatingObject(StudentWorld* world, int imageID, int startX, int startY, int direction = right, int depth = 1)
		: Actor(world, imageID, startX, startY, direction, depth)
	{}

	bool onObject(Player* player, int onOrPassing);
	bool passingObject(Actor* player);
	/*template<typename func>
	void processAction(func action);*/

private:

};

class CoinSquare : public ActivatingObject
{
public:
	CoinSquare(StudentWorld* world, int imageID, int startX, int startY, int coins)
		: ActivatingObject(world, imageID, startX, startY), coinsGiven(coins)
	{}
	virtual void doSomething();

private:
	void processCoinSquare(Player* player);
	void coinSound() const;
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
	void processStarSquare(Player* player);
};

class DirectionSquare : public ActivatingObject
{
public:
	DirectionSquare(StudentWorld* world, int imageID, int startX, int startY, int direction)
		: ActivatingObject(world, imageID, startX, startY, direction), dir(direction)
	{}
	virtual void doSomething();

private:
	void processDirectionSquare(Player* player);
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
	void processBankSquare(Player* player);
};

class EventSquare : public ActivatingObject
{
public:
	EventSquare(StudentWorld* world, int imageID, int startX, int startY)
		: ActivatingObject(world, imageID, startX, startY)
	{}
	virtual void doSomething();

private:
	void processEventSquare(Player* player1, Player* player2);

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
