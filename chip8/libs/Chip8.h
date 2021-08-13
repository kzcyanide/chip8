#pragma once

#include<iostream>


class Chip8 {

public:
	unsigned char fontset[80]; //chip8 fontset
	unsigned short opcode; //32 opcodes 2 byte each
	unsigned char memory[4096]; //4kb memory
	unsigned char V[16]; //16 8bit registers (V0 - VF) VF for carry flag
	unsigned short I; // index register to store the current memory address
	unsigned short pc; //program counter Default starting addr=0x200
	unsigned int VIDEO_HEIGHT = 32;
	unsigned int VIDEO_WIDTH = 64;
	uint32_t  canvas[64 * 32];//canvas for drawing sprites and pixels
	unsigned char delay_timer; //the chip8's delay timer
	unsigned char sound_timer; //the chip8's sound timer
	unsigned short stack[16]; //16 level stack to keep track of operations
	unsigned short sp; //stack pointer
	unsigned char key[16]; // 16 hex keys for chip8's input 0x0 - 0xF (0 - 9 + A-F)
	unsigned char draw_flag;
	unsigned char sound_flag;
	const unsigned int START_ADDRESS = 0x200;
	unsigned char chip8_fontset[80] = {
	0xF0, 0x90, 0x90, 0x90, 0xF0,  // 0
	0x20, 0x60, 0x20, 0x20, 0x70,  // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0,  // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0,  // 3
	0x90, 0x90, 0xF0, 0x10, 0x10,  // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0,  // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0,  // 6
	0xF0, 0x10, 0x20, 0x40, 0x40,  // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0,  // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0,  // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90,  // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0,  // B
	0xF0, 0x80, 0x80, 0x80, 0xF0,  // C
	0xE0, 0x90, 0x90, 0x90, 0xE0,  // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0,  // E
	0xF0, 0x80, 0xF0, 0x80, 0x80   // F
	}; //chip8's font set


/*==================================================================================================================*/

	//void initialize();
	Chip8();
	void emulateCycle();
	bool loadGame(char const* filename);
	void debugRender();


};
