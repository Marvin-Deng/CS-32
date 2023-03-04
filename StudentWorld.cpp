#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <iostream>

using namespace std;

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}


StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    peach = nullptr;
    yoshi = nullptr;
    actors.clear();
    bd = new Board();
    bank = 0;
    coinsTaken = -5;
}

Player* StudentWorld::getPeach() const {
    return peach;
}

Player* StudentWorld::getYoshi() const {
    return yoshi;
}

Board* StudentWorld::getBoard() const{
    return bd;
} 

//// BANK ////

int StudentWorld::getBankCoins() const {
    return bank;
}

void StudentWorld::depositBankCoins(int coins) {
    playSound(SOUND_DEPOSIT_BANK);
    bank += coins;
}

void StudentWorld::resetBankCoins() {
    playSound(SOUND_WITHDRAW_BANK);
    bank = 0;
}

int StudentWorld::getCoinsTaken() const {
    return coinsTaken;
}

//// POSITION FUNCTIONS ////

bool StudentWorld::isValidPos(int newX, int newY) const {

    Board::GridEntry ge = bd->getContentsOf(newX / SPRITE_WIDTH, newY / SPRITE_HEIGHT);

    switch (ge) {

    case Board::empty:
        return false;
    default:
        return true;
    }
}

void StudentWorld::getRandomPoint(int currX, int currY, int& randX, int& randY) {
    bool found = false;
    while (!found) {
        int randIdx = randInt(0, actors.size() - 1);
        int newX = actors[randIdx]->getX();
        int newY = actors[randIdx]->getY();
        if (currX != newX && currY != newY) {
            found = true;
            randX = newX;
            randY = newY;
        }
    }
}

void StudentWorld::swapPlayers() {
    // Player* player1, Player* player2

    cout << "Players swapped" << endl;
    
    // Swap position
    int peachX = peach->getX(), peachY = peach->getY();
    int yoshiX = yoshi->getX(), yoshiY = yoshi->getY();
    peach->moveTo(yoshiX, yoshiY);
    yoshi->moveTo(peachX, peachY);
    
    // Swap ticks
    int peachT = peach->getTicks();
    int yoshiT = yoshi->getTicks();
    peach->setTicks(yoshiT);
    yoshi->setTicks(peachT);

    // Swap walk direction
    int peachM = peach->getMoveDir();
    int yoshiM = yoshi->getMoveDir();
    peach->setMoveDir(yoshiM);
    yoshi->setMoveDir(peachM);

    // Swap sprite direction
    int peachS = peach->getDirection();
    int yoshiS = yoshi->getDirection();
    peach->setDirection(yoshiS);
    yoshi->setDirection(peachS);

    // Swap waiting state
    int peachW = peach->getWaiting();
    int yoshiW = yoshi->getWaiting();
    peach->setWaiting(yoshiW);
    yoshi->setWaiting(peachW);
}


//// GAME FUNCTIONS ////

int StudentWorld::init()
{
    string board_file = assetPath() + "board0" + to_string(getBoardNumber()) + ".txt"; // get the correct board
    Board::LoadResult result = getBoard()->loadBoard(board_file);
    if (result == Board::load_fail_file_not_found)
        return GWSTATUS_BOARD_ERROR;
    else if (result == Board::load_fail_bad_format)
        return GWSTATUS_BOARD_ERROR;
    else if (result == Board::load_success) {
        cerr << "Successfully loaded board\n";

        for (int x = 0; x < BOARD_WIDTH; x++) {
            for (int y = 0; y < BOARD_HEIGHT; y++) {
                Board::GridEntry ge = getBoard()->getContentsOf(x, y);
                int xPos = x * BOARD_WIDTH;
                int yPos = y * BOARD_HEIGHT;
                switch (ge) {

                case Board::empty:
                    break;

                case Board::player:
                    peach = new Player(this, IID_PEACH, xPos, yPos, 1);
                    yoshi = new Player(this, IID_YOSHI, xPos, yPos, 2);
                    actors.push_back(new CoinSquare(this, IID_BLUE_COIN_SQUARE, xPos, yPos, 3));
                    break;

                case Board::blue_coin_square:
                    actors.push_back(new CoinSquare(this, IID_BLUE_COIN_SQUARE, xPos, yPos, 3));
                    break;

                case Board::red_coin_square:
                    actors.push_back(new CoinSquare(this, IID_RED_COIN_SQUARE, xPos, yPos, -3));
                    break;

                case Board::star_square:
                    actors.push_back(new StarSquare(this, IID_STAR_SQUARE, xPos, yPos));
                    break;

                case Board::right_dir_square:
                    actors.push_back(new DirectionSquare(this, IID_DIR_SQUARE, xPos, yPos, 0));
                    break;

                case Board::left_dir_square:
                    actors.push_back(new DirectionSquare(this, IID_DIR_SQUARE, xPos, yPos, 180));
                    break;

                case Board::up_dir_square:
                    actors.push_back(new DirectionSquare(this, IID_DIR_SQUARE, xPos, yPos, 90));
                    break;

                case Board::down_dir_square:
                    actors.push_back(new DirectionSquare(this, IID_DIR_SQUARE, xPos, yPos, 270));
                    break;

                case Board::bank_square:
                    actors.push_back(new BankSquare(this, IID_BANK_SQUARE, xPos, yPos));
                    break;

                case Board::event_square:
                    actors.push_back(new EventSquare(this, IID_EVENT_SQUARE, xPos, yPos));
                    break;

                case Board::bowser:
                    actors.push_back(new CoinSquare(this, IID_BLUE_COIN_SQUARE, xPos, yPos, 3));
                    break;

                case Board::boo:
                    actors.push_back(new CoinSquare(this, IID_BLUE_COIN_SQUARE, xPos, yPos, 3));
                    break;
                }
            }
        }
        startCountdownTimer(99);  // 99 second timer
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    setGameStatText("P1 Coins: " + to_string(peach->getPlayerCoins()) + " Stars: " + to_string(peach->getPlayerStars())
        + " P2 Coins: " + to_string(yoshi->getPlayerCoins()) + " Stars: " + to_string(yoshi->getPlayerStars())
        + " Bank: " + to_string(bank)
    );

    // Check if game is over
    if (timeRemaining() <= 0) {
        playSound(SOUND_GAME_FINISHED);
        return GWSTATUS_NOT_IMPLEMENTED;
    }

    // Ask players and acotrs ot do something
    peach->doSomething();
    yoshi->doSomething();
    for (Actor* a : actors) {
        if (a == nullptr) {
            continue;
        }
        a->doSomething();
        if (!a->isAlive()) {
            delete a;
            a = nullptr;
        }
    }

    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    // Delete vector of dynamically allocated objects
    for (Actor* a : actors) {
        delete a; 
    }
    actors.clear(); // clears all elements of vector, makes size 0

    // Delete players
    delete peach; 
    delete yoshi;
    peach = nullptr;
    yoshi = nullptr;
}

StudentWorld::~StudentWorld() { 
    cleanUp();
    delete bd;
}
