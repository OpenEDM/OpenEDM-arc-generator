# OpenEDM Arc Generator V1: Key Inputs For Power Inductor Design

1. Saturation current of the inductor must match the expected maximum arc current. For the OpenEDM generator, this is currently around 30 A.
1. Inductance should be as high as possible; the higher the inductance, the lower the required operating frequency of the current control loop, which in turn improves the efficiency of the arc generator.

# Core Selection

There are various core options available, for example those manufactured by [Magnetics](https://mag-inc.com), [Micrometals](https://micrometals.com), and others. Unfortunately, purchasing these products in small quantities can be quite difficult.

After a long search, I managed to find a suitable low-cost core on AliExpress: [KSF250-060A](https://es.aliexpress.com/item/1005005806889630.html). Its specifications are provided in the product description on AliExpress.

It’s also necessary to calculate the other parameters of the inductor, such as saturation current, inductance versus current, and so on. I’ve found two useful resources on calculation methods:

1. [Inductor Design with Magnetics Powder Cores](https://www.mag-inc.com/Design/Design-Guides/Inductor-Design-with-Magnetics-Powder-Cores.aspx) article from Magnetics.
1. [Magnetic Powder Cores Catalog from Chang Sung](https://github.com/OpenEDM/.github/blob/main/docs/chang_sung_magnetic_powder_cores_catalog.pdf), "Magnetic Design Formulas" section.

Here is the Python code for calculating these parameters:

```python
from math import pi

# KSF250-060A

A_L = 192 # nH / (N^2)
l_e = 14.37 # cm
N = 51 # turns
I = 25 # A

a = 1
b = 3.56e-05
c = 1.985

L_0 = A_L * (N ** 2) / 1000 # uH
H = 0.4 * pi * N * I / l_e # Oe
rolloff = 100 / (a + b * (H ** c)) # %
L = L_0 * rolloff / 100 # uH

print(f"{L_0 = :.2f} uH")
print(f"{H = :.2f} Oe")
print(f"{rolloff = :.2f} %")
print(f"L at {I}A = {L:.2f} uH")
```

The code prints the following:

```
L_0 = 499.39 uH
H = 111.50 Oe
rolloff = 70.80 %
L at 25A = 353.59 uH
```

Thus, with the number of turns set to 25 (this number was fine-tuned), this inductor will have an inductance of 500 µH at zero current and about 354 µH at 25 A, which is quite acceptable.

# Winding Calculation

First, we need to calculate the required wire cross-section. [This table](https://www.mag-inc.com/getattachment/Design/Design-Guides/Inductor-Design-with-Magnetics-Powder-Cores/WireTable2017.pdf?lang=en-US) could be used for that. Let’s assume a current density of 500 A/cm², as recommended in the Magnetics design guide. This gives us approximately 10 AWG for the core's saturation current of ~25 A, corresponding to a copper wire diameter of 2.67 mm.

Because of the [skin effect](https://en.wikipedia.org/wiki/Skin_effect), the wire used to wind the inductor should ideally not be a single solid conductor. One option is to use [litz wire](https://en.wikipedia.org/wiki/Litz_wire), though it can be hard to source. Another practical option is to twist several strands of enameled copper wire together.

To roughly determine the winding parameters, [this calculator](https://www.coilwindingmachines.eu/engineers_corner/toroidal_calculations.html) could be used.
