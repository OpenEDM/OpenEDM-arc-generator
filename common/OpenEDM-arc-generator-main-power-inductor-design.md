# OpenEDM Arc Generator: Key Inputs for Power Inductor Design

1. The inductor must retain sufficient inductance at the maximum expected peak current. For the OpenEDM generator, the design target is currently around 25 A.
1. Inductance should be as high as possible; the higher the inductance, the lower the required operating frequency of the current control loop, which in turn improves the efficiency of the arc generator.

# Core Selection

⚠️ The KSF250-060A core with 51 turns has been tested in practice in OpenEDM. The other four cores below have not been tested in practice; they are candidates evaluated by calculation only.

The following table compares five inductor cores:

| Core | Material / permeability | Coated dimensions, mm | Turns | L at 0 A, µH | L at 25 A, µH |
| ---------------------------------------------- | ---------------- | --------------------- | -- | --- | --- |
| [KSF250-060A][ksf-datasheet]                   | Si–Fe / 60       | 63.10 × 31.37 × 26.27 | 51 | 499 | 354 |
| [Magnetics 0078617A7][xflux60-datasheet]       | XFlux / 60       | 63.09 × 31.70 × 25.91 | 52 | 511 | 379 |
| [Magnetics 0078615A7][xflux26-datasheet]       | XFlux / 26       | 63.09 × 31.70 × 25.91 | 70 | 402 | 381 |
| [Magnetics 0074617A7][ultra60-datasheet]       | XFlux Ultra / 60 | 63.09 × 31.70 × 25.91 | 52 | 511 | 402 |
| [Micrometals MS-250060-2][sendust60-datasheet] | Sendust / 60     | 64.77 × 30.48 × 25.90 | 51 | 536 | 236 |

Notes:
1. Dimensions include the core coating, but exclude the winding, and are listed as `outside diameter × inside diameter × height`.
2. Inductance values are nominal estimates based on the manufacturers' DC-bias curves. They do not include manufacturing tolerances and should not be interpreted as guaranteed minimum values.

Purchase options checked on **15 November 2026**:

| Core | Supplier | Listed price for one core | Minimum order / listed stock |
| --- | --- | ---: | --- |
| 0078617A7 | [DigiKey][xflux60-buy] | €20.06 | 1 piece / 56 pieces |
| 0078615A7 | [DigiKey][xflux26-buy] | €20.06 | 1 piece / 73 pieces |
| 0074617A7 | [DigiKey][ultra60-buy] | €24.94 | 1 piece / 58 pieces |
| MS-250060-2 | [TME][sendust60-buy] | €12.17 | 1 piece / 15 pieces |

# Inductance Calculation

Useful design references include:

