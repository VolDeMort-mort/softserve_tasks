#include <iostream>
#include <filesystem>
#include "../task3_lib/ProjectScanner.h"
#include "../task3_lib/FileInfoWriter.h"

int main(int argc, char* argv[]) {
	if (argc == 1) {
		while (true) {
			std::cout << "Enter the path to the directory to scan:\n";
			std::string strDir;
			std::getline(std::cin, strDir);

			std::cout << "\nEnter the path to the result file\n";
			std::string strResultFile;
			std::getline(std::cin, strResultFile);

			std::filesystem::path dir(strDir);
			std::filesystem::path resultFile(strResultFile);

			bool isAppend(true);
			while (true) {
				std::cout << "\nOutput file mode: append or overwrite? [a/o]: \n";
				std::string answer;
				std::getline(std::cin, answer);

				if (answer[0] == 'o' || answer[0] == 'O') { isAppend = false; break; }
				if (answer[0] == 'a' || answer[0] == 'A') { isAppend = true; break; }	
			}

			ProjectScanner prScanner(dir, std::make_unique<FileInfoWriter>(resultFile, isAppend));
			prScanner.proccess();

			bool runAgain = false;
			while (true) {
				std::cout << "\nScan again? [y/n]: \n";
				std::string answer;
				std::getline(std::cin, answer);

				if (answer[0] == 'y' || answer[0] == 'Y') { runAgain = true; break; }
				if (answer[0] == 'n' || answer[0] == 'N') { runAgain = false; break; }
				std::cout << "Please enter 'y' or 'n'.\n";
			}
			if (!runAgain) break;
		}
	}
	else if (argc >= 3) {
		std::filesystem::path dir(argv[1]);
		std::filesystem::path resultFile(argv[2]);

		bool isAppend = true;
		for (int i = 0; i < argc; ++i) {
			std::string f = argv[i];
			if (f == "--overwrite" || f == "-o") isAppend = false;
			if (f == "--append" || f == "-a") isAppend = true;
		}
		
		ProjectScanner prScanner(dir, std::make_unique<FileInfoWriter>(resultFile, isAppend));
		prScanner.proccess();
	}
	else {
		std::cout << "How to use Project scanner:\n"
			"Execute programm without parametrs to enter the interactive mode\n"
			"Execute with 2 parametrs:\n"
			"\t1. Path to the project directory\n"
			"\t2. Path to the result file\n"
			"Available flags:\n"
			"\t --a: append to the result file\n"
			"\t --o: overwright the result file\n";
	}
	return 0;
}