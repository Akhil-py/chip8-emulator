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
        int const FONT_ADDRESS = 0X50; // Load Fonts at this address

        /* Initializations and utility functions*/
        void loadRom(string ROM);
        void loadFonts();
        void updateTimers();
        void cycle();

        /* OP Codes */
};

/**
 * Constructor
 */
Chip8::Chip8() {
    pc = START_ADDRESS;
}


/* Initialization functions */

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

/**
 * Main fetch-decode-execute cycle
 */
void Chip8::cycle() {
    // Fetch
    uint16_t instruction = (memory[pc] << 8) |  memory[pc+1];

    pc += 2;

    // Decode
    switch (instruction) {
        
    }

    // Execute
}

int main() {
    cout << "Starting" << endl;
    Chip8 chip8;
    chip8.loadRom("../roms/IBM Logo.ch8");
    chip8.loadFonts();

    return 0;
}