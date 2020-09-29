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

## Chapter 5 Exercise 9

Effective potential is

$$
V(r)=-\alpha\frac{4\hbar c}{3r} + \frac{r}{\hbar c a^2} + \frac{l(l+1)\hbar^2}{2\mu r^2}
$$

where $\mu=m_c/2$ is the reduced mass. So, the quantization rule is 

$$
\begin{aligned}
       &\int_{r_{\min}}^{r_{\max}}\sqrt{2\mu\left(E-V(r\right)}dr=\left(n+\eta\right)\pi\hbar\\
       \Rightarrow & \int_{r_{\min}}^{r_{\max}}\sqrt{\frac{2\mu E}{\hbar^2}+\frac{8\alpha\mu c}{3\hbar r}-\frac{2\mu r}{\hbar^3ca^2}-\frac{l(l+1)}{r^2}}dr=\left(n+\eta\right)\pi
\end{aligned}
$$

where $\eta=3/4$ if $l=0$, $\eta=1/3$ if $l=1$ because classically particles cannot reach the cener of the system if the angular momentum is nonzero. Choose $b=10^{-15}m$ as the length unit. The reduced position $\xi=x/a$. The quantization rule can be rewritten as

$$
\int_{\xi_{\min}}^{\xi_{\max}}\sqrt{\epsilon+\frac{\beta}{\xi}-\gamma\xi-\frac{l(l+1)}{\xi^2}}d\xi=\left(n+\eta\right)\pi
$$

where

$$
\left\{
\begin{aligned}
       \epsilon & =\frac{E}{E_0}\\
       E_0 & = \frac{\hbar^2}{2\mu b^2}\approx28.4219 \enspace MeV\\
       \beta & = \frac{8\alpha\mu c b}{3\hbar}\approx3.51768\\
       \gamma & = \frac{2\mu b^3}{\hbar^3 c a^2}\approx30.1959
\end{aligned}
\right.
$$

![Energy](CH5/EX9/energy.png)