# hardware

## peripherals

| mainboard pin   | color          | hardware                      |
|-----------------|----------------|-------------------------------|
| `D2`            | white          | morse-key                     |
| `D3`            | white / green  | speaker                       |
| `A0`            | yellow         | mic `AUD`                     |
| `5V`            | red            | mic `VCC`                     |
| `GND`           | black          | mic `GND`, morse-key, speaker |

## lcd (PCD8544)

| mainboard pin   | color  | lcd     |
|-----------------|--------|---------|
| `D9`            | white  | `LIGHT` |
| `D8`            | yellow | `CLK`   |
| `D7`            | green  | `DIN`   |
| `D6`            | grey   | `DC`    |
| `D5`            | purple | `CE`    |
| `D4`            | blue   | `RST`   |
| `3.3`           | red    | `VCC`   |
| `GND`           | black  | `GND`   |

Connect battery `+` (red) to `VIN` on mainboard, and `-` (black) to `GND`.