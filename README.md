# Chip-8-Interpreter-py

Writing a chip-8 emulator to expand my horizons in computer architecture.

This project is a WIP.

Graphics, sound, and key inputs are processed using SDL2.

## How To Run
Build with make file. Run `make` in the `src` directory to build.
To start the emulator, run:
```
./chip8 ../roms/<name of rom>
```

### Optional Command-Line Arguments
- `--cp_shift`
-- Default: false
-- Enables the alternate implementation of OP_8xy6 and OP_8xyE instructions, following the behavior introduced by Super-Chip and the Chip-48 interpreter.

- `--sc_jump`
-- Default: false
-- Enables the alternate implementation of OP_Bnnn (Jump with offset), following the behavior introduced by the Super-Chip and the Chip-48 interpreter.

- `--cosmac_mem`
-- Default: false
-- Enables the alternate implementation of OP_Fx55 and OP_Fx65 (Store/Load Memory), following the original behavior set by the original COSMAC VIP CHIP-8 Interpreter. The default implementation (without this flag) follows the behavior introduced by the Super-Chip and the Chip-48 interpreter.

- `--scale <value>`
-- Default: 20
-- Specifies the scaling factor for the window size. The actual window dimensions are calculated as WIDTH * SCALE and HEIGHT * SCALE.

- `--speed <value>`
-- Default: 700
-- Specifies the number of instructions to run per second.

** Example Usage: **
```
./chip8 ../roms/IBM Logo.ch8 --cosmac_mem --sc_jump --scale 30 --speed 750
```

## Chip8 Key Mapping
![Chip-8 to Interpretter Layout](src/keypad.png)

## Memory Map
```
+---------------+= 0xFFF (4095) End of Chip-8 RAM
|               |
|               |
|               |
|               |
|               |
| 0x200 to 0xFFF|
|     Chip-8    |
| Program / Data|
|     Space     |
|               |
|               |
|               |
+- - - - - - - -+= 0x600 (1536) Start of ETI 660 Chip-8 programs
|               |
|               |
|               |
+---------------+= 0x200 (512) Start of most Chip-8 programs
| 0x000 to 0x1FF|
| Reserved for  |
|  interpreter  |
+---------------+= 0x000 (0) Start of Chip-8 RAM
```

## Resources:
- https://tobiasvl.github.io/blog/write-a-chip-8-emulator/ (Primary Source)
- http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
- https://austinmorlan.com/posts/chip8_emulator/ 