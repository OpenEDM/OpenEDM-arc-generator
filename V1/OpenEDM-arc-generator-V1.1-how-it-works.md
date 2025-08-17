# OpenEDM Arc Generator V1: How It Works

There are many different approaches to designing EDM arc generators. [Here](https://ietresearch.onlinelibrary.wiley.com/doi/epdf/10.1049/iet-pel.2020.0205) is an excellent paper that explains and compares many of them.

## Topology

![](https://github.com/OpenEDM/.github/blob/main/images/arc_generator_topology.png)

This topology includes MOSFETs **Q1** and **Q2**, Schottky diodes **D1** and **D2**, current sensors **CS1** and **CS2**, as well as the main power inductor **L1**.

## Switching States

During operation, the Arc Generator continuously switches between different states. All of them are shown in the diagram below:

![](https://github.com/OpenEDM/.github/blob/main/images/arc_generator_states.png)

Here is a brief description of all the states:

1. **STATE 0: INIT**. This is the initial state of the generator.
1. **STATE 1: INDUCTOR CHARGING**. In this state, the main power inductor is being charged with current from the power supply.
1. **STATE 2: PAUSE**. A pause state. At this point, there is no need to further charge the main inductor since the current is already within the desired range, and it’s not yet time for the next arc.
1. **STATE 3: IGNITION**. In this state, a high voltage (equal to the power supply voltage) is applied between the electrodes to break down the dielectric fluid and initiate the arc. During this process, current flows back into the power supply (in fact, into the capacitors installed at the output of the power supply).
1. **STATE 4: ARC (POWER FROM INDUCTOR)**. The breakdown has occurred, an arc is established, and current flows through the arc. At this stage, the only source of energy for the arc is the main inductor.
1. **STATE 5: ARC (POWER FROM INDUCTOR & PS)**. Same as **STATE 4**, but with additional energy supplied from the power supply.

Switching between stated can be implemented in different ways. One possible option is shown below. The diagram on the left, titled "Arc", corresponds to the case where the conditions in the gap allow arcs to occur, or when there is a short circuit between the electrodes. The diagram on the right, titled "No arc", corresponds to the case where no arcs are generated; for example, when the distance between the electrodes is too large.

![](https://github.com/OpenEDM/.github/blob/main/images/arc_generator_state_machines.png)
