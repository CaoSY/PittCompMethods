# Assignment 7

#### Shuyang Cao

## Chapter 6 Exercise 1

The initial code is copied from `ACP-Misc/SKELETONS/CH6/STACK`.

Output from the program.

```bash
$ ./client 
Size of stack 8
bottom
1
2
4
8
16
32
64
128
top
Popping 128 from the stack
Popping 64 from the stack
bottom
1
2
4
8
16
32
top
bottom
top
bottom
32
16
8
4
2
1
top
bottom
32
16
8
4
2
1
top

```

If we change `STACK_MAX` to a smaller value, such as 4, an error will be thrown out during the execution of the program.

```bash
$ ./client 
terminate called after throwing an instance of 'std::out_of_range'
  what():  Exceed the capacity of the stack.
```

The `STACK_MAX` is set back to its original value.

## Chapter 6 Exercise 2

The code inherits from `CH6/EX2`.

The content of c is printed at the end of the output. Note that c should be the same as b.

```bash
$ ./client 
Size of stack 8
bottom
1
2
4
8
16
32
64
128
top

Popping 128 from the stack
Popping 64 from the stack
bottom
1
2
4
8
16
32
top

bottom
top

bottom
32
16
8
4
2
1
top

bottom
32
16
8
4
2
1
top

bottom
32
16
8
4
2
1
top
```

Output after messages are added.

```bash
$ ./client 
HELLO from the default constructor.
Size of stack 8
bottom
1
2
4
8
16
32
64
128
top

Popping 128 from the stack
Popping 64 from the stack
bottom
1
2
4
8
16
32
top

HELLO from the default constructor.
bottom
top

bottom
32
16
8
4
2
1
top

ASSIGN from the assignment operator
bottom
32
16
8
4
2
1
top

HELLO from the copy constructor.
bottom
32
16
8
4
2
1
top

GOOD-BYE from the destructor.
GOOD-BYE from the destructor.
GOOD-BYE from the destructor.
```

## Chapter 6 Exercise 3

An extra member variable `_capacity` is added to represent of memory a Stack possesses. Stack is initialzed to be able to accommodate 2 data. Every time the internal array in a Stack is full, the internal array size is doubled at next push. Note that `_capacity` is not a semantic state of a Stack. So `_capacity` will not be copied directly. Instead, in the copy constructor, `_capacity` will be set to `_count`. In the assignment operator, `new` operator will only be invoked when `_capacity` in the destination is smaller than `_capacity` in the source and `_capacity` will only be set to `_count` in this case, otherwise, remain unchanged. This will help increase memory efficiency and performance.

A message is printed out when the stack is extended. So `a` is extended twice to accommodate 8 data (2^3 = 8).

```bash
$ ./client 
HELLO from the default constructor.
Stack is extended.
Stack is extended.
Size of stack 8
bottom
1
2
4
8
16
32
64
128
top

Popping 128 from the stack
Popping 64 from the stack
bottom
1
2
4
8
16
32
top

HELLO from the default constructor.
Stack is extended.
Stack is extended.
bottom
top

bottom
32
16
8
4
2
1
top

ASSIGN from the assignment operator
bottom
32
16
8
4
2
1
top

HELLO from the copy constructor.
bottom
32
16
8
4
2
1
top

GOOD-BYE from the destructor.
GOOD-BYE from the destructor.
GOOD-BYE from the destructor.
```