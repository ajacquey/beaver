# Linear Burger's viscoelastic model

This problem considers a squared medium subject to external stress leading to creep. The material deforms following a Burger's viscoelastic constitutive model.

## Setup

The squared medium is subject to a compressive stress horizontally and tension stress vertically resulting in a deviatoric stress build-up. The setup is sketched in [!ref](fig_linear_kelvin_setup).

!media media/linear_kelvin.png style=display:block;margin:auto;width:80%; caption=Setup for the Burger's viscoelastic medium. id=fig_linear_kelvin_setup

## Solutions

The scalar equivalent creep strain evolution is governed by the following constitutive model:

!equation
\dot{\epsilon}^{c} = \dot{\epsilon}^{M} + \dot{\epsilon}^{K},

where $\epsilon^{M}$ and $\epsilon^{K}$ are the Maxwell and Kelvin creep strain defined as:

!equation
\dot{\epsilon}^{M} = \frac{\sigma_{e}}{3 \eta_{M}}

!equation
\dot{\epsilon}^{K} = \frac{1}{3 \eta_{K}} \left(\sigma_{e} - 3G_{K} \epsilon^{K}\right)

where $\dot{\epsilon}^{c} = \sqrt{\frac{2}{3} \dot{e}^{c}_{ij}:\dot{e}^{c}_{ij}}$ is the scalar equivalent creep strain rate and $\sigma_{e} = \sqrt{\frac{3}{2} \tau_{ij}:\tau_{ij}}$ the scalar equivalent stress.

The strain solution is given as:

!equation
\epsilon = \frac{\sigma_{e}}{3 G_{K}} \left[1 - \exp\left(-\frac{G_{K}}{\eta_{K}} t\right)\right] + \frac{\sigma_{e}}{3 \eta_{M}} t

where:

!equation
\sigma_{e} = \sqrt{3} \sigma_{0}

With the following dimensionless quantities: $\overline{t} = \frac{G_{K}}{\eta_{K}} t$ and $\overline{\epsilon} = \frac{G_{K}}{\sigma_{0}} \epsilon$, the solution can be expressed as:

!equation id=strain
\overline{\epsilon} = \frac{1}{\sqrt{3}} \left[1 - \exp\left(- \overline{t}\right) + \frac{\eta_{K}}{\eta_{M}} \overline{t}\right] 

!media media/burger_strain.png style=display:block;margin:auto;width:60%; caption=Creep strain evolution in a Burger's viscoelastic medium. id=fig_burger

## Complete Source Files

- [burger.i](https://github.com/ajacquey/beaver/blob/main/examples/viscoelasticity/burger/burger.i)

!content pagination use_title=True
                    previous=viscoelasticity/linear_kelvin.md
                    next=viscoelasticity/lubby2.md