//FINAL

#ifndef LEVELLIST_H
#define LEVELLIST_H

#include "Sequence.h"
#include <limits>

const ItemType NO_LEVEL = std::numeric_limits<unsigned long>::max();

class LevelList
{
  public:
    LevelList();       // Create an empty level list.

    bool add(ItemType level);
      // If the level is valid (a value from 30 to 400) and the level list
      // has room for it, add it to the level list and return true.
      // Otherwise, leave the level list unchanged and return false.

    bool remove(ItemType level);
      // Remove one instance of the specified level from the level list.
      // Return true if a level was removed; otherwise false.

    int size() const;  // Return the number of levels in the list.

    ItemType minimum() const;
      // Return the lowest-valued level in the level list.  If the list is
      // empty, return NO_LEVEL.

    ItemType maximum() const;
      // Return the highest-valued level in the level list.  If the list is
      // empty, return NO_LEVEL.

  private:
    Sequence m_sequence;
    
};

#endif 