1. [Inductor Design with Magnetics Powder Cores](https://www.mag-inc.com/design/design-guides/powder-core-inductor-design).
2. [Magnetic Powder Cores Catalog from Chang Sung](https://github.com/OpenEDM/.github/blob/main/docs/chang_sung_magnetic_powder_cores_catalog.pdf), especially the "Magnetic Design Formulas" section.
3. [Magnetics XFlux material curves][xflux-curves] and [XFlux Ultra material curves][ultra-curves].

The following Python code reproduces the inductance comparison above. All coefficients are normalized to the same form:

```text
L_0 = A_L * (N ** 2) / 1000           # µH, with A_L in nH/turn²
H = 0.4 * pi * N * I / l_e            # Oe, with l_e in cm
retained_fraction = 1 / (a + b * H**c)
L = L_0 * retained_fraction           # µH
```

The Magnetics and Micrometals sources express their fits as a percentage, using `a = 0.01`. The code below multiplies their `a` and `b` coefficients by 100 to return a fraction instead. Do not mix the two conventions, or reuse coefficients for a different material or permeability.

```python
from math import pi

# A_L: nH/turn²; l_e: cm; N: turns.
# Coefficients return the retained fraction, not a percentage.
# Each core uses its own material-specific empirical fit.

cores = [
    {
        "name": "KSF250-060A",
        "A_L": 192,
        "l_e": 14.37,
        "N": 51,
        "a": 1.0,
        "b": 3.56e-5,
        "c": 1.985,
    },
    {
        "name": "0078617A7",
        "A_L": 189,
        "l_e": 14.40,
        "N": 52,
        "a": 1.0,
        "b": 1.489e-6,
        "c": 2.613,
    },
    {
        "name": "0078615A7",
        "A_L": 82,
        "l_e": 14.40,
        "N": 70,
        "a": 1.0,
        "b": 6.304e-8,
        "c": 2.714,
    },
    {
        "name": "0074617A7",
        "A_L": 189,
        "l_e": 14.40,
        "N": 52,
        "a": 1.0,
        "b": 4.483e-7,
        "c": 2.815,
    },
    {
        "name": "MS-250060-2",
        "A_L": 206,
        "l_e": 14.314,
        "N": 51,
        "a": 1.0,
        "b": 2.151e-4,
        "c": 1.841,
    },
]


def inductance(core, current):
    L_0 = core["A_L"] * core["N"]**2 / 1000
    H = 0.4 * pi * core["N"] * current / core["l_e"]
    retained_fraction = 1 / (core["a"] + core["b"] * H**core["c"])
    return L_0 * retained_fraction


print(f"{'Core':<16} {'Turns':>5}{'L(0 A)':>10}{'L(25 A)':>10}")

for core in cores:
    values = [inductance(core, current) for current in (0, 25)]
    print(
        f"{core['name']:<16} {core['N']:>5}"
        + "".join(f"{value:>10.1f}" for value in values)
    )

print("Inductance values are in µH.")
```

Expected output:

```text
Core             Turns    L(0 A)   L(25 A)
KSF250-060A         51     499.4     353.6
0078617A7           52     511.1     379.0
0078615A7           70     401.8     381.5
0074617A7           52     511.1     401.5
MS-250060-2         51     535.8     235.7
Inductance values are in µH.
```

# Winding Calculation

First, we need to calculate the required wire cross-section. [This table](https://www.mag-inc.com/Media/Magnetics/File-Library/Product%20Literature/Powder%20Core%20Literature/Wire-Table.pdf) could be used for that. Let’s assume a current density of 500 A/cm², as recommended in the Magnetics design guide. This gives us approximately 10 AWG for the core's saturation current of ~25 A, corresponding to a copper wire diameter of 2.67 mm.

Because of the [skin effect](https://en.wikipedia.org/wiki/Skin_effect), the wire used to wind the inductor should ideally not be a single solid conductor. One option is to use [litz wire](https://en.wikipedia.org/wiki/Litz_wire), though it can be hard to source. Another practical option is to twist several strands of enameled copper wire together.

To roughly determine the winding parameters, [this calculator](https://www.coilwindingmachines.eu/engineers_corner/toroidal_calculations.html) could be used.

[ksf-datasheet]: https://kdm-mag.com/uploads/file/20201007/1602053279257233.pdf
[xflux60-datasheet]: https://www.mag-inc.com/Media/Magnetics/Datasheets/0078617A7.pdf
[xflux26-datasheet]: https://www.mag-inc.com/Media/Magnetics/Datasheets/0078615A7.pdf
[ultra60-datasheet]: https://www.mag-inc.com/Media/Magnetics/Datasheets/0074617A7.pdf
[sendust60-datasheet]: https://datasheets.micrometals.com/MS-250060-2-DataSheet.pdf
[xflux-curves]: https://www.mag-inc.com/products/powder-cores/xflux-cores/xflux-material-curves
[ultra-curves]: https://www.mag-inc.com/products/powder-cores/xflux-ultra-cores/xflux-ultra-material-curves
[xflux60-buy]: https://www.digikey.fi/en/products/detail/magnetics-a-division-of-spang-co/0078617A7/18626674
[xflux26-buy]: https://www.digikey.fi/en/products/detail/magnetics-a-division-of-spang-co/0078615A7/18626779
[ultra60-buy]: https://www.digikey.fi/en/products/detail/magnetics-a-division-of-spang-co/0074617A7/21221965
[sendust60-buy]: https://www.tme.eu/fi/en/details/ms-250060-2/ring-ferrites/micrometals/
