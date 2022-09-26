#include "Player.h"
#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>
#include <string>

using namespace std;

//*********************************************************************
//  AwfulPlayer
//*********************************************************************

class AwfulPlayer : public Player
{
  public:
    AwfulPlayer(string nm, const Game& g);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                                bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
  private:
    Point m_lastCellAttacked;
};

AwfulPlayer::AwfulPlayer(string nm, const Game& g)
 : Player(nm, g), m_lastCellAttacked(0, 0)
{}

bool AwfulPlayer::placeShips(Board& b)
{
      // Clustering ships is bad strategy
    for (int k = 0; k < game().nShips(); k++)
        if ( ! b.placeShip(Point(k,0), k, HORIZONTAL))
            return false;
    return true;
}

Point AwfulPlayer::recommendAttack()
{
    if (m_lastCellAttacked.c > 0)
        m_lastCellAttacked.c--;
    else
    {
        m_lastCellAttacked.c = game().cols() - 1;
        if (m_lastCellAttacked.r > 0)
            m_lastCellAttacked.r--;
        else
            m_lastCellAttacked.r = game().rows() - 1;
    }
    return m_lastCellAttacked;
}

void AwfulPlayer::recordAttackResult(Point /* p */, bool /* validShot */,
                                     bool /* shotHit */, bool /* shipDestroyed */,
                                     int /* shipId */)
{
      // AwfulPlayer completely ignores the result of any attack
}

void AwfulPlayer::recordAttackByOpponent(Point /* p */)
{
      // AwfulPlayer completely ignores what the opponent does
}

//*********************************************************************
//  HumanPlayer
//*********************************************************************

bool getLineWithTwoIntegers(int& r, int& c)
{
    bool result(cin >> r >> c);
    if (!result)
        cin.clear();  // clear error state so can do more input operations
    cin.ignore(10000, '\n');
    return result;
}

class HumanPlayer : public Player {
  public:
    HumanPlayer(string nm, const Game& g);
    virtual bool isHuman() const;
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                    bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
  private:
    Point m_lastCellAttacked;
    bool validDirInput(string& input);
};

HumanPlayer::HumanPlayer(string nm, const Game& g) : Player(nm, g), m_lastCellAttacked(0, 0) {}

bool HumanPlayer::isHuman() const { return true; }

bool HumanPlayer::validDirInput(string& input) {
    getline(cin, input);
    if (input[0] != 'h' && input[0] != 'v') {
        return false;
    }
    return true;
}

bool HumanPlayer::placeShips(Board& b) {
    cout << name() << " must place " << game().nShips() << " ships." << endl;
    
    string sDir;
    for (int i = 0; i < game().nShips(); i++) {
        b.display(false);
        cout << "Enter h or v for direction of " << game().shipName(i) << " (length " << game().shipLength(i) << "): ";
        string sDir;
        // check if input is valid
        bool validDir = validDirInput(sDir);
        while (!validDir) { // check that this works properly
            cout << "Direction must be h or v.\n";
            cout << "Enter h or v for direction of " << game().shipName(i) << " (length " << game().shipLength(i) << "): ";
            validDir = validDirInput(sDir);
        }
        Direction dir = HORIZONTAL;
        if (sDir[0] == 'v')
            dir = VERTICAL;
        
        int row = -1; int col = -1;
        cout << "Enter row and column of leftmost cell (e.g., 3 5): ";
        bool validInts = getLineWithTwoIntegers(row, col);
        while (!validInts) {
            cout << "You must enter two integers.\n";
            cout << "Enter row and column of leftmost cell (e.g., 3 5): ";
            validInts = getLineWithTwoIntegers(row, col);
        }
        
        bool validPlacement = b.placeShip(Point(row, col), i, dir);
        while (!validPlacement) {
            cout << "The ship can not be placed there.\n";
            cout << "Enter row and column of leftmost cell (e.g., 3 5): ";
            validInts = getLineWithTwoIntegers(row, col);
            while (!validInts) {
                cout << "You must enter two integers.\n";
                cout << "Enter row and column of leftmost cell (e.g., 3 5): ";
                validInts = getLineWithTwoIntegers(row, col);
            }
            validPlacement = b.placeShip(Point(row, col), i, dir);
        }
    }
    
    return true;
}
Point HumanPlayer::recommendAttack()
{
    int row = -1; int col = -1;
    cout << "Enter the row and column to attack (e.g., 3 5): ";
    bool validInts = getLineWithTwoIntegers(row, col);
    while (!validInts) {
        cout << "Enter the row and column to attack (e.g., 3 5): ";
        validInts = getLineWithTwoIntegers(row, col);
        while (!validInts) {
            cout << "You must enter two integers.\n";
            cout << "Enter the row and column to attack (e.g., 3 5): ";
            validInts = getLineWithTwoIntegers(row, col);
        }
    }
    m_lastCellAttacked = Point(row,col);
    return m_lastCellAttacked;
}

