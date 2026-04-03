# OpenEDM Arc Generator V2.1

![](https://github.com/OpenEDM/.github/blob/main/images/arc_generator_v2.1.png)

## Status

During the development process, it became clear that this design is “living on the specification edge”. The problem lies in the chosen method of cooling the MOSFETs (via the PCB). This method greatly improves the manufacturability of the PCBa, but at the same time imposes limitations on the max dissipated power of each MOSFET, i.e., on Rdson. It seemed to me that the MOSFETs used in V2.1 were well within these limitations. However, it later became clear that it is also important to consider the maximum voltage Vds that the MOSFET can withstand. During switching, due to parasitic inductances, this voltage can be quite high, and the MOSFETs need to be able to withstand it. However, the V2.1 MOSFETs can only withstand 200V, which does not provide sufficient margin. And we unfortunately couldn't find any other MOSFETs that would solve this puzzle. Because there are too many restrictions to satisfy: low Rdson, high Vds voltage, acceptable cost. Eventually we decided to put V2.1 on hold for now until better times.

# OpenEDM Arc Generator V2.1

1. [Schematic.](https://github.com/OpenEDM/OpenEDM-arc-generator/blob/main/V2.1/OpenEDM-arc-generator-V2.1-schematic.pdf)
1. [Switching states.](https://github.com/OpenEDM/OpenEDM-arc-generator/blob/main/V2.1/OpenEDM-arc-generator-V2.1-switching-states.pdf)
1. [Main power inductor design.](https://github.com/OpenEDM/OpenEDM-arc-generator/blob/main/common/OpenEDM-arc-generator-main-power-inductor-design.md)
1. [Simulation.](https://github.com/OpenEDM/OpenEDM-arc-generator/tree/main/V2.1/Simulation) You need to have [QSPICE](https://www.qorvo.com/design-hub/design-tools/interactive/qspice) installed to open the project. **It has not yet been confirmed that the simulation accurately matches reality**, so do not blindly trust the results.