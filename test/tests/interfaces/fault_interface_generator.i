[Mesh]
  [file]
    type = FileMeshGenerator
    file = mesh-2D.msh
  []
  [interface_gen]
    type = BVFaultInterfaceGenerator
    input = file
    sidesets = 'interface'
  []
[]