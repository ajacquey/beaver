# Mandel's problem

The original Mandel's problem considered a rectangular soil sample subject to a constant force (vertical stress) at its top where the fluid and the solid were incompressible.
Here we consider the extension of the Mandel's problem in which the fluid and solid particles are compressible. This extension to the Mandel's problem was first introduced by [!cite](Cheng1988).

## Setup

A rectangular soil sample (width $2a$ and height $b$) is subject to a vertical stress $F$ at its top through a rigid and frictionless plate of length $2a$. 
The lateral boundary of the sample are drained and deformation is in-plane. At $t = 0$ the vertical force of magnitude $F$ is applied and fluid pressure increases due to poroelastic effect. It then drains out of the lateral boundary over time. See a sketch of this setup in [!ref](fig_mandel_setup).

!media media/mandel.png style=display:block;margin:auto;width:80%; caption=Setup for the Mandel's problem. id=fig_mandel_setup

## Parameters

!table id=mandel_properties caption=List of parameters used for the Mandel's problem.
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
- Undrained bulk modulus: $K_{u} = \frac{\alpha^{2} + K S}{S}$
- Skempton's coefficient: $B = \frac{\alpha}{\alpha^{2} + K S}$
- Undrained Poisson's ratio: $\nu_{u} = \frac{1}{2} \frac{K_{u} - K \frac{1 - 2 \nu}{1 + \nu}}{K_{u} + K \frac{1 - 2 \nu}{2 \left(1 + \nu\right)}}$
- Consolidation coefficient: $c_{v} = \frac{k}{\mu} \frac{B^{2} K \left(1 - 2\nu\right) \left(1 - \nu\right) {\left(1 + \nu_{u}\right)}^2}{3 \left(1 + \nu\right) \left(1 - \nu_{u}\right) \left(\nu_{u} - \nu\right)}$

## Solutions

The solutions for this problem are given by [!cite](Cheng1988). 
The fluid pressure solution is given as:

!equation id=mandel_pf
p\left(x, t\right) = \frac{2 p_{0}}{a} \sum_{j=1}^{N} \frac{\sin\xi_{j}}{\xi_{j} - \sin \xi_{j} \cos \xi_{j}} \left(\cos\left(\xi_{j} \frac{x}{a}\right) - \cos \xi_{j}\right) \exp \left(-\xi_{j}^{2} \frac{c_{v} t}{a^{2}}\right),

where:

!equation
p_{0} = \frac{F B \left(1 + \nu_{u}\right)}{3 a}.

The vertical displacement solution is given as:

!equation id=mandel_u
u_{y}\left(t\right) = u_{y}\left(+\infty\right) - 2 u_{y}\left(0\right) \frac{\sin \xi_{j} \cos \xi_{j}}{\xi_{j} - \sin \xi_{j} \cos \xi_{j}} \exp \left(-\xi_{j}^{2} \frac{c_{v} t}{a^{2}}\right),

where 

!equation
u_{y}\left(+\infty\right) = - \frac{F \left(1 - \nu^{2}\right)}{3 K \left(1 - 2\nu\right)} \frac{b}{a},

and

!equation
u_{y}\left(0\right) = - \frac{F \left(1 - \nu_{u}\right) \left(1 + \nu\right)}{3 K \left(1 - 2\nu\right)} \frac{b}{a}.

The coefficient $\xi_{j}$ are the roots verifying the following equations:

!equation id=roots
\tan \xi_{j} = \frac{1 - \nu}{\nu_{u} - \nu} \xi_{j}.

!media media/mandel_pf_t.png style=display:block;margin:auto;width:60%; caption=Fluid pressure solution for the Mandel's problem. id=fig_mandel_pf_t

!media media/mandel_pf_x.png style=display:block;margin:auto;width:60%; caption=Fluid pressure solution for the Mandel's problem. id=fig_mandel_pf_x

!media media/mandel_u.png style=display:block;margin:auto;width:80%; caption=Consolidation solution for the Mandel's problem. id=fig_mandel_u

## Complete Source Files

- [mandel.i](https://github.com/ajacquey/beaver/blob/main/examples/poroelasticity/mandel/mandel.i)

!bibtex bibliography

!content pagination use_title=True
                    previous=poroelasticity/terzaghi.md
                    next=poroelasticity/cryer.md