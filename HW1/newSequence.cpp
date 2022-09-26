// FINAL

#include "newSequence.h"
Sequence::Sequence()
{
    m_size = 0;
    m_max = DEFAULT_MAX_ITEMS;
    m_array = new ItemType[m_max];
}

Sequence::Sequence(int max)
{
    m_size = 0;
    m_max = max;
    m_array = new ItemType[m_max];
} // new

Sequence::Sequence(const Sequence& other)
{
    m_size = other.m_size;
    m_max = other.m_max;
    m_array = new ItemType[m_max];
    for (int i = 0; i < m_size; i++) {
        m_array[i] = other.m_array[i];
    }
} // new copy constructor

Sequence& Sequence::operator=(const Sequence& rhs)
{
    if (this != &rhs) {
        Sequence temp(rhs);
        swap(temp);
    }
    return *this;
} // new assignment operator

Sequence::~Sequence()
{
    delete [] m_array;
} // new destructor

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
    if (size() < m_max && 0 <= pos && pos <= size()) {
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
    if (size() >= m_max) {
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

void Sequence::swap(Sequence& other) {
    // swap size
    int tempSize = other.m_size;
    other.m_size = this->m_size;
    this->m_size = tempSize;
    // swap max
    int tempMax = other.m_max;
    other.m_max = this->m_max;
    this->m_max = tempMax;
    // swap pointers
    ItemType* tempPtr = other.m_array;
    other.m_array = this->m_array;
    this->m_array = tempPtr;
}