void HumanPlayer::recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId) {}

void HumanPlayer::recordAttackByOpponent(Point p) {}

//*********************************************************************
//  MediocrePlayer
//*********************************************************************

class MediocrePlayer : public Player {
  public:
    MediocrePlayer(string nm, const Game& g);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                    bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
  private:
    int state;
    Point m_lastCellAttacked;
    Point m_currState2Cell;
    vector<Point> availableCells;

    bool recursive(Board& b, int shipId, int shipsLeft);
    Point m_randomPoint();
    void removePointFromAvailableCells(Point p);
};

MediocrePlayer::MediocrePlayer(string nm, const Game& g) : Player(nm, g), m_lastCellAttacked(0, 0) {
    state = 1;
    m_lastCellAttacked = Point(0,0);
    m_currState2Cell = Point(0,0);
    for (int r = 0; r < game().rows(); r++) {
        for (int c = 0; c < game().cols(); c++) {
            availableCells.push_back(Point(r,c));
        }
    }
}

Point MediocrePlayer::m_randomPoint() {
    int random = rand() % availableCells.size();
    return availableCells[random];
}

void MediocrePlayer::removePointFromAvailableCells(Point p) {
    vector<Point>::iterator it = availableCells.begin();
    while (it != availableCells.end()) {
        if (it->r == p.r && it->c == p.c) {
            availableCells.erase(it);
            break;
        } else {
            it++;
        }
    }
}

bool MediocrePlayer::placeShips(Board& b) {
    int numTries = 0;
    while (numTries < 50) {
        b.block();
        if (recursive(b, 0, game().nShips())) {
            b.unblock();
            return true;
        }
        b.unblock();
        numTries++;
    }
    return false;
}

bool MediocrePlayer::recursive(Board& b, int shipId, int shipsLeft) {
    if (shipsLeft == 0)
        return true;
    
    for (int r = 0; r < game().rows(); r++) {
        for (int c = 0; c < game().cols(); c++) {
            if (b.placeShip(Point(r,c), shipId, HORIZONTAL)) {
                if (recursive(b, shipId + 1, shipsLeft - 1))
                    return true;
                else
                    b.unplaceShip(Point(r,c), shipId, HORIZONTAL);
            }
            if (b.placeShip(Point(r,c), shipId, VERTICAL)) {
                if (recursive(b, shipId + 1, shipsLeft - 1))
                    return true;
                else
                    b.unplaceShip(Point(r,c), shipId, VERTICAL); // backtrack
            }
        }
    }
    return false;
}

Point MediocrePlayer::recommendAttack() {
    while (true) {
        m_lastCellAttacked = m_randomPoint();
        if (state == 1) {
            removePointFromAvailableCells(m_lastCellAttacked); // cell has been used, cannot be used again
            break;
        } else { // state == 2
            if ((m_lastCellAttacked.c == m_currState2Cell.c && m_lastCellAttacked.r >= (m_currState2Cell.r - 4) && m_lastCellAttacked.r <= (m_currState2Cell.r + 4))
                || // same col, different row OR same row, different col
                (m_lastCellAttacked.r == m_currState2Cell.r && m_lastCellAttacked.c >= (m_currState2Cell.c - 4) && m_lastCellAttacked.c <= (m_currState2Cell.c + 4))) {
                removePointFromAvailableCells(m_lastCellAttacked);
                break;
            }
        }
    }
    return m_lastCellAttacked;
}

