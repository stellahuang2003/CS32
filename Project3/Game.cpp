#include "Game.h"
#include "Board.h"
#include "Player.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <cctype>

using namespace std;

class GameImpl
{
  public:
    GameImpl(int nRows, int nCols);
    int rows() const;
    int cols() const;
    bool isValid(Point p) const;
    Point randomPoint() const;
    bool addShip(int length, char symbol, string name);
    int nShips() const;
    int shipLength(int shipId) const;
    char shipSymbol(int shipId) const;
    string shipName(int shipId) const;
    Player* play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause);
  private:
    int m_nRows;
    int m_nCols;
    int m_nShips;
    vector<int> m_shipLengths; // ask TA if static array is okay
    vector<char> m_shipSymbols;
    vector<string> m_shipNames;
};

void waitForEnter()
{
    cout << "Press enter to continue: ";
    cin.ignore(10000, '\n');
}

GameImpl::GameImpl(int nRows, int nCols)
{
    m_nRows = nRows;
    m_nCols = nCols;
    m_nShips = 0;
}

int GameImpl::rows() const
{
    return m_nRows;
}

int GameImpl::cols() const
{
    return m_nCols;
}

bool GameImpl::isValid(Point p) const
{
    return p.r >= 0  &&  p.r < rows()  &&  p.c >= 0  &&  p.c < cols();
}

Point GameImpl::randomPoint() const
{
    return Point(randInt(rows()), randInt(cols()));
}

bool GameImpl::addShip(int length, char symbol, string name)
{
    if (symbol != 'X' && symbol != 'o' && symbol != '.') {
        m_shipLengths.push_back(length);
        m_shipSymbols.push_back(symbol);
        m_shipNames.push_back(name);
        m_nShips++;
        return true;
    }
        
    return false;
}

int GameImpl::nShips() const
{
    return m_nShips;
}

int GameImpl::shipLength(int shipId) const
{
    return m_shipLengths[shipId];
}

char GameImpl::shipSymbol(int shipId) const
{
    return m_shipSymbols[shipId];
}

string GameImpl::shipName(int shipId) const
{
    return m_shipNames[shipId];
}

Player* GameImpl::play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause)
{
    Player* winner;
    bool p1shipsPlaced = p1->placeShips(b1);
    bool p2ShipsPlaced = p2->placeShips(b2);

    if (!p1shipsPlaced || !p2ShipsPlaced)
        return nullptr;
    
    bool validShot = false;
    bool shotHit = false;
    bool shipDestroyed = false;
    int shipId;
    
    while (true) {
        
        // Player 1's turn
        cout << p1->name() << "'s turn. Board for " << p2->name() << ":\n";
        b2.display(p1->isHuman()); // shotsOnly true when isHuman()
        
        Point aPoint = p1->recommendAttack();
        validShot = b2.attack(aPoint, shotHit, shipDestroyed, shipId);

        if (validShot) {
            if (!shotHit) {
                cout << p1->name() << " attacked (" << aPoint.r << "," << aPoint.c << ") and missed, resulting in:\n";
            }
            if (shotHit && !shipDestroyed) {
                cout << p1->name() << " attacked (" << aPoint.r << "," << aPoint.c << ") and hit something, resulting in:\n";
            }
            if (shotHit && shipDestroyed) {
                cout << p1->name() << " attacked (" << aPoint.r << "," << aPoint.c << ") and destroyed the " << shipName(shipId) << ", resulting in:\n";
            }
            b2.display(p1->isHuman());
        } else { // attack was invalid
            cout << p1->name() << " wasted a shot at (" << aPoint.r << "," << aPoint.c << ").\n";
        }
        if (b2.allShipsDestroyed()) {
            winner = p1;
            cout << p1->name() << " wins!\n";
            if (p2->isHuman()) {
                cout << "Here is where " << p1->name() << "'s ships were:\n";
                b1.display(false);
            }
            break;
        }
        p1->recordAttackResult(aPoint, validShot, shotHit, shipDestroyed, shipId);
        p2->recordAttackByOpponent(aPoint);
        if (shouldPause) {
            waitForEnter();
        }
        
        // Player 2's turn
        cout << p2->name() << "'s turn. Board for " << p1->name() << ":\n";
        b1.display(p2->isHuman());

        aPoint = p2->recommendAttack();
        validShot = b1.attack(aPoint, shotHit, shipDestroyed, shipId);
        
        if (validShot) {
            if (!shotHit) {
                cout << p2->name() << " attacked (" << aPoint.r << "," << aPoint.c << ") and missed, resulting in: \n";
            }
            if (shotHit && !shipDestroyed) {
                cout << p2->name() << " attacked (" << aPoint.r << "," << aPoint.c << ") and hit something, resulting in:\n";
            }
            if (shotHit && shipDestroyed) {
                cout << p2->name() << " attacked (" << aPoint.r << "," << aPoint.c << ") and destroyed the " << shipName(shipId) << ", resulting in:\n";
            }
            b1.display(p2->isHuman());
        } else {
            cout << p2->name() << " wasted a shot at (" << aPoint.r << "," << aPoint.c << ").\n";
        }
        if (b1.allShipsDestroyed()) {
            winner = p2;
            cout << p2->name() << " wins!\n";
            if (p1->isHuman()) {
                cout << "Here is where " << p2->name() << "'s ships were:\n";
                b2.display(false);
            }
            break;
        }
        
        p2->recordAttackResult(aPoint, validShot, shotHit, shipDestroyed, shipId);
        p1->recordAttackByOpponent(aPoint);
        if (shouldPause) {
            waitForEnter();
        }
    }
    
    return winner;
}

