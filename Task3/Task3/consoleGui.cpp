#include "consoleGui.h"

#include <windows.h>

bool consoleGui::tick() noexcept {
	system("cls");
	drawBorder();
	switch (currentState) {
	case WELCOME:
		processWelcome();
		break;
	case INIT:
		processInit();
		break;
	case ANALYZING:
		processAnalyzing();
		break;
	case RESULTS:
		processResults();
		break;
	case EXIT:
		processEnd();
		return false;
		break;
	default:
		break;
	}
	return true;
}

void consoleGui::setStyle(const std::string& textColor, const std::string& backgroundColor,
                          const std::string& textStyle) noexcept {
	std::cout << ("\033[" + textStyle + ";" + textColor + ";" + backgroundColor + "m");
}

void consoleGui::setForeground(const std::string& textColor) noexcept { std::cout << "\033[" + textColor + "m"; }

void consoleGui::setBackground(const std::string& backgroundColor) noexcept {
	std::cout << "\033[" + backgroundColor + "m";
}

void consoleGui::setTextStyle(const std::string& textStyle) noexcept { std::cout << "\033[" + textStyle + "m"; }

void consoleGui::resetStyle() noexcept { std::cout << "\033[0m"; }

std::string consoleGui::repeatN(const std::string& str, size_t n) const noexcept {
	std::string result;
	for (size_t i = 0; i < n; ++i) { result += str; }
	return result;
}

std::string consoleGui::repeatN(const char str, size_t n) const noexcept {
	std::string result;
	for (size_t i = 0; i < n; ++i) { result += str; }
	return result;
}

void consoleGui::printCentered(const std::string& text, const size_t y) noexcept {
	std::string buff = textShorten(text, innerW);
	size_t leftPadding = (innerW - buff.size()) / 2;

	setCursor(2, y);

	std::cout << std::left << std::setfill(SPACE) << std::setw(innerW) << (std::string(leftPadding, SPACE) + buff);
}

void consoleGui::printLeft(const std::string& text, size_t y, size_t padding, size_t maxW) noexcept {
	size_t blockW = (maxW == 0) ? innerW : maxW;

	size_t textLimit = (blockW > padding) ? blockW - padding : 0;

	std::string buff;
	if (textLimit < 3) buff = "@";
	else buff = textShorten(text, textLimit);

	setCursor(2 + padding, y);
	std::cout << std::left << std::setfill(SPACE) << std::setw(textLimit) << buff;
}

void consoleGui::printRight(const std::string& text, size_t y, size_t padding, size_t maxW) noexcept {
	size_t blockW = (maxW == 0) ? innerW : maxW;
	size_t textLimit = (blockW > padding) ? blockW - padding : 0;

	std::string buff;
	if (textLimit < 3) buff = "@";
	else buff = textShorten(text, textLimit);

	size_t startX = (frameW - 1) - padding - buff.size() + 1;

	setCursor(startX, y);
	std::cout << buff;
}

void consoleGui::drawBorder() noexcept {
	resetStyle();
	setCursor(1, 1);
	setForeground(borderColor);

	std::cout << CORNER_LEFT_TOP << repeatN(BORDER_HORIZONTAL, frameW - 2) << CORNER_RIGHT_TOP << "\n";
	for (size_t i = 0; i < frameH - 2; ++i) {
		std::cout << BORDER_VERTICAL << repeatN(SPACE, frameW - 2) << BORDER_VERTICAL << "\n";
	}
	std::cout << CORNER_LEFT_BOTTOM << repeatN(BORDER_HORIZONTAL, frameW - 2) << CORNER_RIGHT_BOTTOM << "\n";

	resetStyle();
}

void consoleGui::drawRow(size_t y, size_t type, size_t start, size_t end) noexcept {
	resetStyle();
	setForeground(borderColor);
	end = (end == 0) ? frameW : end;
	setCursor(start, y);

	if (type == 0) {
		std::cout << BORDER_T_JUNCTION_LEFT;

		for (size_t i = start + 1; i < end; ++i) { std::cout << BORDER_HORIZONTAL; }

		setCursor(end, y);
		std::cout << BORDER_T_JUNCTION_RIGHT;
	}
	else if (type == 1) {
		setCursor(start + 1, y);
		for (size_t i = start + 1; i < end; ++i) { std::cout << "-"; }
	}

	resetStyle();
}

