#ifndef SMALLTOOLBOX_TEXT_H
#define SMALLTOOLBOX_TEXT_H

#include <sstream>
#include <string>
#include <vector>


namespace stbox {

namespace Text {

// Value to string.
template <typename T>
static auto toStr(T value) -> std::string
{
    std::stringstream ss;
    ss << value;

    return ss.str();
}

// Values to string.
template<typename T, typename... Args>
static auto toStr(T value, Args... args) -> std::string
{
    return toStr(value) + toStr(args...);
}

// Initializer list to string.
template <class T>
static auto toStr(std::initializer_list<T> list) -> std::string
{
    std::string result;
    for (auto value : list) {
        result += toStr(value) + " ";
    }

    return result;
}

// Split string by delimiter.
static auto split(const std::string &str, const char &delimiter) -> std::vector<std::string>
{
    std::vector<std::string> result;
    std::string strTemp;
    for (char character : str) {
        if (character == delimiter) {
            result.push_back(strTemp);
            strTemp.clear();
        }
        else {
            strTemp += character;
        }
    }
    if (!strTemp.empty()) {
        result.push_back(strTemp);
    }

    return result;
}

// Replace all occurrences of the character with the string.
static auto replace(const std::string &str, const char &character, const std::string &strReplace) -> std::string
{
    std::string result{};
    for (char characterTemp : str) {
        result += (characterTemp == character ? strReplace : std::string{characterTemp});
    }

    return result;
}

// Replace all occurrences of the character with another character.
static auto replace(const std::string &str, const char &character, const char &charReplace) -> std::string
{
    return Text::replace(str, character, std::string{charReplace});
}

// Trim string : Remove characters to the left.
static auto ltrim(const std::string &str, const char &trimmer) -> std::string
{
    int left = 0;
    auto right = str.size() - 1;
    right = right < 0 ? 0 : right;
    while (left < str.size()) {
        if (str[left] != trimmer) {
            break;
        }
        left++;
    }

    return str.substr(left, 1 + right - left);
}

// Trim string : Remove characters to the right.
static auto rtrim(const std::string &str, const char &trimmer) -> std::string
{
    int left = 0;
    auto right = str.size() - 1;
    right = right < 0 ? 0 : right;
    while (right >= 0) {
        if (str[right] != trimmer) {
            break;
        }
        right--;
    }

    return str.substr(left, 1 + right - left);
}

// Trim string.
static auto trim(const std::string &str, const char &trimmer) -> std::string
{
    return rtrim(ltrim(str, trimmer), trimmer);
}

// Trim strings.
static auto trim(std::vector<std::string> vStr, const char &trimmer) -> std::vector<std::string>
{
    for (auto &item : vStr) {
        item = Text::trim(item, trimmer);
    }

    return vStr;
}

// Trim zeros: Formats numbers.
static auto trimZeros(const double &value) -> std::string
{
    auto str = std::to_string(value);
    return Text::rtrim(str, '0') + '0';
}

// Join strings.
static auto join(const std::vector<std::string> &vStr, const char &delimiter) -> std::string
{
    std::string result{};
    for (unsigned i = 0; i < vStr.size(); i++) {
        result += vStr[i] + (i < vStr.size() - 1 ? std::string{delimiter} : "");
    }

    return result;
}

// String with all characters in uppercase.
static auto strUpper(const std::string &str) -> std::string
{
    std::locale loc;
    std::string result{str};
    for (char &charcater : result) {
        charcater = toupper(charcater, loc);
    }

    return result;
}

// String with all characters in lowercase.
static auto strLower(const std::string &str) -> std::string
{
    std::locale loc;
    std::string result{str};
    for (char &charcater : result) {
        charcater = tolower(charcater, loc);
    }

    return result;
}

}; // namespace Text

}; // namespace stbox

#endif // SMALLTOOLBOX_TEXT_H
