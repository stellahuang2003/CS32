//
//  Sequence.cpp
//  Project2
//
//  Created by Stella Huang on 4/14/22.
//

#include "Sequence.h"
#include <iostream>

Sequence::Sequence() {
    head = new Node;
    head->next = head;
    head->previous = head;
    
    m_size = 0;
}

Sequence::Sequence(const Sequence& other) { // check
    m_size = other.m_size;
    
    if (other.empty()) {
        head = new Node;
        head->next = head;
        head->previous = head;
        return;
    }
    // for start of linked list
    head = new Node;
    head->next = head;
    head->previous = head;

    Node* tempOther = other.head->next;
    Node* temp = head->next;

    // only for linked lists with m_size > 1
    for (; tempOther != other.head; tempOther = tempOther->next) {
        Node* newGuy = new Node;
        newGuy->m_value = tempOther->m_value;
        
        newGuy->next = temp->next; // this and the line below insert newGuy in
        newGuy->previous = temp;
        temp->next = newGuy;
        head->previous = newGuy; // newGuy is at the end of the list so it is head's new previous
        
        temp = temp->next;
    }
    // we've reached other's end
} // new copy constructor

Sequence& Sequence::operator=(const Sequence& rhs) {
    if (this != &rhs) {
        Sequence temp(rhs);
        swap(temp);
    }
    return *this;
} // new assignment operator

Sequence::~Sequence() {
        
    while (!empty()) {
        erase(0);
    }
    
    delete head;

} // new destructor

bool Sequence::empty() const {
    return (m_size == 0);
}

int Sequence::size() const {
    return m_size;
}

int Sequence::insert(int pos, const ItemType& value) {
    if (pos < 0 || pos > size())
        return -1;
    
    if (empty()) {
        Node* newGuy = new Node;
        newGuy->m_value = value;
        
        newGuy->previous = head;
        newGuy->next = head;
        head->next = newGuy;
        head->previous = newGuy;
        m_size++;
        return pos;
    }
    // in between - doesn't work for end of list bc index for pos doesn't exist
    int index = 0;
    for (Node* p = head->next; p != head; p = p->next) {
        if (pos == index) {
            Node* newGuy = new Node;
            newGuy->m_value = value;
            
            newGuy->next = p;
            newGuy->previous = p->previous;
            p->previous->next = newGuy;
            p->previous = newGuy;
            m_size++;
            return pos;
        }
        index++;
    }
    // end of list
    Node* p = head->next;
    while (p->next != head) { // brings us to end of list
        p = p->next;
    }
    Node* newGuy = new Node;
    newGuy->m_value = value;
    
    newGuy->previous = p;
    newGuy->next = head;
    p->next = newGuy;
    head->previous = newGuy;
    m_size++;
    return pos;
}

int Sequence::insert(const ItemType& value) {
    int index = 0;
    for (Node* p = head->next; p != head; p = p->next) {
        if (p->m_value >= value) {
            break;
        }
        index++;
    }
    insert(index, value);
    
    return index;
}

bool Sequence::erase(int pos) {
    if (pos < 0  ||  pos >= size() || empty())
        return false;
    
    if (size() == 1) {
        Node* temp = head->next; // splices temp out of list
        head->next = head; // head gets reset to empty
        head->previous = head;
        
        temp->next = nullptr;
        temp->previous = nullptr;
        
        delete temp;
        m_size--;
        return true;
    }
    
    int index = 0;
    for (Node* p = head->next; p != head; p = p->next) {
        if (index == pos) {
            (p->previous)->next = p->next; // splices p (temp) out of list
            (p->next)->previous = p->previous; // p gets skipped over so now no element in the list points at p
            
            p->next = nullptr; // special defense to make sure whatever p's pointing to doesn't get deleted
            p->previous = nullptr;
            
            delete p;
            m_size--;
            break;
        }
        index++;
    }
    return true;
    
}

int Sequence::remove(const ItemType& value) {
    int numRemoved = 0;
    int keepPos;
    while (true) {
        keepPos = find(value);
        if(keepPos == -1)
            break;
        erase(keepPos);
        numRemoved++;
    }

    return numRemoved;
}

bool Sequence::get(int pos, ItemType& value) const {
    if (pos < 0  ||  pos >= size() || empty())
        return false;
        
    int index = 0;
    for (Node* p = head->next; p != head; p = p->next) {
        if (index == pos) {
            value = p->m_value;
            break;
        }
        index++;
    }
    
    return true;
}

bool Sequence::set(int pos, const ItemType& value) {
    if (pos < 0  ||  pos >= size() || empty())
        return false;
        
    int index = 0;
    for (Node* p = head->next; p != head; p = p->next) {
        if (index == pos) {
            p->m_value = value;
            break;
        }
        index++;
    }
    
    return true;
}

int Sequence::find(const ItemType& value) const {
    if (empty())
        return -1;
        
    int index = 0;
    for (Node* p = head->next; p != head; p = p->next) {
        if (p->m_value == value) {
            return index;
        }
        index++;
    }
    
    return -1;
}

void Sequence::swap(Sequence& other) {
    // swap nodes
    Node* tempHead = head;
    head = other.head;
    other.head = tempHead;
    
    // swap sizes
    int tempSize = size();
    m_size = other.size();
    other.m_size = tempSize;
}

// nonmember functions
int subsequence(const Sequence& seq1, const Sequence& seq2) {
    if (seq2.empty() || seq2.size() > seq1.size())
        return -1;
    ItemType value1;
    ItemType value2;
    for (int i = 0; i < seq1.size(); i++) {
        bool same = true;
        for (int j = 0; j < seq2.size(); j++) { // parse through segment of seq1 to see if seq2 matches
            seq1.get(i+j, value1);
            seq2.get(j, value2);
            if (value1 != value2) {
                same = false; // if it doesn't match then they aren't the same
            }
        }
        if (same) // if they do match and same remains true then return that index
            return i;
    }
    return -1;
}

void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result) {
    if (!result.empty()) {
        while (!result.empty()) {
            result.erase(0);
        }
    }

    ItemType value;
    int index_largerArray = 0; // used to keep track of index for larger sequence
    int i_size = 0; // take the size of the smaller sequence so we don't go outta bounds
    int index_result = 0; // using this to properly increment index for result

    if (seq1.size() > seq2.size()) {
        i_size = seq2.size();
    } else {
        i_size = seq1.size();
    }

    for (int i = 0; i < i_size; i++) {
        seq1.get(i, value);
        result.insert(index_result, value);
        index_result++; // result's index is incremented because new element is added in

        seq2.get(i, value);
        result.insert(index_result, value);
        index_result++;

        index_largerArray++; // the larger array's index gets incremented - helps keep track of how many elements of the larger array we've inserted
    }

    if (seq1.size() > seq2.size()) { // this if-else addresses cases where seq1 and seq2 aren't the same size and have extra elements to be appended to result's end
        for (int i = index_largerArray; i < seq1.size(); i++) {
            seq1.get(i, value);
            result.insert(index_result, value);
            index_result++;
        }
    } else if (seq1.size() < seq2.size()) {
        for (int i = index_largerArray; i < seq2.size(); i++) {
            seq2.get(i, value);
            result.insert(index_result, value);
            index_result++;
        }
    }
}

void Sequence::dump() const {
    std::cerr << "m_size = " << size() << std::endl;
    if (!empty()) {
        for (Node* p = head->next; p != head; p = p->next) {
            std::cerr << p->m_value << std::endl;
        }
    }
}
