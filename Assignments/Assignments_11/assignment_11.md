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

## Chapter 11 Exercise 10

### a

The analytic solution when $\zeta=1/2$ is

$$
\begin{aligned}
    x(\tau) & = \frac{e^{-\frac{\tau }{2}} \left(\sqrt{3} c_1 \cos \left(\frac{\sqrt{3} \tau }{2}\right)+(c_1+2 c_2) \sin \left(\frac{\sqrt{3} \tau }{2}\right)\right)}{\sqrt{3}} \\
    \eta(\tau) & = \frac{e^{-\frac{\tau }{2}} \left(\sqrt{3} c_2 \cos \left(\frac{\sqrt{3} \tau }{2}\right)-(2 c_1+c_2) \sin \left(\frac{\sqrt{3} \tau }{2}\right)\right)}{\sqrt{3}}
\end{aligned}
$$

For part b and c, we choose $c_1=c_2=1$.

$$
\begin{aligned}
    x(\tau) & = 2 e^{-\frac{\tau }{2}} \sin \left(\frac{\sqrt{3} \tau }{2}+\frac{\pi }{6}\right) \\
    \eta(\tau) & = 2 e^{-\frac{\tau }{2}} \sin \left(\frac{\pi }{6}-\frac{\sqrt{3} \tau }{2}\right)
\end{aligned}
$$

In other words, we choose initial conditions as

$$
x(\tau) = \eta(\tau) = 1
$$

### b & c

1. CIRCLE: Euler
2. SQUARE: Midpoint
3. TRIANGLE_L: Trapezoid
4. TRIANGLE_U: Classical Runge-Kutta

Different slopes in "Error of X v.s. h"
 or "Error of Eta v.s. h" suggests that those algorithms are of different order fo accuracy.

In "Error of X v.s. tolerance" or "Error of Eta v.s. tolerance", slopes are the same because the error is directly controlled by the user-defined tolerance. Their different orders of accuracy are exhibited in different total number of steps they need to reach the same error.

![X v.s. h](CH11/EX10/Xh.png)
![Eta v.s. h](CH11/EX10/Etah.png)
![X v.s. h](CH11/EX10/Xtol.png)
![Eta v.s. h](CH11/EX10/Etatol.png)