#include "../STACK/Stack.h"
#include <iostream>
#include <stdexcept>
#include <cstring>
using namespace std;

// The constructor is called every time an object is created.
// This insures that the object starts from a well defined state
// ... without requiring further initialization from the client.

// unimplemented!
Stack::Stack(){
  // ==> Insert code here:
  _count = 0;
  _capacity = STACK_INIT_SIZE;
  _data = new int[_capacity];
  std::cout << "HELLO from the default constructor." << std::endl;
}

Stack::Stack(const Stack& s)
{
  _count = s._count;
  _capacity = _count;
  _data = new int[_capacity];
  std::memcpy(_data, s._data, _count * sizeof(int));
  std::cout << "HELLO from the copy constructor." << std::endl;
}

Stack::~Stack()
{
  delete []_data;
  std::cout << "GOOD-BYE from the destructor." << std::endl;
}

Stack& Stack::operator=(const Stack& s)
{
  if (&s != this)
  {
    _count = s._count;

    if (_capacity < s._capacity)
    {
      delete []_data;
      _capacity = _count;
      _data = new int[_capacity];
    }
    
    std::memcpy(_data, s._data, _count * sizeof(int));
  }

  std::cout << "ASSIGN from the assignment operator" << std::endl;
  return *this;
}

// The push function increases the length of the stack by one element
// add the value to the end of the stack.  

// unimplemented!  
void Stack::push(int i) {
  // ==> insert code here
  if (_count >= _capacity)
  {
    _capacity = _capacity << 1;
    int *tmp_data = new int[_capacity];
    std::memcpy(tmp_data, _data, _count * sizeof(int));
    delete []_data;
    _data = tmp_data;
    std::cout << "Stack is extended." << std::endl;
  }
  
  _data[_count++] = i;
}

// The pop function decreases the length of the stack by one element, 
// and returs the value that is popped off

// unimplemented!  
int Stack::pop(){
  //  ==> Change the following line to something sensible...

  if (_count <= 0)
  {
    throw std::out_of_range("Pop from an empty stack.");
  }
  
  return _data[--_count];
}

// The size function returns the size of the stack. The stack does
// not change so this function is "const".

// unimplemented!  
unsigned int Stack::size() const {
  //  ==> Change the following line to something sensible...
  return _count;
}

// The print function dumps the entire stack to std::cout, in a 
// human-readable format.  The stack does not change, so this function
// is "const"

// unimplemented!  
void Stack::print() const {
  // ==> insert code here
  std::cout << "bottom" << std::endl;
  for (size_t i = 0; i < _count; i++)
  {
    std::cout << _data[i] << std::endl;
  }
  std::cout << "top" << std::endl << std::endl;
}

// The isEmpty() query function returns true or false, depending on 
// whether the stack is empty (length 0).  The stack does not change,
// so this function is "const"

// unimplemented!  
bool Stack::isEmpty() const {
  // Change the following line to something sensible
  return _count == 0;
}


