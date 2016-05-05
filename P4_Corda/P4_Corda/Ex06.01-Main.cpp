#include "Game.h"


int main(int argc, char ** argv) {
	Game game("P4 Corda", WIDTH, HEIGHT,true,60,false);

	try {
		game.run();
	}
	catch (std::exception e) {
		std::cerr << "ERROR: " << e.what() << std::endl;
	}
	return 0;
}