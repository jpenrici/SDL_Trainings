#ifndef SMALLTOOLBOX_IMAGE_H
#define SMALLTOOLBOX_IMAGE_H

#include <array>
#include <cmath>
#include <locale>
#include <string>
#include <vector>


namespace stbox {

namespace Image {

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

class SVG {

public:
    // Metadata setup.
    // creator             : String with the name of the creator or developer,
    // title               : String with title,
    // publisherAgentTitle : String with the name of the publisher,
    // date                : String with creation date, if empty use current date.
    struct Metadata {
        std::string creator = "SVG created automatically by algorithm in C++.";
        std::string title = "SVG";
        std::string publisherAgentTitle;
        std::string date;

        Metadata() = default;
        Metadata(const std::string &creator, const std::string &title, const std::string &publisher)
            : creator(creator), title(title), publisherAgentTitle(publisher) {}
    };

    // Drawing setup.
    // name          : ID used in SVG element,
    // fill          : Fill color in hexadecimal string format (#FFFFFF),
    // stroke        : Stroke color in hexadecimal string format (#FFFFFF),
    // strokeWidth   : Line width,
    // fillOpacity   : Fill opacity or alpha value from 0 to 255.
    // strokeOpacity : Stroke opacity or alpha value from 0 to 255.
    struct Style {
        std::string name{"Shape"}, fill{Color::WHITE}, stroke{Color::BLACK};
        double strokeWidth{1.0};
        double fillOpacity{255.0}, strokeOpacity{255.0}; // 0.0 = 0%; 255 = 1.0 = 100%

        Style() = default;

        Style(const std::string &name, const std::string &fill, const std::string &stroke,
              const double &strokeWidth)
            : name(name), fill(fill), stroke(stroke), strokeWidth(strokeWidth) {}
        Style(const std::string &name, const std::string &fill, const std::string &stroke,
              const double &strokeWidth, const double &fillOpacity, const double &strokeOpacity)
            : name(name), fill(fill), stroke(stroke), strokeWidth(strokeWidth), fillOpacity(fillOpacity),
            strokeOpacity(strokeOpacity) {}
    };

    // Polygon and Polyline.
    // points : Points vector (x,y).
    struct NormalShape : Style {
        std::vector<std::array<double, 2> > points;

        NormalShape() = default;

        NormalShape(const std::string &name, const std::string &fill, const std::string &stroke,
                    const double &strokeWidth)
            : Style(name, fill, stroke, strokeWidth) {}
        NormalShape(const std::string &name, const std::string &fill, const std::string &stroke,
                    const double &strokeWidth, const double &fillOpacity, const double &strokeOpacity)
            :  Style(name, fill, stroke, strokeWidth, fillOpacity, strokeOpacity) {}

        NormalShape(const std::string &name, const std::string &fill, const std::string &stroke,
                    const double &strokeWidth, const std::vector<std::array<double, 2> > &points)
            :  Style(name, fill, stroke, strokeWidth), points(points) {}
        NormalShape(const std::string &name, const std::string &fill, const std::string &stroke,
                    const double &strokeWidth, const double &fillOpacity, const double &strokeOpacity,
                    const std::vector<std::array<double, 2> > &points)
            :  Style(name,  fill, stroke, strokeWidth, fillOpacity, strokeOpacity), points(points) {}
    };

    // Circle and Ellipse.
    // center           : Central point, (x,y)
    // horizontalRadius : Horizontal radius of circle, rx
    // verticalRadius   : Vertical radius of circle, ry
    struct CircleShape : Style {
        std::array<double, 2> center{0, 0};
        double horizontalRadius{1};
        double verticalRadius{1};

        CircleShape() = default;

        CircleShape(const std::string &name, const std::string &fill, const std::string &stroke,
                    const double &strokeWidth)
            : Style(name, fill, stroke, strokeWidth) {}
        CircleShape(const std::string &name, const std::string &fill, const std::string &stroke,
                    const double &strokeWidth, const double &fillOpacity, const double &strokeOpacity)
            :  Style(name, fill, stroke, strokeWidth, fillOpacity, strokeOpacity) {}

        CircleShape(const std::string &name, const std::string &fill, const std::string &stroke,
                    const double &strokeWidth, const std::array<double, 2> &center, const double &horizontalRadius,
                    const double &verticalRadius)
            :  Style(name, fill, stroke, strokeWidth), center(center),
            horizontalRadius(horizontalRadius), verticalRadius(verticalRadius) {}
        CircleShape(const std::string &name, const std::string &fill, const std::string &stroke,
                    const double &strokeWidth, const double &fillOpacity, const double &strokeOpacity,
                    const std::array<double, 2> &center, const double &horizontalRadius, const double &verticalRadius)
            :  Style(name, fill, stroke, strokeWidth, fillOpacity, strokeOpacity),
            center(center), horizontalRadius(horizontalRadius), verticalRadius(verticalRadius) {}
    };

