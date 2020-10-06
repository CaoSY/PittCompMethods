#ifndef _Stack_h_
#define _Stack_h_
#define STACK_MAX 1024
#define STACK_INIT_SIZE 2
class Stack {

 public:
  
  // Constructor:
  Stack();
  Stack(const Stack& s);

  // Destructor
  ~Stack();

  // assignment
  Stack& operator=(const Stack& s);

  // Push:
  void push(int i);

  // Pop:
  int pop();

  // How big is the stack?
  unsigned int size() const;

  // Print (to std::cout) 
  void print() const;

  // Test if the stack is empty
  bool isEmpty() const;

 private:

  unsigned int       _count;            // Actual length of stack.
  unsigned int       _capacity;         // size of memory allocated
  int                *_data;            // An array to store data.
};
#endif
