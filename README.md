# OpenEDM-arc-generator

## ⚠️ Safety Warning & Disclaimer

THIS PROJECT INVOLVES LETHAL VOLTAGES AND OTHER SERIOUS HAZARDS THAT CAN CAUSE SEVERE INJURY OR DEATH. YOU MUST READ THE FULL [SAFETY WARNING AND DISCLAIMER DOCUMENT](https://github.com/OpenEDM/.github/blob/main/docs/SAFETY_WARNING_AND_DISCLAIMER.md) BEFORE USING ANY PROJECT FILES. BY PROCEEDING, YOU ACKNOWLEDGE AND ACCEPT ALL RISKS AND AGREE TO USE THIS PROJECT ENTIRELY AT YOUR OWN RESPONSIBILITY.

## Main Specifications

<div align="center">

| Parameter          | V1                                           | V2                    |
| ------------------ | -------------------------------------------- | --------------------- |
| Input voltage (DC) | max. 150V                                    | 48V                   |
| Input power        | max. 200W                                    | max. 300W             |
| Arc current (peak) | adjustable, max. 30A                         | adjustable, max. 30A  |
| Ignition voltage   | fixed (same as the input voltage), max. 150V | adjustable, max. 175V |

</div>

## Hardware Versions

- **[V2.1]**. Status: put on hold due to some issues identified during simulation. Details are [here](https://github.com/OpenEDM/OpenEDM-arc-generator/tree/main/V2.1#status).

![](https://github.com/OpenEDM/.github/blob/main/images/arc_generator_v2.1.png)

- **[V1.2]**. Status: preparing for testing. Details are [here](https://github.com/OpenEDM/OpenEDM-arc-generator/tree/main/V1.2).

![](https://github.com/OpenEDM/.github/blob/main/images/arc_generator_v1.2.png)

- **[V1.1]**. Status: tested, all files except the PCB design are released and available [here](https://github.com/OpenEDM/OpenEDM-arc-generator/blob/main/V1.1/README.md).

![](https://github.com/OpenEDM/.github/blob/main/images/arc_generator_v1.1.png)

## Hardware Versioning Scheme

The format is: **`V<Topology>.<Version>r<Revision>`**
* Example: `V1.2r1`

### How to read the version:

* **Topology / Product Line (`1` or `2`)**: Defines the fundamental architecture.
  * `V1.x`: The simpler, base topology.
  * `V2.x`: The advanced topology (adjustable ignition voltage etc).
* **Version (`.0`, `.1`, ...)**: Major changes within the topology. Usually means changes in the schematic, new features, or a significantly different BOM.
* **Revision (`r1`, `r2`, ...)**: Minor patches and bug fixes. The schematic remains essentially the same. This includes fixing component footprints, adjusting silkscreen, or rerouting tracks to reduce noise. Note: If it's the first release of a version, the `r` suffix is omitted. E.g., `V1.2` is followed by `V1.2r1`.

## Project Logs on YouTube (V1.1 design)

[![](https://github.com/OpenEDM/.github/blob/main/images/youtube_playlist_thumbnails.jpg)](https://www.youtube.com/@OpenEDM)
