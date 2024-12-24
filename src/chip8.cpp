#include <cstdint>
#include <iostream>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include "Window.h"
using namespace std;

class Chip8 {
    public:
        /* Constants */
        static int const WIDTH = 64; // Display's x dimension 
        static int const HEIGHT = 32; // Display's y dimension
        int const START_ADDRESS = 0x200; // Load ROM from this address onwards (512 in base 10)
        int const FONT_ADDRESS = 0x50; // Load Fonts at this address

        /* Instance variables */
        uint8_t memory[4096]{};
        uint32_t display[WIDTH * HEIGHT]{}; // Monochrome display
        uint16_t pc{};
        uint16_t I{};
        uint16_t stack[16]{};
        uint8_t sp{}; // Stack pointer
        uint8_t delayTimer{}; // Decrements at 60Hz
        uint8_t soundTimer{}; // Decrements at 60Hz, buzzes when non-zero
        uint8_t registers[16]{}; // v0-vF
        size_t keys[16]{}; // Chip 8's input keys: 1 - 0xF 
        Window window;

        /* Initializations and utility functions */
        Chip8();
        void loadRom(string ROM);
        void loadFonts();
        void updateTimers();
        void cycle();

        /* OP Codes */
        void OP_00E0(); // CLS
        void OP_00EE(); // RET
        void OP_0nnn(uint16_t nnn); // SYS addr
        void OP_1nnn(uint16_t nnn); // JP addr
        void OP_2nnn(uint16_t nnn); // CALL addr
        void OP_3xkk(uint8_t x, uint8_t kk); // SE Vx, byte
        void OP_4xkk(uint8_t x, uint8_t kk); // SNE Vx, byte
        void OP_5xy0(uint8_t x, uint8_t y); // SE Vx, Vy
        void OP_6xkk(uint8_t x, uint8_t kk); // LD Vx, byte
        void OP_7xkk(uint8_t x, uint8_t kk); // ADD Vx, byte
        void OP_8xy0(uint8_t x, uint8_t y); // LD Vx, Vy
        void OP_8xy1(uint8_t x, uint8_t y); // OR Vx, Vy
        void OP_8xy2(uint8_t x, uint8_t y); // AND Vx, Vy
        void OP_8xy3(uint8_t x, uint8_t y); // xOR Vx, Vy
        void OP_8xy4(uint8_t x, uint8_t y); // ADD Vx, Vy
        void OP_8xy5(uint8_t x, uint8_t y); // SUB Vx, Vy
        void OP_8xy6(uint8_t x, uint8_t y); // SHR Vx {, Vy}
        void OP_8xy7(uint8_t x, uint8_t y); // SUBN Vx, Vy
        void OP_8xyE(uint8_t x, uint8_t y); // SHL Vx {, Vy}
        void OP_9xy0(uint8_t x, uint8_t y); // SNE Vx, Vy
        void OP_Annn(uint16_t nnn); // LD I, addr
        void OP_Bnnn(uint16_t nnn); // JP V0, addr
        void OP_Cxkk(uint8_t x, uint8_t kk); // RND Vx, byte
        void OP_Dxyn(uint8_t x, uint8_t y, uint8_t n); // DRW Vx, Vy, nibble
        void OP_Ex9E(uint8_t x); // SKP Vx
        void OP_ExA1(uint8_t x); // SKNP Vx
        void OP_Fx07(uint8_t x); // LD Vx, DT
        void OP_Fx0A(uint8_t x); // LD Vx, K
        void OP_Fx15(uint8_t x); // LD DT, Vx
        void OP_Fx18(uint8_t x); // LD ST, Vx
        void OP_Fx1E(uint8_t x); // ADD I, Vx
        void OP_Fx29(uint8_t x); // LD F, Vx
        void OP_Fx33(uint8_t x); // LD B, Vx
        void OP_Fx55(uint8_t x); // LD [I], Vx
        void OP_Fx65(uint8_t x); // LD Vx, [I]
};

/**
 * Constructor
 */
Chip8::Chip8() : window(WIDTH, HEIGHT, 20) {
    pc = START_ADDRESS;
}


/* Initialization and utility functions */

/**
 * Load the ROM into memory starting from address 0x200
 */
void Chip8::loadRom(string ROM) {
    ifstream buffer{ROM, ios::binary | ios::in};

    if (!buffer.is_open()) {
        cerr << "Unable to read file" << endl;
    }

    buffer.seekg(0, ios::end);
    size_t size = buffer.tellg();
    buffer.seekg(0, ios::beg);
    cout << "Reading " << size << " bytes from " << ROM << endl; 

    char* romData = new char[size];
    buffer.read(romData, size);

    for (size_t i = 0; i < size; i++) {
        memory[i + START_ADDRESS] = static_cast<uint8_t>(romData[i]);
    }

    delete[] romData;

    buffer.close();
}


