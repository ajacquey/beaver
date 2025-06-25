# Modified Munson-Dawson viscoplastic model

This problem considers a squared medium subject to external stress leading to creep. 
The material deforms following the modified Munson-Dawson's viscoplastic constitutive model described in [!cite](azabou2021rock).

## Setup

The squared medium is subject to a compressive horizontal uniaxial stress at a constant temperature, resulting in a uniaxial deformation. 
The setup is sketched in [!ref](fig_munson_dawson_creep_model_setup). 
The uniaxial stress is held constant at 10MPa for a duration of 15 days before dropping to 5MPa for another 15 days to simulate a multi-stage creep test. 

!media media/linear_kelvin.png style=display:block;margin:auto;width:80%; caption=Setup for the munson_dawson's viscoplastic medium. id=fig_munson_dawson_creep_model_setup

## Solutions

The scalar equivalent creep strain evolution is governed by the following constitutive model:

\begin{equation}
  \dot\gamma_{vp} = 
  \begin{cases} 
  A \left( 1 - \frac{\gamma_{vp}}{\bar{\gamma}_{vp}} \right)^n R(\sigma, T) & \text{if } \gamma_{vp} \leq \bar{\gamma}_{vp} \\ 
  -B \left( \frac{\gamma_{vp}}{\bar{\gamma}_{vp}} - 1 \right)^m R(\sigma, T) & \text{if } \gamma_{vp} \geq \bar{\gamma}_{vp} 
  \end{cases}
\end{equation}

where, $A$, $B$, $m$, and $n$ are material parameters, with $A > 0$, $B > 0$, and $m > 1$, $n > 1$ [!cite](azabou2021rock). 
The parameter $\bar{\gamma}_{vp}$ describes the saturation strain, similar to that expressed in the Munson-Dawson model, 
and corresponds to the threshold of transient deformation. It is defined as:

\begin{equation} 
  \bar{\gamma}_{vp}(\sigma, T)= \left\langle \frac{q}{A_1} \right\rangle^{n_1}
  \label{eq:29}
\end{equation}
here $A_1$ and $n_1$ are material parameters and $q = \sqrt{\frac{3}{2}\tau_{ij}\tau_{ij}}$ 
is the equivalent stress, representing the loading function. $\tau_{ij}$ is the deviatoric stress tensor.

The function $R$ corresponds to the Lemaitre’s scalar equivalent creep strain evolution, 
ensuring that $\dot\gamma_{vp}$ have similar order of magnitude as the Lemaitre's scalar equivalent creep model [!cite](azabou2021rock):
  
\begin{equation}
  R(\sigma, T) = \exp \left( A_R \left( \frac{1}{T_r} - \frac{1}{T} \right) \right) \left\langle \frac{q}{A_2} \right\rangle^{n_2}
\end{equation}

This model is equivalent to the Munson-Dawson model provided the following conditions are satisfied:

- Parameter $A = \exp{\Delta}$
- Parameter $n =
        \begin{cases} 
        -0.0098\Delta^3 + 0.2040\Delta^2 + 0.5622\Delta + 2.0252, & \text{if } \Delta \leq 6 \\
        1.9987\Delta - 1.4567, & \text{if } \Delta > 6
        \end{cases}$
- Parameter $B = 0$ (i.e., the transient strain threshold is not exceeded). Hence, the strain-rate is always transient.

The analytical solution for this problem is given as [!cite](azabou2021rock):

\begin{equation*}
    \gamma_{vp} \left( t \right) = {\gamma_{vp}}_0 + (\bar{\gamma}_{vp} + {\gamma_{vp}}_0) \, \xi\left(t\right),
\end{equation*}
\begin{equation*}
    \xi \left( t \right) = \left(1 - \frac{1}{1 + V(t - t_0)} \right)^{\frac{1}{k-1}},
\end{equation*}
\begin{equation*}
    V = (k - 1) \, U \, \frac{A_{1}^{n_{1}}}{A_{2}^{n_{2}}} \, q^{n_{2} - n_{1}},
\end{equation*}
\begin{equation*}
  \begin{cases}
    \text{when } \gamma_{vp} \leq \bar{\gamma}_{vp}; \quad k = n; \quad U = A \left( 1 - \frac{{\gamma_{vp}}_0}{\bar{\gamma}_{vp}} \right)^{n-1} \\
    \text{when } \gamma_{vp} \geq \bar{\gamma}_{vp}; \quad k = m; \quad U = B \left( \frac{{\gamma_{vp}}_0}{\bar{\gamma}_{vp}} - 1 \right)^{m-1} 
  \end{cases}
\end{equation*}

The following creep curve shows a comparison between the analytical and the numerical solutions of the modified Munson-Dawson model.

!media media/munson_dawson_strain.png style=display:block;margin:auto;width:60%; caption=Creep strain evolution in a Munson-Dawson's viscoplastic medium. id=fig_munson_dawson

## Complete Source Files

- [munson_dawson.i](https://github.com/ajacquey/beaver/blob/main/examples/viscoplasticity/munson_dawson/munson_dawson.i)

!bibtex bibliography

!content pagination use_title=True
                    previous=viscoplasticity/lemaitre.md
