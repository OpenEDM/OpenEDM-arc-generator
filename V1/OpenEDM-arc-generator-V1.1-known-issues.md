# OpenEDM Arc Generator V1.1: Known Issues

1. TVS diodes D4 and D9 must be soldered directly to the power MOSFET leads as close to the MOSFET's body as possible, which is very inconvenient.
1. Diode D8 heats up significantly and requires good cooling. This is expected, as it often carries a very high current. It would make sense to replace it with several MOSFETs connected in parallel.
1. Capacitors C19, C20, and C21 are critical for the device to function; without them, the MOSFETs will burn out. However, there are no designated mounting locations for them on the PCB, so they must be installed in a hacky way, removing the solder mask in the necessary places to solder the leads. This is very inconvenient.
1. Capacitor C19 heats up considerably, which is unsafe and shortens its lifespan. Most likely, the three capacitors C19, C20, and C21 are insufficient, and this part of the circuit should be redesigned.
1. The power MOSFETs used in the design are rated for excessively high voltage and current, and are therefore excessively expensive, at around $10 each.
1. The design uses an unfortunate method of cooling the power components. The leads of the power elements must be bent in a special way and soldered in specific positions and orientations; in addition, a heat sink with threaded holes arranged in a complex pattern is required.
