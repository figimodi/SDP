#ifndef _MYLIST_H
#define _MYLIST_H

#include <iostream> //for strings
#include <list>

using namespace std;

class myList {
    list<string> l;

    public:
        myList();
        void enqueue(string s);
        string dequeue();
        void visit();
        int getSize();
        bool empty();
};

#endif
