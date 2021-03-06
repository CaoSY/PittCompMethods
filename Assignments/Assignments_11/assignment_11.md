# Assignment 11

#### Shuyang Cao


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [Assignment 11](#assignment-11)
      - [Shuyang Cao](#shuyang-cao)
  - [Chapter 11 Exercise 4](#chapter-11-exercise-4)
  - [Chapter 11 Exercise 10](#chapter-11-exercise-10)
    - [a](#a)
    - [b & c](#b-c)
  - [Chapter 11 Exercise 14](#chapter-11-exercise-14)
    - [a](#a-1)
    - [b](#b)
    - [c](#c)
    - [d](#d)
      - [i](#i)
      - [ii](#ii)
  - [Chapter 11 Exercise 15](#chapter-11-exercise-15)
    - [a](#a-2)
    - [b](#b-1)
    - [c](#c-1)
    - [d](#d-1)

<!-- /code_chunk_output -->


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

## Chapter 11 Exercise 14

### a

[Reference 1](https://diego.assencio.com/?index=1500c66ae7ab27bb0106467c68feebc6)

$$
\begin{aligned}
    L = & \frac{1}{2}\left(m_1+m_2\right){l_1}^2\dot{\theta_1}^2 + \frac{1}{2}m_2{l_2}^2\dot{\theta_2}^2 + m_2 l_1 l_2\dot{\theta_1}\dot{\theta_2}\cos\left(\theta_1-\theta_2\right)\\
    & + \left(m_1+m_2\right)gl_1\cos\theta_1 + m_2gl_2\cos\theta_2
\end{aligned}
$$

### b

[Reference 2](https://diego.assencio.com/?index=e5ac36fcb129ce95a61f8e8ce0572dbf)

$$
\begin{aligned}
    H = & \frac{m_2{l_2}^2p_{\theta_1}^2 + \left(m_1+m_2\right){l_1}^2p_{\theta_2}^2 - 2 m_2 l_1 l_2 p_{\theta_1} p_{\theta_2}\cos\left(\theta_1-\theta_2\right)}{2m_2 {l_1}^2 {l_2}^2 \left[m_1 + m_2 \sin^2\left(\theta_1-\theta_2\right)\right]} \\
    & - \left(m_1+m_2\right)gl_1\cos\theta_1 - m_2gl_2\cos\theta_2 \\
    \\
    p_{\theta_1} = & \left(m_1+m_2\right){l_1}^2\dot{\theta_1} + m_2 l_1 l_2 \dot{\theta_2} \cos\left(\theta_1 - \theta_2\right) \\
    \\
    p_{\theta_2} = & m_2 {l_2}^2\dot{\theta_2} + m_2 l_1 l_2 \dot{\theta_1} \cos\left(\theta_1 - \theta_2 \right)
\end{aligned}
$$

### c

[Reference 2](https://diego.assencio.com/?index=e5ac36fcb129ce95a61f8e8ce0572dbf)

$$
\begin{aligned}
    \dot{\theta_1} = & \frac{l_2 p_{\theta_1} - l_1 p_{\theta_2}\cos\left(\theta_1 - \theta_2\right)}{{l_2}^2 l_2 \left[m_1 + m_2 \sin^2\left(\theta_1 - \theta_2\right)\right]} \\
    \\
    \dot{\theta_2} = & \frac{-m_2 l_2 p_{\theta_1}\cos\left(\theta_1 - \theta_2\right) + \left(m_1 + m_2\right)l_1 p_{\theta_2}}{m_2 l_1 {l_2}^2 \left[m_1 + m_2 \sin^2\left(\theta_1 - \theta_2\right)\right]} \\
    \\
    \dot{p}_{\theta_1} = & -\left(m_1 + m_2\right)gl_1\sin\theta_1 - h_1 + h_2\sin\left[2\left(\theta_1-\theta_2\right)\right] \\
    \\
    \dot{p}_{\theta_2} = & -m_2 g l_2 \sin\theta_2 + h_1 - h_2 \sin\left[2\left(\theta_1-\theta_2\right)\right]
\end{aligned}
$$

where

$$
\begin{aligned}
    h_1 = & \frac{p_{\theta_1} p_{\theta_2} \sin\left(\theta_1 - \theta_2\right)}{l_1 l_2 \left[m_1 + m_2 \sin^2\left(\theta_1 - \theta_2\right)\right]} \\
    \\
    h_2 = & \frac{m_2 {l_2}^2 p^2_{\theta_1} + \left(m_1+m_2\right){l_1}^2 p^2_{\theta_1} - 2m_2 l_1 l_2 p_{\theta_1} p_{\theta_2} \cos\left(\theta_1-\theta_2\right)}{2{l_1}^2 {l_2}^2 {\left[m_1+m_2\sin^2\left(\theta_1-\theta_2\right)\right]}^2}
\end{aligned}
$$

### d

For simplicity, set $m_1 = 1, m_2 = 2, l_1 = 2, l_2 = 1, g = 1$. This is nothing special but choosing a new unit system.

The simulation time goes from 0 to 105.

#### i

The orbit fills a parallelogram.

![Orbit I](CH11/EX14/Orbit%20I%20(t=105.000000).png)

#### ii

The initial condition is an unstable equilibrium point. Theoretically, the system will stay at the intial point. But error in the numerical integration will add a perturbation in the system. Therefore, eventually, the system will slide off the equilibrium point.

The following two figures show how error accumulated during integration affect the behavior of system. The system slides off the equilibrium point suddenly between $t=100$ and $t=105$.

![Orbit II 1](CH11/EX14/Orbit%20II%20(t=100.000000).png)

![Orbit II 2](CH11/EX14/Orbit%20II%20(t=105.000000).png)

## Chapter 11 Exercise 15

### a

$$
\begin{aligned}
    \frac{dS}{dt} = & b\cdot N + v\cdot R - \left(d+\lambda\right) \cdot S \\
    \\
    \frac{dE}{dt} = & \lambda\cdot S - \left(d + \alpha \right) \cdot E \\
    \\
    \frac{dI}{dt} = & \alpha\cdot E - \left(d + \gamma\right)\cdot I \\
    \\
    \frac{dR}{dt} = & \gamma\cdot I - \left(d + v\right)\cdot R
\end{aligned}
$$

### b

Add up above four equations.

$$
\begin{aligned}
    & \frac{d}{dt}\left(S+E+I+R\right) = b\cdot N -d\cdot\left(S+E+I+R\right) \\
    \\
    \Rightarrow & \frac{dN}{dt} = \left(b-d\right)\cdot N
\end{aligned}
$$

$N$ is constant when $b=d$. Namely, the population is static when $b=d$.

### c

Being infectious will not affect the death rate, which means this disease is nonfatal.

### d

```bash
$ ./seir 
Ratio at 6000 days: 0.00536684
```

![Infected Population Ratio](CH11/EX15/ratio.png)

The ratio oscillates first and reaches stable value eventually. This is not the general behaviour of this coupled differential system. Not all sets of rates and intial conditions will produce a damped oscillation which is the case of our problem. Assuming the system will be stablized eventually as in our problem, we actually can compute the final stable ratio. Let all derivatives be zero.

$$
\begin{aligned}
    0 = & b\cdot N + v\cdot R - \left(d+\lambda\right) \cdot S \\
    \\
    0 = & \lambda\cdot S - \left(d + \alpha \right) \cdot E \\
    \\
    0 = & \alpha\cdot E - \left(d + \gamma\right)\cdot I \\
    \\
    0 = & \gamma\cdot I - \left(d + v\right)\cdot R
\end{aligned}
$$

There are two solutions.

Solution 1:
$$
S=\frac{b n}{d},\quad E=0,\quad I=0,\quad R=0
$$

Solution 2:
$$
\begin{aligned}
    S =& \frac{n (\alpha +d) (\gamma +d)}{\alpha  \beta }\\
    \\
    E =& -\frac{n (\gamma +d) (d+v) \left(-\alpha  b \beta +d^2 (\alpha +\gamma )+d^3+\alpha  \gamma  d\right)}{\alpha  \beta  d (\gamma  (\alpha +d+v)+(\alpha +d) (d+v))}\\
    \\
    I =& -\frac{n (d+v) \left(-\alpha  b \beta +d^2 (\alpha +\gamma )+d^3+\alpha  \gamma  d\right)}{\beta  d (\gamma  (\alpha +d+v)+(\alpha +d) (d+v))}\\
    \\
    R =&-\frac{\gamma  n \left(-\alpha  b \beta +d^2 (\alpha +\gamma )+d^3+\alpha  \gamma  d\right)}{\beta  d (\gamma  (\alpha +d+v)+(\alpha +d) (d+v))}\\
\end{aligned}
$$

From the figure, we alreay know that solution 1 is not the solution to our problem. Thus, we choose solution 2. Then, the stable infected population ratio is

$$
ratio=-\frac{(d+v) \left(-\alpha  b \beta +d^2 (\alpha +\gamma )+d^3+\alpha  \gamma  d\right)}{\beta  d (\gamma  (\alpha +d+v)+(\alpha +d) (d+v))}
$$

Plug in our data.

$$
ratio\approx0.00537078
$$

This agrees with our numerical result.