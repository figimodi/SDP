#include <iostream>
#include "my_stack.h"

using namespace std;

int main(void) {
    myStack stack;

    stack.push(1);
    stack.push(2);
    cout << stack.empty() << endl;
    stack.visit();
    stack.pop();
    stack.visit();
    stack.pop();
    stack.empty();
}