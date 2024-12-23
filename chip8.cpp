#include <cstdint>
#include <iostream>
#include <fstream>
//#include "Window.cpp"
using namespace std;

class Chip8 {
    public:
        uint8_t memory[4096]{};
        uint32_t display[64 * 32]{}; // Monochrome display
        uint16_t pc{};
        uint16_t I{};
        uint16_t stack[16]{};
        uint8_t sp{}; // Stack pointer
        uint8_t delayTimer{}; // Decrements at 60Hz
        uint8_t soundTimer{}; // Decrements at 60Hz, buzzes when non-zero
        uint8_t registers[16]{}; // v0-vF
        size_t keys[16]{}; // Chip 8's input keys: 1 - 0xF 

        /* Constants */
        int const START_ADDRESS = 0x200; // Load ROM from this address onwards (512 in base 10)
        int const FONT_ADDRESS = 0x50; // Load Fonts at this address

        /* Initializations and utility functions */
        void loadRom(string ROM);
        void loadFonts();
        void updateTimers();
        void cycle();

        /* OP Codes */
        void OP_00E0(); // CLS
        void OP_00EE(); // RET
        void OP_0nnn(); // SYS addr
        void OP_1nnn(); // JP addr
        void OP_2nnn(); // CALL addr
        void OP_3xkk(); // SE Vx, byte
        void OP_4xkk(); // SNE Vx, byte
        void OP_5xy0(); // SE Vx, Vy
        void OP_6xkk(); // LD Vx, byte
        void OP_7xkk(); // ADD Vx, byte
        void OP_8xy0(); // LD Vx, Vy
        void OP_8xy1(); // OR Vx, Vy
        void OP_8xy2(); // AND Vx, Vy
        void OP_8xy3(); // xOR Vx, Vy
        void OP_8xy4(); // ADD Vx, Vy
        void OP_8xy5(); // SUB Vx, Vy
        void OP_8xy6(); // SHR Vx {, Vy}
        void OP_8xy7(); // SUBN Vx, Vy
        void OP_8xyE(); // SHL Vx {, Vy}
        void OP_9xy0(); // SNE Vx, Vy
        void OP_Annn(); // LD I, addr
        void OP_Bnnn(); // JP V0, addr
        void OP_Cxkk(); // RND Vx, byte
        void OP_Dxyn(); // DRW Vx, Vy, nibble
        void OP_Ex9E(); // SKP Vx
        void OP_ExA1(); // SKNP Vx
        void OP_Fx07(); // LD Vx, DT
        void OP_Fx0A(); // LD Vx, K
        void OP_Fx15(); // LD DT, Vx
        void OP_Fx18(); // LD ST, Vx
        void OP_Fx1E(); // ADD I, Vx
        void OP_Fx29(); // LD F, Vx
        void OP_Fx33(); // LD B, Vx
        void OP_Fx55(); // LD [I], Vx
        void OP_Fx65(); // LD Vx, [I]
};

/**
 * Constructor
 */
Chip8::Chip8() {
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

void Chip8::OP_00E0() {
    
}

void Chip8::OP_00EE() {
    
}

void Chip8::OP_0nnn() {
    
}

void Chip8::OP_1nnn() {
    
}

void Chip8::OP_2nnn() {
    
}

void Chip8::OP_3xkk() {
    
}

void Chip8::OP_4xkk() {
    
}

void Chip8::OP_5xy0() {
    
}

void Chip8::OP_6xkk() {
    
}

void Chip8::OP_7xkk() {
    
}

void Chip8::OP_8xy0() {
    
}

void Chip8::OP_8xy1() {
    
}

void Chip8::OP_8xy2() {
    
}

void Chip8::OP_8xy3() {
    
}

void Chip8::OP_8xy4() {
    
}

void Chip8::OP_8xy5() {
    
}

void Chip8::OP_8xy6() {
    
}

void Chip8::OP_8xy7() {
    
}

void Chip8::OP_8xyE() {
    
}

void Chip8::OP_9xy0() {
    
}

void Chip8::OP_Annn() {
    
}

void Chip8::OP_Bnnn() {
    
}

void Chip8::OP_Cxkk() {
    
}

void Chip8::OP_Dxyn() {
    
}

void Chip8::OP_Ex9E() {
    
}

void Chip8::OP_ExA1() {
    
}

void Chip8::OP_Fx07() {
    
}

void Chip8::OP_Fx0A() {
    
}

void Chip8::OP_Fx15() {
    
}

void Chip8::OP_Fx18() {
    
}

void Chip8::OP_Fx1E() {
    
}

void Chip8::OP_Fx29() {
    
}

void Chip8::OP_Fx33() {
    
}

void Chip8::OP_Fx55() {
    
}

void Chip8::OP_Fx65() {
    
}


/**
 * Main fetch-decode-execute cycle
 */
void Chip8::cycle() {
    // Fetch
    uint16_t instruction = (memory[pc] << 8) |  memory[pc+1];

    pc += 2;

    // Decode & Execute
    uint8_t b1 = (instruction & 0xFF00) >> 8;
    uint8_t b2 = instruction & 0x00FF;
    uint8_t n1 = (b1 & 0xF0) >> 4;
    uint8_t n2 = b1 & 0x0F;
    uint8_t n3 = (b2 & 0xF0) >> 4;
    uint8_t n4 = b2 & 0x0F;

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
            OP_1nnn();
            break;

        case 2:
            OP_2nnn();
            break;

        case 3:
            OP_3xkk();
            break;

        case 4:
            OP_4xkk();
            break;

        case 5:
            OP_5xy0();
            break;

        case 6:
            OP_6xkk();
            break;

        case 7:
            OP_7xkk();
            break;

        case 8:
            switch (n4) {
                case 0:
                    OP_8xy0();
                    break;

                case 1:
                    OP_8xy1();
                    break;

                case 2:
                    OP_8xy2();
                    break;
                
                case 3:
                    OP_8xy3();
                    break;
                
                case 4:
                    OP_8xy4();
                    break;
                
                case 5:
                    OP_8xy5();
                    break;
                
                case 6:
                    OP_8xy6();
                    break;
                
                case 7:
                    OP_8xy7();
                    break;
                
                case 0xE:
                    OP_8xyE();
                    break;    
            }
            break;

        case 9:
            OP_9xy0();
            break;

        case 0xA:
            OP_Annn();
            break;

        case 0xB:
            OP_Bnnn();
            break;

        case 0xC:
            OP_Cxkk();
            break;

        case 0xD:
            OP_Dxyn();
            break;

        case 0xE:
            switch (b2) {
                case 0x9E:
                    OP_Ex9E();
                    break;
                
                case 0xA1:
                    OP_ExA1();
                    break;
            }
            break;

        case 0xF:
            switch (b2) {
                case 0x07:
                    OP_Fx07();
                    break;

                case 0x0A:
                    OP_Fx0A();
                    break;

                case 0x15:
                    OP_Fx15();
                    break;

                case 0x18:
                    OP_Fx18();
                    break;
                
                case 0x1E:
                    OP_Fx1E();
                    break;

                case 0x29:
                    OP_Fx29();
                    break;

                case 0x33:
                    OP_Fx33();
                    break;
                
                case 0x55:
                    OP_Fx55();
                    break;

                case 0x65:
                    OP_Fx65();
                    break;
            }
            break;
    }
}

int main() {
    cout << "Starting" << endl;
    Chip8 chip8;
    chip8.loadRom("../roms/IBM Logo.ch8");
    chip8.loadFonts();

    return 0;
}