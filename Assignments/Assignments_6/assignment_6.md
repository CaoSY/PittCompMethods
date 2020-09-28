# Assignment 5

#### Shuyang Cao

## Chapter 4 Exercise 7

Cubic spline interpolation is used.

```bash
 cat ./data.txt | ./dipersion 
```

![Index of refraction](CH4/EX7/index.png)

## Chapter 5 Exercise 7

Use $M(\hat{A})$ to denote the matrix of operator $\hat{A}$ and $\hat{F}(\hat{A})$ to denote the function of operator $\hat{A}$. Then, we have

$$
M(\hat{F}(\hat{A})) = F(M(\hat{A})) 
$$

So, for $\hat{H}, \hat{x}, \hat{D}$, we have

$$
\begin{array}{lcl}
    M(\hat{x}^2) & = & M(\hat{x})^2\\
    M(\hat{D}^2) & = & M(\hat{D})^2\\
    M(\hat{H}) & = & -\frac{1}{2} M(\hat{D})^2 + \frac{1}{2} M(\hat{x})^2
\end{array}
$$

Note that the basis we use the the eigen basis of $\hat{H}$. Thus, the matrix of $\hat{H}$ is diagonal. A value whose magnitude is less than 1e-12 is printed as 0.

```bash
$ ./operator 
x
              0       0.707107              0              0              0
       0.707107              0              1              0              0
              0              1              0        1.22474              0
              0              0        1.22474              0        1.41421
              0              0              0        1.41421              0

x^2
            0.5              0       0.707107              0              0
              0            1.5              0        1.22474              0
       0.707107              0            2.5              0        1.73205
              0        1.22474              0            3.5              0
              0              0        1.73205              0            4.5

D
              0       0.707107              0              0              0
      -0.707107              0              1              0              0
              0             -1              0        1.22474              0
              0              0       -1.22474              0        1.41421
              0              0              0       -1.41421              0

D2
           -0.5              0       0.707107              0              0
              0           -1.5              0        1.22474              0
       0.707107              0           -2.5              0        1.73205
              0        1.22474              0           -3.5              0
              0              0        1.73205              0           -4.5

Hamiltonian
            0.5              0              0              0              0
              0            1.5              0              0              0
              0              0            2.5              0              0
              0              0              0            3.5              0
              0              0              0              0            4.5
```