/**
 * Load the fonts into memory in address 0x50 - 0x9F
 */
void Chip8::loadFonts() {
    uint8_t fonts[] = {
            0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
            0x20, 0x60, 0x20, 0x20, 0x70, // 1
            0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
            0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
            0x90, 0x90, 0xF0, 0x10, 0x10, // 4
            0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
            0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
            0xF0, 0x10, 0x20, 0x40, 0x40, // 7
            0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
            0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
            0xF0, 0x90, 0xF0, 0x90, 0x90, // A
            0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
            0xF0, 0x80, 0x80, 0x80, 0xF0, // C
            0xE0, 0x90, 0x90, 0x90, 0xE0, // D
            0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
            0xF0, 0x80, 0xF0, 0x80, 0x80  // F
            };

    for (int i = 0; i < 80; i++) {
        memory[i + FONT_ADDRESS] = fonts[i];
    }
}

/**
 * Decrement sound and delay timer by 1 if they are greater than 0    
 */
void Chip8::updateTimers() {
    if (soundTimer > 0) {
        soundTimer--;
    }
    if (delayTimer > 0) {
        delayTimer--;
    }
}


/* OP Codes */

/**
 * Clear Screen
 */
void Chip8::OP_00E0() {
    memset(display, 0, WIDTH * HEIGHT * sizeof(uint32_t));
    window.update(display);
}

/**
 * Return - Return from a subroutine
 */
void Chip8::OP_00EE() {
    sp--;
    pc = stack[sp];
    stack[sp] = 0;
}

/**
 * Execute machine language routine
 * 
 * NOTE: THIS FUNCTION IS NOT IMPLEMENTED IN MODERN INTERPRETERS
 */
void Chip8::OP_0nnn(uint16_t nnn) {
    // Do nothing
}

/**
 * Jump
 * 
 * @param nnn - Set PC to nnn
 */
void Chip8::OP_1nnn(uint16_t nnn) {
    pc = nnn;
}

/**
 * Call - Call subroutine at nnn
 * 
 * @param nnn - Address to jump to
 */
void Chip8::OP_2nnn(uint16_t nnn) {
    stack[sp] = pc;
    sp++;
    pc = nnn;
}

/**
 * Skip if equal
 * 
 * @param x - Register Vx
 * @param kk - Value to compare Vx to
 */
void Chip8::OP_3xkk(uint8_t x, uint8_t kk) {
    if (registers[x] == kk) {
        pc += 2;
    }
}

/**
 * Skip if not equal
 * 
 * @param x - Register Vx
 * @param kk - Value to compare Vx to
 */
void Chip8::OP_4xkk(uint8_t x, uint8_t kk) {
    if (registers[x] != kk) {
        pc += 2;
    }
}

/**
 * Skip if registers are equal
 * 
 * @param x - Register Vx
 * @param y - Register Vy
 */
void Chip8::OP_5xy0(uint8_t x, uint8_t y) {
    if (registers[x] == registers[y]) {
        pc += 2;
    }
}

/**
 * Set - set the register Vx to the value kk
 * 
 * @param x - Vx register to be set
 * @param kk - value to put in Vx
 */
void Chip8::OP_6xkk(uint8_t x, uint8_t kk) {
    registers[x] = kk;
}

/**
 * Add - Add the value kk to Vx
 * 
 * @param x - Register Vx
 * @param kk - Value to be added to Vx
 */
void Chip8::OP_7xkk(uint8_t x, uint8_t kk) {
    registers[x] += kk;
}

/**
 * Set - Set Vx to Vy
 * 
 * @param x - Register Vx
 * @param y - Register Vy
 */
void Chip8::OP_8xy0(uint8_t x, uint8_t y) {
    registers[x] = registers[y];
}

/**
 * OR - Set Vx to Vx OR Vy
 * 
 * @param x - Register Vx
 * @param y - Register Vy
 */
void Chip8::OP_8xy1(uint8_t x, uint8_t y) {
    registers[x] |= registers[y];
}

/**
 * AND - Set Vx to Vx AND Vy
 * 
 * @param x - Register Vx
 * @param y - Register Vy
 */
void Chip8::OP_8xy2(uint8_t x, uint8_t y) {
    registers[x] &= registers[y];
}

