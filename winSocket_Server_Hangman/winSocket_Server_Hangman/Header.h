#pragma comment (lib, "ws2_32.lib")

#include <WS2tcpip.h>
#include <iostream>
#include <string>

// define anything, including mehthods and structs
#ifndef MYSTRUCT_H
#define MYSTRUCT_H 
struct hangMan {
	char word[5] = "word";
	int lifes = 10;
	int tries = 0;

	int game(std::string guess) {
		if (guess == std::string(word))
		{
			tries++;
			return 0;
		}	
		else
		{
			if (lifes > 1)
			{
				tries++;
				lifes--;
				return lifes;
			}
			else
			{
				lifes--;
				return -1;
			}

		}
	}
};
#endif
// need to implement/define content to use across programm globally in here