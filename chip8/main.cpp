#include "libs/libs.h"
#include"libs/canvas.h"
#include "libs/Chip8.h" 



int main(int argc, char** argv) {

	
	if (argc < 2)
	{
		std::cerr << "Usage: " << argv[0] << " [rom filename]\n";
		return 1;
	}

	Chip8 mychip8;

	//initialize the chip8's system and load the game into the memory

	if (!mychip8.loadGame(argv[1])) {
		
		return 1;
	}

	
	// graphics window for displaying stuff 
	canvas mycanvas("pong",mychip8.VIDEO_WIDTH * 10,mychip8.VIDEO_HEIGHT * 10, mychip8.VIDEO_WIDTH, mychip8.VIDEO_HEIGHT);

	int videoPitch = sizeof(mychip8.canvas[0]) * mychip8.VIDEO_WIDTH;
	int cycleDelay = 2;

	auto lastCycleTime = std::chrono::high_resolution_clock::now();
	bool quit = false;


	// emulation loop
	while (!quit) {

		auto currentTime = std::chrono::high_resolution_clock::now();
		float dt = std::chrono::duration<float, std::chrono::milliseconds::period>(currentTime - lastCycleTime).count();


		//if the draw flag is set update the screen
		//0x00E0 opcode clears the screen
		//0xDXYN opcode draws a sprite on the screen
		//store key press state

		quit = mycanvas.ProcessInput(mychip8.key);
		

		if (dt > cycleDelay)
		{
			lastCycleTime = currentTime;

			//emulate one cycle
			mychip8.emulateCycle();

			if (mychip8.draw_flag) {
				mycanvas.Update(mychip8.canvas, videoPitch);
				mychip8.draw_flag = 0;
				//mychip8.debugRender();
			}

		}
			
	}

	return 0;

}