/**
 * XOR - Set Vx to Vx XOR Vy
 * 
 * @param x - Register Vx
 * @param y - Register Vy
 */
void Chip8::OP_8xy3(uint8_t x, uint8_t y) {
    registers[x] ^= registers[y];
}

/**
 * Add with carry flag - Add the value Vy to Vx and set VF to 1 if an overflow occurs
 * 
 * @param x - Register Vx
 * @param y - Register Vy
 */
void Chip8::OP_8xy4(uint8_t x, uint8_t y) {
    registers[x] += registers[y];
    if (registers[x] < registers[y]) {
        registers[0xF] = 1;
    }
    else {
        registers[0xF] = 0;
    }
}

/**
 * Substract - Substract the value Vy from Vx and set VF as appropriate
 * 
 * @param x - Register Vx
 * @param y - Register Vy
 */
void Chip8::OP_8xy5(uint8_t x, uint8_t y) {
    if (registers[x] > registers[y]) {
        registers[0xF] = 1;
    }
    else {
        registers[0xF] = 0;
    }
    registers[x] -= registers[y];
}

void Chip8::OP_8xy6(uint8_t x, uint8_t y) {
    
}

/**
 * Substract - Substract the value Vx from Vy and set VF as appropriate
 * 
 * @param x - Register Vx
 * @param y - Register Vy
 */
void Chip8::OP_8xy7(uint8_t x, uint8_t y) {
    if (registers[y] > registers[x]) {
        registers[0xF] = 1;
    }
    else {
        registers[0xF] = 0;
    }
    registers[x] = registers[y] - registers[x];
}

void Chip8::OP_8xyE(uint8_t x, uint8_t y) {
    
}

/**
 * Skip if registers are not equal
 * 
 * @param x - Register Vx
 * @param y - Register Vy
 */
void Chip8::OP_9xy0(uint8_t x, uint8_t y) {
    if (registers[x] != registers[y]) {
        pc += 2;
    }
}

/**
 * Set index
 * 
 * @param nnn - sets the index register I to the value nnn
 */
void Chip8::OP_Annn(uint16_t nnn) {
    I = nnn;
}

void Chip8::OP_Bnnn(uint16_t nnn) {
    
}

void Chip8::OP_Cxkk(uint8_t x, uint8_t kk) {
    
}

/**
 * Display
 * 
 * Draw an N pixels tall sprite from the memory location that the I index 
 * register is holding to the screen, at the horizontal X coordinate in VX and 
 * the Y coordinate in VY
 * 
 * @param x - Value of Vx is the x coordinate to start drawing from
 * @param y - Value of Vy is the y coordinate to start drawing from
 * @param n - Height of sprite in pixels
 */
void Chip8::OP_Dxyn(uint8_t x, uint8_t y, uint8_t n) {  
    uint8_t x_coord = registers[x] % WIDTH;
    uint8_t y_coord = registers[y] % HEIGHT;
    registers[0xF] = 0;

    cout << "Start of Dxyn - Drawing  n: " << int(n) << endl;  
    for (size_t i = 0; i < n; i++) {
        uint8_t spriteData = memory[I+i];
        for (size_t j = 0; j < 8; j++) {
            uint8_t pixel = (spriteData >> (8 - j - 1)) & 0x1 ;
            if (pixel) {
                if (display[y_coord*WIDTH + x_coord + j]) {
                    registers[0xF] = 1;
                }
                display[y_coord*WIDTH + x_coord + j] ^= 0xFFFFFFFF;
                cout << "Just drew a pixel at (" << int(x_coord + j)<<", " << int(y_coord) << ")" << endl;
            }
            if (x_coord + j + 1 >= WIDTH) break;

            //x++;
            //cout << "X: " << int(x) << " Y: " << int(y) << " I: " << I << " N: " << int(n) << " Sprite: " << int(spriteData) << " Pixel: " << int(pixel) << " Display: " << display[y_coord*WIDTH + x_coord] << endl;
        }
        cout << "Moving on to the next row: " << int(y_coord) << endl;
        y_coord++;

        if (y_coord + 1 >= HEIGHT) {
            cout << "EDGE OF (y) SCREEN - BREAKING \n\n\n" << endl;
            break;
        }
    }
    cout << "End of Dxyn - Drawing " << display << endl;
    window.update(display);
}

void Chip8::OP_Ex9E(uint8_t x) {
    
}

void Chip8::OP_ExA1(uint8_t x) {
    
}

void Chip8::OP_Fx07(uint8_t x) {
    
}

