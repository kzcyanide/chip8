#include "libs/Chip8.h"
#include <fstream>



Chip8::Chip8() {

	pc = 0x200; // starting addrs of the program counter
	opcode = 0; // reset current opcode
	I = 0; // reset index register
	sp = 0; //reset stack pointer

	for(int i=0;i<4096;i++)
	memory[i]=0;  // reset the memory

	for (int i = 0; i < 16; i++)
	key[i]=V[i] = 0; // clear the registors

	for (int i = 0; i < 16; i++)
	stack[i] = 0; // clear the stack

	for (int i = 0; i < 64 * 32; i++)
	canvas[i] = 0; //clear the display


	// loading the fontset onto the chip8's memory

	for (int i = 0; i < 80; i++) {
		memory[i] = Chip8::chip8_fontset[i];
	}

	// resetting the timers;
	delay_timer = 0;
	sound_timer = 0;

	// resetting the flags;
	draw_flag = 0;
	sound_flag = 0;

	draw_flag = true;
}

//one emulation cycle
void Chip8::emulateCycle() {

	opcode = memory[pc] << 8 | memory[pc + 1];
	unsigned char x = (opcode & 0x0F00) >> 8;
	unsigned char y = (opcode & 0x00F0) >> 4;
	unsigned short addr = opcode & 0x0FFF;
	unsigned char kk = opcode & 0x00FF;
	unsigned char diff;
	unsigned char button;
	unsigned char sum;
	switch (opcode & 0xF000) {

		//some opcodes

	case 0xA000:
		I = addr;
		pc += 2;
		break;
	
	case 0x0000:
		switch (opcode & 0x000F)
		{
		case 0x0000: // 00E0 -CLS Clear the display
			//memset(canvas, '0', sizeof(canvas));
			for (int i = 0; i < 64 * 32; i++)
				canvas[i] = 0; //clear the display
			draw_flag = true;
			pc += 2;
			break;
		case 0x000E : // 00EE -RET return from a subroutine
			--sp;
			pc = stack[sp];
			pc += 2;
			break;

		default:
			std::cout << "Unknown opcode [0x0000]" << opcode << std::endl;
		}

		break;

	case 0x2000: // 2nnn -CAll addr Call subroutine at nnn
		stack[sp] = pc;
		sp++;
		pc = addr;
		break;

	case 0x1000 : // 1nnn -JP addr jump to location nnn
		pc = addr;
		break;

	case 0x3000 : // 3xkk -SE Vx, byte Skip next instruction if Vx=kk
		if (V[x] == kk)
			pc += 2;
		pc += 2;
		break;

	case 0x4000 : // 4xkk -SNE Vx, byte  Skip next instruction if Vx!=kk
		if (V[x] != kk)
			pc += 2;
		pc += 2;
		break;

	case 0x5000 : // 5xy0 -SE Vx, Vy Skip next instruction if Vx=Vy;
		if (V[x] == V[y])
			pc += 2;
		pc += 2;
		break;

	case 0x6000 : // 6xkk -LD Vx, byte Set Vx=kk
		V[x] = kk;
		pc += 2;
		break;

	case 0x7000 : // 7xkk -ADD Vx, byte Set Vx=Vx+kk
		V[x] += kk;
		pc += 2;
		break;

	case 0x8000:
		switch (opcode & 0x000F) {

		case 0x0000: // 8xy0 -LD Vx, Xy Set Vx=Vy
			V[x] = V[y];
			pc += 2;
			break;

		case 0x0001: // 8xy1 -OR Vx, Vy Set Vx=Vx OR Vy
			V[x] |= V[y];
			pc += 2;
			break;

		case 0x0002: // 8xy2 -AND Vx, Vy Set Vx=Vx AND Vy
			V[x] &= V[y];
			pc += 2;
			break;

		case 0x0003: // 8xy3 -XOR Vx, Vy Set Vx=Vx XOR Vy
			V[x] ^= V[y];
			pc += 2;
			break;

		case 0x0004: // 8xy4 -ADD Vx, Vy Set Vx = Vx + Vy, set VF = carry

			sum = V[x] + V[y];

			if (sum > 0xFF)
				V[0xF] = 1;

			else
				V[0xF] = 0;

			V[x] = sum;

			pc += 2;
			break;

		case 0x0005: // 8xy5 -SUB Vx, Vy Set Vx = Vx - Vy, set VF = NOT borrow

			diff = V[x] - V[y];

			if (V[x] > V[y])
				V[0xF] = 1;

			else
				V[0xF] = 0;

			V[x] = diff;

			pc += 2;
			break;

		case 0x0006: // 8xy6 -SHR Vx {, Vy} Set Vx = Vx SHR 1

			V[0xF] = (V[x] & 0x1);

			V[x] >>=1;

			pc += 2;
			break;

		case 0x0007: // 8xy7 -SUBN Vx, Vy Set Vx = Vy - Vx, set VF = NOT borrow

			diff = V[y] - V[x];

			if (V[y] > V[x])
				V[0xF] = 1;

			else
				V[0xF] = 0;

			V[x] = diff;

			pc += 2;
			break;

		case 0x000E: // 8xyE -SHL Vx {, Vy} Set Vx = Vx SHL 1

			V[0xF] = (V[x] & 0x1);

			V[x] <<= 1;

			pc += 2;
			break;

		default:
			std::cout << "Unknown opcode [0x8000]" << opcode << std::endl;



		}

		break;

	case 0x9000: // 9xy0 -SE Vx, Vy Skip next instruction if Vx!=Vy
		if (V[x] != V[y])
			pc += 2;
		pc += 2;
		break;

	case 0xB000: // Bnnn -JP V0,addr
		pc = addr + V[0];
		break;

	case 0xC000: // 0xCxkk -RND Vx, byte
		V[x] = (rand() % 256) & kk;
		pc += 2;
		break;

	case 0xD000: {// 0xDxyn -DRW Vx, Vy, nibble Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.

		

		unsigned short height = opcode & 0x000F;
		unsigned short pixel;

		x = V[x];
		y = V[y];

		V[0xF] = 0;

		for (int yline = 0; yline < height; yline++) {

			pixel = memory[I + yline];

			for (int xline = 0; xline < 8; xline++) {
				if ((pixel & (0x80 >> xline)) != 0) {
					if (canvas[x + xline + ((y + yline) * 64)] == 1)
						V[0xF] = 1;
					canvas[x + xline + ((y + yline) * 64)] ^= 1;
				}
			}
		}

		draw_flag = true;
		pc += 2;
		break;
		 }
		

	case 0xE000:
		switch (opcode & 0x00FF)
		{
		case 0x009E: {// Ex9E - SKP Vx Skip next instruction if key with the value of Vx is pressed.
			button = V[x];

			if (key[button]!=0)
				pc += 2;

			pc += 2;
			break;
			}
			

		case 0x00A1: { // ExA1 - SKNP Vx Skip next instruction if key with the value of Vx is not pressed.
			button = V[x];

			if (key[button]==0)
				pc += 2;

			pc += 2;
			break;
			 }
			

		default:

			std::cout << "Unknown opcode [0xE000]" << opcode << std::endl;

			break;
		}
		break;

	case 0xF000:
		switch (opcode & 0x0FF)
		{

		case 0x0007: // LD Vx, DT Set Vx = delay timer value.
			V[x] = delay_timer;
			pc += 2;
			break;

		case 0x000A: // Fx0A - LD Vx, K Wait for a key press, store the value of the key in Vx.
			
			for (int i = 0; i < 16; i++) {
				if (key[i]) {
					V[x] = i;
					pc += 2;
					break;
				}
			}
			break;

		case 0x0015: // LD DT, Vx Set delay timer = Vx.
			delay_timer = V[x];
			pc += 2;
			break;

		case 0x0018: // LD ST, Vx Set sound timer = Vx.
			sound_timer = V[x];
			pc += 2;
			break;

		case 0x001E: // ADD I, Vx Set I = I + Vx.
			I += V[x];
			pc += 2;
			break;

		case 0x0029: // LD F, Vx Set I = location of sprite for digit Vx.

			I = 5 * V[x];
			pc += 2;
			break;

		case 0x0033: // LD B, Vx Store BCD representation of Vx in memory locations I, I+1, and I+2.
			memory[I] = V[x] / 100;
			memory[I + 1] = (V[x] / 10) % 10;
			memory[I + 2] = (V[x] % 100) % 10;
			pc += 2;
			break;

		case 0x0055: // LD [I], Vx Store registers V0 through Vx in memory starting at location I.
			for (int i = 0; i <= x; i++) {
				memory[I + i] = V[i];
			}

			pc += 2;
			break;

		case 0x0065: // LD Vx, [I] Read registers V0 through Vx from memory starting at location I.
			for (int i = 0; i <= x; i++) {
				V[i] = memory[I+i];
			}

			pc += 2;
			break;


		default:

			std::cout << "Unknown opcode [0xF000]" << opcode << std::endl;
			std::cout << "At location" <<  pc << std::endl;

			break;
		}

		break;


	default:
		std::cout << "Unknown opcode" << opcode << std::endl;
		std::cout << "At location" << pc << std::endl;

	}

	if (delay_timer > 0)
		--delay_timer;

	if (sound_timer > 0) {

		if (sound_timer == 1)
			std::cout << "BEEP!" << std::endl;
		--sound_timer;
	}
	
}




//function to load the rom into the emu's memory
bool Chip8::loadGame(char const* filename) {

	std::ifstream file(filename, std::ios::binary | std::ios::ate);

		if (file.is_open()) {
			std::streampos size = file.tellg();
			char* buffer = new char[size];


			file.seekg(0, std::ios::beg);
			file.read(buffer, size);
			file.close();

			for (int i = 0; i < size; i++) {
				
				memory[START_ADDRESS + i] = buffer[i];
			}

			delete[] buffer;
			return true;

	}

		else {
			std::cout << "Error while opening the file";
			return false;
		}

}


void Chip8::debugRender()
{
	// Draw
	for (int y = 0; y < 32; ++y)
	{
		for (int x = 0; x < 64; ++x)
		{
			if (canvas[(y * 64) + x] == 0)
				printf("O");
			else
				printf(" ");
		}
		printf("\n");
	}
	printf("\n");
}