    // Returns SVG: <g> Elements </g>
    static auto group(std::string id, const std::string &elements) -> std::string
    {
        id = id.empty() ? "<g>\n" : "<g id=\"" + id + "\" >\n";
        return elements.empty() ? "" : id + elements + "</g>\n";
    }

private:

    // Validates and formats entries.
    static auto style(Style style, const std::string &name) -> std::string
    {
        style.name = name.empty() ? "Shape" : name;
        style.stroke = style.stroke.empty() ? "#000000" : style.stroke;
        style.fillOpacity = style.fillOpacity < 0 ? 0 : std::min(style.fillOpacity / 255, 1.0);
        style.strokeOpacity = style.strokeOpacity < 0 ? 0 : std::min(style.strokeOpacity / 255, 1.0);

        return {
                "id=\"" + style.name + "\"\nstyle=\"" +
                "opacity:" + std::to_string(style.fillOpacity) + ";fill:" + style.fill +
                ";stroke:" + style.stroke + ";stroke-width:" + std::to_string(style.strokeWidth) +
                ";stroke-opacity:" + std::to_string(style.strokeOpacity) +
                ";stroke-linejoin:round;stroke-linecap:round\"\n" };
    }

public:

    // Return SVG: <polyline ... />
    static auto polyline(const NormalShape &shape) -> std::string
    {
        if (shape.points.empty()) {
            return "<!-- Empty -->\n";
        }

        std::string values;
        for (const auto &point : shape.points) {
            values += std::to_string(point.front()) + " " + std::to_string(point.back());
        }

        return {
            "<polyline\n" + style(shape, shape.name) + "points=\"" + values + "\" />\n"
        };
    }

    // Return SVG: <path ... />
    static auto polygon(const NormalShape &shape) -> std::string
    {
        if (shape.points.empty()) {
            return "<!-- Empty -->\n";
        }

        std::string values;
        for (unsigned i = 0; i < shape.points.size() - 1; i++) {
            values += std::to_string(shape.points[i].front()) + " " + std::to_string(shape.points[i].back()) + " L ";
        }
        values += std::to_string(shape.points[shape.points.size() - 1].front()) + " " +
                  std::to_string(shape.points[shape.points.size() - 1].back());

        return {
            "<path\n" + style(shape, shape.name) + "d=\"M " + values + " Z\" />\n"
        };
    }

    // Return SVG : <ellipse ... />
    static auto circle(const CircleShape &circle) -> std::string
    {
        if (circle.horizontalRadius < 1 || circle.verticalRadius < 1) {
            return "<!-- Empty -->\n";
        }
        return {
            "<ellipse\n" + style(circle, circle.name) +
            "cx=\"" + std::to_string(circle.center.front()) + "\" " +
            "cy=\"" + std::to_string(circle.center.back()) + "\" " +
            "rx=\"" + std::to_string(circle.horizontalRadius) + "\" " +
            "ry=\"" + std::to_string(circle.verticalRadius) + "\" />\n"
        };
    }

    // Return clone SVG.
    // referenceName : references ID of the original SVG element.
    // rotationAngle : rotation applied to the clone object.
    // center        : center of rotation of the clone object.
    // position      : clone object coordinate. (x,y)
    static auto clone(const std::string &referenceName, const int &rotationAngle, const std::array<double, 2> &center,
                      std::array<double, 2> position) -> std::string
    {
        static int counter = 0;

        position.front() += center.front() < 0.0 ? -center.front()  : 0.0;
        position.back() += center.back() < 0.0 ? -center.back() : 0.0;

        // Matrix - Rotate and Translate
        auto a = rotationAngle * std::numbers::pi / 180.0;
        auto cx = center.front();
        auto cy = center.back();
        auto px = position.front();
        auto py = position.back();
        std::vector<double> matrix{
            std::cos(a), -std::sin(a), -cx *std::cos(a) + cy * sin(a) + cx + px,
            std::sin(a),  std::cos(a), -cx *std::sin(a) - cy * cos(a) + cy + py,
            0, 0, 1
        };
        std::string transform{};
        for (std::vector<int> index{ 0, 3, 1, 4, 2, 5 }; auto &i : index) {
            transform += std::to_string(matrix[i]) + " ";
        }

        std::string label = "Clone_" + referenceName + "_" + std::to_string(counter++);

        return {
            "<use \n"
            "x=\"0\"\n"
            "y=\"0\"\n"
            "xlink:href=\"#" + referenceName + "\"\n" +
            "id=\"" + label + "\"\n" +
            "transform=\"matrix( " + transform + ")\"\n" +
            "width=\"100%\"\n"
            "height=\"100%\" />"
        };
    }

