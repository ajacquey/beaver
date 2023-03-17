# Terzaghi's consolidation problem

The classical Terzaghi's consolidation problem analyzes the one-dimensional deformation delay when compressing soil. A confined soil sample, fully-saturated with water is loaded vertically by a constant stress and its deformation and fluid pressure is observed as a function of time on the vertical axis. This problem is a classical poroelastic problem which can be simplified in one dimension. For more information, please see Chapter 2 in [!cite](Verruijt2016).

## Setup

A confined soil column of height $h$ is loaded vertically at constant stress. The top surface is drained and at $t = 0$, the vertical force of magnitude $F$ is applied. This setup is sketched in [!ref](fig_terzaghi_setup).

!media media/terzaghi.png style=display:block;margin:auto;width:80%; caption=Setup for the Terzaghi's problem. id=fig_terzaghi_setup

## Parameters

!table id=mandel_properties caption=List of parameters used for the Terzaghi's problem.
| Symbol | Value | Unit | Name |
| :- | :- | :- | :- |
| $K$  | $4.0$ | Pa | Bulk modulus |
| $G$  | $3.0$ | Pa | Shear modulus |
| $\alpha$ | $0.6$ | - | Biot's poroelastic coefficient |
| $k$ | $1.5$ | m$^{2}$ | Permeability |
| $\mu$ | $1.0$ | Pa s | Fluid viscosity |
| $\phi$ | $0.1$ | - | Porosity |
| $K_{f}$ | $8.0$ | Pa | Fluid bulk modulus |
| $F$ | $1.0$ | Pa | Applied force |

Here are definitions of some poroelastic parameters:

- Solid bulk modulus: $K_{s} = \frac{K}{1 - \alpha}$
- Storage coefficient: $S = \frac{\phi}{K_{f}} + \frac{\alpha - \phi}{K_{s}}$
- Confined compressibilty: $m_{v} = \frac{1}{K + \frac{4}{3} G}$
- Consolidation coefficient: $c_{v} = \frac{k}{\mu \left(S + \alpha^{2} m_{v}\right)}$

## Solutions

[!cite](Verruijt2016) gives the following solution for the fluid pressure:

!equation
\frac{p\left(z, t\right)}{p_{0}} = \frac{4}{\pi} \sum_{k=1}^{+\infty} \frac{{\left(-1\right)}^{k-1}}{2k - 1} \cos \left[\left(2k - 1\right) \frac{\pi}{2} \frac{z}{h}\right] \exp\left[-{\left(2k - 1\right)}^{2} \frac{\pi^{2}}{4} \frac{c_{v} t}{h^{2}}\right].

For small values of the time parameter $\frac{c_{v} t}{h^{2}} \ll 1$, the pressure solution can be simplified as:

!equation
\frac{p}{p_{0}} \approx \text{erfc} \left(\frac{h - z}{2 \sqrt{c_{v}t}}\right).

The degree of consolidation $U$ is defined as:

!equation
U = \frac{u - u_{0}}{u_{\infty} - u_{0}},

where $u$ is the vertical displacement, $u_{0}$ the immediate displacement after application of the vertical load $F$ and $u_{\infty}$ the final displacement.
The degree of consolidation varies between $0$ (at the moment of loading) and $1$ (after consolidation has been completed) and evolves over time.

Its expression is given by [!cite](Verruijt2016) as:

!equation
U = 1 - \frac{8}{\pi^{2}} \sum_{k = 1}^{+\infty} \frac{1}{{\left(2k - 1\right)}^{2}} \exp\left[-{\left(2k - 1\right)}^{2} \frac{\pi^{2}}{4} \frac{c_{v} t}{h^{2}}\right].

!media media/terzaghi_pf.png style=display:block;margin:auto;width:60%; caption=Fluid pressure solution for the Terzaghi's consolidation problem. id=fig_terzaghi_pf

!media media/terzaghi_u.png style=display:block;margin:auto;width:80%; caption=Consolidation solution for the Terzaghi's consolidation problem. id=fig_terzaghi_u

## Complete Source Files

- [terzaghi.i](https://github.com/ajacquey/beaver/blob/main/examples/poroelasticity/terzaghi/terzaghi.i)

!bibtex bibliography

!content pagination use_title=True
                    next=poroelasticity/mandel.md