void consoleGui::drawColumn(size_t x, size_t type, size_t start, size_t end) noexcept {
	resetStyle();
	setForeground(borderColor);
	end = (end == 0) ? frameH : end;

	setCursor(x, start);

	if (type == 0) {
		std::cout << BORDER_T_JUNCTION_TOP;

		for (size_t i = start + 1; i < end; ++i) {
			setCursor(x, i);
			std::cout << BORDER_VERTICAL;
		}

		setCursor(x, end);
		std::cout << BORDER_T_JUNCTION_BOTTOM;
	}
	else if (type == 1) {
		for (size_t i = start + 1; i < end; ++i) {
			setCursor(x, i);
			std::cout << "-";
		}
	}

	resetStyle();
}

void consoleGui::clearRow(size_t y) noexcept {
	setCursor(2, y);
	resetStyle();
	setForeground(mainBgColor);

	for (size_t i = 0; i < innerW; ++i) { std::cout << SPACE; }
}

void consoleGui::clearColumn(size_t x) noexcept {
	setCursor(x, 2);
	resetStyle();
	setForeground(mainBgColor);

	for (size_t i = 0; i < innerH; ++i) {
		std::cout << SPACE;
		setCursor(x, 2 + i + 1);
	}
}

void consoleGui::processWelcome() noexcept {
	size_t currentY = 7;
	std::string path;
	std::string scopeInput;
	size_t scopeNumber = 0;

	resetStyle();
	setForeground(headerTextColor);
	printCentered(WELCOME_HEADER, 3);

	setForeground(versionTextColor);
	printRight(WELCOME_VERSION, 3, 3);
	resetStyle();

	drawRow(5);
	setForeground(TextColor::Yellow);
	printCentered("Welcome!", currentY++);
	setForeground(TextColor::BrightBlack);
	printCentered("Files shark is a simple CLI for analyzing your project.", currentY++);
	printCentered("Explore your project and get detailed statistics", currentY++);
	printCentered("quickly, clearly, and right here in your terminal.", currentY++);
	printCentered("Ready to dive deep into your source code and file structure.", currentY++);
	resetStyle();
	currentY += 1;

	setForeground(borderColor);
	drawRow(currentY++, 1);
	resetStyle();

	do {
		currentY = 15;
		setForeground(TextColor::Cyan);
		printCentered("Please enter the path to analyze", currentY++);


		currentY++;
		setTextStyle(TextStyle::Blink);
		setForeground(TextColor::Cyan);
		printLeft(">>", innerH, 3);
		resetStyle();

		drawRow(innerH - 1, 0);
		setCursor(2 + 3 + 2 + 1, innerH);

		setForeground(mainTextColor);
		std::getline(std::cin, path);

		if (std::filesystem::exists(path) && std::filesystem::is_directory(path)) { break; }
		else {
			setForeground(TextColor::Red);
			printCentered("Invalid path! Please try again.", innerH - 2);
			resetStyle();
		}
	}
	while (true);

	clearRow(currentY);
	clearRow(currentY - 1);
	clearRow(currentY - 2);


	do {
		currentY = 14;
		clearRow(innerH);

		setForeground(TextColor::Cyan);
		printCentered("Select Analysis Scope:", currentY++);
		printCentered("(1) Current Directory Only", currentY++);
		printCentered("(2) Include All Subfolders", currentY++);

		setCursor(3, innerH);
		setTextStyle(TextStyle::Blink);
		setForeground(TextColor::Cyan);
		printLeft(">>", innerH, 3);
		resetStyle();

		setCursor(2 + 3 + 2 + 1, innerH);
		std::getline(std::cin, scopeInput);

		if (scopeInput == "1") {
			scopeNumber = 0;
			break;
		}
		else if (scopeInput == "2") {
			scopeNumber = 1;
			break;
		}
		else {
			setForeground(TextColor::Red);
			printCentered("Invalid scope! Please try again.", innerH - 2);
			resetStyle();
		}
	}
	while (true);

	analyzePath = path;
	scopeType = scopeNumber;
	currentState = INIT;
}

