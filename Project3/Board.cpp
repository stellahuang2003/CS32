#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>

using namespace std;

class BoardImpl
{
  public:
    BoardImpl(const Game& g);
    void clear();
    void block();
    void unblock();
    bool placeShip(Point topOrLeft, int shipId, Direction dir);
    bool unplaceShip(Point topOrLeft, int shipId, Direction dir);
    void display(bool shotsOnly) const;
    bool attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId);
    bool allShipsDestroyed() const;

  private:
      // TODO:  Decide what private members you need.  Here's one that's likely
      //        to be useful:
    char m_board[MAXROWS][MAXCOLS];
    const Game& m_game;
};

BoardImpl::BoardImpl(const Game& g)
 : m_game(g)
{
    clear();
}

void BoardImpl::clear()
{
    for (int r = 0; r < m_game.rows(); r++)
        for (int c = 0; c < m_game.cols(); c++)
            m_board[r][c] = '.';
}

void BoardImpl::block()
{
    int i = 0;
    while (i < (m_game.rows() * m_game.cols() / 2)) {
        Point randPoint = m_game.randomPoint();
        if (m_board[randPoint.r][randPoint.c] != '#') {
            m_board[randPoint.r][randPoint.c] = '#';
            i++;
        }
    }
}

void BoardImpl::unblock()
{
    for (int r = 0; r < m_game.rows(); r++) {
        for (int c = 0; c < m_game.cols(); c++) {
            if (m_board[r][c] == '#')
                m_board[r][c] = '.'; // reset to default
        }
    }
}

bool BoardImpl::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    // check if shipId is valid
    if (shipId < 0 || shipId > (m_game.nShips() - 1)) {
        return false;
    }
    
    // check that topOrLeft's row and col are valid
    if (topOrLeft.r < 0 || topOrLeft.r >= m_game.rows()
        || topOrLeft.c < 0 || topOrLeft.c >= m_game.cols()) {
        return false;
    }
    
    if (dir == HORIZONTAL) {
        for (int i = 0; i < m_game.shipLength(shipId); i++) {
            int tempCol = topOrLeft.c + i;
            if (tempCol < m_game.cols()) { // check that ship is fully inside board
                // accounts for blocked and already-occupied spaces
                if (m_board[topOrLeft.r][tempCol] != '.') {
                    return false;
                }
            } else {
                return false;
            }
        }
    } else { // dir == VERTICAL
        for (int i = 0; i < m_game.shipLength(shipId); i++) {
            int tempRow = topOrLeft.r + i;
            if (tempRow < m_game.rows()) { // check that ship is fully inside board
                // accounts for blocked and already-occupied spaces
                if (m_board[tempRow][topOrLeft.c] != '.')
                    return false;
            } else {
                return false;
            }
        }
    }
    
    // check that ship has not been placed on board yet
    for (int r = 0; r < m_game.rows(); r++) {
        for (int c = 0; c < m_game.cols(); c++) {
            if(m_board[r][c] == m_game.shipSymbol(shipId))
                return false;
        }
    }
    
    // passed all conditions so board (and game) is updated w new ship
    if (dir == HORIZONTAL) {
        for (int i = 0; i < m_game.shipLength(shipId); i++)
            m_board[topOrLeft.r][topOrLeft.c + i] = m_game.shipSymbol(shipId);
    } else { // dir == VERTICAL
        for (int i = 0; i < m_game.shipLength(shipId); i++)
            m_board[topOrLeft.r + i][topOrLeft.c] = m_game.shipSymbol(shipId);
    }
    
    return true;
}

