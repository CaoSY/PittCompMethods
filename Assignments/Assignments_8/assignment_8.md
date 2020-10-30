# Assignment 8

#### Shuyang Cao

## Chapter 7 Exercise 15

For $N$ identically distributied random variables $y_i$, whose variance is $V$ and standard deviation is $\sigma$, define
$$
\bar{y}=\frac{1}{N}\sum_{i=1}^N y_i
$$
Then
$$
V_{\bar{y}}=\frac{1}{N}\sum_{i=1}^N V_{y_i}=\frac{V}{N}
$$
$$
\sigma_{\bar{y}} = \sqrt{V_{\bar{y}}} = \frac{\sigma_{\bar{y}}}{\sqrt{N}}
$$

Define
$$
f_1(x) = \frac{e^x-1}{e-1}
$$
Hence,
$$
\langle f_1\rangle=\int_0^1 f_1(x)dx = \frac{e-2}{e-1}
$$

### a

For a single draw,
$$
\langle f_1^2\rangle=\int_0^1 f_1^2 dx = \frac{e (e-4)+5}{2 (e-1)^2}
$$
$$
V_1 = \langle f_1^2\rangle - {\langle f_1\rangle}^2
$$
$$
\sigma_1 = \sqrt{V_1} \approx 0.286316
$$
$$
\sigma = \frac{\sigma_1}{\sqrt{N}} \approx \frac{0.286316}{\sqrt{N}}
$$

For $\sigma/{\langle f_1\rangle}<1\%$, we need $N\geq4692$.

### b

The sampling probility density function is
$$
p(x)=2x
$$
Thus,
$$
\left\langle \frac{f_1}{2x} \right\rangle = \int_0^1 \frac{f_1}{2x}2xdx = \langle f_1\rangle
$$
$$
\left\langle {\left(\frac{f_1}{2x}\right)}^2 \right\rangle = \int_0^1 {\left(\frac{f_1}{2x}\right)}^2 2x dx = \frac{-2 \text{Ei}(1)+\text{Ei}(2)+\gamma -\log (2)}{2 (e-1)^2}
$$
$$
V_2 = \left\langle {\left(\frac{f_1}{2x}\right)}^2 \right\rangle -{\langle f_1\rangle}^2
$$
$$
\sigma_2 = \sqrt{V_2} \approx 0.0523935
$$
$$
\sigma = \frac{\sigma_2}{\sqrt{N}} \approx \frac{0.0523935}{\sqrt{N}}
$$
For $\sigma/{\langle f\rangle}<1\%$, we need $N\geq158$.

### c

$$
\langle f\rangle = \int_0^1 \prod_i f(x_i) d^D x = \prod_i \int_0^1 f(x) dx = {\langle f_1 \rangle}^D
$$
$$
\langle f^2 \rangle = \int_0^1 \prod_i f^2(x_i) d^D x = \prod_i \int_0^1 f^2(x) dx = {\langle f_1^2 \rangle}^D
$$
$$
\left\langle\frac{f}{\prod_i 2x_i}\right\rangle = \int_0^1 \frac{\prod_i f(x_i)}{\prod_i 2x_i}\prod_i 2x_i d^D x = \prod_i \int_0^1 \frac{f(x)}{2x} 2x dx = {\left\langle\frac{f_1}{2x}\right\rangle}^D
$$
$$
\left\langle{\left(\frac{f}{\prod_i 2x_i}\right)}^2\right\rangle = \int_0^1 {\left(\frac{f}{\prod_i 2x_i}\right)}^2 \prod_i 2x_i d^D x = \prod_i \int_0^1 \left(\frac{f_1}{2x}\right)2x dx={\left\langle{\left(\frac{f_1}{2x}\right)}^2\right\rangle}^D
$$
$$
V_a = \frac{\langle f^2 \rangle - {\langle f\rangle}^2}{N} = \frac{{\langle f_1^2 \rangle}^D - {\langle f_1 \rangle}^{2D}}{N}
$$
$$
V_b = \frac{\left\langle{\left(\frac{f}{\prod_i 2x_i}\right)}^2\right\rangle - {\left\langle\frac{f}{\prod_i 2x_i}\right\rangle}^2}{N} = \frac{{\left\langle{\left(\frac{f_1}{2x}\right)}^2\right\rangle}^D - {\left\langle\frac{f_1}{2x}\right\rangle}^{2D}}{N}
$$

Note that
$$
\left\langle{\left(\frac{f_1}{2x}\right)}^2\right\rangle \geq {\left\langle\frac{f_1}{2x}\right\rangle}^{2} = {\left\langle f_1\right\rangle}^{2}
$$
$$
\left\langle{f_1}^2\right\rangle \geq {\left\langle f_1\right\rangle}^{2}
$$

For large D, we have
$$
\begin{aligned}
    \frac{V_b}{V_a} &= \frac{{\left\langle{\left(\frac{f_1}{2x}\right)}^2\right\rangle}^D - {\left\langle\frac{f_1}{2x}\right\rangle}^{2D}}{{\langle f_1^2 \rangle}^D - {\langle f_1 \rangle}^{2D}}\\
    &\approx \frac{{\left\langle{\left(\frac{f_1}{2x}\right)}^2\right\rangle}^D}{{\langle f_1^2 \rangle}^D}\\
    &\approx {\left(\frac{{\left\langle{\left(\frac{f_1}{2x}\right)}^2\right\rangle}}{{\langle f_1^2 \rangle}}\right)}^D
\end{aligned}
$$

Therefor,

$$
a = - \log_{10}{\left(\frac{{\left\langle{\left(\frac{f_1}{2x}\right)}^2\right\rangle}}{{\langle f_1^2 \rangle}}\right)} \approx 0.16029
$$