void consoleGui::processInit() noexcept {
	// INITING THE FILE ANALYZER
	setCursor(1, 1);
	drawBorder();

	setForeground(TextColor::Yellow);
	setTextStyle(TextStyle::Blink);
	printCentered("Analyzing directory...", 3);
	drawRow(5);

	fileStats stats;
	auto onProcess = [this](const std::string& path) { this->analyzingCallbackProcess(path); };

	auto onFinish = [this](fileStats total, bool status) { this->analyzingCallbackFinish(total, status); };

	analyzer.init(analyzePath, scopeType, nullptr, onProcess, onFinish);
	stats = analyzer.getStats();

	//=========================================================================
	// MAIN RENDER

	bool isStarting = false;
	auto scopeText = std::string((scopeType == 0) ? "Root Only" : "Subfolders");
	std::string extensionsString = "";

	for (const std::string& s : stats.extensions) {
		extensionsString += s;
		extensionsString += " ";
	}
	size_t currentY = 7;


	drawBorder();
	drawRow(5);
	drawRow(11, 1);
	drawRow(innerH - 1);
	setForeground(TextColor::Yellow);
	printLeft("Directory:", 3, 1, innerW / 2);
	printRight("Scope:", 3, 2 + scopeText.size(), innerW / 2);

	setForeground(TextColor::Cyan);
	setTextStyle(TextStyle::Italic);
	printLeft(analyzePath, 3, 12, innerW / 2);
	printRight(scopeText, 3, 1, innerW / 2);
	resetStyle();

	setForeground(mainTextColor);
	printCentered("Analysis Complete!", currentY++);
	printLeft("Extensions analyzed: " + extensionsString, currentY++, 2);
	printLeft("Files found: " + std::to_string(stats.filesFound), currentY++, 2);

	do {
		std::string temp;
		currentY = 13;

		setForeground(TextColor::Cyan);
		printLeft("Do you want to begin?", currentY++, 2);
		setForeground(TextColor::Green);
		printLeft("[Y] Let's dive in!", currentY++, 6);
		setForeground(TextColor::Red);
		printLeft("[N] Not now, return!", currentY++, 6);
		resetStyle();

		setCursor(3, innerH);
		setTextStyle(TextStyle::Blink);
		setForeground(TextColor::Cyan);
		printLeft(">>", innerH, 3);
		resetStyle();

		setCursor(2 + 3 + 2 + 1, innerH);
		std::getline(std::cin, temp);

		if (temp == "Y") {
			isStarting = true;
			break;
		}
		else if (temp == "N") {
			isStarting = false;
			currentState = WELCOME;
			break;
		}

		else {
			setForeground(TextColor::Red);
			printCentered("Invalid option! Please try again.", innerH - 2);
			resetStyle();
		}
	}
	while (true);

	if (isStarting) { currentState = ANALYZING; }
}

