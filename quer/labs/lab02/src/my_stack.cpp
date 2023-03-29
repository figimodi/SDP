#include <iostream>
#include "my_stack.h"

using namespace std;

myStack::myStack() {
    v = {};
}

void myStack::push(int e) {
    v.push_back(e);
}

int myStack::pop() {
    int e = v.back();
    v.pop_back(); 
    return e;
}

void myStack::visit() {
    auto it = v.begin();
    while(it != v.end())
        cout << *(it++) << " ";
    cout << endl;
}

int myStack::getSize() {
    return v.size();
}

bool myStack::empty() {
    return (v.size() == 0 ? true : false);
}
