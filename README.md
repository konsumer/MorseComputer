# MorseComputer

This is a morse code computer toy/tool I designed for my girlfriend's dad.

It can:

*  Understand keyed morse code
*  Understand audio morse code
*  Play "simon" with you, telling you to key in letters & keeping score

## software

*  [Arduino IDE](http://arduino.cc/en/main/software)
*  [Adafruit-GFX-Library](https://github.com/adafruit/Adafruit-GFX-Library)
*  [Adafruit-PCD8544-Nokia-5110-LCD-library](https://github.com/adafruit/Adafruit-PCD8544-Nokia-5110-LCD-library)
*  [MorseEnDecoder](https://code.google.com/p/morse-endecoder/)


## hardware

I included colors for the wires, so he could fix it easier if something came unhooked. I used an Arduino Nano.

Connect battery `+` (red) to `VIN` on mainboard, and `-` (black) to `GND`.

--

![Morse Key](http://i.ebayimg.com/00/s/MzQ3WDUwMA==/z/V9sAAMXQeW5TYCQu/$_12.JPG)

1 wire (white) goes to arduino `D2` , the other (black) goes to `GND`.

--

![speaker](https://cdn.sparkfun.com//assets/parts/2/5/7/0/09151-03-L.jpg)

1 wire goes to `D3` (white/green), the other goes to `GND`.

--

![microphone](https://cdn.sparkfun.com//assets/parts/4/0/9/7/09964-01.jpg)

`AUD` (yellow) goes to`A0`, `VCC` (red) to `5V`, `GND` (black) to `GND`

--

![PCD8544](https://encrypted-tbn3.gstatic.com/images?q=tbn:ANd9GcSIz_18tP3tDKlHnlJNDRjcZPimxqHfV-pPhzplJAzO9-HGfOgE)

| arduino pin     | color  | lcd     |
|-----------------|--------|---------|
| `D9`            | white  | `LIGHT` |
| `D8`            | yellow | `CLK`   |
| `D7`            | green  | `DIN`   |
| `D6`            | grey   | `DC`    |
| `D5`            | purple | `CE`    |
| `D4`            | blue   | `RST`   |
| `3.3`           | red    | `VCC`   |
| `GND`           | black  | `GND`   |