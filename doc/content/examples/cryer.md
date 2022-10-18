# Cryer's problem

## Setup

## Parameters

!table id=mandel_properties caption=List of parameters used for the Cryer's problem.
| Symbol | Value | Unit | Name |
| :- | :- | :- | :- |
| $K$  | $1.0$ | Pa | Bulk modulus |
| $\nu$  | $0.25$ | - | Poisson's ratio |
| $\alpha$ | $0.6$ | - | Biot's poroelastic coefficient |
| $k$ | $1.5$ | m$^{2}$ | Permeability |
| $\mu$ | $1.0$ | Pa s | Fluid viscosity |
| $\phi$ | $0.1$ | - | Porosity |
| $K_{f}$ | $8.0$ | Pa | Fluid bulk modulus |
| $F$ | $1.0$ | Pa | Applied force |

Here are definitions of some poroelastic parameters:

- Solid bulk modulus: $K_{s} = \frac{K}{1 - \alpha}$
- Storage coefficient: $S = \frac{\phi}{K_{f}} + \frac{\alpha - \phi}{K_{s}}$
- Unnamed parameter $m = \frac{1 - \nu}{1 - 2 \nu}$
- Unnamed parameter $\eta = m \left(1 + \frac{KS}{\alpha^{2}}\right)$
- Consolidation coefficient: $c_{v} = \frac{k}{\mu} \frac{K + \frac{4}{3} G}{\alpha^{2} + S\left(K + \frac{4}{3}G\right)}$


## Solutions

[!cite](Verruijt2016) gives the full solution of this problem using Laplace transforms. Here, we only look at the evolution over time of the fluid pressure at the center of the sphere noted $p_{c}$ which is given as:

!equation
\frac{p_{c}\left(t\right)}{p_{0}} = \eta  \sum_{j=1}^{+\infty} \frac{\sin \xi_{j} - \xi_{j}}{\left(\eta - 1\right) \sin \xi_{j} + \frac{\eta}{2} \xi_{j} \cos \xi_{j}} \exp\left(- \xi_{j}^{2} \frac{c_{v}t}{a^{2}}\right),

where $p_{0}$ is given as:

!equation
p_{0} = \frac{F}{\alpha \left(1 + \frac{KS}{\alpha^{2}}\right)}.

The coefficients $\xi_{j}$ are the positive roots of the equation:

!equation
\left(1 - \eta \frac{\xi_{j}^{2}}{2}\right) \tan \xi_{j} = \xi_{j}.

## Files

!bibtex bibliography