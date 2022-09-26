//
//  Sequence.hpp
//  Project2
//
//  Created by Stella Huang on 4/14/22.
//

#ifndef SEQUENCE_H
#define SEQUENCE_H
#include <string> // make sure to comment out

//using ItemType = unsigned long;
using ItemType = std::string;

class Sequence
{
  public:
    Sequence();
    
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
    void dump() const;
  private:
    struct Node {
        ItemType m_value;
        Node* previous;
        Node* next;
    };
    Node* head;
    int m_size = 0;
};

int subsequence(const Sequence& seq1, const Sequence& seq2);
void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result);

#endif
