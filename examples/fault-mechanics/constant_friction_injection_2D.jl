using DDMFaultSlip
using StaticArrays
using SpecialFunctions
using LinearAlgebra

###### PHYSICAL PARAMETERS ######
const T = parse(Float64, ARGS[1])::Float64
const λ = 5.33e+01::Float64
const μ = 5.33e+01::Float64
const h = 5.33e-04::Float64
const f = 1.0::Float64
const σ₀ = 1.0::Float64
const t₀ = 0.0::Float64 # start time
const tₑ = 1.0::Float64 # end time
const L = 6.0::Float64 # length of the domain
###### NUMERICAL PARAMETERS ######
const n = 10000::Int64
const nₜ = 500::Int64

###### T-DEPENDENT PARAMETERS ######
# Background shear stress
function computeTDependentShearStress(T::Float64)::Float64
    return σ₀ - T
end

const τ₀ = computeTDependentShearStress(T)::Float64 # background shear stress

###### INITIAL CONDITIONS ######
function sigma_ic(X)
    return σ₀ * ones(length(X))
end

function tau_ic(X)
    return τ₀ * ones(length(X))
end

###### FLUID PRESSURE ######
function fluid_pressure(X, time)
    return erfc.([abs(X[idx][1]) for idx in eachindex(X)] / sqrt(time))
end

function main()
    ###### MESH ######
    start_point = SVector(-L / 2.0, 0.0)
    end_point = SVector(L / 2.0, 0.0)
    mesh = DDMesh1D(start_point, end_point, n)

    ###### PROBLEM ######
    problem = ShearDDProblem(mesh; μ=μ)

    ###### ICs ######
    addNormalStressIC!(problem, sigma_ic)
    addShearStressIC!(problem, tau_ic)

    ###### FLUID COUPLING ######
    addFluidCoupling!(problem, FunctionPressure(mesh, fluid_pressure))

    ###### FRICTION ######
    addFrictionConstraint!(problem, ConstantFriction(f, μ / h))

    ###### OUTPUTS ######
    outputs = [CSVDomainOutput(mesh, "outputs/julia/constant_friction_injection_2D_T_$(T)/constant_friction_injection_2D_T_$(T)")]

    ###### TIME SEQUENCE ######
    # time_seq = collect(exp10.(range(log10(t₀+dt₀), stop=log10(tₑ), length=nₜ)))
    # # Quick fix
    # time_seq[end] = tₑ
    # time_stepper = TimeSequence(time_seq; start_time=0.0, end_time=tₑ)
    time_stepper = ConstantDT(t₀, tₑ, nₜ)

    ###### RUN PROBLEM ######
    run!(problem, time_stepper; outputs=outputs)
end

main()