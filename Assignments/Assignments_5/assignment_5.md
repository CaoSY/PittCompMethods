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