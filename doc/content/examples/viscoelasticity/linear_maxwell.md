# Linear Maxwell viscoelasticity

This problem considers a squared medium subject to external velocities leading to a stress build-up. The material deforms following a linear Maxwell viscoelastic constitutive model. This problem was first introduced by [!cite](Gerya2007).

## Setup

The squared medium is subject to a compressive velocity horizontally and tension velocity vertically resulting in a deviatoric stress build-up. The setup is sketched in [!ref](fig_linear_maxwell_setup).

!media media/linear_maxwell.png style=display:block;margin:auto;width:80%; caption=Setup for the linear Maxwell viscoelastic medium. id=fig_linear_maxwell_setup

## Solutions

The stress evolution is governed by the following constitutive model:

!equation
\dot{\tau}_{ij} = 2 G \left(\dot{e}_{ij} - \frac{\tau_{ij}}{2 \eta}\right)

where $\tau_{ij}$ is the deviatoric stress tensor and $e_{ij}$ the deviatoric strain tensor.

The solution for this problem is given by [!cite](Gerya2007). 
The stress solution is given as:

!equation id=stress
\tau = \eta \dot{\gamma} \left(1 - \exp\left(- \frac{G t}{\eta}\right)\right)

where:

!equation
\sigma_{xx} = - \tau, \quad \sigma_{yy} = \tau.

!equation
\dot{\gamma} = \frac{v}{h}

!media media/linear_maxwell_tau.png style=display:block;margin:auto;width:60%; caption=Stress evolution in a linear viscoelastic Maxwell medium. id=fig_linear_maxwell

## Complete Source Files

- [linear_maxwell.i](https://github.com/ajacquey/beaver/blob/main/examples/viscoelasticity/linear_maxwell/linear_maxwell.i)

!bibtex bibliography

<!-- !content pagination use_title=True
                    next=viscoelasticity/cryer.md -->