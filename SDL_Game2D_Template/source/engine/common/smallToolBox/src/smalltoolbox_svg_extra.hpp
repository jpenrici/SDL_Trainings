#ifndef SMALLTOOLBOX_SVGEXTRA_H
#define SMALLTOOLBOX_SVGEXTRA_H

#include <algorithm>

#include "../src/smalltoolbox_image.hpp"
#include "../src/smalltoolbox_math.hpp"
#include "../src/smalltoolbox_system.hpp"
#include "../src/smalltoolbox_text.hpp"


// Sketch
// Organizes SVG development.
class Sketch : public stbox::Image::SVG, public stbox::Image::Color {

public:

    // Return basic SVG::Shape with Polygon base.
    static auto normalShape(stbox::Math::Base base, const std::string &label) -> stbox::Image::SVG::NormalShape
    {
        stbox::Image::SVG::NormalShape shape;
        shape.name = label;
        shape.points = base.pointsXY();

        return shape;
    }

    // Return basic SVG::CircleShape with Ellipse base.
    static auto circleShape(const stbox::Math::Ellipse &ellipse, const std::string &label) -> stbox::Image::SVG::CircleShape
    {
        stbox::Image::SVG::CircleShape shape;
        shape.name = label;
        shape.center = std::array<double, 2>{ellipse.center.X.value, ellipse.center.Y.value};
        shape.horizontalRadius = ellipse.horizontalRadius;
        shape.verticalRadius = ellipse.verticalRadius;

        return shape;
    }

    // Return SVG::polyline with Polygon base.
    static auto svgPolyline(const stbox::Math::Base &base, const std::string &label) -> std::string
    {

        return stbox::Image::SVG::polygon(normalShape(base, label));
    }

    // Return SVG::polyline with Polygon base.
    static auto svgPolyline(stbox::Math::Base base, const std::string &label, const stbox::Image::Color::RGBA &fill,
                            const stbox::Image::Color::RGBA &stroke) -> std::string
    {

        return stbox::Image::SVG::polyline(stbox::Image::SVG::NormalShape(label,
                                              RGB2hex(fill.R, fill.G, fill.B),
                                              RGB2hex(stroke.R, stroke.G, stroke.B),
                                              1.0,    // strokeWidth
                                              base.pointsXY()));
    }

    // Return SVG::polyline with Polygon base.
    static auto svgPolyline(stbox::Math::Base base, const std::string &label, const stbox::Image::Color::RGBA &fill,
                            const stbox::Image::Color::RGBA &stroke,
                            const double &fillOpacity, const double &strokeOpacity) -> std::string
    {

        return stbox::Image::SVG::polyline(stbox::Image::SVG::NormalShape(label,
                                              RGB2hex(fill.R, fill.G, fill.B),
                                              RGB2hex(stroke.R, stroke.G, stroke.B),
                                              1.0,    // strokeWidth
                                              fillOpacity,
                                              strokeOpacity,
                                              base.pointsXY()));
    }

    // Return SVG::polyline with Polygon base.
    static auto svgPolyline(stbox::Math::Base base, const std::string &label, const stbox::Image::Color::RGBA &fill,
                            const stbox::Image::Color::RGBA &stroke,
                            const double &fillOpacity, const double &strokeOpacity,
                            const double &strokeWidth) -> std::string
    {

        return stbox::Image::SVG::polyline(stbox::Image::SVG::NormalShape(label,
                                              RGB2hex(fill.R, fill.G, fill.B),
                                              RGB2hex(stroke.R, stroke.G, stroke.B),
                                              strokeWidth,
                                              fillOpacity,
                                              strokeOpacity,
                                              base.pointsXY()));
    }

    // Return SVG::polygon with Polygon base.
    static auto svgPolygon(const stbox::Math::Base &base, const std::string &label) -> std::string
    {

        return stbox::Image::SVG::polygon(normalShape(base, label));
    }

