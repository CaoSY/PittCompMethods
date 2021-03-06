# Assignment 2

#### Shuyang Cao

- [Assignment 2](#assignment-2)
      - [Shuyang Cao](#shuyang-cao)
  - [Problem 1](#problem-1)
    - [The maximum](#the-maximum)
    - [The minimum](#the-minimum)
    - [The epsilon](#the-epsilon)
    - [The output from the program](#the-output-from-the-program)
  - [Problem 2](#problem-2)
  - [Problem 6](#problem-6)
  - [Problem 7](#problem-7)
  - [Problem 8](#problem-8)
  - [Problem 9](#problem-9)
  - [Problem 10](#problem-10)

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

## Problem 6

The relative error is defined to be (M_PI - pi) / M_PI. The output from the program is

```bash
$ ./pi 
side number     pi                                relative error                    
6               2.59807621135331601180951111      0.1730066568673118698740154286    
12              3                                 0.04507034144862794716734910594   
24              3.105828541230248873716845992     0.0113840705346308287970735762    
48              3.132628613281237761611919268     0.002853342650363166563970418466  
96              3.139350203046866738532116869     0.0007137941770916747833905513509 
192             3.141031950890509349960666441     0.0001784772123919597789677704558 
384             3.141452472285461539058815106     4.462109502687957928801845697e-05 
768             3.141557607911857452620552067     1.115538575493479461200538316e-05 
1536            3.141583892148317680437230592     2.788853438851836437204932337e-06 
3072            3.141590463228049578958689381     6.972137972866043348024278452e-07 
6144            3.141592105999270856386829109     1.743034768157793128383011884e-07 
12288           3.141592516692156777935451828     4.357587104159864282342659787e-08 
24576           3.141592619365383587393125708     1.089396790175764746345211882e-08 
49152           3.141592645033690622824451566     2.723492010778908141671590768e-09 
98304           3.141592651450767270659980568     6.808730733737200006253260931e-10 
196608          3.141592653055036876708072668     1.702181976644370866476914067e-10 
393216          3.141592653456103612086280918     4.255469077409511160390015597e-11 
786432          3.141592653556370962064647756     1.063860201453085631439925295e-11 
1572864         3.14159265358143757751463454      2.659650503632714078599813238e-12 
3145728         3.141592653587704120354828774     6.649479654046392119636504603e-13 
6291456         3.141592653589270867087179795     1.662369913511598029909126151e-13 
12582912        3.14159265358966255377026755      4.155924783778995074772815377e-14 
25165824        3.141592653589761141574854264     1.017777498068325334093680156e-14 
50331648        3.141592653589785566481396017     2.403085759327990437904042471e-15 
100663296       3.141592653589791339641124068     5.654319433712918851434711377e-16 
201326592       3.141592653589793115997963469     0                                 
402653184       3.141592653589793560087173319     -1.413579858428229712858677844e-16
805306368       3.141592653589793560087173319     -1.413579858428229712858677844e-16
1610612736      3.141592653589793560087173319     -1.413579858428229712858677844e-16
3221225472      3.141592653589793560087173319     -1.413579858428229712858677844e-16
```

## Problem 7

The code is under "CH2/EX7".

Dimensions of physical quantities are

$$
[Energy] = \frac{\hbar^2}{2m}
$$
$$
[wave vector] = \sqrt{\frac{2m}{\hbar^2}}
$$
$$
[distance] = \frac{1}{[wavevector]}
$$

The program gives the wave function without the time evolution term. The incident wave is assumed to incident from left and to be of unit magnitude.

```bash
# help: stepbarrier E V [pos1 pos2 ...]

# total transmision
$ ./stepbarrier 1 0 -3 -2 -1 0 1 2 3
Refection:	0
Transmission:	1

x                   psi                                               amplitude           
-3                  (-0.9899924966004454,-0.1411200080598672)         0.9999999999999999  
-2                  (-0.4161468365471424,-0.9092974268256817)         1                   
-1                  (0.5403023058681398,-0.8414709848078965)          1                   
0                   (1,0)                                             1                   
1                   (0.5403023058681398,0.8414709848078965)           1                   
2                   (-0.4161468365471424,0.9092974268256817)          1                   
3                   (-0.9899924966004454,0.1411200080598672)          0.9999999999999999 

# partial transmission
$ ./stepbarrier 1 0.5 -3 -2 -1 0 1 2 3
Refection:	0.0294373
Transmission:	0.970563

x                   psi                                               amplitude           
-3                  (-1.159848355721881,-0.116907642521195)           1.165725355798104   
-2                  (-0.4875463458213129,-0.7532866528443087)         0.8972971752307578  
-1                  (0.6330035259921999,-0.6970973885017507)          0.9416146945404572  
0                   (1.17157287525381,0)                              1.17157287525381    
1                   (0.8906819484920702,0.761097016589113)            1.17157287525381    
2                   (0.1826994028539722,1.157239789424509)            1.17157287525381    
3                   (-0.6128894806747177,0.9984735782727323)          1.17157287525381

# total reflection
$ ./stepbarrier 1 2 -3 -2 -1 0 1 2 3
Refection:	1
Transmission:	0

x                   psi                                               amplitude           
-3                  (-0.8488724885405782,0.8488724885405782)          1.200486986019485   
-2                  (0.4931505902785393,-0.4931505902785393)          0.6974202530642076  
-1                  (1.381773290676036,-1.381773290676036)            1.954122527798952   
0                   (1,-1)                                            1.414213562373095   
1                   (0.3678794411714423,-0.3678794411714423)          0.5202600950228889  
2                   (0.1353352832366127,-0.1353352832366127)          0.1913929930208219  
3                   (0.04978706836786394,-0.04978706836786394)        0.0704095473166297

# negative barrier
$ ./stepbarrier 1 -10 -3 -2 -1 0 1 2 3
Refection:	0.28802
Transmission:	0.71198

x                   psi                                               amplitude           
-3                  (-0.4586882319780852,-0.216855594302406)          0.5073669706784542  
-2                  (-0.1928112155946173,-1.397294661493214)          1.410534840369605   
-1                  (0.2503355432120637,-1.293067460861644)           1.317076817248887   
0                   (0.46332495807108,0)                              0.46332495807108    
1                   (-0.4562457896846544,-0.08068330785604995)        0.46332495807108    
2                   (0.4352246105581992,0.1589010860127515)           0.46332495807108    
3                   (-0.4009037882746606,-0.2322631467077678)         0.4633249580710799 
```

## Problem 8

The code is under "CH2/EX8".

```bash
$ ./testidentity 
x                           abs(sin(ix) - i sinh(x)     abs(cos(ix) - cosh(x)       
-3                          0                           0                           
-2.5                        0                           0                           
-2                          0                           0                           
-1.5                        0                           0                           
-1                          0                           0                           
-0.5                        0                           0                           
0                           0                           0                           
0.5                         0                           0                           
1                           0                           0                           
1.5                         0                           0                           
2                           0                           0                           
2.5                         0                           0                           
3                           0                           0 
```

## Problem 9

The code is under "CH2/EX9".

```bash
$ cat test.txt | ./solve 
x1 -> (0,0)                                             x2 -> (-4,0)                                            	
x1 -> (-0.5857864376269049,0)                           x2 -> (-3.414213562373095,0)                            	
x1 -> (-2,1.414213562373095)                            x2 -> (-2,-1.414213562373095)                           	
x1 -> (1.414213562373095,-2)                            x2 -> (-1.414213562373095,-2)                           	
x1 -> (2.350832480035795e-08,-1.999999989581165)        x2 -> (-2.350832480035795e-08,-2.000000010418835)       	
x1 -> (1.224646799147353e-16,-2.220446049250313e-16)    x2 -> (-1.224646799147353e-16,-4)                       	
x1 -> (1.482393534508254,-0.5971629856028875)           x2 -> (-3.482393534508254,-1.402837014397113)           	
x1 -> (0,2.220446049250313e-16)                         x2 -> (-2,-2)                                           	
x1 -> (-0.5971629856028875,1.482393534508254)           x2 -> (-1.402837014397113,-3.482393534508254) 
```

## Problem 10

The characteristic equation is solved by hard-coded analytic expression.

The homogeneous linear equations of each eigenvalue are solved by a random algorithm.

1. $B=A-\lambda I$
2. Conjugate each row vector of $B$ and find basis $\{u_i\}$ of row space using Gram-Schmidt process.. Note that $Dimension(\{u_i\})=Rank(B)=N-\mu(\lambda)$, where $\mu$ is the multiplicity of $\lambda$. $\mu(\lambda)$ is equal to dimension of null space.
3. Generate a random vector $v$.
4. Make $v$ orthogonal to to $\{u_i\}$ using Gram-Schmidt process.
5. Repeat 3,4 $\mu$ times.
6. Orthonormalize $\{v_i\}$.

In theory there exists possibilities that $\{v_i\}$ are linearly dependent. But the probablity is $0^+$.

```bash
$ cat test.txt | ./eig33 
Matrix:
(0.333333,0) (-0.244017,0) (0.910684,0) 
(0.910684,0) (0.333333,0) (-0.244017,0) 
(-0.244017,0) (0.910684,0) (0.333333,0) 

Eigenvalues:                  Order
(-5.01123e-08,-1)             1
(-5.01123e-08,1)              1
(0.999999,-0)                 1

Eigenvectors:
(-0.288675,-0.5)               (0.57735,-4.75362e-08)         (0.57735,1.38778e-17)          
(0.577351,-1.77408e-07)        (-0.288675,-0.5)               (0.577351,-4.16334e-17)        
(-0.288675,0.5)                (-0.288675,0.5)                (0.57735,-5.55112e-17)         
 
```