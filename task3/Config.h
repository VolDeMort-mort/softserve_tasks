#ifndef CONFIG_H
#define CONFIG_H

#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class Config {
public:
    std::vector<std::string> extensions = {".c", ".cpp", ".h", ".hpp", ".cc", ".cxx"};
    std::uint32_t threadCount = 4;
    std::string outputFilePath = "output.txt";

    Config() = default;

    bool loadFromFile(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            std::cerr << "Config: could not open " << path << ", using defaults.\n";
            return false;
        }
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()) continue;
            auto eqPos = line.find('=');
            if (eqPos == std::string::npos) continue;

            std::string key = trim(line.substr(0, eqPos));
            std::string value = trim(line.substr(eqPos + 1));

            if (key == "extensions") {
                extensions = splitExtensions(value);
            } else if (key == "threadCount") {
                auto parsed = std::stoul(value);
                threadCount = static_cast<std::uint32_t>(parsed);
                if (threadCount == 0) threadCount = 1;
            } else if (key == "outputFilePath") {
                outputFilePath = value;
            }
        }
        return true;
    }

private:
    static std::string trim(const std::string& s) {
        auto start = s.find_first_not_of(" \t\r\n");
        if (start == std::string::npos) return "";
        auto end = s.find_last_not_of(" \t\r\n");
        return s.substr(start, end - start + 1);
    }

    static std::vector<std::string> splitExtensions(const std::string& value) {
        std::vector<std::string> result;
        std::istringstream stream(value);
        std::string token;
        while (std::getline(stream, token, ',')) {
            std::string trimmed = trim(token);
            if (!trimmed.empty()) result.push_back(trimmed);
        }
        return result;
    }
};

#endif
