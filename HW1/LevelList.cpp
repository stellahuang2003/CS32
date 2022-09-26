//FINAL

#include "LevelList.h"

LevelList::LevelList() : m_sequence() {}

bool LevelList::add(ItemType level)
{
    if (level >= 30 && level <= 400 && m_sequence.size() < DEFAULT_MAX_ITEMS) {
        m_sequence.insert(level);
        return true;
    }
    return false;
}
  
bool LevelList::remove(ItemType level)
{
    int pos = m_sequence.find(level);
    return m_sequence.erase(pos); // check
}

int LevelList::size() const
{
    return m_sequence.size();
}

ItemType LevelList::minimum() const
{
    if (!m_sequence.empty()) {
        ItemType lowest;
        m_sequence.get(0, lowest);
        ItemType tempLowest;
        for (int i = 0; i < m_sequence.size(); i++) {
            m_sequence.get(i, tempLowest);
            if (tempLowest < lowest) {
                lowest = tempLowest;
            }
        }
        return lowest;
    }
    return NO_LEVEL;
}

ItemType LevelList::maximum() const
{
    if (!m_sequence.empty()) {
        ItemType highest;
        m_sequence.get(0, highest);
        ItemType tempHighest;
        for (int i = 0; i < m_sequence.size(); i++) {
            m_sequence.get(i, tempHighest);
            if (tempHighest > highest) {
                highest = tempHighest;
            }
        }
        return highest;
    }
    return NO_LEVEL;
}
