#include <iostream>
#include "my_list.h"

using namespace std;

myList::myList() {
    l = {};
}

void myList::enqueue(string s) {
    l.push_front(s);
}

string myList::dequeue() {
    string s = l.back();
    l.pop_back();
    return s;
}

void myList::visit() {
    auto it = l.begin();
    while(it != l.end())
        cout << *(it++) << " ";
    cout << endl;
}

int myList::getSize() {
    return l.size();
}

bool myList::empty() {
    return (l.size() == 0 ? true : false);
}
