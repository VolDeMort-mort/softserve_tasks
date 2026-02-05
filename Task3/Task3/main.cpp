#include <iostream>

#include "fileAnalyzer.h"
#include "consoleGui.h"
#include <windows.h>

int main() {
	consoleGui gui;
	int i = 0;
	fileStats stats;

	while (gui.tick()) {}



	while (0) {
		gui.renderStats(stats, i++);

		Sleep(200);
		std::cout << "\033[2J\033[H" << std::flush;
	}	

	std::cout << "\033[" << 25 << ";" << 0 << "H" << std::flush;
	/*
	system("cls");
	gui.demonstrateBackground();
	gui.demonstrateForeground();
	gui.demonstrateStyles();

	*/
}
