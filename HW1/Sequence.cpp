//FINAL

#include "Sequence.h"

Sequence::Sequence()
{
    m_size = 0;
}

bool Sequence::empty() const
{
    return (m_size == 0);
}

int Sequence::size() const
{
    return m_size;
}

int Sequence::insert(int pos, const ItemType& value)
{
    if (size() < DEFAULT_MAX_ITEMS && 0 <= pos && pos <= size()) {
        if (pos == size()) {
            m_array[size()] = value;
        } else if (empty()) {
            m_array[0] = value;
        } else {
            for (int i = size(); i > pos; i--) {
                m_array[i] = m_array[i-1];
            }
            m_array[pos] = value;
        }
        m_size++;
        return pos;
    }

    return -1;
}

int Sequence::insert(const ItemType& value)
{
    if (size() >= DEFAULT_MAX_ITEMS) {
        return -1;
    }
    int p = size();
    for (int i = 0; i < size(); i++) {
        if (value <= m_array[i]) {
            p = i;
            insert(i, value);
            break;
        }
    }
    if (p == size()) { // for cases where p is not less than the other values
        insert(p, value);
    }
    return p;
}
 
bool Sequence::erase(int pos)
{
    if (pos == size() - 1) { // if it's last element or first element in array size 1
        m_array[pos] = ItemType();
        m_size--;
        return true;
    }
    if (!empty() && 0 <= pos && pos < size()) {
        for (int i = pos; i < size(); i++) {
            m_array[i] = m_array[i+1];
        }
        m_size--;
        return true;
    }
    return false;
}

int Sequence::remove(const ItemType& value)
{
    int numRemoved = 0;
    for (int i = 0; i < size(); i++) {
        if (m_array[i] == value) {
            erase(i);
            numRemoved++;
            i--;
        }
    }
    return numRemoved;
}

bool Sequence::get(int pos, ItemType& value) const
{
    if (!empty() && 0 <= pos && pos < size()) {
        value = m_array[pos];
        return true;
    }
    return false;
}

bool Sequence::set(int pos, const ItemType& value)
{
    if (!empty() && 0 <= pos && pos < size()) {
        m_array[pos] = value; // check
        return true;
    }
    return false;
}

int Sequence::find(const ItemType& value) const
{
    int p = -1;
    for (int i = 0; i < size(); i++) {
        if (m_array[i] == value) {
            p = i;
            break;
        }
    }
    return p;
}

void Sequence::swap(Sequence& other)
{
    int length = 0;
    if (size() < other.size()) {
        length = other.size();
    } else {
        length = size();
    }
    for (int i = 0; i < length; i++) {
        ItemType temp = m_array[i];
        m_array[i] = other.m_array[i];
        other.m_array[i] = temp;
    }
    int tempSize = size();
    m_size = other.size();
    other.m_size = tempSize;
}

void Sequence::dump() const
{
    for (int i = 0; i < size(); i++) {
        std::cerr << m_array[i];
    }
    std::cerr << std::endl;
}