void Chip8::OP_Fx0A(uint8_t x) {
    
}

void Chip8::OP_Fx15(uint8_t x) {
    
}

void Chip8::OP_Fx18(uint8_t x) {
    
}

void Chip8::OP_Fx1E(uint8_t x) {
    
}

void Chip8::OP_Fx29(uint8_t x) {
    
}

void Chip8::OP_Fx33(uint8_t x) {
    
}

void Chip8::OP_Fx55(uint8_t x) {
    
}

void Chip8::OP_Fx65(uint8_t x) {
    
}


/**
 * Main fetch-decode-execute cycle
 */
void Chip8::cycle() {
    // Fetch
    uint16_t instruction = (memory[pc] << 8) |  memory[pc+1];

    pc += 2;

    // Decode & Execute
    uint8_t b1 = (instruction & 0xFF00) >> 8; // TODO: Make this a logical shift
    uint8_t b2 = instruction & 0x00FF;
    uint8_t n1 = (b1 & 0xF0) >> 4; // TODO: Make this a logical shift
    uint8_t n2 = b1 & 0x0F;
    uint8_t n3 = (b2 & 0xF0) >> 4; // TODO: Make this a logical shift
    uint8_t n4 = b2 & 0x0F;

    uint8_t x = n2; // Second nibble: Register lookup Vx (V0 - VF)
    uint8_t y = n3; // Third nibble: Register lookup Vx (V0 - VF)
    uint8_t n = n4; // Fourth nibble: A 4-bit number
    uint8_t kk = b2; // Second byte: An 8-bit immediate number
    uint16_t nnn = instruction & 0x0FFF;  // The second, third and fourth nibbles. A 12-bit immediate memory address
 
    switch (n1) {
        case 0:
            switch (b2) {
                case 0xE0:
                    OP_00E0();
                    break;
                
                case 0xEE:
                    OP_00EE();
                    break;
            }
            break;

        case 1:
            OP_1nnn(nnn);
            break;

        case 2:
            OP_2nnn(nnn);
            break;

        case 3:
            OP_3xkk(x, kk);
            break;

        case 4:
            OP_4xkk(x, kk);
            break;

        case 5:
            OP_5xy0(x, y);
            break;

        case 6:
            OP_6xkk(x, kk);
            break;

        case 7:
            OP_7xkk(x, kk);
            break;

        case 8:
            switch (n4) {
                case 0:
                    OP_8xy0(x, y);
                    break;

                case 1:
                    OP_8xy1(x, y);
                    break;

                case 2:
                    OP_8xy2(x, y);
                    break;
                
                case 3:
                    OP_8xy3(x, y);
                    break;
                
                case 4:
                    OP_8xy4(x, y);
                    break;
                
                case 5:
                    OP_8xy5(x, y);
                    break;
                
                case 6:
                    OP_8xy6(x, y);
                    break;
                
                case 7:
                    OP_8xy7(x, y);
                    break;
                
                case 0xE:
                    OP_8xyE(x, y);
                    break;    
            }
            break;

        case 9:
            OP_9xy0(x, y);
            break;

        case 0xA:
            OP_Annn(nnn);
            break;

        case 0xB:
            OP_Bnnn(nnn);
            break;

        case 0xC:
            OP_Cxkk(x, kk);
            break;

        case 0xD:
            OP_Dxyn(x, y, n);
            break;

        case 0xE:
            switch (b2) {
                case 0x9E:
                    OP_Ex9E(x);
                    break;
                
                case 0xA1:
                    OP_ExA1(x);
                    break;
            }
            break;

        case 0xF:
            switch (b2) {
                case 0x07:
                    OP_Fx07(x);
                    break;

                case 0x0A:
                    OP_Fx0A(x);
                    break;

                case 0x15:
                    OP_Fx15(x);
                    break;

                case 0x18:
                    OP_Fx18(x);
                    break;
                
                case 0x1E:
                    OP_Fx1E(x);
                    break;

                case 0x29:
                    OP_Fx29(x);
                    break;

                case 0x33:
                    OP_Fx33(x);
                    break;
                
                case 0x55:
                    OP_Fx55(x);
                    break;

                case 0x65:
                    OP_Fx65(x);
                    break;
            }
            break;
    }
}

int main() {
    cout << "Starting" << endl;
    Chip8 chip8 = Chip8();
    chip8.loadRom("../roms/IBM Logo.ch8");
    chip8.loadFonts();

    size_t i = 0;
    while (true) {
        chip8.cycle();
        chip8.updateTimers();
        i++;
    }

    sleep(30);

    return 0;
}