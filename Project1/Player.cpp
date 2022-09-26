//
//  Player.cpp
//  garks
//
//  Created by Stella Huang on 3/31/22.
//

#include "Player.h"
#include "Mesa.h"
#include <iostream>

///////////////////////////////////////////////////////////////////////////
//  Player implementations
///////////////////////////////////////////////////////////////////////////

Player::Player(Mesa* mp, int r, int c)
 : m_mesa(mp), m_row(r), m_col(c), m_age(0), m_dead(false)
{
    if (mp == nullptr)
    {
        std::cout << "***** The player must be created in some Mesa!" << std::endl;
        exit(1);
    }
    if (r < 1  ||  r > mp->rows()  ||  c < 1  ||  c > mp->cols())
    {
        std::cout << "**** Player created with invalid coordinates (" << r
             << "," << c << ")!" << std::endl;
        exit(1);
    }
}

int Player::row() const
{
    return m_row;
}

int Player::col() const
{
    return m_col;
}

int Player::age() const
{
    return m_age;
}

void Player::stand()
{
    m_age++;
}

void Player::moveOrAttack(int dir)
{
    m_age++;
    int r = m_row;
    int c = m_col;
    if (m_mesa->determineNewPosition(r, c, dir))
    {
        if (m_mesa->numGarksAt(r, c) > 0)
            m_mesa->attackGarkAt(r, c, dir);
        else
        {
            m_row = r;
            m_col = c;
        }
    }
}

bool Player::isDead() const
{
    return m_dead;
}

void Player::setDead()
{
    m_dead = true;
}
