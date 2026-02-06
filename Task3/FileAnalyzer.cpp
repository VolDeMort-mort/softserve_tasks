#include "FileAnalyzer.h"
#include <fstream>
#include <string>

std::atomic<int> total_files{ 0 };
std::atomic<long long> total_blank{ 0 };
std::atomic<long long> total_comment{ 0 };
std::atomic<long long> total_code{ 0 };

void analyze_file(const std::string& path) {
     std::ifstream file(path);
     if (!file.is_open()) return;

     long long blank = 0, comment = 0, code = 0;
     std::string line;
     bool in_multiline_comment = false;

     while (std::getline(file, line)) {
          auto start = line.find_first_not_of(" \t\r\n");
          if (start == std::string::npos) {
               blank++;
               continue;
          }

          std::string trimmed = line.substr(start);

          if (in_multiline_comment) {
               comment++;
               if (trimmed.find("*/") != std::string::npos) in_multiline_comment = false;
          }
          else if (trimmed.compare(0, 2, "//") == 0) {
               comment++;
          }
          else if (trimmed.compare(0, 2, "/*") == 0) {
               comment++;
               if (trimmed.find("*/") == std::string::npos) in_multiline_comment = true;
          }
          else {
               code++;
          }
     }

     total_files++;
     total_blank += blank;
     total_comment += comment;
     total_code += code;
}