void consoleGui::processAnalyzing() noexcept {
	setCursor(1, 1);
	drawBorder();
	drawRow(5);
	drawRow(9, 1);

	setForeground(TextColor::Yellow);
	setTextStyle(TextStyle::Blink);
	printLeft("Analyzing in progress...", 3, 3);
	resetStyle();

	setForeground(TextColor::Cyan);
	printLeft("Progress:", 7, 2, 12);
	resetStyle();

	setForeground(TextColor::BrightBlack);
	printLeft("[" + repeatN(BLOCK_LIGHT, progressBarW) + "]", 7, progressBarX - 3);
	resetStyle();

	setForeground(mainBgColor);
	printCentered("Processing History", 11);
	resetStyle();

	setTextStyle(TextStyle::Italic);
	setForeground(TextColor::Cyan);
	printRight("[ 0% ]", 7, 3);
	printCentered("[ Analyzed: 0 / 0 ]", innerH - 1);
	resetStyle();

	auto startTime = std::chrono::high_resolution_clock::now();
	long long lastSeconds = -1;
	size_t lastPercent = 999;
	size_t currentY = 7;

	std::thread worker([this]() { analyzer.run(); });


	do {
		auto now = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - startTime).count();

		if (duration != lastSeconds) {
			lastSeconds = duration;

			char timeBuf[16];
			snprintf(timeBuf, sizeof(timeBuf), "%02lld:%02lld", (long long)duration / 60, (long long)duration % 60);

			setForeground(TextColor::Yellow);
			setTextStyle(TextStyle::Italic);
			printRight(timeBuf, 3, 3);
			resetStyle();
		}

		if (needUpdate.exchange(false)) {
			auto s = analyzer.getStats();
			size_t total = s.filesFound.load();
			size_t analyzed = s.filesAnalyzed.load();


			if (total > 0) {
				size_t progressPercent = (analyzed * 100 / total);

				if (progressPercent != lastPercent) {
					size_t filledW = (analyzed * progressBarW / total);

					setForeground(TextColor::Green);
					setCursor(progressBarX, 7);
					for (size_t i = 0; i < filledW; i++) std::cout << BLOCK_FULL;
					resetStyle();

					setTextStyle(TextStyle::Italic);
					setForeground(TextColor::Cyan);
					printRight("[ " + std::to_string(progressPercent) + "% ]", 7, 3);
					resetStyle();

					lastPercent = progressPercent;
				}
			}
			setTextStyle(TextStyle::Italic);
			setForeground(TextColor::Cyan);
			printCentered("[ Analyzed: " + std::to_string(analyzed) + " / " + std::to_string(total) + " ]", innerH - 1);
			resetStyle();

			{
				std::scoped_lock<std::mutex> lock(currentFileMutex);

				for (size_t i = 0; i < HISTORY_MAX_LINES; ++i) {
					std::string displayText = "";

					if (i < lastFiles.size()) {
						if (lastFiles.size() > HISTORY_MAX_LINES && i == 0) { displayText = "[...] ..."; }
						else {
							size_t index = (lastFiles.size() > HISTORY_MAX_LINES) ? i + 1 : i;
							if (index < lastFiles.size()) { displayText = lastFiles[index]; }
						}
					}

					setForeground(TextColor::BrightBlack);
					printLeft(displayText, 12 + i, 5);
					resetStyle();
				}
			}
		}
	}
	while (currentState == ANALYZING);

	if (worker.joinable()) { worker.join(); }
}