bool BoardImpl::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    // check if shipId is valid
    if (shipId < 0 || shipId > (m_game.nShips() - 1))
        return false;
    
    // check that board contains entire ship at indicated locations
    if (dir == HORIZONTAL) {
        for (int i = 0; i < m_game.shipLength(shipId); i++) {
            if (m_board[topOrLeft.r][topOrLeft.c + i] != m_game.shipSymbol(shipId))
                return false;
        }
    } else { // dir == VERTICAL
        for (int i = 0; i < m_game.shipLength(shipId); i++) {
            if (m_board[topOrLeft.r + 1][topOrLeft.c] != m_game.shipSymbol(shipId))
                return false;
        }
    }
    
    // passed previous conditions, so proceed with removing the ship
    if (dir == HORIZONTAL) {
        for (int i = 0; i < m_game.shipLength(shipId); i++)
            m_board[topOrLeft.r][topOrLeft.c + i] = '.';
    } else { // dir == VERTICAL
        for (int i = 0; i < m_game.shipLength(shipId); i++)
            m_board[topOrLeft.r + i][topOrLeft.c] = '.';
    }

    return true;
}

void BoardImpl::display(bool shotsOnly) const // ask if other ppl had to do this std thing
{
    // first line
    std::cout << std::string("  ");
    for (int c = 0; c < m_game.cols(); c++)
        std::cout << c;
    std::cout << std::endl;
    
    // rest of the lines
    for (int r = 0; r < m_game.rows(); r++) {
        std::cout << r << std::string(" ");
        for (int c = 0; c < m_game.cols(); c++) {
            switch (m_board[r][c]) {
                case 'X':
                case '.':
                case 'o':
                    std::cout << m_board[r][c];
                    break;
                default: // undamaged ship segment
                    if (shotsOnly)
                        std::cout << '.';
                    else
                        std::cout << m_board[r][c];
                    break;
            }
        }
        std::cout << std::endl;
    }
}

bool BoardImpl::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    // checks if attack is invalid
    if (p.r < 0 || p.r >= m_game.rows() || p.c < 0 || p.c >= m_game.cols())
        return false;
    if (m_board[p.r][p.c] == 'X' || m_board[p.r][p.c] == 'o')
        return false;
    
    // if undamaged segment of ship is at position p
    char shipSymbol;
    if (m_board[p.r][p.c] != '.') { // check if this needs to be updated to 'X' here
        shipSymbol = m_board[p.r][p.c];
        m_board[p.r][p.c] = 'X';
        shotHit = true;
    } else {
        m_board[p.r][p.c] = 'o';
        shotHit = false;
        return true;
    }
    
    // update shipDestroyed
    int numSymbols = 0;
    for (int r = 0; r < m_game.rows(); r++) {
        for (int c = 0; c < m_game.cols(); c++) {
            if (m_board[r][c] == shipSymbol)
                numSymbols++;
            if (numSymbols == 1)
                break;
        }
    }
    if (numSymbols == 0) {
        shipDestroyed = true;
    } else {
        shipDestroyed = false;
        return true;
    }
    
    // update shipId
    for (int i = 0; i < m_game.nShips(); i++) {
        if (m_game.shipSymbol(i) == shipSymbol) {
            shipId = i;
            break;
        }
    }
    
    return true;
}

bool BoardImpl::allShipsDestroyed() const
{
    for (int r = 0; r < m_game.rows(); r++) {
        for (int c = 0; c < m_game.cols(); c++) {
            switch (m_board[r][c]) {
                case '.':
                case 'o':
                case 'X':
                    break;
                default:
                    return false;
            }
        }
    }
    return true;
}

//******************** Board functions ********************************

// These functions simply delegate to BoardImpl's functions.
// You probably don't want to change any of this code.

Board::Board(const Game& g)
{
    m_impl = new BoardImpl(g);
}

Board::~Board()
{
    delete m_impl;
}

void Board::clear()
{
    m_impl->clear();
}

void Board::block()
{
    return m_impl->block();
}

void Board::unblock()
{
    return m_impl->unblock();
}

bool Board::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->placeShip(topOrLeft, shipId, dir);
}

bool Board::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->unplaceShip(topOrLeft, shipId, dir);
}

void Board::display(bool shotsOnly) const
{
    m_impl->display(shotsOnly);
}

bool Board::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    return m_impl->attack(p, shotHit, shipDestroyed, shipId);
}

bool Board::allShipsDestroyed() const
{
    return m_impl->allShipsDestroyed();
}
