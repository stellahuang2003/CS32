//
//  Game.h
//  garks
//
//  Created by Stella Huang on 3/31/22.
//

#ifndef GAME_H
#define GAME_H

class Mesa;

class Game
{
  public:
        // Constructor/destructor
    Game(int rows, int cols, int nGarks);
    ~Game();

        // Mutators
    void play();

  private:
    Mesa* m_mesa;
};

#endif
