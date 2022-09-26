#include "NameTable.h"
#include <string>
#include <vector>
#include <iostream>

using namespace std;

struct item {
    item(string id, const int& lineNum, const int& scope) : m_id(id), m_lineNum(lineNum), m_scope(scope) {
        m_next = nullptr;
    }
    string m_id;
    int m_lineNum;
    int m_scope;
    item* m_next;
};

class NameTableImpl
{
  private:
    static const int BUCKET = 19997;
    int m_scope;
    // containers
    item* hashTable[BUCKET];
    vector<item**> m_items; // vector of pointers to hashTable's item pointers
  public:
    NameTableImpl();
    void enterScope();
    bool exitScope();
    bool declare(const string& id, int lineNum);
    int find(const string& id) const;
    // auxiliary functions
    int hashFunction(const string& id) const;
    int lookup(const string& id, int& scope) const;
    ~NameTableImpl();
};

NameTableImpl::NameTableImpl() {
    m_scope = 0;
    for (int i = 0; i < BUCKET; i++) {
        hashTable[i] = nullptr;
    }
}

int NameTableImpl::hashFunction(const string& id) const {
    size_t h(0);
    size_t size = id.length();
    if (size > 3) {
        size = 3;
    }
    for (size_t i = 0; i < size; i++) {
        h = (h << 6) ^ (h >> 26) ^ id[i];
    }
    return h % BUCKET;
}

// prevent memory leaks
NameTableImpl::~NameTableImpl()
{
    size_t k = m_items.size();
    while (k > 0) {
        k--;
        if (m_items[k] != nullptr) {
            delete *m_items[k];
        }
    }
}

void NameTableImpl::enterScope() // O(1)
{
    m_items.push_back(nullptr); // start of new scope
    m_scope++;
}

bool NameTableImpl::exitScope() // O(N)
{
    size_t k = m_items.size();
    while (!m_items.empty())
    {
        k--;
        if (m_items[k] == nullptr) { // we've reached the start of a scope
            m_items.pop_back();
            m_scope--;
            return true;
        }
        *m_items[k] = nullptr; // marks items in hashTable nullptr too, so declare knows where m_next should be
        delete *m_items[k];
        m_items.pop_back();
    }
    return false;
}

bool NameTableImpl::declare(const string& id, int lineNum) // O(1)
{
    if (id.empty())
        return false;
    
    int scope;
    if (lookup(id, scope) != -1 && scope == m_scope)
        return false;
    
    int pos = hashFunction(id);
    item* q = hashTable[pos];
    if (q == nullptr) { // empty bucket
        hashTable[pos] = new item(id, lineNum, m_scope);
        m_items.push_back(&hashTable[pos]);
    } else { // collision
        while (q->m_next != nullptr)
            q = q->m_next;
        q->m_next = new item(id, lineNum, m_scope);
        m_items.push_back(&(q->m_next));
    }
    return true;
}

int NameTableImpl::find(const string& id) const // O(1)
{
    if (id.empty())
        return -1;
    
    int scope;
    return lookup(id, scope);
}

int NameTableImpl::lookup(const string& id, int& scope) const {
    int line = -1;
    int pos = hashFunction(id);
    item* p = hashTable[pos];
    while (p != nullptr) {
        if (p->m_id == id) {
            scope = p->m_scope;
            line = p->m_lineNum;
        }
        p = p->m_next;
    }
    return line;
}

//*********** NameTable functions **************

// For the most part, these functions simply delegate to NameTableImpl's
// functions.

NameTable::NameTable()
{
    m_impl = new NameTableImpl;
}

NameTable::~NameTable()
{
    delete m_impl;
}

void NameTable::enterScope()
{
    m_impl->enterScope();
}

bool NameTable::exitScope()
{
    return m_impl->exitScope();
}

bool NameTable::declare(const string& id, int lineNum)
{
    return m_impl->declare(id, lineNum);
}

int NameTable::find(const string& id) const
{
    return m_impl->find(id);
}
