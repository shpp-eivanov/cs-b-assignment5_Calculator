/*
 * File: myStack.h
 * -------------
 * This file implements stack function based on the internal Vector
 */

#ifndef _myStack_h
#define _myStack_h

#include "error.h"
#include "vector.h"

/*
 * Class: MyStack<ValueType>
 * -----------------------
 */
template <typename ValueType>
class MyStack {
public:
    /*
     * Constructor: MyStack
     * Usage: MyStack<ValueType> stack;
     * ------------------------------
     * Initializes a new empty stack.
     */
    MyStack();

    /*
     * Destructor: ~Stack
     * ------------------
     * Frees any heap storage associated with this stack.
     */
    virtual ~MyStack();
    
    /*
     * Method: push
     * Usage: stack.push(value);
     * -------------------------
     * Pushes the specified value onto the top of this stack.
     * A synonym for the push method.
     */
    void push(const ValueType& value);
    
    /*
     * Method: isEmpty
     * Usage: if (stack.isEmpty()) ...
     * -------------------------------
     * Returns <code>true</code> if this stack contains no elements.
     */
    bool isEmpty() const;
    
    /*
     * Method: pop
     * Usage: ValueType top = stack.pop();
     * -----------------------------------
     * Removes the top element from this stack and returns it.  This
     * method signals an error if called on an empty stack.
     */
    ValueType pop();

    /*
     * Method: peek
     * Usage: ValueType top = stack.peek();
     * ------------------------------------
     * Returns the value of top element from this stack, without removing
     * it.  This method signals an error if called on an empty stack.
     */
    ValueType peek() const;

    /*
     * Method: size
     * Usage: int n = stack.size();
     * ----------------------------
     * Returns the number of values in this stack.
     */
    int size() const;

    /* Private section */    
private:
    Vector<ValueType> elements; 
};

/*
 * Stack class implementation
 * --------------------------
 * The Stack is internally managed using a Vector.  This layered design
 * makes the implementation extremely simple, to the point that most
 * methods can be implemented in as single line.
 */

template <typename ValueType>
MyStack<ValueType>::MyStack() {

}

template <typename ValueType>
MyStack<ValueType>::~MyStack() {

}

template <typename ValueType>
bool MyStack<ValueType>::isEmpty() const {
    return elements.size() == 0;
}

template <typename ValueType>
ValueType MyStack<ValueType>::pop() {
    if (isEmpty()) {
        error("MyStack::pop: Attempting to pop an empty stack");
    }
    ValueType top = elements[elements.size() - 1];
    elements.remove(elements.size() - 1);
    return top;
}

template <typename ValueType>
void MyStack<ValueType>::push(const ValueType &value) {
    elements.add(value);
}

template <typename ValueType>
ValueType MyStack<ValueType>::peek() const {
    if (isEmpty()) {
        error("Stack::peek: Attempting to peek at an empty stack");
    }
    return elements.get(elements.size() - 1);
}

template <typename ValueType>
int MyStack<ValueType>::size() const {
    return elements.size();
}


#endif
