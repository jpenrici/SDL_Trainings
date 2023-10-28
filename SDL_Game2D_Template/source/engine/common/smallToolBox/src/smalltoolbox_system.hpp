#ifndef SMALLTOOLBOX_SYSTEM_H
#define SMALLTOOLBOX_SYSTEM_H

#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>


namespace stbox {

namespace Sys {

class IO {

public:
    // Returns the extension of a file.
    static auto getExtension(const std::string &filePath) -> std::string
    {
        auto extFile = std::filesystem::path(filePath).extension();

        return std::string{extFile};
    }

    // Returns the name of a file.
    static auto getFilename(const std::string &filePath) -> std::string
    {
        auto fileName = std::filesystem::path(filePath).filename();

        return std::string{fileName};
    }

    // Check if file exists.
    static auto exists(const std::string &filePath) -> bool
    {
        if (filePath.empty()) {
            return false;
        }

        const auto fpath = std::filesystem::path(filePath);
        if (!std::filesystem::exists(fpath)) {
            std::cerr << "File not found!\n";
            return false;
        }

        return true;
    }

    // Load text file.
    static auto load(const std::string &filePath, const std::string &filenameExtension) -> std::string
    {
        if (filePath.empty()) {
            return {};
        }

        // Check extension.
        if (!filenameExtension.empty()) {
            if (!filePath.ends_with(filenameExtension)) {
                std::cerr << " Invalid file!\n";
                return {};
            }

            if (!IO::exists(filePath)) {
                return {};
            }
        }

        std::string str{};
        try {
            std::ifstream fileIn(filePath, std::ios::in);
            if (fileIn.is_open()) {
                std::string line{};
                while (getline(fileIn, line)) {
                    str += line + "\n";
                }
                fileIn.close();
            }
        }
        catch (...) {
            // pass
        }

        return str;
    }

    // Save text file.
    static auto save(const std::string &text, std::string filePath) -> bool
    {
        if (text.empty()) {
            std::cerr << "Empty text! Export failed!\n";
            return false;
        }

        if (filePath.empty()) {
            filePath = "output.txt";
        }

        try {
            std::ofstream file(filePath, std::ios::out);
            file << text;
            file.close();
        }
        catch (const std::exception &e) {
            std::cout << "Error handling file writing.\n";
            std::cerr << e.what() << "\n";
            return false;
        }

        return true;
    }

    // Add suffix to file name.
    static auto addSuffix(const std::string &filePath, const std::string &suffix)
    {
        return getFilename(filePath) + suffix + getExtension(filePath);
    }
};

class Console {

public:
    // Std::cout : double.
    static void view(const double &value)
    {
        std::cout << std::to_string(value) << '\n';
    }

    // Std::cout : Vector of numbers.
    template<typename T>
    static void view(const std::vector<T> &values)
    {
        std::string str{};
        for (unsigned i = 0; i < values.size(); i++) {
            str += std::to_string(values[i]) + (i < values.size() - 1 ? "," : "");
        }

        if (str.empty()) {
            std::cout << "Empty\n";
        }
        else {
            std::cout << str << '\n';
        }
    }

    // Std::cout : Vector of strings.
    static void view(const std::vector<std::string> &values)
    {
        std::string str{};
        for (unsigned i = 0; i < values.size(); i++) {
            str += values[i] + (i < values.size() - 1 ? "," : "");
        }

        if (str.empty()) {
            std::cout << "Empty\n";
        }
        else {
            std::cout << str << '\n';
        }
    }

    // Std::cout : string.
    static void view(const std::string &str)
    {
        std::cout << str << '\n';
    }
};

}; // namespace Sys

}; // namespace stbox

#endif // SMALLTOOLBOX_SYSTEM_H
