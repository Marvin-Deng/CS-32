#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Board.h"
#include "Actor.h"
#include <string>
#include <vector>


// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp


class StudentWorld : public GameWorld
{
public:
  StudentWorld(std::string assetPath);
  Player* getPeach() const;
  Player* getYoshi() const;
  Board* getBoard() const;
  virtual int init();
  virtual int move();
  virtual void cleanUp();
  ~StudentWorld();

private:
	Player* peach;
	Player* yoshi;
	std::vector<Actor*> actors;
	Board* bd;
};

#endif // STUDENTWORLD_H_
