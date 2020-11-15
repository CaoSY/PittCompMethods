# Assignment 11

#### Shuyang Cao

## Chapter 11 Exercise 4

Choosing generalized coordinates as $\theta$ and $\phi$, the Hamiltonian is

$$
H=\frac{1}{2ma^2}\left({p_\theta}^2+\frac{{p_\phi}^2}{\sin^2\theta}\right)-mga\cos\theta
$$

where
$$
\begin{aligned}
    p_\phi & = ma^2 \sin^2\theta \dot{\phi}\\
    p_\theta & = ma^2 \dot{\theta}
\end{aligned}
$$

The intial condition is

$$
\left\{
\begin{aligned}
\theta(t=0) & =\theta_0 \\
\phi(t=0) & = 0 \\
p_\theta(t=0) & = 0 \\
p_\phi(t=0) & = \lambda \equiv ma^{3/2}g^{1/2}
\end{aligned} \right.
$$

The Hamilton's equations are
$$
\begin{aligned}
    \frac{d\theta}{dt} & = \frac{\partial H}{\partial p_\theta} \\
    \frac{d\phi}{dt} & = \frac{\partial H}{\partial p_\phi} \\
    \frac{d p_\theta}{dt} & = -\frac{\partial H}{\partial \theta} \\
    \frac{d p_\phi}{dt} & = -\frac{\partial H}{\partial \phi}
\end{aligned}
$$

For simplicty, we set $a=1, g=1, m=1$. We also set $\theta_0=\pi/9$. The figure is plotted using following two functions.

$$
\begin{aligned}
    x(t) & = \sin(\theta)\cos(\phi+\Omega t) \\
    y(t) & = \sin(\theta)\sin(\phi+\Omega t) \\
\end{aligned}
$$

where $\Omega$ is the precession rate to be determined. Setting $\Omega=0$ gives the solution. Setting $\Omega$ to the precession rate aligns the orbit in one ellipse.

Through a binary search, it is found that orbits become indistinguishable whe $\Omega\in[0.0353,0.0356]$

![Omega=0](CH11/EX4/Omega0.png)
![Omega=0.034](CH11/EX4/Omega0.0354.png)