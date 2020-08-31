# Assignment 2

#### Shuyang Cao

## Problem 1

IEEE 754 is adopted.

### The maximum

The exponent with 1 in all bits is reserved for special float numbers. Hence the maximun of exponent is `(1<<exponent_width)-2`. The fraction part doesn't have any special meaning unless the exponent is all one. Hence,

The maximum of float is (2^24-1) / (2^23) * (2^(2^8-2-127)).

The maximum of double is (2^53-1) / (2^52) * (2^(2^11-2-1023)). 

### The minimum

The minimum of float or double is defined to be the positive smallest normalized number, NOT the smallest positive representable number. So exponent is 1 NOT zero and the fraction is zero for the minimum.

The minimum of float is 1*2^(1-127) = 2^(-126).

The minimum of double is 1*2^(1-1023) = 2^(-1022).

### The epsilon

The epsilon is defined to be the difference between 1 and the smallest representable number larger than 1. So only the last bit of fraction in $1+\epsilon$ is different from 1.

The epsilon of float is 2^(-23).

The epsilonn of double is 2^(-52).

### The output from the program

The code is under "CH2/EX1". Actually these constants are compiler-specific. The output from the program suggests that GCC on x64 platform follows IEEE 754.

```bash
$ ./precision 
                    base10         sign bit  exponent bit        fraction bit                                           
max of float        3.40282e+38    0         11111110 (254)      11111111111111111111111                                
max of double       1.79769e+308   0         11111111110 (2046)  1111111111111111111111111111111111111111111111111111   
min of float        1.17549e-38    0         00000001 (1)        00000000000000000000000                                
min of double       2.22507e-308   0         00000000001 (1)     0000000000000000000000000000000000000000000000000000   
epsilon of float    1.19209e-07    0         01101000 (104)      00000000000000000000000                                
epsilon of double   2.22045e-16    0         01111001011 (971)   0000000000000000000000000000000000000000000000000000   
```
