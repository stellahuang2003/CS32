//#include "NameTable.h"
//#include <string>
//#include <vector>
//#include <iostream>
//#include <list>
//#include <stack>
//
//using namespace std;
//
//struct item {
//    item(string id, int lineNum, int scope) : m_id(id), m_lineNum(lineNum), m_scope(scope) {
//        m_next = nullptr;
//    }
//    string m_id;
//    int m_lineNum;
//    int m_scope;
//    item* m_next;
//};
//
//class HashTable {
//  private:
//    static const int BUCKET = 19997;
//    int hashFunction(const string& id) const;
////    list<item*> hashTable[BUCKET];
//    item* hashTable[BUCKET];
//    // for debugging purposes
////    int numTimesUsed[BUCKET];
//  public:
//    HashTable() {
//        for (int i = 0; i < BUCKET; i++) {
//            hashTable[i] = nullptr;
//            // COMMENT OUT LATER
////            numTimesUsed[i] = 0;
//            // COMMENT OUT LATER
//        }
//    }
//    item** insert(const string& id, const int& lineNum, const int& scope);
//    int lookup(const string& id, int& scope);
//    // for debugging purposes
////    ~HashTable() {
////        for (int i = 0; i < BUCKET; i++)
////            cout << "BUCKET " << i << ": " << numTimesUsed[i] << endl;
////    }
//};
//
//int HashTable::hashFunction(const string& id) const {
////    int sm = 0;
////    for(int i = 0; i < id.length(); i++) {
////        sm += id[i];
////        sm *= 2;
////    }
////    sm = sm % BUCKET;
////    return sm;
//    size_t h(0);
//    size_t size = id.length();
//    if (size > 3) {
//        size = 3;
//    }
//    for (size_t i=0; i<size; i++) {
//      h = (h << 6) ^ (h >> 26) ^ id[i];
//    }
//    return h % BUCKET;
//}
//
//item** HashTable::insert(const string& id, const int& lineNum, const int& scope) {
//    int pos = hashFunction(id);
//    item* p = hashTable[pos];
//    // COMMENT OUT LATER
////  numTimesUsed[pos]++;
//    // COMMENT OUT LATER
//    // empty bucket
//    if (p == nullptr) {
//        hashTable[pos] = new item(id, lineNum, scope);
//        return &hashTable[pos];
//    }
//    // collision
//    while (p->m_next != nullptr)
//        p = p->m_next;
//    p->m_next= new item(id, lineNum, scope);
//    return &(p->m_next);
////    while (!hashTable[pos].empty() && *hashTable[pos].rbegin() == nullptr) {
////        hashTable[pos].pop_back();
////    }
////    hashTable[pos].push_back(new item(id, lineNum, scope));
////    return &(*hashTable[pos].rbegin());
//}
//
//int HashTable::lookup(const string& id, int& scope) {
//    int line = -1;
//    int pos = hashFunction(id);
//    item* p = hashTable[pos];
//    while (p != nullptr) {
//        if (p->m_id == id) {
//            scope = p->m_scope;
//            line = p->m_lineNum;
//        }
//        p = p->m_next;
//    }
//    return line;
////    while (!hashTable[pos].empty() && *hashTable[pos].rbegin() == nullptr) {
////        hashTable[pos].pop_back();
////    }
////    auto it = hashTable[pos].rbegin();
////    while (it != hashTable[pos].rend())
////    {
////        if (*it != nullptr && (*it)->m_id == id)
////        {
////            scope = (*it)->m_scope;
////            lineNum = (*it)->m_lineNum;
////            return true;
////        }
////        it++;
////    }
////    lineNum = -1;
////    return false;
//}
//
//class NameTableImpl
//{
//  private:
//    HashTable* hashTable;
////    list<item**> m_items;
////    vector<item**> m_items; // vector of pointers to hashTable's item pointers
//    stack<item**> m_items;
//    int m_scope;
//  public:
//    NameTableImpl() {
//        m_scope = 0;
//        hashTable = new HashTable;
//    }
//    ~NameTableImpl();
//    void enterScope();
//    bool exitScope();
//    bool declare(const string& id, int lineNum);
//    int find(const string& id) const;
//};
//
//// prevent memory leaks
//NameTableImpl::~NameTableImpl()
//{
//    delete hashTable;
//    while (!m_items.empty()) {
//        delete *m_items.top();
//        m_items.pop();
//    }
////
////    auto it = m_items.begin();
////    while(it != m_items.end()) {
////        if (*it != nullptr) {
////            delete **it;
////            it = m_items.erase(it);
////        } else {
////            it++;
////        }
////    }
////
////    size_t k = m_items.size();
////    while (k > 0) {
////        k--;
////        if (m_items[k] != nullptr) {
////            delete *m_items[k];
////        }
////    }
//}
//
//void NameTableImpl::enterScope()
//{
//    m_items.push(nullptr);
////    m_items.push_back(nullptr); // start of new scope
//    m_scope++;
//}
//
//bool NameTableImpl::exitScope()
//{
//    while (!m_items.empty()) {
//        if (m_items.top() == nullptr) {
//            m_items.pop();
//            m_scope--;
//            return true;
//        }
//        *m_items.top() = nullptr;
//        delete *m_items.top();
//        m_items.pop();
//    }
//    return false;
////    while(!m_items.empty()) {
////        if (m_items.back() == nullptr) {
////            m_items.pop_back();
////            m_scope--;
////            return true;
////        }
////        *m_items.back() = nullptr;
////        delete *m_items.back();
////        m_items.pop_back();
////    }
////    return false;
////
////    size_t k = m_items.size();
////    while (!m_items.empty())
////    {
////        k--;
//////        cout << k;
////        if (m_items[k] == nullptr) { // we've reached the start of a scope
////            m_items.pop_back(); // get rid of start of scope
////            m_scope--;
////            return true;
////        }
////        *m_items[k] = nullptr; // marks items in hashTable nullptr too, so insert knows where m_next should be
////        delete *m_items[k];
////        m_items.pop_back();
////    }
////    return false;
//}
//
//bool NameTableImpl::declare(const string& id, int lineNum)
//{
//    if (id.empty())
//        return false;
//
//    int scope;
//    if (hashTable->lookup(id, scope) != -1 && scope == m_scope)
//        return false;
////    cout << lineNum;
//    m_items.push(hashTable->insert(id, lineNum, m_scope));
////    m_items.push_back(hashTable->insert(id, lineNum, m_scope));
//    return true;
//}
//
//int NameTableImpl::find(const string& id) const
//{
//    if (id.empty())
//        return -1;
//    int scope;
//    return hashTable->lookup(id, scope);
//}
//
////*********** NameTable functions **************
//
//// For the most part, these functions simply delegate to NameTableImpl's
//// functions.
//
//NameTable::NameTable()
//{
//    m_impl = new NameTableImpl;
//}
//
//NameTable::~NameTable()
//{
//    delete m_impl;
//}
//
//void NameTable::enterScope()
//{
//    m_impl->enterScope();
//}
//
//bool NameTable::exitScope()
//{
//    return m_impl->exitScope();
//}
//
//bool NameTable::declare(const string& id, int lineNum)
//{
//    return m_impl->declare(id, lineNum);
//}
//
//int NameTable::find(const string& id) const
//{
//    return m_impl->find(id);
//}
