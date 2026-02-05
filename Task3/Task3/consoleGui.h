#pragma once

#include <string>
#include <vector>
#include <mutex>
#include "fileAnalyzer.h"

class consoleGui {
public:
	struct TextStyle {
		inline static const std::string Reset = "0";
		inline static const std::string Bold = "1";
		inline static const std::string Dim = "2";
		inline static const std::string Italic = "3";
		inline static const std::string Underline = "4";
		inline static const std::string Blink = "5";
		inline static const std::string Reverse = "7";
		inline static const std::string Strike = "9";
		inline static const std::string Normal = "22";
	};

	struct TextColor {
		inline static const std::string Black = "30";
		inline static const std::string Red = "31";
		inline static const std::string Green = "32";
		inline static const std::string Yellow = "33";
		inline static const std::string Blue = "34";
		inline static const std::string Magenta = "35";
		inline static const std::string Cyan = "36";
		inline static const std::string White = "37";

		inline static const std::string BrightBlack = "90";
		inline static const std::string BrightRed = "91";
		inline static const std::string BrightGreen = "92";
		inline static const std::string BrightYellow = "93";
		inline static const std::string BrightBlue = "94";
		inline static const std::string BrightMagenta = "95";
		inline static const std::string BrightCyan = "96";
		inline static const std::string BrightWhite = "97";
	};

	struct BackgroundColor {
		inline static const std::string Black = "40";
		inline static const std::string Red = "41";
		inline static const std::string Green = "42";
		inline static const std::string Yellow = "43";
		inline static const std::string Blue = "44";
		inline static const std::string Magenta = "45";
		inline static const std::string Cyan = "46";
		inline static const std::string White = "47";

		inline static const std::string BrightBlack = "100";
		inline static const std::string BrightRed = "101";
		inline static const std::string BrightGreen = "102";
		inline static const std::string BrightYellow = "103";
		inline static const std::string BrightBlue = "104";
		inline static const std::string BrightMagenta = "105";
		inline static const std::string BrightCyan = "106";
		inline static const std::string BrightWhite = "107";
	};

	enum State {
		WELCOME,
		INIT,
		ANALYZING,
		RESULTS,
		EXIT, 
		ERROR
	};

	consoleGui() {
		system("cls");
		drawBorder();
	}

	bool tick() noexcept;

	static void setStyle(const std::string& textColor = TextColor::White,
	                     const std::string& backgroundColor = BackgroundColor::Black,
	                     const std::string& textStyle = TextStyle::Normal) noexcept;
	static void setForeground(const std::string& textColor) noexcept;
	static void setBackground(const std::string& backgroundColor) noexcept;
	static void setTextStyle(const std::string& textStyle) noexcept;
	static void resetStyle() noexcept;

	//void demonstrateForeground() {
	//	std::string names[] = {"Black", "Red", "Green", "Yellow", "Blue", "Magenta", "Cyan", "White"};
	//	std::string normalCodes[] = {
	//		TextColor::Black, TextColor::Red, TextColor::Green, TextColor::Yellow, TextColor::Blue, TextColor::Magenta,
	//		TextColor::Cyan, TextColor::White
	//	};
	//	std::string brightCodes[] = {
	//		TextColor::BrightBlack, TextColor::BrightRed, TextColor::BrightGreen, TextColor::BrightYellow,
	//		TextColor::BrightBlue, TextColor::BrightMagenta, TextColor::BrightCyan, TextColor::BrightWhite
	//	};
	//
	//	std::cout << "--- [ FOREGROUND COLORS ] ---" << std::endl;
	//	for (int i = 0; i < 8; ++i) {
	//		setStyle(normalCodes[i], BackgroundColor::Black, TextStyle::Bold);
	//		std::cout
	//			<< std::setw(12) << names[i] << "  ";
	//		resetStyle();
	//
	//		setStyle(brightCodes[i], BackgroundColor::Black, TextStyle::Bold);
	//		std::cout
	//			<< "Bright " << names[i] << std::endl;
	//		resetStyle();
	//	}
	//}
	//
	//void demonstrateBackground() {
	//	std::string names[] = {"Black", "Red", "Green", "Yellow", "Blue", "Magenta", "Cyan", "White"};
	//	std::string bgCodes[] = {
	//		BackgroundColor::Black, BackgroundColor::Red, BackgroundColor::Green, BackgroundColor::Yellow,
	//		BackgroundColor::Blue, BackgroundColor::Magenta, BackgroundColor::Cyan, BackgroundColor::White
	//	};
	//	std::string brightBgCodes[] = {
	//		BackgroundColor::BrightBlack, BackgroundColor::BrightRed, BackgroundColor::BrightGreen,
	//		BackgroundColor::BrightYellow, BackgroundColor::BrightBlue, BackgroundColor::BrightMagenta,
	//		BackgroundColor::BrightCyan, BackgroundColor::BrightWhite
	//	};
	//
	//	std::cout << "\n--- [ BACKGROUND COLORS ] ---" << std::endl;
	//	for (int i = 0; i < 8; ++i) {
	//		setStyle(TextColor::Black, bgCodes[i], TextStyle::Normal);
	//		std::cout << "  " << std::setw(10) << names[i] << "  " << " ";
	//		resetStyle();
	//
	//		setStyle(TextColor::Black, brightBgCodes[i], TextStyle::Normal);
	//		std::cout << "  " << std::setw(10) << "Br." + names[i] << "  " << std::endl;
	//		resetStyle();
	//	}
	//}

