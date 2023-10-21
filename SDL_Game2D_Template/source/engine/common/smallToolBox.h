////////////////////////////////////////////////////////////////////////////////
// svgToolBox.h
// Small support toolbox.
// 2023-10-17
////////////////////////////////////////////////////////////////////////////////

#ifndef SMALLTOOLBOX_H
#define SMALLTOOLBOX_H

#include <algorithm>
#include <array>
#include <cmath>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <initializer_list>
#include <iomanip>
#include <iostream>
#include <locale>
#include <map>
#include <numbers>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>


namespace stbox {

#define Numbers     std::vector<double>
#define Point2D     stbox::Point
#define Points      std::vector<stbox::Point>
#define Polygon     stbox::RegularPolygon
#define Strings     std::vector<std::string>
#define PI          std::numbers::pi
#define MAXNUMBER   std::numeric_limits<float>::max()

class Text {

public:
    // String Handler.
    template <typename T>
    static inline auto toStr(T value) -> std::string
    {
        std::stringstream ss;
        ss << value;

        return ss.str();
    }

    template<typename T, typename... Args>
    static inline auto toStr(T value, Args... args) -> std::string
    {
        return toStr(value) + toStr(args...);
    }

    template <class T>
    static inline auto toStr(std::initializer_list<T> list) -> std::string
    {
        std::string result;
        for (auto value : list) {
            result += toStr(value) + " ";
        }

        return result;
    }

