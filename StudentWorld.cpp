#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <vector>
#include <iostream>
using namespace std;

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

Board* StudentWorld::getBoard() const {
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

//// SQUARE AND ENEMY FUNCTIONS ////

// Event square random teleport
void StudentWorld::getRandomPoint(int currX, int currY, int& randX, int& randY) {
    bool found = false;
    while (!found) {
        int randIdx = randInt(0, actors.size() - 1);
        Actor* actor = actors[randIdx];
        if (actor == nullptr) {
            continue;
        }
        int newX = actor->getX();
        int newY = actor->getY();
        if (currX != newX && currY != newY && actor->isSquare()) { // can't be current position or on a vortex
            randX = newX;
            randY = newY;
            found = true;
        }
    }
}

void StudentWorld::swapPlayers(Player* player) {

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

    // Swap walk and sprite direction
    int peachM = peach->getMoveDir();
    int yoshiM = yoshi->getMoveDir();
    peach->changeMoveDir(yoshiM);
    yoshi->changeMoveDir(peachM);

    // Swap waiting state
    int peachW = peach->getWaiting();
    int yoshiW = yoshi->getWaiting();
    peach->setWaiting(yoshiW);
    yoshi->setWaiting(peachW);

    Player* other;
    if (player == peach) {
        other = yoshi;
    }
    else {
        other = peach;
    }
    if (peachX != yoshiX && peachY != yoshiY) {
        player->setIsNew(true);
    }
    other->setIsNew(false);
}

// Boo: coin swap
void StudentWorld::swapCoinsStars() {
    int swap = randInt(1, 2); // (1, 2)
    if (swap == 1) { // Swap coins
        int peachCoins = peach->getPlayerCoins();
        int yoshiCoins = yoshi->getPlayerCoins();
        peach->replaceCoins(yoshiCoins);
        yoshi->replaceCoins(peachCoins);
    }
    else if (swap == 2) { // Swap stars
        int peachStars = peach->getPlayerStars();
        int yoshiStars = yoshi->getPlayerStars();
        peach->replaceStars(yoshiStars);
        yoshi->replaceStars(peachStars);
    }
}

// Bowser: add droppping square
void StudentWorld::addDroppingSquare(int x, int y) {
    for (int i = 0; i < static_cast<int>(actors.size()); i++) {
        if (actors[i]->getX() == x && actors[i]->getY() == y && actors[i]->isSquare()) {
            delete actors[i];
            actors[i] = new DroppingSquare(this, IID_DROPPING_SQUARE, x, y);
        }
    }
}

// Player: add vortex
void StudentWorld::addVortex(int x, int y, int dir) {
    int addX = 0, addY = 0;
    if (dir == 0) {
        addX = SPRITE_WIDTH;
    }
    else if (dir == 180) {
        addX = -SPRITE_WIDTH;
    }
    else if (dir == 90) {
        addY = SPRITE_HEIGHT;
    }
    else if (dir == 270) {
        addY = -SPRITE_HEIGHT;
    }
    actors.push_back(new Vortex(this, IID_VORTEX, x + addX, y + addY, dir));
}

// Vortex: check hit by vortex
bool StudentWorld::hitByVortex(int x, int y) const {
    for (int i = 0; i < static_cast<int>(actors.size()); i++) {
        Actor* actor = actors[i];
        if (actor->canBeHit() && abs(x - actor->getX()) < SPRITE_WIDTH && abs(y - actor->getY()) < SPRITE_HEIGHT) {
            actors[i]->setIsHit(true);
            return true;
        }
    }
    return false;
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

                case Board::bowser: {
                    vector<Actor*>::iterator p = actors.begin();
                    actors.insert(p, new Bowser(this, IID_BOWSER, xPos, yPos));
                    actors.push_back(new CoinSquare(this, IID_BLUE_COIN_SQUARE, xPos, yPos, 3));
                    break;
                }

                case Board::boo: {
                    vector<Actor*>::iterator p = actors.begin();
                    actors.insert(p, new Boo(this, IID_BOO, xPos, yPos));
                    actors.push_back(new CoinSquare(this, IID_BLUE_COIN_SQUARE, xPos, yPos, 3));
                    break;
                }

                }
            }
        }
        startCountdownTimer(99);  // 99 second timer
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    string pRoll = to_string(peach->getTicks() / 8);
    string pStars = to_string(peach->getPlayerStars());
    string pCoins = to_string(peach->getPlayerCoins());
    string pVortex = peach->hasVortex() ? "VOR" : "";
    string time = to_string(timeRemaining());
    string bankAccount = to_string(bank);
    string yRoll = to_string(yoshi->getTicks() / 8);
    string yStars = to_string(yoshi->getPlayerStars());
    string yCoins = to_string(yoshi->getPlayerCoins());
    string yVortex = yoshi->hasVortex() ? "VOR" : "";

    setGameStatText(pVortex + " P1 Roll: " + pRoll + " Stars: " + pStars + " $$: " + pCoins +
        " | Time: " + time + " | Bank: " + bankAccount +
        " | P2 Roll: " + yRoll + " Stars: " + yStars + " $$: " + yCoins + " " + yVortex
    );

    // Check if game is over
    if (timeRemaining() <= 0) {
        playSound(SOUND_GAME_FINISHED);
        if (pStars > yStars) {
            this->setFinalScore(peach->getPlayerStars(), peach->getPlayerCoins());
            return GWSTATUS_PEACH_WON;
        }
        else if (yStars > pStars) {
            this->setFinalScore(yoshi->getPlayerStars(), yoshi->getPlayerCoins());
            return GWSTATUS_YOSHI_WON;
        }

        if (pCoins > yCoins) {
            this->setFinalScore(peach->getPlayerStars(), peach->getPlayerCoins());
            return GWSTATUS_PEACH_WON;
        }
        else if (yCoins > pCoins) {
            this->setFinalScore(yoshi->getPlayerStars(), yoshi->getPlayerCoins());
            return GWSTATUS_YOSHI_WON;
        }
        else {
            int randWinner = randInt(1, 2);
            if (randWinner == 1) {
                this->setFinalScore(peach->getPlayerStars(), peach->getPlayerCoins());
                return GWSTATUS_PEACH_WON;
            }
            else if (randWinner == 2) {
                this->setFinalScore(yoshi->getPlayerStars(), yoshi->getPlayerCoins());
                return GWSTATUS_YOSHI_WON;
            }
        }
    }

    // Ask players and actors to do something
    peach->doSomething();
    yoshi->doSomething();

    for (int i = static_cast<int>(actors.size() - 1); i >= 0; i--) {
        actors[i]->doSomething();
    }

    vector<Actor*>::iterator p = actors.begin();
    while (p != actors.end()) {
        if (!(*p)->isAlive()) {
            delete (*p);
            p = actors.erase(p);
        }
        else {
            p++;
        }
    }

    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    // Delete vector of dynamically allocated objects
    for (int i = 0; i < static_cast<int>(actors.size()); i++) {
        delete actors[i];
    }
    actors.clear(); // clears all elements of vector, makes size 0

    // Delete players
    delete peach;
    delete yoshi;
    peach = nullptr;
    yoshi = nullptr;
}

StudentWorld::~StudentWorld() {
    //cleanUp();
    delete bd;
}