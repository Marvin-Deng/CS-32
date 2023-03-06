#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Board.h"
#include "Actor.h"
#include <string>
#include <vector>

class GameObject;


class StudentWorld : public GameWorld
{
public:
  StudentWorld(std::string assetPath);

  Player* getPeach() const;
  Player* getYoshi() const;
  Board* getBoard() const;

  int getBankCoins() const;
  void depositBankCoins(int coins);
  void resetBankCoins();
  int getCoinsTaken() const;

  bool isValidPos(int newX, int newY) const;
  void getRandomPoint(int currX, int currY, int& randX, int& randY);
  
  void swapPlayers(); // Event square
  void swapCoinsStars(); // Boo
  void addDroppingSquare(int x, int y); // Bowser
  void addVortex(int x, int y, int dir); // Player
  bool hitByVortex(int x, int y) const; // Vortex

  virtual int init();
  virtual int move();
  virtual void cleanUp();
  ~StudentWorld();

private:
	Player* peach;
	Player* yoshi;
	std::vector<Actor*> actors;
	Board* bd;
	int bank;
	int coinsTaken;
};

#endif // STUDENTWORLD_H_