void consoleGui::processResults() noexcept {

	std::filesystem::path p(finalStats.defaultPath);
	std::string projectName = "";
	if (p.has_filename()) { projectName = p.filename().string(); }
	else { projectName = p.parent_path().filename().string(); }

	size_t diagramW = 74;
	size_t total = finalStats.lines.load();

	size_t codePercent = 0;
	size_t commentsPercent = 0;
	size_t blankPercent = 0;

	size_t filledCode = 0;
	size_t filledComments = 0;
	size_t filledBlank = 0;

	size_t statsX = 26;
	size_t statsNameW = 20;
	size_t diagramY = 12;

	if (total > 0) {
		filledCode = (finalStats.code.load() * diagramW / total);
		filledComments = (finalStats.comments.load() * diagramW / total);
		filledBlank = diagramW - filledCode - filledComments;

		codePercent = finalStats.code.load() * 100 / finalStats.lines.load();
		commentsPercent = finalStats.comments.load() * 100 / finalStats.lines.load();
		blankPercent = finalStats.blank.load() * 100 / finalStats.lines.load();
	}


	setCursor(1, 1);
	drawBorder();
	drawRow(3);
	drawRow(innerH - 1);
	drawRow(innerH - 6, 1);
	setForeground(TextColor::Yellow);
	printLeft("Overview of your {" + projectName + "} project", 2, 3);
	setForeground(TextColor::BrightBlack);
	printRight(
		"Analyzed " + std::to_string(finalStats.filesAnalyzed.load()) + "/" + std::to_string(finalStats.filesFound.load()) +
		" files", 2, 3);
	resetStyle();


	setForeground(mainTextColor);
	printLeft("[", 5, 2);
	printLeft("Elapsed time", 5, 4);
	printLeft("] -->", 5, statsNameW);

	printLeft("[", 6, 2);
	printLeft("Total lines", 6, 4);
	printLeft("] -->", 6, statsNameW);

	printLeft("[", 7, 2);
	setForeground(TextColor::Magenta);
	printLeft("Code lines", 7, 4);
	setForeground(mainTextColor);
	printLeft("] -->", 7, statsNameW);

	printLeft("[", 8, 2);
	setForeground(TextColor::Green);
	printLeft("Commented lines", 8, 4);
	setForeground(mainTextColor);
	printLeft("] -->", 8, statsNameW);

	printLeft("[", 9, 2);
	setForeground(TextColor::Red);
	printLeft("Blank lines", 9, 4);
	setForeground(mainTextColor);
	printLeft("] -->", 9, statsNameW);

	resetStyle();

	setForeground(TextColor::Cyan);
	setTextStyle(TextStyle::Italic);
	printLeft(std::to_string(
		          std::chrono::duration_cast<std::chrono::milliseconds>(finalStats.timerEnd - finalStats.timerBegin).count()) +
	          " ms", 5, statsX);
	printLeft(std::to_string(finalStats.lines.load()), 6, statsX);
	printLeft(std::to_string(finalStats.code.load()), 7, statsX);
	printLeft(std::to_string(finalStats.comments.load()), 8, statsX);
	printLeft(std::to_string(finalStats.blank.load()), 9, statsX);
	resetStyle();

	setForeground(mainTextColor);
	printCentered("LOC Distribution Diagram", 11);

	setForeground(TextColor::Magenta);
	printLeft(repeatN(BLOCK_FULL, filledCode), diagramY, 2);
	setForeground(TextColor::Green);
	printLeft(repeatN(BLOCK_FULL, filledComments), diagramY, 2 + filledCode);
	setForeground(TextColor::Red);
	printLeft(repeatN(BLOCK_FULL, filledBlank), diagramY, 2 + filledCode + filledComments);

	if (filledCode > 3) {
		std::string txt = std::to_string(codePercent) + "%";
		setCursor(4 + (filledCode / 2) - (txt.length() / 2), diagramY);
		setForeground(mainTextColor);
		setBackground(BackgroundColor::Magenta);
		std::cout << txt;
		resetStyle();
	}
	if (filledComments > 3) {
		std::string txt = std::to_string(commentsPercent) + "%";
		setCursor(4 + filledCode + (filledComments / 2) - (txt.length() / 2), diagramY);
		setForeground(mainTextColor);
		setBackground(BackgroundColor::Green);
		std::cout << txt;
		resetStyle();
	}
	if (filledBlank > 3) {
		std::string txt = std::to_string(blankPercent) + "%";
		setCursor(4 + filledCode + filledComments + (filledBlank / 2) - (txt.length() / 2), diagramY);
		setForeground(mainTextColor);
		setBackground(BackgroundColor::Red);
		std::cout << txt;
		resetStyle();
	}

	do {
		std::string temp;
		size_t currentY = 14;

		setForeground(TextColor::Cyan);
		printCentered("Do you want to save the result into file?", currentY++);
		setForeground(TextColor::Green);
		printLeft("[Y] Save it!", currentY++, 6);
		setForeground(TextColor::Red);
		printLeft("[N] No, exit.", currentY++, 6);
		resetStyle();

		setCursor(3, innerH);
		setTextStyle(TextStyle::Blink);
		setForeground(TextColor::Cyan);
		printLeft(">>", innerH, 3);
		resetStyle();

		setCursor(2 + 3 + 2 + 1, innerH);
		std::getline(std::cin, temp);

		if (temp == "Y") {
			do {
				size_t currentY = 14;

				setForeground(TextColor::Cyan);
				printCentered("Save file to:", currentY++);
				setForeground(TextColor::Green);
				printLeft("[1] Default path", currentY++, 6);
				setForeground(TextColor::Red);
				printLeft("[2] Custom path", currentY++, 6);
				resetStyle();

				setCursor(3, innerH);
				setTextStyle(TextStyle::Blink);
				setForeground(TextColor::Cyan);
				printLeft(">>", innerH, 3);
				resetStyle();

				setCursor(2 + 3 + 2 + 1, innerH);
				std::getline(std::cin, temp);

				if (temp == "1") {
					if (analyzer.saveToFile(finalStats.defaultPath, "")) {
						setForeground(TextColor::Green);
						printCentered("File saved successfully!", innerH - 3);
						printCentered(p.string() + "\\", innerH - 2);
						resetStyle();
						std::this_thread::sleep_for(std::chrono::seconds(1));
						currentState = EXIT;
						return;
					}
					else {
						setForeground(TextColor::Red);
						printCentered("Error saving file!", innerH - 2);
						resetStyle();
					}
				}
				else if (temp == "2") {
					do {
						size_t currentY = 15;
						std::string dirPath, fileName;
						namespace fs = std::filesystem;
						std::error_code ec;

						clearRow(14);
						clearRow(15);
						clearRow(16);

						setForeground(TextColor::Cyan);
						printCentered("Enter the directory path (Enter for current)", currentY);
						setCursor(3, innerH);
						setTextStyle(TextStyle::Blink);
						setForeground(TextColor::Cyan);
						printLeft(">>", innerH, 3);
						resetStyle();

						setCursor(2 + 3 + 2 + 1, innerH);
						std::getline(std::cin, dirPath);

						if (dirPath.empty()) dirPath = "./";

						fs::path p(dirPath);
						if ((fs::exists(p, ec) && !fs::is_directory(p)) || (!fs::exists(p) && !fs::create_directories(p, ec))) {
							setForeground(TextColor::Red);
							printCentered("Invalid directory path! Try again.", innerH - 2);
							resetStyle();
							continue;
						}

						setForeground(TextColor::Cyan);
						printCentered("Enter the file name (Enter for SHARK_REPORT)", currentY);

						setCursor(3, innerH);
						setTextStyle(TextStyle::Blink);
						setForeground(TextColor::Cyan);
						printLeft(">>", innerH, 3);
						resetStyle();

						setCursor(2 + 3 + 2 + 1, innerH);
						std::getline(std::cin, fileName);

						if (analyzer.saveToFile(dirPath, fileName)) {
							setForeground(TextColor::Green);
							printCentered("File saved successfully!", innerH - 3);
							printCentered(p.string() + "\\" + fileName, innerH - 2);
							resetStyle();
							std::this_thread::sleep_for(std::chrono::seconds(1));
							currentState = EXIT;
							return;
						}
						else {
							setForeground(TextColor::Red);
							printCentered("Error: Permission denied or file busy!", innerH - 2);
							resetStyle();
						}
					} while (true);
				}
				else {
					setForeground(TextColor::Red);
					printCentered("Invalid option! Please try again.", innerH - 2);
					resetStyle();
				}
			}
			while (true);
		}
		else if (temp == "N") {
			currentState = EXIT;
			return;
		}

		else {
			setForeground(TextColor::Red);
			printCentered("Invalid option! Please try again.", innerH - 2);
			resetStyle();
		}
	}
	while (currentState == RESULTS);
}

void consoleGui::processEnd() noexcept {
	setCursor(1, 1);
	drawBorder();
	setForeground(TextColor::Yellow);
	printCentered("Thank you for using Files Shark!", innerH / 2);
	resetStyle();
	std::this_thread::sleep_for(std::chrono::seconds(2));
	return;
}

void consoleGui::analyzingCallbackProcess(const std::string& path) noexcept {
	auto s = analyzer.getStats();
	size_t currentNum = s.filesAnalyzed.load();

	{
		std::scoped_lock<std::mutex> lock(currentFileMutex);

		// Форматируем строку
		std::string entry = "[ " + std::to_string(currentNum + 1) + " ] " + path;
		lastFiles.push_back(entry);

		if (lastFiles.size() > (HISTORY_MAX_LINES + 1)) { lastFiles.pop_front(); }
	}
	needUpdate = true;
}


void consoleGui::analyzingCallbackFinish(fileStats total, bool status) noexcept { currentState = RESULTS; finalStats = total; }

std::string consoleGui::textShorten(const std::string& text, const size_t width) {
	if (text.size() <= width) return text;
	else return text.substr(0, width - 3) + "...";
}