void MediocrePlayer::recordAttackResult(Point p, bool validShot, bool shotHit,
                                bool shipDestroyed, int shipId) {
    if (!validShot)
        return;
    if (state == 1) {
        // if attack missed or ship destroyed, stay in state 1
        if (shotHit && !shipDestroyed) {
            m_currState2Cell = p;
            state = 2;
        }
    } else { // state 2
        // if attack missed or ship not destroyed, stay in state 2
        if (shotHit && shipDestroyed) {
            state = 1;
        }
    }
}

void MediocrePlayer::recordAttackByOpponent(Point p) {}

//*********************************************************************
//  GoodPlayer
//*********************************************************************

// TODO:  You need to replace this with a real class declaration and
//        implementation.
class GoodPlayer : public Player {
  public:
    GoodPlayer(string nm, const Game& g);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                    bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
  private:
    // members
    int state;
    int currShipId;
    Direction currShipDir;

    Point m_lastCellAttacked;
    Point m_currState2Cell;
    Point m_originalState2Cell;
    
    bool continuousAttack;
    
    char attackedCells[MAXROWS][MAXCOLS];
    char checkerBoard[MAXROWS][MAXCOLS];
    char m_ships[MAXROWS][MAXCOLS];
    vector<Point> toBeAttacked;
    vector<Point> availableCells;
    
    // place ship functions
    bool tryRandom(Board& b);
    Direction randDirection();
    bool recursive(Board& b, int shipId, int shipsLeft);
    bool areShipsTouching();
    void m_shipsPlaceShip(Point topOrLeft, int shipId, Direction dir);
    void m_shipUnplaceShip(Point topOrLeft, int shipId, Direction dir);

    // recommend attack functions
    void cleanoutAttackedCells();
    void cleanoutToBeAttacked();
    void updateToBeAttacked(Point p);
    Point m_randomPoint();
    void removePointFromAvailableCells(Point p);
};

GoodPlayer::GoodPlayer(string nm, const Game& g) : Player(nm, g), m_lastCellAttacked(0, 0) {
    state = 1;
    currShipId = -1;
    continuousAttack = false;
    currShipDir = HORIZONTAL;
    for (int r = 0; r < game().rows(); r++) {
        for (int c = 0; c < game().cols(); c++) {
            attackedCells[r][c] = '.';
            m_ships[r][c] = '.';
            availableCells.push_back(Point(r,c));
            if (r % 2 == 0) {
                if (c % 2 == 0)
                    checkerBoard[r][c] = '.';
                else
                    checkerBoard[r][c] = '/';
            } else { // r is odd
                if (c % 2 == 0)
                    checkerBoard[r][c] = '/';
                else
                    checkerBoard[r][c] = '.';
            }
        }
    }
}

//*********************************************************************
//  GoodPlayer - Place Ship Functions
//*********************************************************************

bool GoodPlayer::placeShips(Board& b) {
    // check if ships can even fit on board
    int totalLength = 0;
    for (int i = 0; i < game().nShips(); i++)
        totalLength += game().shipLength(i);
    
    if (totalLength > game().rows() * game().cols())
        return false;
    
    // try placing ships randomly and away from each other
    int numTries = 0;
    while (numTries < 50) {
        if (tryRandom(b)) {
            return true;
        }
        numTries++;
    }
    
    // can't place ships randomly so default to using mediocrePlayer's placeShips method's code
    b.clear();
    numTries = 0;
    while (numTries < 50) {
        b.block();
        if (recursive(b, 0, game().nShips())) {
            b.unblock();
            return true;
        }
        b.unblock();
        numTries++;
    }
    
    // can't do it using mediocrePlayer's code so get rid of the blocking
    b.clear();
    numTries = 0;
    while (numTries < 50) {
        if (recursive(b, 0, game().nShips())) {
            return true;
        }
        numTries++;
    }
    
    return false;
}

