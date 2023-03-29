#ifndef _MYSTACK_H
#define _MYSTACK_H

#include <vector>

using std::vector;

class myStack {
    vector<int> v;
    
    public:
        myStack();
        void push(int e);
        int pop();
        void visit();
        int getSize();
        bool empty();
};

#endif