    // Return SVG::polygon with Polygon base.
    static auto svgPolygon(stbox::Math::Base base, const std::string &label, const stbox::Image::Color::RGBA &fill,
                           const stbox::Image::Color::RGBA &stroke) -> std::string
    {

        return stbox::Image::SVG::polygon(stbox::Image::SVG::NormalShape(label,
                                             RGB2hex(fill.R, fill.G, fill.B),
                                             RGB2hex(stroke.R, stroke.G, stroke.B),
                                             1.0, // strokeWidth
                                             base.pointsXY()));
    }

    // Return SVG::polygon with Polygon base.
    static auto svgPolygon(stbox::Math::Base base, const std::string &label, const stbox::Image::Color::RGBA &fill,
                           const stbox::Image::Color::RGBA &stroke,
                           const double &fillOpacity, const double &strokeOpacity) -> std::string
    {

        return stbox::Image::SVG::polygon(stbox::Image::SVG::NormalShape(label,
                                             RGB2hex(fill.R, fill.G, fill.B),
                                             RGB2hex(stroke.R, stroke.G, stroke.B),
                                             1.0, // strokeWidth
                                             fillOpacity,
                                             strokeOpacity,
                                             base.pointsXY()));
    }

    // Return SVG::polygon with Polygon base.
    static auto svgPolygon(stbox::Math::Base base, const std::string &label, const stbox::Image::Color::RGBA &fill,
                           const stbox::Image::Color::RGBA &stroke,
                           const double &fillOpacity, const double &strokeOpacity,
                           const double &strokeWidth) -> std::string
    {

        return stbox::Image::SVG::polygon(stbox::Image::SVG::NormalShape(label,
                                             RGB2hex(fill.R, fill.G, fill.B),
                                             RGB2hex(stroke.R, stroke.G, stroke.B),
                                             strokeWidth,
                                             fillOpacity,
                                             strokeOpacity,
                                             base.pointsXY()));
    }

    // Return SVG::circle with Ellipse base.
    static auto svgCircle(const stbox::Math::Ellipse &ellipse, const std::string &label) -> std::string
    {
        return stbox::Image::SVG::circle(circleShape(ellipse, label));
    }

    // Return SVG::circle with Ellipse base.
    static auto svgCircle(const stbox::Math::Ellipse &ellipse, const std::string &label,
                          const stbox::Image::Color::RGBA &fill,
                          const stbox::Image::Color::RGBA &stroke) -> std::string
    {
        return stbox::Image::SVG::circle(stbox::Image::SVG::CircleShape(label,
                                            RGB2hex(fill.R, fill.G, fill.B),
                                            RGB2hex(stroke.R, stroke.G, stroke.B),
                                            1.0,    // strokeWidth
                                            255,    // fillOpacity
                                            255,    // strokeOpacity
                                            std::array<double, 2>{ellipse.center.X.value, ellipse.center.Y.value},
                                            ellipse.horizontalRadius,
                                            ellipse.verticalRadius));
    }

    // Return SVG::circle with Ellipse base.
    static auto svgCircle(const stbox::Math::Ellipse &ellipse, const std::string &label,
                          const stbox::Image::Color::RGBA &fill,
                          const stbox::Image::Color::RGBA &stroke, const double &fillOpacity,
                          const double &strokeOpacity) -> std::string
    {
        return SVG::circle(SVG::CircleShape(label,
                                            RGB2hex(fill.R, fill.G, fill.B),
                                            RGB2hex(stroke.R, stroke.G, stroke.B),
                                            1.0,    // strokeWidth
                                            fillOpacity,
                                            strokeOpacity,
                                            std::array<double, 2>{ellipse.center.X.value, ellipse.center.Y.value},
                                            ellipse.horizontalRadius,
                                            ellipse.verticalRadius));
    }

    // Return SVG::circle with Ellipse base.
    static auto svgCircle(const stbox::Math::Ellipse &ellipse, const std::string &label,
                          const stbox::Image::Color::RGBA &fill,
                          const stbox::Image::Color::RGBA &stroke, const double &fillOpacity,
                          const double &strokeOpacity,
                          const double &strokeWidth) -> std::string
    {
        return SVG::circle(SVG::CircleShape(label,
                                            RGB2hex(fill.R, fill.G, fill.B),
                                            RGB2hex(stroke.R, stroke.G, stroke.B),
                                            strokeWidth,
                                            fillOpacity,
                                            strokeOpacity,
                                            std::array<double, 2>{ellipse.center.X.value, ellipse.center.Y.value},
                                            ellipse.horizontalRadius,
                                            ellipse.verticalRadius));
    }

