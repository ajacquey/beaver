# Cryer's problem

The Cryer's problem consists in a spherical soil sample subject to external load with the surface of the sphere considered drained. The fluid pressure increases within the sphere shortly after the external load is applied with its maximum value at the center of the sphere and then progressively decreases as the over pressure drain via the external surface. The original Cryer problem was somewhat simpler than this problem as we consider here both the fluid and solid particles to be compressible.

## Setup

The sphere of radius $a$ is subject at time $t = 0$ to an external force of magnitude $F$ normal to its external surface. The external surface is drained. A sketch of the problem setup can be found in [!ref](fig_cryer_setup). Due to symmetry of the problem, we model only an eighth of the sphere.

!media media/cryer.png style=display:block;margin:auto;width:80%; caption=Setup for the Cryer's problem. id=fig_cryer_setup

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

!media media/cryer_pf.png style=display:block;margin:auto;width:80%; caption=Fluid pressure solution for the Cryer's problem. id=fig_cryer_pf

## Complete Source Files

- [mandel.i](https://github.com/ajacquey/beaver/blob/main/examples/poroelasticity/cryer/cryer.i)

!bibtex bibliography

!content pagination use_title=True
                    previous=poroelasticity/mandel.md