bool GoodPlayer::tryRandom(Board& b) {
    
    int shipsLeft = game().nShips();
    int shipId = 0;
    int numAttempts = 0;
    
    while (shipsLeft > 0 && numAttempts < 100) {
        Point randPoint = game().randomPoint();
        Direction randDir = randDirection();
        Direction otherDir;
        if (randDir == VERTICAL)
            otherDir = HORIZONTAL;
        else
            otherDir = VERTICAL;
        if (b.placeShip(randPoint, shipId, randDir)) {
            m_shipsPlaceShip(randPoint, shipId, randDir);
            if (areShipsTouching()) {
                b.unplaceShip(randPoint, shipId, randDir);
                m_shipUnplaceShip(randPoint, shipId, randDir);
            } else {
                shipsLeft--;
                shipId++;
            }
        } else if (b.placeShip(randPoint, shipId, otherDir)) {
            m_shipsPlaceShip(randPoint, shipId, otherDir);
            if (areShipsTouching()) {
                b.unplaceShip(randPoint, shipId, otherDir);
                m_shipUnplaceShip(randPoint, shipId, otherDir);
            } else {
                shipsLeft--;
                shipId++;
            }
        }
        numAttempts++;
    }
    if (shipsLeft == 0)
        return true;
    else
        return false;
}

void GoodPlayer::m_shipsPlaceShip(Point topOrLeft, int shipId, Direction dir) {
    if (dir == VERTICAL) {
        for (int i = 0; i < game().shipLength(shipId); i++)
            m_ships[topOrLeft.r+i][topOrLeft.c] = game().shipSymbol(shipId);
        return;
    }
    // HORIZONTAL
    for (int i = 0; i < game().shipLength(shipId); i++)
        m_ships[topOrLeft.r][topOrLeft.c+i] = game().shipSymbol(shipId);
}

void GoodPlayer::m_shipUnplaceShip(Point topOrLeft, int shipId, Direction dir) {
    if (dir == VERTICAL) {
        for (int i = 0; i < game().shipLength(shipId); i++)
            m_ships[topOrLeft.r + i][topOrLeft.c] = '.';
        return;
    }
    // HORIZONTAL
    for (int i = 0; i < game().shipLength(shipId); i++)
        m_ships[topOrLeft.r][topOrLeft.c + i] = '.';
}

bool GoodPlayer::areShipsTouching() {
    for (int r = 0; r < game().rows(); r++) {
        for (int c = 0; c < game().cols(); c++) {
            // check surrounding cells to make sure there's no differing symbols next to each other
            if (m_ships[r][c] != '.') {
                // check up
                if (r > 0 && (m_ships[r-1][c] != m_ships[r][c]) && (m_ships[r-1][c] != '.'))
                    return true;
                // check down
                if (r < (game().rows()-1) && (m_ships[r+1][c] != m_ships[r][c]) && (m_ships[r+1][c] != '.'))
                    return true;
                // check left
                if (c > 0 && (m_ships[r][c-1] != m_ships[r][c]) && (m_ships[r][c-1] != '.'))
                    return true;
                // check right
                if (c < (game().cols()-1) && (m_ships[r][c+1] != m_ships[r][c]) && (m_ships[r][c+1] != '.'))
                    return true;
            }
        }
    }
    return false;
}

bool GoodPlayer::recursive(Board& b, int shipId, int shipsLeft) {
    if (shipsLeft == 0)
        return true;
    
    for (int r = 0; r < game().rows(); r++) {
        for (int c = 0; c < game().cols(); c++) {
            if (b.placeShip(Point(r,c), shipId, HORIZONTAL)) {
                if (recursive(b, shipId + 1, shipsLeft - 1))
                    return true;
                else
                    b.unplaceShip(Point(r,c), shipId, HORIZONTAL);
            }
            if (b.placeShip(Point(r,c), shipId, VERTICAL)) {
                if (recursive(b, shipId + 1, shipsLeft - 1))
                    return true;
                else
                    b.unplaceShip(Point(r,c), shipId, VERTICAL); // backtrack
            }
        }
    }
    return false;
}

//*********************************************************************
//  GoodPlayer - Recommend Attack Functions
//*********************************************************************