    // Return full SVG.
    static auto svg(const int &width, const int &height, const std::string &xml,
                    Metadata metadata) -> std::string
    {
        std::string now;
        try {
            time_t t = time(nullptr);
            tm *const pTm = localtime(&t);
            now = std::to_string(1900 + pTm->tm_year);
        }
        catch (...) {
            // pass
        }

        metadata.date = metadata.date.empty() ? now : metadata.date;

        return {
            "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n"
            "<svg\n"
            "xmlns:dc=\"http://purl.org/dc/elements/1.1/\"\n"
            "xmlns:cc=\"http://creativecommons.org/ns#\"\n"
            "xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"\n"
            "xmlns:svg=\"http://www.w3.org/2000/svg\"\n"
            "xmlns=\"http://www.w3.org/2000/svg\"\n"
            "xmlns:xlink=\"http://www.w3.org/1999/xlink\"\n"
            "width=\"" + std::to_string(width) + "\"\n" +
            "height=\"" + std::to_string(height) + "\"\n" +
            "viewBox= \"0 0 " + std::to_string(width) + " " + std::to_string(height) + "\"\n" +
            "version=\"1.1\"\n" +
            "id=\"svg8\">\n" +
            "<title\n" +
            "id=\"title1\">" + metadata.title + "</title>\n" +
            "<defs\n" +
            "id=\"defs1\" />\n" +
            "<metadata\n" +
            "id=\"metadata1\">\n" +
            "<rdf:RDF>\n" +
            "<cc:Work\n" +
            "rdf:about=\"\">\n" +
            "<dc:format>image/svg+xml</dc:format>\n" +
            "<dc:type\n" +
            "rdf:resource=\"http://purl.org/dc/dcmitype/StillImage\" />\n" +
            "<dc:title>" + metadata.title + "</dc:title>\n" +
            "<dc:date>" + metadata.date + "</dc:date>\n" +
            "<dc:publisher>\n" +
            "<cc:Agent>\n" +
            "<dc:title>" + metadata.publisherAgentTitle + "</dc:title>\n" +
            "</cc:Agent>\n" +
            "</dc:publisher>\n" +
            "<dc:subject>\n" +
            "<rdf:Bag>\n" +
            "<rdf:li></rdf:li>\n" +
            "<rdf:li></rdf:li>\n" +
            "<rdf:li></rdf:li>\n" +
            "<rdf:li></rdf:li>\n" +
            "</rdf:Bag>\n" +
            "</dc:subject>\n" +
            "<dc:creator>\n" +
            "<cc:Agent>\n" +
            "<dc:title>" + metadata.creator + "</dc:title>\n" +
            "</cc:Agent>\n" +
            "</dc:creator>\n" +
            "<cc:license\n" +
            "rdf:resource=\"http://creativecommons.org/publicdomain/zero/1.0/\" />\n" +
            "<dc:description>SVG created automatically by algorithm in C++.</dc:description>\n" +
            "</cc:Work>\n" +
            "<cc:License\n" +
            "rdf:about=\"http://creativecommons.org/publicdomain/zero/1.0/\">\n" +
            "<cc:permits\n" +
            "rdf:resource=\"http://creativecommons.org/ns#Reproduction\" />\n" +
            "<cc:permits\n" +
            "rdf:resource=\"http://creativecommons.org/ns#Distribution\" />\n" +
            "<cc:permits\n" +
            "rdf:resource=\"http://creativecommons.org/ns#DerivativeWorks\" />\n" +
            "</cc:License>\n" +
            "</rdf:RDF>\n" +
            "</metadata>\n" +
            "<!--      Created in C++ algorithm       -->\n" +
            "<!-- Attention: do not modify this code. -->\n" +
            "\n" + xml + "\n" +
            "<!-- Attention: do not modify this code. -->\n" +
            "</svg>"
        };
    }

    // Return full SVG.
    static auto svg(const int &width, const int &height, const std::string &xml) -> std::string
    {
        return svg(width, height, xml, Metadata());
    }
};

}; // namespace Image

}; // namespace stbox

#endif // SMALLTOOLBOX_IMAGE_H