//******************** Game functions *******************************

// These functions for the most part simply delegate to GameImpl's functions.
// You probably don't want to change any of the code from this point down.

Game::Game(int nRows, int nCols)
{
    if (nRows < 1  ||  nRows > MAXROWS)
    {
        cout << "Number of rows must be >= 1 and <= " << MAXROWS << endl;
        exit(1);
    }
    if (nCols < 1  ||  nCols > MAXCOLS)
    {
        cout << "Number of columns must be >= 1 and <= " << MAXCOLS << endl;
        exit(1);
    }
    m_impl = new GameImpl(nRows, nCols);
}

Game::~Game()
{
    delete m_impl;
}

int Game::rows() const
{
    return m_impl->rows();
}

int Game::cols() const
{
    return m_impl->cols();
}

bool Game::isValid(Point p) const
{
    return m_impl->isValid(p);
}

Point Game::randomPoint() const
{
    return m_impl->randomPoint();
}

bool Game::addShip(int length, char symbol, string name)
{
    if (length < 1)
    {
        cout << "Bad ship length " << length << "; it must be >= 1" << endl;
        return false;
    }
    if (length > rows()  &&  length > cols())
    {
        cout << "Bad ship length " << length << "; it won't fit on the board"
             << endl;
        return false;
    }
    if (!isascii(symbol)  ||  !isprint(symbol))
    {
        cout << "Unprintable character with decimal value " << symbol
             << " must not be used as a ship symbol" << endl;
        return false;
    }
    if (symbol == 'X'  ||  symbol == '.'  ||  symbol == 'o')
    {
        cout << "Character " << symbol << " must not be used as a ship symbol"
             << endl;
        return false;
    }
    int totalOfLengths = 0;
    for (int s = 0; s < nShips(); s++)
    {
        totalOfLengths += shipLength(s);
        if (shipSymbol(s) == symbol)
        {
            cout << "Ship symbol " << symbol
                 << " must not be used for more than one ship" << endl;
            return false;
        }
    }
    if (totalOfLengths + length > rows() * cols())
    {
        cout << "Board is too small to fit all ships" << endl;
        return false;
    }
    return m_impl->addShip(length, symbol, name);
}

int Game::nShips() const
{
    return m_impl->nShips();
}

int Game::shipLength(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipLength(shipId);
}

char Game::shipSymbol(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipSymbol(shipId);
}

string Game::shipName(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipName(shipId);
}

Player* Game::play(Player* p1, Player* p2, bool shouldPause)
{
    if (p1 == nullptr  ||  p2 == nullptr  ||  nShips() == 0)
        return nullptr;
    Board b1(*this);
    Board b2(*this);
    return m_impl->play(p1, p2, b1, b2, shouldPause);
}

