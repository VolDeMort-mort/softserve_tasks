#ifndef PRINTER_H
#define PRINTER_H

#include <filesystem>
#include "ReadLiner.h"

class IPrinter {
public:
    void virtual save(const std::filesystem::path& pathToSaveFile, 
                    const std::vector<std::tuple<std::filesystem::path, Lines, int, unsigned char>> &results) = 0;
};

class CSVPrinter : public IPrinter {
public:
    void virtual save(const std::filesystem::path& pathToSaveFile, 
                    const std::vector<std::tuple<std::filesystem::path, Lines, int, unsigned char>> &results);
};

class PrettyPrinter : public IPrinter {
public:
    void virtual save(const std::filesystem::path& pathToSaveFile, 
                    const std::vector<std::tuple<std::filesystem::path, Lines, int, unsigned char>> &results);
};

#endif // PRINTER_H