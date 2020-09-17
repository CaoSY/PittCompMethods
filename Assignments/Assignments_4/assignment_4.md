# Assignment 4

#### Shuyang Cao

## Chapter 3 Exercise 13

equations of motion:

$$
\bold{M}\cdot\ddot{\bold{x}}=-\bold{K}\cdot\bold{x}
$$

where

$$
\bold{M}=
\begin{pmatrix}
    m & 0 & 0 & 0 & 0\\
    0 & 3m 0 & 0 & 0 & 0\\
    0 & 0 & 2m  0 & 0 & 0\\
    0 & 0 & 0 & m & 0 &\\
    0 & 0 & 0 & 0 & 2m\\
\end{pmatrix}
$$

$$
\bold{x}=
\begin{pmatrix}
    x_0\\
    x_1\\
    x_2\\
    x_3\\
    x_4
\end{pmatrix}
$$

$$
\bold{K}=
\begin{pmatrix}
    k & -k & 0 & 0 & 0 \\
    -k & 3k & -2k & 0 & 0 \\
    0 & -2k & 4k & -2k & 0 \\
    0 & 0 & -2k & 3k & -k \\
    0 & 0 & 0 & -k & k\\
\end{pmatrix}
$$

Define
$$
\bold{x}=\bold{a}e^{-i\omega t}
$$

then

$$
\bold{K}\cdot\bold{a}=\omega^2\bold{M}\cdot\bold{a}
$$

Define

$$
\Omega^2\equiv\bold{M}^{-1/2}\bold{K}\bold{M}^{-1/2}
$$

$$
\bold{b}\equiv\bold{M}^{1/2}\bold{a}
$$

then

$$
\Omega^2\cdot\bold{b}=\omega^2\bold{b}
$$

Positions of five balls are shown below, where the black line is the position of the center of mass of the system.

![trace](CH3/EX13/trace.svg)
