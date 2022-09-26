// FINAL

#ifndef NEWSEQUENCE_H
#define NEWSEQUENCE_H

#include <iostream>

typedef unsigned long ItemType;
const int DEFAULT_MAX_ITEMS = 180;

class Sequence
{
  public:
    Sequence(); // Create an empty sequence (i.e., one whose size() is 0).
    
    Sequence(int max); // new
    Sequence(const Sequence& other); // new copy constructor
    Sequence& operator=(const Sequence& rhs); // new assignment operator
    ~Sequence(); // new destructor
    
    bool empty() const;
    int size() const;
    int insert(int pos, const ItemType& value);
    int insert(const ItemType& value);
    bool erase(int pos);
    int remove(const ItemType& value);
    bool get(int pos, ItemType& value) const;
    bool set(int pos, const ItemType& value);
    int find(const ItemType& value) const;
    void swap(Sequence& other);
  
  private:
    int m_size;
    int m_max;
    ItemType* m_array;
};

#endif