    // Returns SVG Elements.
    static auto join(const std::vector<stbox::Math::Base> &bases, const std::string &label = "") -> std::string
    {
        std::string strShape{};
        for (const auto &item : bases) {
            strShape += Sketch::svgPolygon(item, label);
        }

        return strShape;
    }

};

// Interpreter
// Conversion of text into commands for SVG construction.
class Interpreter {

    enum elementType {
        EMPTY, POINTS, LINE, TRIANGLE, RECTANGLE, CIRCLE, ELLIPSE, POLYGON, BASE
    };

    std::vector<std::string> labelType {
        "EMPTY", "POINTS", "LINE", "TRIANGLE", "RECTANGLE",
        "CIRCLE", "ELLIPSE", "POLYGON", "BASE"
    };

    const char SPACE = static_cast<char>(32);
    const std::string ERROR   = "\tERROR";
    const std::string WARNING = "\tWARNING";

public:

    Interpreter() = default;
    ~Interpreter() = default;

    auto svg(std::string line, std::string &error) -> std::string
    {
        if (line.empty()) {
            error = "[LINE EMPTY]\n";
            return {};
        }

        std::string result{};
        std::string bkp = line;

        // Prepare
        line = stbox::Text::trim(line, SPACE); // Remove spaces from the ends.
        transform(line.begin(), line.end(), line.begin(), ::toupper); // Format.

        // Check command.
        std::string command{};
        unsigned element = EMPTY;
        for (unsigned i = 1; i < labelType.size(); i++) {
            command = labelType[i] + ":";   // Syntax.
            if (line.starts_with(command)) {
                element = i;
                break;
            }
        }
        if (element < POINTS || element > POLYGON) {
            error = bkp + WARNING + "[Ignore]\n";
            return result;
        }

        // Prepare
        line = line.substr(command.size()); // Remove command word.

        // Check points container: { }
        auto counter = count_if(line.begin(), line.end(), [](char c) {
            return c == '{';
        });
        if (counter > 1) {
            error = bkp + ERROR + "[Curly braces]\n";
            return result;
        }
        counter -= count_if(line.begin(), line.end(), [](char c) {
            return c == '}';
        });
        if (counter != 0) {
            error = bkp + ERROR + "[Curly braces]\n";
            return result;
        }

        // Check coordinates container. ( )
        counter = count_if(line.begin(), line.end(), [](char c) {
            return c == '(';
        }) -
        count_if(line.begin(), line.end(), [](char c) {
            return c == ')';
        });
        if (counter != 0) {
            error = bkp + ERROR + "[Parentheses]\n";
            return result;
        }

        // Reset
        std::vector<stbox::Math::Point> points{};
        double angle{0};
        double width{0};
        double height{0};
        double length{0};
        double horizontalRadius{0};
        double verticalRadius{0};
        double fillOpacity{255};
        double strokeOpacity{255};
        double strokeWidth{1.0};
        unsigned sides{0};
        std::string label{};
        stbox::Image::Color::RGBA fillColor(255, 255, 255, 255);
        stbox::Image::Color::RGBA strokeColor(0, 0, 0, 255);

        // Check content between braces.
        // Expected: {(x0,y1)(xN,yN)}
        std::string content{};
        unsigned first = line.find_first_of('{');
        unsigned second = line.find_first_of('}');
        if (second < first) {   // } {
            error = bkp + ERROR + "[Curly braces]\n";
            return result;
        }
        if (first < second) {   // { ... } or {}
            content = line.substr(first + 1, second - first - 1);
            line = line.substr(0, first) + line.substr(second + 1);
        }
        if (!content.empty()) {
            content = stbox::Text::replace(content, SPACE, "");  // (x0, y0)( x1,y1) to (x0,y0)(x1,y1)
            content = stbox::Text::replace(content, '(', " (");  // (x0,y0)(x1,y1) to (x0,y0) (x1,y1)
            for (auto str : stbox::Text::split(content, SPACE)) {
                str = stbox::Text::trim(str, SPACE);
                if (str.starts_with('(') && str.ends_with(')')) { // (x,y)
                    str = stbox::Text::trim(stbox::Text::trim(str, '('), ')');  //  x,y
                    try {
                        auto values = stbox::Text::split(str, ',');      //  x y
                        if (values.size() == 2) {
                            // Convert to Point.
                            points.emplace_back(stod(values[0]), stod(values[1]));
                        }
                    }
                    catch (...) {
                        error = bkp + ERROR + "[Invalid numeric formatting!]\n";
                        return result;
                    }
                }
            }
        }

        if (points.empty()) {
            error = bkp + WARNING + "[Ignore]\n";
            return result;
        }

        // Check other arguments.
        for (const auto &str : stbox::Text::split(line, SPACE)) {
            std::vector<std::string> complements {
                "ANGLE", "SIDES", "WIDTH", "HEIGHT", "LENGTH", "RADIUS", "HRADIUS", "VRADIUS",
                "STROKEW"
            };
            for (unsigned i = 0; i < complements.size(); i++) {
                auto arg = complements[i] + "=";
                if (str.starts_with(arg)) {
                    try {
                        auto value = stod(str.substr(arg.size()));
                        switch (i) {
                        case 0:
                            angle = value;
                            break;
                        case 1:
                            sides = static_cast<unsigned>(value);
                            break;
                        case 2:
                            width = value;
                            break;
                        case 3:
                            height = value;
                            break;
                        case 4:
                            length = value;
                            break;
                        case 5:
                        // radius = horizontalRadius
                        case 6:
                            horizontalRadius = value;
                            break;
                        case 7:
                            verticalRadius = value;
                            break;
                        case 8:
                            strokeWidth = value;
                            break;
                        default:
                            break;
                        }
                    }
                    catch (...) {
                        // pass
                    }
                }
            }
            std::string arg = "LABEL=";
            if (str.starts_with(arg)) {
                try {
                    label = str.substr(arg.size());
                }
                catch (...) {
                    // pass
                }
            }
            arg = "FILL=";
            if (str.starts_with("FILL=")) {
                try {
                    auto numbers = stbox::Text::split(str.substr(arg.size()), ',');
                    fillColor = stbox::Image::Color::RGBA(stoi(numbers[0]),
                                                   stoi(numbers[1]),
                                                   stoi(numbers[2]),
                                                   255);
                    fillOpacity = (numbers.size() == 4 ? stoi(numbers[3]) : 255);
                }
                catch (...) {
                    // pass
                }
            }
            arg = "STROKE=";
            if (str.starts_with(arg)) {
                try {
                    auto numbers = stbox::Text::split(str.substr(arg.size()), ',');
                    strokeColor = stbox::Image::Color::RGBA(stoi(numbers[0]),
                                                     stoi(numbers[1]),
                                                     stoi(numbers[2]),
                                                     255);
                    strokeOpacity = (numbers.size() == 4 ? stoi(numbers[3]) : 255);
                }
                catch (...) {
                    // pass
                }
            }
        }

        // Converter
        label = label.empty() ? labelType[element] : label;
        switch (element) {
        case POINTS:
            if (points.size() == 1) {
                // Isolated points in SVG are not recommended.
                error = bkp + WARNING + "[Ignore - A minimum of two points is expected!]\n";
                return {};
            }
            if (points.size() > 1) {
                result = Sketch::svgPolygon(stbox::Math::IrregularPolygon(points),
                                            label, fillColor, strokeColor);
            }
            break;
        case LINE:
            if (points.size() == 1 && angle > 0 && length > 0) {
                result = Sketch::svgPolygon(stbox::Math::Line(points.front(), angle, length),
                                            label, fillColor, strokeColor);
            }
            else if (points.size() == 2) {
                result = Sketch::svgPolygon(stbox::Math::IrregularPolygon(points),
                                            label, fillColor, strokeColor);
            }
            else {
                error = bkp + WARNING + "[Incomplete or wrong syntax!]\n";
                return {};
            }
            break;
        case TRIANGLE:
            if (points.size() == 2) {
                result = Sketch::svgPolygon(stbox::Math::Triangle(points[0], points[1], height),
                                            label, fillColor, strokeColor,
                                            fillOpacity, strokeOpacity, strokeWidth);
            }
            else if (points.size() == 3) {
                result = Sketch::svgPolygon(stbox::Math::IrregularPolygon(points),
                                            label, fillColor, strokeColor,
                                            fillOpacity, strokeOpacity, strokeWidth);
            }
            else {
                error = bkp + WARNING + "[Incomplete or wrong syntax!]\n";
                return {};
            }
            break;
        case RECTANGLE:
            if (points.size() == 1 && width > 0 && height > 0) {
                result = Sketch::svgPolygon(stbox::Math::Rectangle(points.front(), width, height),
                                            label, fillColor, strokeColor,
                                            fillOpacity, strokeOpacity, strokeWidth);
            }
            else if (points.size() == 4) {
                result = Sketch::svgPolygon(stbox::Math::IrregularPolygon(points),
                                            label, fillColor, strokeColor,
                                            fillOpacity, strokeOpacity, strokeWidth);
            }
            else {
                error = bkp + WARNING + "[Incomplete or wrong syntax!]\n";
                return {};
            }
            break;
        case CIRCLE:
            if (points.size() == 1 && horizontalRadius > 0) {
                result = Sketch::svgCircle(stbox::Math::Circle(points.front(), horizontalRadius),
                                           label, fillColor, strokeColor,
                                           fillOpacity, strokeOpacity, strokeWidth);
            }
            else {
                error = bkp + WARNING + "[Incomplete or wrong syntax!]\n";
                return {};
            }
            break;
        case ELLIPSE:
            if (points.size() == 1 && horizontalRadius > 0 && verticalRadius > 0) {
                result = Sketch::svgCircle(stbox::Math::Ellipse(points.front(),
                                                   horizontalRadius,
                                                   verticalRadius),
                                           label, fillColor, strokeColor,
                                           fillOpacity, strokeOpacity, strokeWidth);
            }
            else {
                error = bkp + WARNING + "[Incomplete or wrong syntax!]\n";
                return {};
            }
            break;
        case POLYGON:
            if (points.size() == 1 && horizontalRadius > 0 && angle > 0 && sides > 2) {
                result = Sketch::svgPolygon(stbox::Math::RegularPolygon(points.front(),
                                            horizontalRadius, angle, sides),
                                            label, fillColor, strokeColor,
                                            fillOpacity, strokeOpacity, strokeWidth);
            }
            else if (points.size() > 1) {
                result = Sketch::svgPolygon(stbox::Math::IrregularPolygon(points),
                                            label, fillColor, strokeColor,
                                            fillOpacity, strokeOpacity);
            }
            else {
                error = bkp + WARNING + "[Incomplete or wrong syntax!]\n";
                return {};
            }
            break;
        default:
            error = bkp + WARNING + "[Incomplete or wrong syntax!]\n";
            result = {};
            break;
        }

        return result;
    }

    auto load(const std::string &path, std::string &errors) -> std::string
    {
        auto text = stbox::Sys::IO::load(path, ".txt");
        if (text.empty()) {
            errors = path + ERROR + "[FILE EMPTY]\n";
            return {};
        }

        std::string result{};
        errors.clear();
        auto lines = stbox::Text::split(text, '\n');
        for (unsigned i = 0; i < lines.size(); i++) {
            auto line = lines[i];
            if (line.starts_with('#')) {
                errors += std::to_string(i + 1) + ": [COMMENT LINE]\n";
                continue;
            }
            std::string error{};
            auto res = svg(line, error);
            if (!res.empty()) {
                result += res;
            }
            else {
                errors += std::to_string(i + 1) + ": " + error;
            }
        }

        return result;
    }

};

#endif
