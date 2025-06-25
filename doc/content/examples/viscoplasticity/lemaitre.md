# Lemaitre's viscoplastic model

This problem considers a squared medium subject to external stress leading to creep. The material deforms following the Lemaitre's viscoplastic constitutive model.

## Setup

The squared medium is subject to a compressive horizontal uniaxial stress at a constant temperature, resulting in a uniaxial deformation. The setup is sketched in [!ref](fig_lemaitre_creep_model_setup).

!media media/linear_kelvin.png style=display:block;margin:auto;width:80%; caption=Setup for the Lemaitre's viscoplastic medium. id=fig_lemaitre_creep_model_setup

## Solutions

The scalar equivalent creep strain evolution is governed by the following constitutive model:

!equation
\dot{\gamma}_{vp} = A \left( \frac{q}{A_2} \right)^{\frac{\beta}{\alpha}} \gamma_{vp}^{1-\frac{1}{a}},

where $A$ is a time-scaling model parameter unique to the material under consideration. [!cite](azabou2021rock) expressed this parameter in terms of the Arrhenius law as $\alpha \exp\left(\frac{1}{T_r}-\frac{1}{T}\right)$ to describe the behavior of rock salt. 
In this case, $A \approx \alpha$ for isothermal conditions. $\alpha$ and $\beta$ are material parameters. $A_2$ is a normalizing parameter always taken as 1.0. $\gamma_{vp}$ is the scalar equivalent creep strain.

The analytical solution for this problem is given as:

!equation
\gamma_{vp} = \left( \frac{A}{\alpha} \right)^{\alpha} \left[ \left( \frac{q}{A_2} \right)^{\beta} \right] t^{a},

where $t$ is time. 

The following creep curve shows a comparison between the analytical and the numerical solutions of the Lemaitre model.

!media media/lemaitre_strain.png style=display:block;margin:auto;width:60%; caption=Creep strain evolution in a Lemaitre's viscoplastic medium. id=fig_lemaitre

## Complete Source Files

- [lemaitre.i](https://github.com/ajacquey/beaver/blob/main/examples/viscoplasticity/lemaitre/lemaitre.i)

!bibtex bibliography

!content pagination use_title=True
                    next=viscoplasticity/munson_dawson.md