    // Split string by delimiter.
    static auto split(const std::string &str, const char &delimiter) -> Strings
    {
        Strings result;
        std::string strTemp{};
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

    // Trim string.
    static auto trim(const std::string &str, const char &trimmer) -> std::string
    {
        return rtrim(ltrim(str, trimmer), trimmer);
    }

    // Trim strings.
    static auto trim(Strings vStr, const char &trimmer) -> Strings
    {
        for (auto &item : vStr) {
            item = Text::trim(item, trimmer);
        }

        return vStr;
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

    // Trim zeros: Formats numbers.
    static auto trimZeros(const double &value) -> std::string
    {
        auto str = std::to_string(value);
        return Text::rtrim(str, '0') + '0';
    }

    // Join strings.
    static auto join(const Strings &vStr, const char &delimiter) -> std::string
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
};

class Math {

public:
    template<typename T = float>
    struct Size {
        T width{0};
        T height{0};

        auto str() -> std::string
        {
            return "Width: " + std::string(width) + " Height: " + std::string(height);
        }
    };

    static auto toFloat(int value) -> float
    {
        return static_cast<float>(value);
    }

    static auto toInt(float value) -> int
    {
        return static_cast<int>(value);
    }

    static auto radians(const double &angle) -> double
    {
        return angle * PI / 180.0;
    }

    static auto angle(const double &radians) -> double
    {
        return radians * 180.0 / PI;
    }

    // Returns the angle of the line (x0,y0)(x1,y1).
    static auto angle(const double &x0, const double &y0, const double &x1, const double &y1) -> double
    {
        double result = angle(atan((y1 - y0) / (x1 - x0)));

        if (x0 == x1 && y0 == y1) {
            result = 0;
        }
        if (x0 <  x1 && y0 == y1) {
            result = 0;
        }
        if (x0 == x1 && y0 <  y1) {
            result = 90;
        }
        if (x0 >  x1 && y0 == y1) {
            result = 180;
        }
        if (x0 == x1 && y0 >  y1) {
            result = 270;
        }
        if (x0 >  x1 && y0 <  y1) {
            result += 180;
        }
        if (x0 >  x1 && y0 >  y1) {
            result += 180;
        }
        if (x0 <  x1 && y0 >  y1) {
            result += 360;
        }

        return result;
    }

    // Returns : value + radius * cos(angle).
    static auto cos(const double &value, const double &radius, const double &angle) -> double
    {
        return value + radius * std::cos(radians(angle));
    }

    // Returns : value + radius * sin(angle).
    static auto sin(const double &value, const double &radius, const double &angle) -> double
    {
        return value + radius * std::sin(radians(angle));
    }

    // Returns the distance between two points.
    static auto distance(const double &x0, const double &y0, const double &x1, const double &y1) -> double
    {
        return std::sqrt(std::pow(x0 - x1, 2) + std::pow(y0 - y1, 2));
    }

    // Rounds value to N digits after decimal point.
    // Number of Decimal Places < 0, return the same value.
    // Number of Decimal Places = 0, return the integer value.
    static auto round(const double &value, int decimalPlaces = -1) -> double
    {
        if (decimalPlaces < 0) {
            return value;
        }

        if (decimalPlaces == 0) {
            return static_cast<int>(value);
        }

        const int limit = 10;
        decimalPlaces = decimalPlaces > limit ? limit : decimalPlaces;

        const int base = 10;
        return std::round(value * std::pow(base, decimalPlaces)) / std::pow(base, decimalPlaces);
    }

    // Rounds values.
    // Number of Decimal Places < 0, returns the same value.
    static auto round(Numbers values, int decimalPlaces) -> Numbers
    {
        for (double &value : values) {
            values = round(values, decimalPlaces);
        }

        return values;
    }

    // Sort numbers.
    static auto sort(Numbers numbers, bool ascendingOrder = true) -> Numbers
    {
        if (ascendingOrder) {
            std::sort(numbers.begin(), numbers.end(), std::less<>());
        }
        else {
            std::sort(numbers.begin(), numbers.end(), std::greater<>());
        }

        return numbers;
    }

    // Calculates the area formed by three coordinates.
    static auto triangleArea(const double &x0, const double &y0,
                             const double &x1, const double &y1,
                             const double &x2, const double &y2) -> double
    {
        // Heron's formula
        auto a = distance(x0, y0, x1, y1);
        auto b = distance(x1, y1, x2, y2);
        auto c = distance(x2, y2, x0, y0);
        auto s = (a + b + c) / 2.0;

        return std::sqrt(s * (s - a) * (s - b) * (s - c));
    }

    // Calculates the greatest height formed by three coordinates.
    static auto triangleHeight(const double &x0, const double &y0,
                               const double &x1, const double &y1,
                               const double &x2, const double &y2) -> double
    {
        auto area = triangleArea(x0, y0, x1, y1, x2, y2);
        auto height0 = 2 * area / distance(x0, y0, x1, y1);
        auto height1 = 2 * area / distance(x1, y1, x2, y2);
        auto height2 = 2 * area / distance(x2, y2, x0, y0);

        return std::max(std::max(height0, height1), height2);
    }

    // Pseudo-random positive number generation.
    template<typename T = int>
    static auto randomize(int number1, int number2 = 0) -> T
    {
        number1 = number1 < 0 ? 0 : number1;
        number2 = number2 < 0 ? 0 : number2;
        if (number1 == 0 && number2 == 0) {
            return 0;
        }
        if (number1 == number2) {
            return number1;
        }

        std::random_device rd;
        std::default_random_engine generator(rd());
        std::uniform_int_distribution<int> distribution(std::min(number1, number2), std::max(number1, number2));

        return static_cast<T>(distribution(generator));
    }
};

class Check {

public:
    // Compare groups (vector).
    template<typename T>
    static auto equal(const std::vector<T> &group1, const std::vector<T> &group2, bool compareOrder = false) -> bool
    {
        if (group1.size() != group2.size()) {
            return false;
        }

        if (compareOrder) {
            for (unsigned i = 0; i < group1.size(); i++) {
                if (!(group1[i] == group2[i])) {
                    return false;
                }
            }
        }

        for (const auto &value1 : group1) {
            bool differentFromEveryone = true;
            for (const auto &value2 : group2) {
                if (value1 == value2) {
                    differentFromEveryone = false;
                    break;
                }
            }
            if (differentFromEveryone) {
                return false;
            }
        }

        return true;
    }

    // Error message with line indication.
    static void error(const int &line, const std::string &message)
    {
        throw std::domain_error("[Line: " + std::to_string(line) + "]: " + message);
    }
};

// Color
class Color {

public:
    static constexpr const char *WHITE = "#FFFFFF";
    static constexpr const char *BLACK = "#000000";
    static constexpr const char *RED   = "#FF0000";
    static constexpr const char *GREEN = "#00FF00";
    static constexpr const char *BLUE  = "#0000FF";

    struct RGBA {
        unsigned char R{0}, G{0}, B{0}, A{255};

        RGBA() = default;

        RGBA(const unsigned char  &r, const unsigned char  &g, const unsigned char  &b, const unsigned char &a = 255)
            : RGBA(std::vector<unsigned char>
        {
            r, g, b, a
        }) {}

        explicit RGBA(const std::vector<unsigned char> &rgba)
            : RGBA()
        {
            switch (rgba.size()) {
            case 4:
                A = rgba[3];
            case 3:
                B = rgba[2];
            case 2:
                G = rgba[1];
            case 1:
                R = rgba[0];
                break;
            default:
                break;
            }

            R = R < 0 ? 0 : R % 256;
            G = G < 0 ? 0 : G % 256;
            B = B < 0 ? 0 : B % 256;
            A = A < 0 ? 0 : A % 256;
        }

        auto operator==(const RGBA &rgba) const -> bool
        {
            return equal(rgba);
        }

        auto empty() const -> bool
        {
            return R == 0 && G == 0 && B == 0 && A == 0;
        }

        auto equal(const RGBA &rgba) const  ->bool
        {
            return R == rgba.R && G == rgba.G && B == rgba.B && A == rgba.A;
        }

        auto toStr(bool alpha = true) const  ->std::string
        {
            return {
                std::to_string(R) + "," + std::to_string(G) + "," +
                std::to_string(B) + (alpha ? "," + std::to_string(A) : "")
            };
        }
    };

    // Converts decimal value to hexadecimal.
    static auto int2hex(unsigned value) -> std::string
    {
        std::string digits = "0123456789ABCDEF";
        std::string result;
        if (value < 16) {
            result.push_back('0');
            result.push_back(digits[value % 16]);
        }
        else {
            while (value != 0) {
                result = digits[value % 16] + result;
                value /= 16;
            }
        }
        return result;
    }

    // Converts hexadecimal value to decimal
    static auto hex2int(std::string value) -> int
    {
        if (value.starts_with('#')) {
            value = value.substr(1);
        }

        if (value.empty()) {
            return 0;
        }

        int result = 0;
        try {
            result = std::stoi(value, nullptr, 16);
        }
        catch (...) {
            // pass
        }

        return result;
    }

    // Return vector unsigned {Red, Green, Blue, Alpha}
    static auto hex2RGB(std::string value) -> std::vector<unsigned char>
    {
        if (value.starts_with('#')) {
            value = value.substr(1);
        }

        if (value.empty()) {
            return {};
        }

        if (value.size() > 8) {
            value = value.substr(0, 8);
        }

        std::vector<unsigned char> result;
        if (value.size() % 2 == 0) {
            while (!value.empty()) {
                std::string num = value.substr(0, 2);
                value = value.substr(2);
                result.push_back(hex2int(num));
            }
        }

        return result;
    }

    // Formats values (Red, Green, Blue) to "#RRGGBB" hexadecimal.
    static auto RGB2hex(const int &R, const int &G, const int &B) -> std::string
    {
        return "#" + int2hex(R) + int2hex(G) + int2hex(B);
    }

    // Formats values (Red, Green, Blue, Alpha) to "#RRGGBBAA" hexadecimal.
    static auto RGBA2hex(const int &R, const int &G, const int &B, const int &A) -> std::string
    {
        return RGB2hex(R, G, B) + int2hex(A);
    }
};

// Point 2D (x,y)
class Point {

    struct Coordinate {
        double value{0};

        auto toInt() -> const int
        {
            return static_cast<int>(value);
        }

        auto toFloat() -> const float
        {
            return static_cast<float>(value);
        }

        auto toStr(bool trimZeros = false) const -> std::string
        {
            return trimZeros ? Text::trimZeros(value) : std::to_string(value);
        }
    };

public:
    // Axis
    Coordinate X, Y;

    // Label
    std::string label{"Point"};

    // Point (0, 0)
    Point() : X{0}, Y{0} {};

    // Point (x, y)
    Point(const double &x, const double &y) : X{x}, Y{y} {};

    ~Point() = default;

    auto operator+(const Point &point) const -> Point
    {
        return {X.value + point.X.value, Y.value + point.Y.value};
    }

    auto operator+(const double &value) const -> Point
    {
        return {X.value + value, Y.value + value};
    }

    void operator+=(const Point &point)
    {
        sum(point.X.value, point.Y.value);
    }

    void operator+=(const double &value)
    {
        sum(value, value);
    }

    auto operator-(const Point &point) const -> Point
    {
        return {X.value - point.X.value, Y.value - point.Y.value};
    }

    auto operator-(const double &value) const -> Point
    {
        return {X.value - value, Y.value - value};
    }

    void operator-=(const Point &point)
    {
        sum(-point.X.value, -point.Y.value);
    }

    void operator-=(const double &value)
    {
        sum(-value, -value);
    }

    auto operator*(const Point &point) const -> Point
    {
        return {X.value * point.X.value, Y.value * point.Y.value};
    }

    auto operator*(const double &value) const -> Point
    {
        return {X.value * value, Y.value * value};
    }

    void operator*=(const Point &point)
    {
        multiply(point.X.value, point.Y.value);
    }

    void operator*=(const double &value)
    {
        multiply(value, value);
    }

    auto operator==(const Point &point) const -> bool
    {
        return equal(point);
    }

    // X += x, Y += y
    void sum(const double &x, const double &y)
    {
        X.value += x;
        Y.value += y;
    }

    // Returns each coordinate by adding value.
    template <typename T>
    static auto sum(const Points &points, const T &value) -> Points
    {
        Points result{};
        for (const auto &p : points) {
            result.push_back(p + value);
        }
        return result;
    }

    // Sum : Point (Total X axis, Total Y axis).
    static auto total(const Points &points) -> Point
    {
        Point sum;
        for (const auto &point : points) {
            sum += point;
        }

        return sum;
    }

    // Average : Point (Total X axis / Points, Total Y axis / Points).
    // Returns Point(0,0) and false if vector<Point> empty.
    static auto average(const Points &points) -> std::tuple<Point, bool>
    {
        if (points.empty()) {
            return {Point(), false};
        }

        return {total(points) *(1.0 / static_cast<double>(points.size())), true};
    }

    // X *= x; Y *= y
    void multiply(const double &x, const double &y)
    {
        X.value *= x;
        Y.value *= y;
    }

    // X = 0; Y = 0
    void reset()
    {
        X.value = 0;
        Y.value = 0;
    }

    // Checks if coordinates are equal.
    auto equal(const Point &point) const -> bool
    {
        return X.value == point.X.value && Y.value == point.Y.value;
    }

    // Angle of the imaginary line between the current point and the other.
    auto angle(const Point &point) const -> double
    {
        return Math::angle(X.value, Y.value, point.X.value, point.Y.value);
    }

    // Angle between three points.
    // Signal: True, respects the order of the vectors.
    static auto angle(const Point &origin, const Point &first, const Point &second, bool signal = false) -> double
    {
        auto angle1 = origin.angle(first);
        auto angle2 = origin.angle(second);

        if (signal) {
            return angle1 - angle2;
        }

        return std::max(angle1, angle2) - std::min(angle1, angle2);
    }

    // Distance between the current point and another.
    auto distance(const Point &point) const -> double
    {
        return Math::distance(X.value, Y.value, point.X.value, point.Y.value);
    }

    // Sum all the distances between the vertices.
    static auto sumDistances(const Points &points) -> double
    {
        double sum{0};
        for (int i = 1; i < points.size(); ++i) {
            sum += points[i].distance(points[i - 1]);
        }
        return sum;
    }

    // Position from angle and radius.
    // Current point as origin.
    auto position(const double &angle, const double &horizontalRadius, const double &verticalRadius) const -> Point
    {
        return {Math::cos(X.value, horizontalRadius, angle),
                Math::sin(Y.value, verticalRadius, angle)};
    }

    // Position from angle and radius.
    // Current point as origin.
    auto position(const double &angle, const double &radius) const -> Point
    {
        return position(angle, radius, radius);
    }

    // Area formed by the current point and two others.
    auto triangleArea(const Point &point1, const Point &point2) const -> double
    {
        return Math::triangleArea(X.value, Y.value,
                                  point1.X.value, point1.Y.value,
                                  point2.X.value, point2.Y.value);
    }

    // Area formed by three points.
    static auto triangleArea(const Point &point1, const Point &point2, const Point &point3)  -> double
    {
        return Math::triangleArea(point1.X.value, point1.Y.value,
                                  point2.X.value, point2.Y.value,
                                  point3.X.value, point3.Y.value);
    }

    // Greatest height formed between current point and two others.
    auto triangleHeight(const Point &point1, const Point &point2) const -> double
    {
        return Math::triangleHeight(X.value, Y.value,
                                    point1.X.value, point1.Y.value,
                                    point2.X.value, point2.Y.value);
    }

    // Greatest height formed between three points.
    static auto triangleHeight(const Point &point1, const Point &point2, const Point &point3)  -> double
    {
        return Math::triangleHeight(point1.X.value, point1.Y.value,
                                    point2.X.value, point2.Y.value,
                                    point3.X.value, point3.Y.value);
    }

    // Calculates the point of intersection between two lines.
    // Returns Point(max X, max Y) and -1 if not intersect.
    // Returns Point(max X, max Y) and 0 if the lines are parallel.
    // Returns Point(x0, y0) and 1 if the lines coincident.
    // Return the intersection point and 2 if they are in range.
    // Return the intersection point and 3 if not in range.
    // Line 1 (x0, y0) - (x1, y1),
    // Line 2 (x2, y2) - (x3, y4).
    static auto lineIntersect(const double &x0, const double &y0, const double &x1, const double &y1,
                              const double &x2, const double &y2, const double &x3, const double &y3)
    -> std::tuple<Point, int>
    {
        // Check if lines are coincident or parallel.
        double d = (y3 - y2) * (x1 - x0) - (x3 - x2) * (y1 - y0);
        double a = (x3 - x2) * (y0 - y2) - (y3 - y2) * (x0 - x2);
        double b = (x1 - x0) * (y0 - y2) - (y1 - y0) * (x0 - x2);

        if (d == 0) {
            if (a == 0 && b == 0) {
                return {Point(x0, y0), 1}; // Coincident.
            }
            return {Point(MAXNUMBER, MAXNUMBER), 0}; // Parallel.
        }

        double t =  a / d;
        double u =  b / d;

        if (t >= 0.0 && t <= 1.0 && u >= 0 && u <= 1.0) {
            Point p{x0 + t *(x1 - x0), y0 + t *(y1 - y0)};
            // Check if the intersection point belongs to the two lines.
            if ((p.distance({x0, y0}) + p.distance({x1, y1}) == Point(x0, y0).distance({x1, y1}))
                    && (p.distance({x2, y2}) + p.distance({x3, y3}) == Point(x2, y2).distance({x3, y3}))) {
                return {p, 2};
            }
            // Intersection outside the line range.
            return {p, 3};
        }

        // Lines do not intersect.
        return {Point(MAXNUMBER, MAXNUMBER), -1};
    }

    // Calculates the point of intersection between two lines.
    // Returns Point(max X, max Y) and -1 if not intersect.
    // Returns Point(max X, max Y) and 0 if the lines are parallel.
    // Returns Point(x0, y0) and 1 if the lines coincident.
    // Return the intersection point and 2 if they are in range.
    // Return the intersection point and 3 if not in range.
    // Line 1 (Point 1) - (Point 2),
    // Line 2 (Point 3) - (Point 3).
    static auto lineIntersect(const Point &point1, const Point &point2,
                              const Point &point3, const Point &point4) -> std::tuple<Point, int>
    {
        return lineIntersect(point1.X.value, point1.Y.value,
                             point2.X.value, point2.Y.value,
                             point3.X.value, point3.Y.value,
                             point4.X.value, point4.Y.value);
    }

    // Returns the rounded current coordinates.
    auto round(int decimalPlaces = 2) const -> Point
    {
        return {Math::round(X.value, decimalPlaces),
                Math::round(Y.value, decimalPlaces)};
    }

    // Round points.
    // Number of Decimal Places < 0, returns the same value.
    static auto round(Points points, int decimalPlaces) -> Points
    {
        for (auto &point : points) {
            point = point.round(decimalPlaces);
        }

        return points;
    }

    // Returns Vector[2] with X and Y values.
    auto XY() -> Numbers
    {
        return {X.value, Y.value};
    }

    // Sort points by X or Y axis.
    static auto sort(const Points &points, bool X_axis = true) -> Points
    {
        if (points.empty()) {
            return {};
        }

        if (points.size() == 1) {
            return points;
        }

        std::map<double, Numbers> mapPoint;

        for (const auto &point : points) {
            auto key = X_axis ? point.X.value : point.Y.value;
            auto value = X_axis ? point.Y.value : point.X.value;
            if (mapPoint.find(key) == mapPoint.end()) {
                mapPoint.insert({key, {value}});
            }
            else {
                mapPoint[key].push_back(value);
            }
        }

        Points result;
        for (const auto &item : mapPoint) {
            if (X_axis) {
                for (auto value : item.second) { // Y
                    result.emplace_back(item.first, value);
                }
            }
            else {
                auto values = Math::sort(item.second);
                for (auto value : values) {     // X
                    result.emplace_back(value, item.first);
                }
            }
        }

        return result;
    }

    // Sort the points clockwise using center point.
    static auto organize(const Point &center, const Points &points) -> Points
    {
        if (points.size() < 2) {
            return points;
        }

        // Map : Angle x Point.
        std::map<double, Points> mapPoint;

        for (const auto &value : points) {
            auto key = center.angle(value);
            if (mapPoint.find(key) == mapPoint.end()) {
                mapPoint.insert(make_pair(key, Points{value}));
            }
            else {
                mapPoint[key].push_back(value);
            }
        }

        Points result;
        for (const auto &item : mapPoint) {
            for (const auto &point : item.second) {
                result.push_back(point);
            }
        }

        return result;
    }

    // Sort the points clockwise using origin.
    static auto organize(const Points &points) -> Points
    {
        return organize(Point(0, 0), points);
    }

    auto toStr(bool trimZeros = false) const -> std::string
    {
        return X.toStr(trimZeros) + "," + Y.toStr(trimZeros);
    }

    auto toStrInt() const -> std::string
    {
        return std::to_string(static_cast<int>(X.value)) + "," +
               std::to_string(static_cast<int>(Y.value));
    }
};

// Special point.
static const Point Origin = Point(0, 0);
static const Point Zero   = Point(0, 0);

class Base {

    // Store the last configuration.
    Points m_vertices;

    Point m_first, m_second, m_third, m_fourth, m_origin;

    void update(const Point &first, const Point &second, const Point &third, const Point &fourth)
    {
        m_first = first;
        m_second = second;
        m_third = third;
        m_fourth = fourth;
        m_origin = first;

        if (!m_vertices.empty()) {
            m_vertices[0] = first;
        }
        if (m_vertices.size() > 1) {
            m_vertices[1] = second;
        }
        if (m_vertices.size() > 2) {
            m_vertices[2] = third;
        }
        if (m_vertices.size() > 3) {
            m_vertices[3] = fourth;
        }
    }

    auto state() -> bool
    {
        return (!first.equal(m_first) || !second.equal(m_second) ||
                !third.equal(m_third) || !fourth.equal(m_fourth));
    }

public:
    Point first, second, third, fourth, origin;

    std::string label{"Base"};

    auto setup(const Points &points) -> Points
    {
        if (points.size() < 2) {
            m_vertices.clear();
            return m_vertices;
        }

        m_vertices = points;
        first  = m_vertices[0];
        second = m_vertices[1];
        third  = points.size() > 2 ? m_vertices[2] : Point();
        fourth = points.size() > 3 ? m_vertices[3] : Point();
        origin = first;

        m_first  = first;
        m_second = second;
        m_third  = third;
        m_fourth = fourth;
        m_origin = first;

        return m_vertices;
    }

    auto isConvex() -> bool
    {
        auto sides = m_vertices.size();
        if (sides < 3) {
            return false;
        }

        // Cross product of two vectors.
        auto crossProduct = [](const Point & origin, const Point & first, const Point & second) {
            auto x1 = first.X.value  - origin.X.value;
            auto y1 = first.Y.value  - origin.Y.value;
            auto x2 = second.X.value - origin.X.value;
            auto y2 = second.Y.value - origin.Y.value;
            return x1 * y2 - y1 * x2;
        };

        double previous = 0;
        for (unsigned i = 0; i < sides; i++) {
            auto product = crossProduct(m_vertices[i],
                                        m_vertices[(i + 1) % sides],
                                        m_vertices[(i + 2) % sides]);
            if (product != 0) {
                if (product * previous >= 0) {
                    previous = product;
                }
                else {
                    return false;
                }
            }
        }

        return true;
    }

    // Polygon contains point.
    auto contains(const Point &point) -> bool
    {
        int counter{0};
        for (auto i = 0; i < m_vertices.size(); ++i) {
            auto i1 = (i + 1) % m_vertices.size();
            // Check if point equal vertices.
            if (point == m_vertices[i] || point == m_vertices[i1]) {
                return true;
            }
            // An imaginary horizontal line intersects an edge.
            auto [result, status] = Point::lineIntersect(point, Point(MAXNUMBER, point.Y.value), m_vertices[i], m_vertices[i1]);
            if (status == 1) {  // Coincident.
                // Check if it is out of range.
                if (point.X.value < std::min(m_vertices[i].X.value, m_vertices[i1].X.value) ||
                        point.X.value > std::max(m_vertices[i].X.value, m_vertices[i1].X.value)) {
                    return false;
                }
                // It's on the range.
                return true;
            }
            if (status == 2) { // Intersect
                if (result == point) {
                    return true;
                }
                counter++;
            }
        }
        if (counter % 2 == 0) {
            return false;
        }

        return true;
    }

    // Polygon contains points
    auto contains(const Points &points) -> std::tuple<Points, bool>
    {
        Points pointsInPolygon;
        for (const auto &p : points) {
            if (contains(p)) {
                pointsInPolygon.emplace_back(p);
            }
        }

        return {pointsInPolygon, !pointsInPolygon.empty()};
    }

    // Polygon contains Polygon
    auto contains(const Base &polygon) -> std::tuple<Points, bool>
    {
        return contains(polygon.m_vertices);
    }

    // Calculates the points between the intersection of two polygons.
    auto intersect(const Points &polygonPoints) -> Points
    {
        int result{0};
        Point point;
        Points vertices;
        Points vPoly1 = points();
        for (unsigned i = 0; i < vPoly1.size(); ++i) {
            for (unsigned j = 0; j < polygonPoints.size(); ++j) {
                std::tie(point, result) = Point::lineIntersect(vPoly1[i % vPoly1.size()], vPoly1[(i + 1) % vPoly1.size()],
                                          polygonPoints[j % polygonPoints.size()], polygonPoints[(j + 1) % polygonPoints.size()]);
                if (result == 2) {
                    vertices.emplace_back(point);
                }
            }
        }

        return vertices;
    }

    // Rearrange the polygon points.
    auto organize() -> Points
    {
        return Point::organize(m_vertices);
    }

    // Calculates the Area by triangular subdivisions.
    auto area() -> double
    {
        if (m_vertices.size() < 3) {
            return 0;
        }

        double result = 0;
        if (m_vertices.size() == 3) {
            result = Point::triangleArea(m_vertices[0], m_vertices[1], m_vertices[2]);
        }
        else if (isConvex()) {
            // Calculate for convex.
            for (unsigned i = 2; i < m_vertices.size(); ++i) {
                result += Point::triangleArea(m_vertices[0], m_vertices[i - 1], m_vertices[i]);
            }
        }
        else {
            // Calculate for concave.
            double calc1 = 0;
            double calc2 = 0;
            for (int i = 0; i < m_vertices.size(); ++i) {
                auto j = (i + 1) % m_vertices.size();
                calc1 += m_vertices[i].X.value * m_vertices[j].Y.value;
                calc2 += m_vertices[i].Y.value * m_vertices[j].X.value;
            }
            result = (calc1 - calc2) / 2;
        }

        return result;
    }

    auto perimeter() -> double
    {
        auto perimeter = Point::sumDistances(m_vertices);

        if (m_vertices.size() > 2) {
            perimeter += m_vertices.back().distance(m_vertices.front());
        }

        return perimeter;
    }

    auto equal(Base polygon, bool compareOrder = false) -> bool
    {
        return Check::equal(points(), polygon.points());
    }

    auto round(int decimalPlaces = 2) -> Base
    {
        Base polygon;
        polygon.m_vertices = Point::round(m_vertices, decimalPlaces);

        return polygon;
    }

    // Return the length of each side.
    auto lengthOfSides() -> Numbers
    {
        if (m_vertices.size() < 2) {
            return {0};
        }

        if (m_vertices.size() == 2) {
            return {perimeter()};
        }

        Numbers lengths;
        for (unsigned i = 0; i < m_vertices.size(); i++) {
            lengths.push_back(m_vertices[i].distance(m_vertices[(i + 1) % m_vertices.size()]));
        }

        return lengths;
    }

    // Average length.
    auto averageLength() -> double
    {
        if (!m_vertices.empty()) {
            return perimeter() / static_cast<double>(m_vertices.size());
        }

        return 0;
    }

    // Returns the current vertices.
    auto points() -> Points
    {
        if (state()) {
            update(first, second, third, fourth);
        }

        return m_vertices;
    }
};

// Line (x1,y1)(x2,y2)
class Line : public Base {

public:
    Line() = default;

    // Line : Point 1 (x1,y1) to Point 2 (x2,y2).
    Line(const Point &first, const Point &second)
    {
        setup(first, second);
    };

    // Line : Point (x,y), angle and length.
    Line(const Point &origin, const double &angle, const double &length)
    {
        setup(origin, angle, length);
    }

    // Line : Point 1 (x1,y1) to Point 2 (x2,y2).
    Line(const double &x1, const double &y1, const double &x2, const double &y2)
    {
        setup(Point(x1, y1), Point(x2, y2));
    };

    ~Line() = default;

    // Line : Point 1 (x1,y1) to Point 2 (x2,y2).
    // Returns vertices.
    auto setup(const Point &first, const Point &second = Point(0, 0)) -> Points
    {
        Base::setup({first, second});
        label = "Line";

        return points();
    }

    // Line : Point (x,y), angle and length.
    // Returns vertices.
    auto setup(const Point &origin, const double &angle, const double &length) -> Points
    {
        return setup(origin, origin.position(angle, length));
    }

    // Line : Point 1 (x1,y1) to Point 2 (x2,y2).
    // Returns vertices.
    auto setup(const double &x1, const double &y1, const double &x2, const double &y2) -> Points
    {
        return setup(Point(x1, y1), Point(x2, y2));
    }

    // Distance between the two points.
    auto length() -> double
    {
        return perimeter();
    }

    // Line angle.
    // Direction: first point to second point.
    auto angle() -> double
    {
        // Update
        auto vertices = points();

        if (vertices.size() < 2) {
            return 0;
        }

        return first.angle(second);
    }

    // Midpoint
    auto middle() -> Point
    {
        // Update
        points();

        return first.position(angle(), length() / 2);
    }

    // Returns the intersection point with another line.
    // Returns Point(max X, max Y) and -1 if not intersect.
    // Returns Point(max X, max Y) and 0 if the lines are parallel.
    // Returns Point(x0, y0) and 1 if the lines coincident.
    // Return the intersection point and 2 if they are in range.
    // Return the intersection point and 3 if not in range.
    auto intersection(const Line &line) -> std::tuple<Point, int>
    {
        return Point::lineIntersect(first.X.value, first.Y.value,
                                    second.X.value, second.Y.value,
                                    line.first.X.value, line.first.Y.value,
                                    line.second.X.value, line.second.Y.value);
    }

    // Calculates the point of intersection between two lines.
    // Returns Point(max X, max Y) and -1 if not intersect.
    // Returns Point(max X, max Y) and 0 if the lines are parallel.
    // Returns Point(x0, y0) and 1 if the lines coincident.
    // Return the intersection point and 2 if they are in range.
    // Return the intersection point and 3 if not in range.
    static auto lineIntersect(Line line1, const Line &line2) -> std::tuple<Point, int>
    {
        return line1.intersection(line2);
    }

    // Perpendicular line passing through the point.
    auto perpendicular(const Point &point) -> Line
    {
        // Dummy triangle area.
        double area = first.triangleArea(second, point);
        // Pythagorean theorem : a^2 + b^2 = c^2
        double c = first.distance(point);               // hypotenuse
        double b = 2 * area / first.distance(second);   // area = base * height / 2
        double a = sqrt(pow(c, 2) - pow(b, 2));

        // Line with base slope.
        return {first.position(angle(), a), point};
    }

    // Perpendicular line passing through the point.
    static auto perpendicular(Line line, const Point &point) -> Line
    {
        return line.perpendicular(point);
    }
};

// Triangle (x1,y1)(x2,y2)(x3,y3)
class Triangle : public Base {

public:
    Triangle() = default;

    // Triangle: Points (x1,y1),(x2,y2),(x3,y3)
    Triangle(const Point &first, const Point &second, const Point &third)
    {
        setup(first, second, third);
    };

    // Triangle: Points (x1,y1),(x2,y2) and height.
    // Returns vertices.
    Triangle(const Point &first, const Point &second, const double &height)
    {
        setup(Line(first, second), height);
    }

    ~Triangle() = default;

    // Triangle: Points (x1,y1),(x2,y2),(x3,y3)
    // Returns vertices.
    auto setup(const Point &first, const Point &second, const Point &third) -> Points
    {
        Base::setup({first, second, third});
        label = "Triangle";

        return points();
    }

    // Triangle: Line and height.
    // Returns vertices.
    auto setup(Line side, const double &height) -> Points
    {
        return setup(side.first,
                     side.second,
                     Point(side.middle()).position(90 + side.angle(), height));
    }

    // Triangle: Points (x1,y1),(x2,y2) and height.
    // Returns vertices.
    auto setup(const Point &first, const Point &second, const double &height) -> Points
    {
        return setup(Line(first, second), height);
    }

    auto height() -> double
    {
        return first.triangleHeight(second, third);
    }

};

// Rectangle (x1,y1)(x2,y2)(x3,y3)(x4,y4)
class Rectangle : public Base {

public:
    Rectangle() = default;

    // Rectangle: Points (x1,y1),(x2,y2),(x3,y3),(x4,y4)
    Rectangle(const Point &first, const Point &second, const Point &third, const Point &fourth)
    {
        setup(first, second, third, fourth);
    };

    // Rectangle : Point (x,y), width and heigth.
    Rectangle(const Point &origin, const double &width, const double &heigth)
    {
        setup(origin, width, heigth);
    }

    ~Rectangle() = default;

    // Rectangle: Points (x1,y1),(x2,y2),(x3,y3),(x4,y4)
    // Returns vertices.
    auto setup(const Point &first, const Point &second, const Point &third, const Point &fourth) -> Points
    {
        Base::setup({first, second, third, fourth});
        label = "Rectangle";

        return points();
    }

    // Rectangle : Point (x,y), width and heigth.
    // Returns vertices.
    auto setup(const Point &origin, const double &width, const double &heigth) -> Points
    {
        return setup(origin,
                     origin + Point(width, 0),
                     origin + Point(width, heigth),
                     origin + Point(0, heigth));
    }

    template<typename T = double>
    auto width() -> T
    {
        return static_cast<T>(first.distance(second));
    }

    template<typename T = double>
    auto height() -> T
    {
        return static_cast<T>(first.distance(fourth));
    }
};

// Regular Polygon (x,y)...(xN,yN)
class RegularPolygon : public Base {

    Point last_center{0, 0};
    double last_angle{0};
    double last_horizontalRadius{0};
    double last_verticalRadius{0};
    unsigned last_sides{0};

    void update()
    {
        // If there has been modification in the parameters.
        if (state()) {
            setup(center, horizontalRadius, verticalRadius, angle, sides);
        }
    }

public:
    Point center{0, 0};
    double angle{0};
    double horizontalRadius{0};
    double verticalRadius{0};
    unsigned sides{0};

    // Returns true if data changes.
    auto state() -> bool
    {
        return !(center == last_center &&
                 angle == last_angle &&
                 horizontalRadius == last_horizontalRadius &&
                 verticalRadius == last_verticalRadius &&
                 sides == last_sides);
    }

    RegularPolygon() = default;

    // Center : polygon center point (x,y),
    // radius : distance from the center (>= 1),
    // angle  : starting point angle,
    // sides  : divisions of a circle (>= 3).
    RegularPolygon(const Point &center, const double &radius, const double &angle, const unsigned &sides)
    {
        setup(center, radius, radius, angle, sides);
    }

    // Center : polygon center point (x,y),
    // horizontalRadius : distance from the center on X axis (>= 1),
    // verticalRadius   : distance from the center on Y axis (>= 1),
    // angle  : starting point angle,
    // sides  : divisions of a circle (>= 3).
    RegularPolygon(const Point &center, const double &horizontalRadius, const double &verticalRadius,
                   const double &angle, const unsigned &sides)
    {
        setup(center, horizontalRadius, verticalRadius, angle, sides);
    }

    ~RegularPolygon() = default;

    // Center : polygon center point (x,y),
    // horizontalRadius : distance from the center on X axis (>= 1),
    // verticalRadius   : distance from the center on Y axis (>= 1),
    // angle  : starting point angle,
    // sides  : divisions of a circle (>= 3).
    // Returns vertices.
    auto setup(const Point &center, const double &horizontalRadius, const double &verticalRadius,
               const double &angle, unsigned sides) -> Points
    {
        // Update.
        this->center = center;
        this->angle = angle;
        this->horizontalRadius = horizontalRadius;
        this->verticalRadius = verticalRadius;
        this->sides = sides;

        last_center = center;
        last_angle = angle;
        last_horizontalRadius = horizontalRadius;
        last_verticalRadius = verticalRadius;
        last_sides = sides;

        // Check.
        sides = sides > 360 ? 360 : sides;
        if (horizontalRadius < 1 || verticalRadius < 1 || sides < 3) {
            return Base::setup({});
        }

        Points points;
        int begin = static_cast<int>(angle);
        int end = 360 + static_cast<int>(angle);
        int step = 360 / static_cast<int>(sides);
        for (auto a = begin; a < end; a += step) {
            points.emplace_back(center.position(a, horizontalRadius, verticalRadius));
        }

        label = "Polygon";

        return Base::setup(points);
    }

    // Center : polygon center point (x,y),
    // radius : distance from the center (>= 1),
    // angle  : starting point angle,
    // sides  : divisions of a circle (>= 3).
    auto setup(const Point &center, const double &radius, const double &angle, const unsigned &sides) -> Points
    {
        return setup(center, radius, radius, angle, sides);
    }

    auto sideLength(int decimalPlaces = 2) -> double
    {
        if (sides < 2) {
            return 0;
        }

        auto result = first.distance(second);

        if (decimalPlaces > 0) {
            return result;
        }

        const int base = 10;
        return std::round(result * std::pow(base, decimalPlaces)) / std::pow(base, decimalPlaces);
    }

    // Returns the current vertices.
    auto points() -> Points
    {
        update();

        return Base::points();
    }
};

class Ellipse : public RegularPolygon {

public:
    Ellipse() = default;

    Ellipse(const Point &center, const double &horizontalRadius, const double &verticalRadius)
    {
        setup(center, horizontalRadius, verticalRadius);
    }

    ~Ellipse() = default;

    auto setup(const Point &center, const double &horizontalRadius, const double &verticalRadius) -> Points
    {
        RegularPolygon::setup(center, horizontalRadius, verticalRadius, 0, 360);
        label = "Ellipse";

        return points();
    }

    auto area() -> double
    {
        return horizontalRadius * verticalRadius * PI;
    }

    auto perimeter() -> double
    {
        // Ramanujan
        auto a = std::max(horizontalRadius, verticalRadius);
        auto b = std::min(horizontalRadius, verticalRadius);
        auto h = std::pow((a - b), 2) / std::pow((a + b), 2);

        return PI * (a + b) * (1 + ((3 * h) / (10 + std::sqrt(4 - 3 * h))));
    }

    auto contains(const Point &point) -> bool
    {
        auto result = (std::pow((point.X.value - center.X.value), 2) / std::pow(horizontalRadius, 2)) +
                      (std::pow((point.Y.value - center.Y.value), 2) / std::pow(verticalRadius, 2));

        return result <= 1;
    }
};

class Circle : public Ellipse {

public:
    Circle() = default;

    Circle(const Point &center, const double &radius)
    {
        setup(center, radius);
    }

    ~Circle() = default;

    auto setup(const Point &center, const double &radius) -> Points
    {
        Ellipse::setup(center, radius, radius);
        label = "Circle";

        return points();
    }
};

// Irregular Polygon (x,y)...(xN,yN)
class IrregularPolygon : public Base {

public:
    IrregularPolygon() = default;

    explicit IrregularPolygon(const Points &points)
    {
        setup(points);
    }

    explicit IrregularPolygon(Triangle &triangle)
    {
        setup(triangle.points());
    }

    explicit IrregularPolygon(Rectangle &rectangle)
    {
        setup(rectangle.points());
    }

    explicit IrregularPolygon(RegularPolygon &polygon)
    {
        setup(polygon.points());
    }

    ~IrregularPolygon() = default;
};

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

    // Checks if the file extension is the expected one.
    static auto equal(const std::string &filePath,
                      const std::string &fileExtensionExpected) -> bool
    {
        return Text::strUpper(getExtension(filePath)) ==  Text::strUpper(fileExtensionExpected);
    }

    static auto isBitmap(const std::string &filePath) -> bool
    {
        auto extFile = Text::strUpper(getExtension(filePath));
        return extFile == "PNG" || extFile == "JPEG" || extFile == "JPG" ||
               extFile == "BMP";
    }

    static auto isSVG(const std::string &filePath) -> bool
    {
        return Text::strUpper(getExtension(filePath)) == "SVG";
    }

    // Load text file.
    static auto load(const std::string &filePath, const std::string &filenameExtension) -> std::string
    {
        if (filePath.empty()) {
            return {};
        }

        // Check extension.
        if (!filenameExtension.empty()) {
            if (!equal(filePath, filenameExtension)) {
                std::cerr << "Invalid file!\n";
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

    // Std::cout : Point(x,y).
    static void view(const Point &point)
    {
        std::cout << "(" << point.toStr() << ")" << '\n';
    }

    // Std::cout : Vector of Point(x,y).
    static void view(const Points &points)
    {
        std::string str{};
        for (unsigned i = 0; i < points.size(); i++) {
            str += "(" + points[i].toStr() + ")" + (i < points.size() - 1 ? "," : "");
        }

        if (str.empty()) {
            std::cout << "Empty\n";
        }
        else {
            std::cout << str << '\n';
        }
    }

    // Std::cout : Base object
    static void view(Base &base)
    {
        view(base.points());
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
    static void view(const Strings &values)
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

} // namespace smalltoolbox

#endif // SMALLTOOLBOX_H_
