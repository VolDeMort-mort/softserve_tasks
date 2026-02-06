//#include <iostream>
//#include <fstream>
//#include <vector>
//#include <filesystem>
//#include <thread>
//#include <chrono>
//#include "FileAnalyzer.h"
//
//namespace fs = std::filesystem;
//
//int main() {
//     std::string root_path;
//     std::cout << "Enter project root folder: ";
//     std::getline(std::cin, root_path);
//
//     if (!fs::exists(root_path)) {
//          std::cerr << "Path does not exist." << std::endl;
//          return 1;
//     }
//
//     auto start_time = std::chrono::high_resolution_clock::now();
//     SafeQueue<std::string> file_queue;
//
//     unsigned int num_threads = std::thread::hardware_concurrency();
//     std::vector<std::thread> workers;
//     for (unsigned int i = 0; i < num_threads; ++i) {
//          workers.emplace_back([&file_queue]() {
//               std::string path;
//               while (file_queue.pop(path)) {
//                    analyze_file(path);
//               }
//               });
//     }
//
//     for (const auto& entry : fs::recursive_directory_iterator(root_path)) {
//          if (entry.is_regular_file()) {
//               std::string ext = entry.path().extension().string();
//               if (ext == ".h" || ext == ".hpp" || ext == ".c" || ext == ".cpp") {
//                    file_queue.push(entry.path().string());
//               }
//          }
//     }
//     file_queue.set_finished();
//
//     for (auto& t : workers) t.join();
//
//     auto end_time = std::chrono::high_resolution_clock::now();
//     std::chrono::duration<double> elapsed = end_time - start_time;
//
//     auto report = [&](std::ostream& os) {
//          os << "Total processed files: " << total_files << "\n";
//          os << "Blank lines:           " << total_blank << "\n";
//          os << "Comment lines:         " << total_comment << "\n";
//          os << "Code lines:            " << total_code << "\n";
//          os << "Execution time:        " << elapsed.count() << "s\n";
//          };
//
//     report(std::cout);
//     std::ofstream out("statistics.txt");
//     if (out.is_open()) report(out);
//
//     return 0;
//}