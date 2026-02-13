# OpenEDM Arc Generator V1: How It Works

There are many different approaches to designing EDM arc generators. [Here](https://ietresearch.onlinelibrary.wiley.com/doi/epdf/10.1049/iet-pel.2020.0205) is an excellent paper that explains and compares many of them.

## Topology

<p align="center"><img src="https://github.com/OpenEDM/.github/blob/main/images/arc_generator_topology.svg" width="40%"></p>

The **OpenEDM Arc Generator V1** topology includes MOSFETs **Q1** and **Q2**, Schottky diodes **D1** and **D2**, current sensors **CS1** and **CS2**, as well as the main power inductor **L1**.

## Switching States

During operation, the Arc Generator continuously switches between different states. All of them are shown in the diagram below:

<p align="center"><img src="https://github.com/OpenEDM/.github/blob/main/images/arc_generator_states.svg/" width="100%"></p>

Here is a brief description of all the states:

1. **STATE 0: INIT**. This is the initial state of the generator.
1. **STATE 1: INDUCTOR CHARGING**. In this state, the main power inductor is being charged with current from the power supply.
1. **STATE 2: PAUSE**. A pause state. At this point, there is no need to further charge the main inductor since the current is already within the desired range, and it’s not yet time for the next arc.
1. **STATE 3: IGNITION**. In this state, a high voltage (equal to the power supply voltage) is applied between the electrodes to break down the dielectric fluid and initiate the arc. During this process, current flows back into the power supply (in fact, into the capacitors installed at the output of the power supply).
1. **STATE 4: ARC (POWER FROM INDUCTOR)**. The breakdown has occurred, an arc is established, and current flows through the arc. At this stage, the only source of energy for the arc is the main inductor.
1. **STATE 5: ARC (POWER FROM INDUCTOR & PS)**. Same as **STATE 4**, but with additional energy supplied from the power supply.

## Arc Generator Control

The **OpenEDM Arc Generator V1** can be controlled in various ways, depending on the requirements. The power stage is quite universal and flexible, supporting multiple operating modes. Below is a description of generator control in iso-frequency mode (more details about the iso-frequency and iso-pulse modes can be found [here](https://www.revtn.ro/_legacy/pdf3-2014/21_Toma%20Emanoil.pdf)). This diagram shows the MOSFET control signals as a function of time:

<p align="center"><img src="https://github.com/OpenEDM/.github/blob/main/images/arc_generator_q1_q2_switching.svg" width="70%"></p>

The **main idea** here is that the current in the main inductor is always maintained at the level required for the arcs. Current regulation is handled by the transistor Q1, which operates in cycle-by-cycle current limiting mode. Transistor Q2 is responsible for controlling ignition and arcs.

Some details of this control method:

1. **Q2** control. This transistor is controlled purely based on time, using the parameters `t_on` (ignition delay + arc duration) and `t_off` (pause between arcs).
1. **Q1** control. The period of this transistor’s control signal is the same as that of **Q2**, but its phase is shifted by half a period. This transistor maintains the current in the main inductor using cycle-by-cycle current limiting method. In simple terms, **Q1** turns on according to the timer and turns off when the current through **CS1** exceeds the target value.
1. As can be seen, this control method uses only one current sensor, **CS1**.
1. In this control method, **STATE 5: ARC (POWER FROM INDUCTOR & PS)** is not used.

Below is the same information presented in the form of finite state machines. The diagram on the left, titled "Arc", corresponds to the case where the conditions in the gap allow arcs to occur, or when there is a short circuit between the electrodes. The diagram on the right, titled "No arc", corresponds to the case where no arcs are generated; for example, when the distance between the electrodes is too large.

<p align="center"><img src="https://github.com/OpenEDM/.github/blob/main/images/arc_generator_state_machines.svg/" width="100%"></p>
