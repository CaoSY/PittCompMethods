# Assignment 5

#### Shuyang Cao

## Chapter 5 Exercise 1

$$
\int_0^1 \tanh (x) \, dx = \log (\cosh (1))
$$

$$
\int_0^1 \sqrt{\coth (x)} \, dx = \frac{\pi }{2}+\coth ^{-1}\left(\sqrt{\coth (1)}\right)-\tan ^{-1}\left(\sqrt{\coth (1)}\right)
$$



```bash
$ ./ex1 
Usage: ex1 N n
N: max bin number = 2^N, 1 <= N::int <= 32
n=1: tanh(x)
n=2: sqrt(cosh(x))
```

Depending on ```N```, the elapse time varies from seconds to minutes.

$\tanh (x)$ can be well approximated by taylor series. So different integration methods work as expected. Midpoint rule is slightly better than trapezoid rule. Simpson's rule converges faster than midpoint rule and trapezoid rule.

```bash
$ ./ex1 29 1
```

![tanh(x)](CH5/EX1/tanh(x).png)

$\sqrt{\coth(x)}$ diverges at x = 0. To avoid the singularity, we use open trapezoid rule instead of closed trapeoid rule. For Simpson's rule, we use a composite integration, where the leftmost bin is integrated by midpoint rule and the rest bins are integrated by Simpson's rule. In this case, the convergence rate of Simpson's rule is degraded to be the same as midpoint rule. Open trapezoid rule is slightly better than midpoint rule.

```bash
 ./ex1 29 2
```

![sqrt(coth(x))](CH5/EX1/sqrt(coth(x)).png)


## Chapter 5 Exercise 2

$$
mgl\left(1-\cos(\theta_{\max})\right) = \frac{1}{2}m\left(l\dot{\theta}\right)^2 + mgl(1-\cos(\theta)) 
$$

$$
\Rightarrow  \dot{\theta} = \sqrt{\frac{2g\left(\cos\theta - \cos\theta_{\max} \right)}{l}}
$$

$$
\Rightarrow dt = d\theta \sqrt{\frac{l}{2g\left(\cos\theta - \cos\theta_{\max}\right)}}
$$

$$
\Rightarrow T = T_0\int_{-\theta_{\max}}^{\theta_{\max}}{\frac{1}{\pi\sqrt{2\left(\cos\theta - \cos\theta_{\max}\right)}}d\theta}
$$

$$
\Rightarrow T_0 = 2\pi\sqrt{\frac{l}{g}}
$$

$$
f\left(\theta\right)=\int_{-\theta_{\max}}^{\theta_{\max}}{\frac{1}{\pi\sqrt{2\left(\cos\theta - \cos\theta_{\max}\right)}}d\theta}
$$

In the limit $\theta \ll 1$,

$$
T \approx T_0 \int_{-\theta_{\max}}^{\theta_{\max}}{\frac{1}{\pi\sqrt{2\left({\theta_{\max}}^2 - \theta^2\right)}}d\theta} = T_0
$$

In the program, the integration is completed using three methods. Note that the integrand is singular at end points. To avoid singularities, we need to use midpoint rule and open Romberg method. Actucally, due to singularities, midpoint rule and Romberg method don't work very well in this case. However, these singularities can be perfectly handled by Gauss Chebyshev quadrature of the first kind.

If we rewrite the integration as the following.

$$
f\left(\theta\right)=\frac{\theta_{\max}}{\pi\sqrt{2}}\int_{-1}^{1}{\frac{1}{\sqrt{1-x^2}}\frac{\sqrt{1-x^2}}{\sqrt{\left(\cos\left(\theta_{\max}x\right) - \cos\theta_{\max}\right)}}d\theta}
$$

Define

$$
g\left(\theta\right)=\frac{\sqrt{1-x^2}}{\sqrt{\left(\cos\left(\theta_{\max}x\right) - \cos\theta_{\max}\right)}}
$$

Check convergence of $g\left(\theta\right)$ at endpoints.

$$
\lim_{x\to\pm1}{g\left(\theta\right)} = \sqrt{\frac{2}{\theta_{\max}\sin\theta_{\max}}}
$$

Therefore, as long as $\theta\neq0, \pi$, $g\left(\theta\right)$ doesn't have singular points in the range $\left[-1,1\right]$ and can be well approximated by Chebyshev polynomials of the first kind.

The average relative errors using 1024 sample points are shown below. It's obvious that Gauss Chebyshev quadrature of the first kind works much better than the other two. The machine precision results are computed by Mathematica.

```bash
$ ./ex2 
Simple relative error: 0.0119797
Romberg relative error: 0.00481546
Chebyshev relative error: 4.80984e-09
```

![period](CH5/EX2/perioid.png)

## Chapter 5 Exercise 4

Stationary Schrödinger equation

$$
-\frac{\hbar^2}{2m}\frac{\partial^2}{{\partial x}^2}\psi\left(x\right) - V_0 e^{-\frac{x^2}{2a^2}}\psi\left(x\right) = E\psi\left(x\right)
$$

Choose length unit $x_0=a$ and the reduced position $\xi=x/a$

$$
-\frac{\hbar^2}{2ma^2}\frac{\partial^2}{{\partial \xi}^2}\psi\left(\xi\right) - V_0 e^{-\frac{1}{2}\xi^2}\psi\left(\xi\right) = E\psi\left(\xi\right)
$$

Choose energy unit $E_0=\hbar^2/(2ma^2)$, reduced potential $v=V_0/E_0$ and reduced energy $\epsilon=E/E_0$

$$
-\frac{\partial^2}{{\partial \xi}^2}\psi\left(\xi\right) - v e^{-\frac{1}{2}\xi^2}\psi\left(\xi\right) = \epsilon\psi\left(\xi\right)
$$

The action (eqn 5.16)

$$
\mathcal{J}\left(E\right) = 2 \int_{x_{\min}}^{x_{\max}}{\sqrt{2m\left(E+V_0 e^{-\frac{x^2}{2a^2}}\right)}dx}
$$

Note that $\mathcal{J}\left(E\right)$ is a monotonically increasing function when $E<0$. The number of bound states is

$$
N = \left\lfloor\frac{\mathcal{J}\left(0\right)}{\hbar}-0.5\right\rfloor
$$

$$
\begin{aligned}
    N &  = \left\lfloor\frac{\mathcal{J}\left(0\right)}{\hbar}-0.5\right\rfloor\\
    & = \left\lfloor\frac{4 \sqrt{2 \pi } a \sqrt{m V_0}}{\hbar}-0.5\right\rfloor\\
    & = \left\lfloor4\sqrt{\pi}\sqrt{\frac{V_0}{\frac{\hbar^2}{2ma^2}}}-0.5\right\rfloor\\
    & = \left\lfloor4\sqrt{\pi}\sqrt{v}-0.5\right\rfloor
\end{aligned}
$$

```bash
$ ./ex4
```

![Number of Bound States](CH5/EX4/number.png)