Point GoodPlayer::recommendAttack() {
    cleanoutAttackedCells();
    cleanoutToBeAttacked();
    if (state == 2) {
        if (currShipDir == HORIZONTAL) {
              // attack left
            if (m_currState2Cell.c != 0 && attackedCells[m_currState2Cell.r][m_currState2Cell.c-1] == '.') {
                m_lastCellAttacked = Point(m_currState2Cell.r, m_currState2Cell.c-1);
                return m_lastCellAttacked;
            } // attack right
            else if (m_currState2Cell.c != game().cols()-1 && attackedCells[m_currState2Cell.r][m_currState2Cell.c+1] == '.') {
                m_lastCellAttacked = Point(m_currState2Cell.r, m_currState2Cell.c+1);
                return m_lastCellAttacked;
            } // the direction of the ship is confirmed to be HORIZONTAL via shooting all the way left
              // we now shoot right of the starting point to fully destroy the ship
            else if (continuousAttack) {
                m_currState2Cell = m_originalState2Cell;
                if (m_currState2Cell.c != (game().cols()-1) && attackedCells[m_currState2Cell.r][m_currState2Cell.c+1] == '.') {
                    m_lastCellAttacked = Point(m_currState2Cell.r, m_currState2Cell.c+1);
                    return m_lastCellAttacked;
                }
            } // we've checked both left and right to confirm the ship's direction isn't HORIZONTAL, so change to vertical
            else {
                currShipDir = VERTICAL;
            }
        }
        if (currShipDir == VERTICAL) {
              // attack up
            if (m_currState2Cell.r != 0 && attackedCells[m_currState2Cell.r-1][m_currState2Cell.c] == '.') {
                m_lastCellAttacked = Point(m_currState2Cell.r-1, m_currState2Cell.c);
                return m_lastCellAttacked;
            } // attack down
            else if (m_currState2Cell.r != game().rows()-1 && attackedCells[m_currState2Cell.r+1][m_currState2Cell.c] == '.') {
                m_lastCellAttacked = Point(m_currState2Cell.r+1, m_currState2Cell.c);
                return m_lastCellAttacked;
            } // the direction of the ship is confirmed to be VERTICAL via shooting all the way up
              // we now shoot down of the starting point to fully destroy the ship
            else if (continuousAttack) {
                m_currState2Cell = m_originalState2Cell;
                if (m_currState2Cell.r != (game().rows()-1) && attackedCells[m_currState2Cell.r+1][m_currState2Cell.c] == '.') {
                    m_lastCellAttacked = Point(m_currState2Cell.r+1, m_currState2Cell.c);
                    return m_lastCellAttacked;
                }
            }
        }
    }
    // we are either in state 1, or failed to recommend a point in state 2
    // just take a Point from toBeAttacked (a vector of prospective attacking points) or if toBeAttacked doesn't have any points,
    // randomly select a point, prioritizing the checkerBoard pattern
    if (!toBeAttacked.empty()) {
        m_lastCellAttacked = toBeAttacked[0];
        vector<Point>::iterator it = toBeAttacked.begin();
        toBeAttacked.erase(it);
        return m_lastCellAttacked;
    }
    
    // get a random checkerBoard Point
    int j = 0;
    while (j < 20) {
        m_lastCellAttacked = m_randomPoint();
        if (attackedCells[m_lastCellAttacked.r][m_lastCellAttacked.c] == '.'
            && checkerBoard[m_lastCellAttacked.r][m_lastCellAttacked.c] == '.') {
            return m_lastCellAttacked;
        }
//        restoreRandPoint(m_lastCellAttacked);
        j++;
    }

    // no checkerBoard Points left, so get a random available point
    while (true) {
        m_lastCellAttacked = m_randomPoint();
        if (attackedCells[m_lastCellAttacked.r][m_lastCellAttacked.c] == '.') {
            break;
        }
    }
    return m_lastCellAttacked;
}

void GoodPlayer::cleanoutToBeAttacked() {
    vector<Point>::iterator it = toBeAttacked.begin();
    while (it != toBeAttacked.end()) {
        if (attackedCells[it->r][it->c] != '.') {
            it = toBeAttacked.erase(it);
        } else {
            it++;
        }
    }
}

Direction GoodPlayer::randDirection() {
    int rand = randInt(2);
    if (rand == 0)
        return VERTICAL;
    return HORIZONTAL;
}

