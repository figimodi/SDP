#include "my_list.h"
#include <iostream>

using namespace std;

int main(void) {
    myList list;
    list.enqueue("ciao");
    list.enqueue("come");
    cout << list.getSize() << endl;
    cout << list.empty() << endl;
    list.visit();
    cout << list.dequeue() << endl;
    cout << list.dequeue() << endl;
    cout << list.empty() << endl;
}