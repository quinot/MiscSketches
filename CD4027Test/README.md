CD4027Test
==========

Test whether a CD4027 IC behaves according to the [TI data sheet](https://www.ti.com/lit/ds/symlink/cd4027b.pdf).

Rule numbers refer to the lines in the truth table:
![truth table from data sheet](truth_table.jpg?raw=true "truth table")

Connections (for first flip-flop):
![breadboard schematic](CD4027Test_bb.png?raw=true "breadboard schematic")

Serial terminal connection:
```
tio -b 115200 -m ONLCRNL,INLCRNL /dev/ttyACM0
```