// go through attacked cells and see which cells, even if checked, would be a ship length 1 or less
void GoodPlayer::cleanoutAttackedCells() {
    // if there are ships on this board that are length 1, don't proceed
    for (int i = 0; i < game().nShips(); i++) {
        if (game().shipLength(i) == 1)
            return;
    }
    
    bool isWorthy = false;
    // get rid of cells that are surrounded by %'s as potential attacking positions, they are unworthy of being checked
    for (int r = 0; r < game().rows(); r++) {
        for (int c = 0; c < game().cols(); c++) {
            if (attackedCells[r][c] == '.') {
                // check up
                if (r > 0 && (attackedCells[r-1][c] == '.' || attackedCells[r-1][c] == 'X'))
                    isWorthy = true;
                // check down
                if (r < (game().rows()-1) && (attackedCells[r+1][c] == '.' || attackedCells[r+1][c] == 'X'))
                    isWorthy = true;
                // check left
                if (c > 0 && (attackedCells[r][c-1] == '.' || attackedCells[r][c-1] == 'X'))
                    isWorthy = true;
                // check right
                if (c < (game().cols()-1) && (attackedCells[r][c+1] == '.' || attackedCells[r][c+1] == 'X'))
                    isWorthy = true;
                if (!isWorthy)
                    attackedCells[r][c] = 'o';
            }
            isWorthy = false;
        }
    }
}

void GoodPlayer::recordAttackResult(Point p, bool validShot, bool shotHit,
                                    bool shipDestroyed, int shipId) {
    attackedCells[p.r][p.c] = 'o';
    removePointFromAvailableCells(p);

    if (state == 1) {
        // if attack missed or ship destroyed, stay in state 1
        if (shotHit && !shipDestroyed) {
            attackedCells[p.r][p.c] = 'X';
            currShipId = shipId;
            m_originalState2Cell = p;
            m_currState2Cell = p;
            state = 2;
            updateToBeAttacked(m_originalState2Cell);
        }
    } else { // state 2
        if (!shotHit)
            continuousAttack = false;
        
        if (shotHit && !shipDestroyed) {
            attackedCells[p.r][p.c] = 'X';
            continuousAttack = true;
            if (currShipId != shipId) {
                continuousAttack = false;
                m_originalState2Cell = m_currState2Cell;
                updateToBeAttacked(m_originalState2Cell);
                currShipDir = HORIZONTAL;
            }
            m_currState2Cell = p;
        }
        
        if (shotHit && shipDestroyed) {
            attackedCells[p.r][p.c] = 'X';
            continuousAttack = false;
            currShipDir = HORIZONTAL;
            state = 1;
        }
    }
}

void GoodPlayer::updateToBeAttacked(Point p) {
    if (p.r != 0 && attackedCells[p.r-1][p.c] == '.')
        toBeAttacked.push_back(Point(p.r-1,p.c));
    if (p.r != game().rows()-1 && attackedCells[p.r+1][p.c] == '.')
        toBeAttacked.push_back(Point(p.r+1,p.c));
    if (p.c != 0 && attackedCells[p.r][p.c-1] == '.')
        toBeAttacked.push_back(Point(p.r,p.c-1));
    if (p.c != game().cols()-1 && attackedCells[p.r][p.c+1] == '.')
        toBeAttacked.push_back(Point(p.r,p.c+1));
}

Point GoodPlayer::m_randomPoint() {
    int random = rand() % availableCells.size();
    return availableCells[random];
}

void GoodPlayer::removePointFromAvailableCells(Point p) {
    vector<Point>::iterator it = availableCells.begin();
    while (it != availableCells.end()) {
        if (it->r == p.r && it->c == p.c) {
            availableCells.erase(it);
            break;
        } else {
            it++;
        }
    }
}

void GoodPlayer::recordAttackByOpponent(Point p) {}

//*********************************************************************
//  createPlayer
//*********************************************************************

Player* createPlayer(string type, string nm, const Game& g)
{
    static string types[] = {
        "human", "awful", "mediocre", "good"
    };
    
    int pos;
    for (pos = 0; pos != sizeof(types)/sizeof(types[0])  &&
                                                     type != types[pos]; pos++)
        ;
    switch (pos)
    {
      case 0:  return new HumanPlayer(nm, g);
      case 1:  return new AwfulPlayer(nm, g);
      case 2:  return new MediocrePlayer(nm, g);
      case 3:  return new GoodPlayer(nm, g);
      default: return nullptr;
    }
}
