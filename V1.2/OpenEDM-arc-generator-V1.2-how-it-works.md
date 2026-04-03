# OpenEDM Arc Generator V1.2: How It Works

There are many different approaches to designing EDM arc generators. [Here](https://ietresearch.onlinelibrary.wiley.com/doi/epdf/10.1049/iet-pel.2020.0205) is an excellent paper that explains and compares many of them.

## Topology

<p align="center"><img src="https://github.com/OpenEDM/.github/blob/main/images/arc_generator_topology_v1.2.svg" width="40%"></p>

The **OpenEDM Arc Generator V1.1** topology includes MOSFETs **Q1**, **Q2** and **Q3**, Schottky diode **D1**, current sensors **CS1** and **CS2**, as well as the main power inductor **L1**. MOSFETs **Q1** and **Q3** form a half-bridge.

## Switching States

During operation, the Arc Generator continuously switches between different states. All of them are shown in the diagram below:

<p align="center"><img src="https://github.com/OpenEDM/.github/blob/main/images/arc_generator_states_v1.2.svg/" width="100%"></p>

Here is a brief description of all the states:

1. **STATE 0: INIT**. This is the initial state of the generator.
1. **STATE 1: INDUCTOR CHARGING**. In this state, the main power inductor is being charged with current from the power supply.
1. **STATE 2: PAUSE**. A pause state. At this point, there is no need to further charge the main inductor since the current is already within the desired range, and it’s not yet time for the next arc.
1. **STATE 3: IGNITION TYPE 1**. In this state, a high voltage (equal to the power supply voltage) is applied between the electrodes to break down the dielectric fluid and initiate the arc. During this process, current flows back into the power supply (in fact, into the capacitors installed at the output of the power supply).
1. **STATE 4: IGNITION TYPE 2**. In this state, a high voltage (equal to the power supply voltage) is applied between the electrodes to break down the dielectric fluid and initiate the arc. During this process, current does NOT return to the power supply but circulates through **Q1** and **D1**.
1. **STATE 5: ARC (POWER FROM INDUCTOR)**. The breakdown has occurred, an arc is established, and current flows through the arc. At this stage, the only source of energy for the arc is the main inductor.
1. **STATE 6: ARC (POWER FROM INDUCTOR & PS)**. Same as **STATE 5**, but with additional energy supplied from the power supply.

## Arc Generator Control

The **OpenEDM Arc Generator V1.2** and **OpenEDM Arc Generator V1.1** are quite similar in terms of control, so see the [corresponding paragraph for V1.1](https://github.com/OpenEDM/OpenEDM-arc-generator/blob/main/V1.1/OpenEDM-arc-generator-V1.1-how-it-works.md#arc-generator-control).