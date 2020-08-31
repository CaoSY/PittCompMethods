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

## Problem 2

Since $\gamma_\epsilon$ is much more precise than $\gamma_\beta$, we compare $\gamma_\beta$ with $\gamma_\epsilon$ and define the relative compuational error to be $(\gamma_\beta - \gamma_\epsilon)/\gamma_\epsilon$. From the output below, the relative error of $\gamma_\beta$ starts to exceed the error threshold of 0.1% is when $\beta = 1 - 10^{-16}$.

```bash
$ ./gamma
beta                gamma from beta     epsilon             gamma from epsilon  relative error      
0.9                 2.294157338705618   0.1                 2.294157338705618   0                   
0.99                7.088812050083354   0.01                7.088812050083359   -6.264649234773272e-16
0.999               22.36627204212937   0.001               22.36627204212922   6.830225792687734e-15
0.9999              70.71244595191452   0.0001              70.71244595190174   1.806691625072123e-13
0.99999             223.6073567696247   1e-05               223.6073567690697   2.48236906511553e-12
0.9999990000000001  707.1069579884842   1e-06               707.106957963309    3.560303589445676e-11
0.9999999000000001  2236.068033945294   1e-07               2236.068033401491   2.431959986119411e-10
0.9999999900000001  7071.067852978736   1e-08               7071.067829543143   3.314293337057601e-09
0.999999999         22360.68009119951   1e-09               22360.67978058807   1.389096612914218e-08
0.9999999999        70710.67519334106   1e-10               70710.67812042251   -4.139518292731546e-08
0.99999999999       223606.7884993249   1e-11               223606.797750538    -4.137268277047632e-08
0.999999999999      707114.6025254686   1e-12               707106.7811867243   1.106104332803255e-05
0.9999999999999001  2236962.133333333   1e-13               2236067.977499845   0.0003998786452313597
0.99999999999999    7073895.380882599   1e-14               7071067.811865492   0.0003998786452539518
0.999999999999999   22369621.33333333   1e-15               22360679.7749979    0.000399878645256068
0.9999999999999999  67108864            1e-16               70710678.11865474   -0.05093734375748434
1                   inf                 1e-17               223606797.749979    inf                 
1                   inf                 1e-18               707106781.1865475   inf                 
1                   inf                 1e-19               2236067977.49979    inf                 
1                   inf                 1e-20               7071067811.865476   inf 
```