Implement a 16-bit multiplier
=============================

Let $A$ and $B$ be two 16-bit unsigned integers. Let the bits of $B$ be $b_0, b_1, \ldots, b_{15}$.
Then $B$ can be expressed as

$$
B = \sum_{i=0}^{15} 2^i \cdot b_i.
$$

Therefore,

$$
\begin{align*}
  A \cdot B &= A \sum_{i=0}^{15} 2^i \cdot b_i \\
  &= \sum_{i=0}^{15} A \cdot 2^i \cdot b_i \\
  &= \sum_{i=0}^{15} \mathrm{shiftLeft}(A, i) \cdot b_i
\end{align*}
$$

Let $b_i^{16}$ be the 16-bit number with all the bits are $b_i$. 
For any arbitrary 16-bit number $U$, the identity 

$$U\cdot b_i = \mathrm{AND}_{16}(U, b_i^{16}),$$

where $\mathrm{AND}_{16}(\cdot, \cdot)$ is the 16-bit `AND` operation, holds. 
Thus, multiplying $b_i$ can be replaced with taking the $\mathrm{AND}$ with $b_i^{16}$. 
Another option is to implement the multiplication with a 16-bit selector.