	//void demonstrateStyles() {
	//	struct StyleInfo {
	//		std::string name;
	//		std::string code;
	//	};
	//	StyleInfo styles[] = {
	//		{"Normal", TextStyle::Normal},
	//		{"Bold", TextStyle::Bold},
	//		{"Dim", TextStyle::Dim},
	//		{"Italic", TextStyle::Italic},
	//		{"Underline", TextStyle::Underline},
	//		{"Blink", TextStyle::Blink},
	//		{"Reverse", TextStyle::Reverse},
	//		{"Strike", TextStyle::Strike}
	//	};
	//	std::cout << "--- [ TEXT STYLES DEMO ] ---" << std::endl;
	//	for (const auto& s : styles) {
	//		setStyle(TextColor::BrightCyan, BackgroundColor::Black, s.code);
	//		std::cout << std::setw(12) << s.name << ": " << "The quick brown fox jumps over the lazy dog"
	//			<< " (Code: " << s.code << ")" << std::endl;
	//		resetStyle();
	//	}
	//}

private:
	size_t frameW = 80;
	size_t frameH = 20;
	size_t innerW = frameW - 2;
	size_t innerH = frameH - 1;

	State currentState = State::WELCOME;

	std::string analyzePath = "C:/";
	size_t scopeType = 0;

	fileAnalyzer analyzer;
	std::mutex coutMutex;

	std::atomic<bool> needUpdate{ false };
	std::deque<std::string> lastFiles;
	std::mutex currentFileMutex;

	fileStats finalStats;

	const std::string BLOCK_FULL = std::string(1, (char)219); // █
	const std::string BLOCK_DARK = std::string(1, (char)178); // ▓
	const std::string BLOCK_MED = std::string(1, (char)177); // ▒
	const std::string BLOCK_LIGHT = std::string(1, (char)176); // ░

	const std::string CORNER_LEFT_TOP = std::string(1, (char)201); // ╔
	const std::string CORNER_RIGHT_TOP = std::string(1, (char)187); // ╗
	const std::string CORNER_LEFT_BOTTOM = std::string(1, (char)200); // ╚
	const std::string CORNER_RIGHT_BOTTOM = std::string(1, (char)188); // ╝

	const std::string BORDER_T_JUNCTION_TOP = std::string(1, (char)203); // ╦
	const std::string BORDER_T_JUNCTION_BOTTOM = std::string(1, (char)202); // ╩
	const std::string BORDER_T_JUNCTION_LEFT = std::string(1, (char)204); // ╠
	const std::string BORDER_T_JUNCTION_RIGHT = std::string(1, (char)185); // ╣
	const std::string BORDER_T_СROSSING = std::string(1, (char)206); // ╬

	const std::string BORDER_HORIZONTAL = std::string(1, (char)205); // ═
	const std::string BORDER_VERTICAL = std::string(1, (char)186); // ║
	const char SPACE = ' ';

	const std::string WELCOME_HEADER = "FILES_SHARK";
	const std::string WELCOME_VERSION = "[beta] v0.0.1";

	// COLORS AND STYLES
	const std::string mainTextColor = TextColor::White;
	const std::string headerTextColor = TextColor::Yellow;
	const std::string versionTextColor = TextColor::Cyan;
	const std::string mainBgColor = BackgroundColor::Black;
	const std::string borderColor = TextColor::Blue;
	const std::string progressBgColor = BackgroundColor::BrightBlack;
	const std::string progressFillColor = TextColor::BrightGreen;

	const size_t progressBarW = 50;
	const size_t progressBarX = 16;
	const size_t progressBarSmallW = 30;
	const size_t progressBarSmallX = 14;

	const size_t HISTORY_MAX_LINES = 5;



	std::string repeatN(const std::string& str, size_t n) const noexcept;
	std::string repeatN(const char str, size_t n) const noexcept;

	void printCentered(const std::string& text, const size_t y) noexcept;
	void printLeft(const std::string& text, size_t y, size_t padding = 0, size_t maxW = 0) noexcept;
	void printRight(const std::string& text, size_t y, size_t padding = 0, size_t maxW = 0) noexcept;

	void drawBorder() noexcept;
	void drawRow(size_t y, size_t type = 0, size_t start = 1, size_t end = 0) noexcept;
	void drawColumn(size_t x, size_t type = 0, size_t start = 1, size_t end = 0) noexcept;
	void clearRow(size_t y) noexcept;
	void clearColumn(size_t x) noexcept;

	void processWelcome() noexcept;
	void processInit() noexcept;
	void processAnalyzing() noexcept;
	void processResults() noexcept;
	void processEnd() noexcept;

	void analyzingCallbackProcess(const std::string& path) noexcept;
	void analyzingCallbackFinish(fileStats total, bool status) noexcept;


	inline void setCursor(int x, int y) { std::cout << "\033[" << y << ";" << x << "H" << std::flush; }


	std::string textShorten(const std::string& text, const size_t width);
};
