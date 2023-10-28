#include "../src/smalltoolbox_image.hpp"
#include "../src/smalltoolbox_math.hpp"
#include "../src/smalltoolbox_svg_extra.hpp"

#include <cassert>
#include <iostream>
#include <vector>


// Tests
void color();
void svg();
void interpreter();
void loadTxt();

auto main() -> int
{
    // Sequence
    color();
    svg();
    interpreter();
    loadTxt();

    std::cout << "Image test finished!\n";

    return 0;
}

void color()
{
    using namespace stbox;

    assert(Image::Color::RGBA(1, 0, 1, 0).equal(Image::Color::RGBA(0, 1, 0, 1)) == false);
    assert(Image::Color::int2hex(0) == "00");
    assert(Image::Color::int2hex(10) == "0A");
    assert(Image::Color::int2hex(255) == "FF");

    assert(Image::Color::hex2int("#")   ==   0);
    assert(Image::Color::hex2int("##")  ==   0);
    assert(Image::Color::hex2int("#0#") ==   0);
    assert(Image::Color::hex2int("#0")  ==   0);
    assert(Image::Color::hex2int("0A")  ==  10);
    assert(Image::Color::hex2int("ff")  == 255);
    assert(Image::Color::hex2int("FF")  == 255);

    assert(Image::Color::hex2RGB("#").empty());
    assert(Image::Color::hex2RGB("#1234567").empty());
    assert((Image::Color::hex2RGB("#0102") == std::vector<unsigned char> {1, 2}));
    assert((Image::Color::hex2RGB("#01020304") == std::vector<unsigned char> {1, 2, 3, 4}));
    assert((Image::Color::hex2RGB("#7F7F7F") == std::vector<unsigned char> {127, 127, 127}));        // RGB
    assert((Image::Color::hex2RGB("#7F7F7FFF") == std::vector<unsigned char> {127, 127, 127, 255})); // RGBA
}

void svg()
{
    using namespace stbox;

    std::vector<Math::Base> shapes {
        Math::Line({10, 150},  {500, 150}),
        Math::Triangle({10, 120},  {150, 120}, -100),
        Math::Rectangle({300, 160}, 150, 300),
        Math::Circle({250, 80}, 50),
        Math::Ellipse({400, 80}, 80, 50),
        Math::RegularPolygon({200, 300}, 100, 90, 6),
        Math::IrregularPolygon(Math::Point::sum(Math::RegularPolygon({200, 300}, 100, 90, 6).points(), Math::Point(-100, 200)))
    };

    std::string label = "TriOriginal"; // Reference for the clones.
    Math::Triangle triAngle({-50, 0}, {-25, -50},  {0, 0});  // Original object.
    auto svgOriginal = Image::SVG::polygon(Image::SVG::NormalShape(label, Image::Color::RED, Image::Color::BLUE, 2.0, triAngle.pointsXY()));
    auto svgClone1 = Image::SVG::clone(label, 60, triAngle.first.XY(), Math::Point(180, 180).XY());
    auto svgClone2 = Image::SVG::clone(label, 60, triAngle.first.XY(), Math::Point(180, 380).XY());

    auto svgShapes = Sketch::join(shapes);
    svgShapes = Text::join({svgShapes, svgOriginal, svgClone1, svgClone2}, '\n');

    auto svg = Sketch::svg(600, 600, svgShapes, Sketch::Metadata());
    Sys::IO::save(svg, "Resources/svgOutput1.svg");
}

void interpreter()
{
    using namespace stbox;

    Interpreter interpreter;

    std::vector<std::string> commands {
        "",                     // Empty
        "Text",                 // Ignore : Invalid command!
        "Point",                // Ignore : Invalid command!
        "Points:",              // Ignore : Incomplete!
        "PoInTs:",              // Ignore : Incomplete!
        "Circle:",              // Ignore : Incomplete!
        "Points: {",            // Error  : expected '{ something }'
        "Points: }",            // Error  : expected '{ something }'
        "Points: {}",           // Ignore : expected '{ something }'
        "Points: } {",          // Ignore : expected '{ something }'
        "Points: {} {}",        // Error  : only a '{}' is expected
        "Points: { ( ) }",      // Ignore : expected '{ (x,y) }'
        "Points: { ( }",        // Error  : expected '{ (x,y) }'
        "Points:{(1,1)}",       // Ignore : one point
        "Points:{(1,1)(1,a)}",  // Error  : invalid argument
        "Points:{(1,1)(1,2)}",
        "Points:{(1,1) (1,2) (1,3) }",
        "Points:{(1,1) (1,2)} angle=45 label=Test",
        "Line:{(0,0) (5,5)}",
        "Line: {(0,0)} angle=90 length=20",
        "Line: angle=90 {(0,0)} length=20 label=Line1",
        "Rectangle:{(1,1)} width=100 height=100"
    };

    std::string error;
    for (auto command : commands) {
        auto result = interpreter.svg(command, error);
        if (!result.empty()) {
            Sys::Console::view(command);
            Sys::Console::view(result);
        }
        else {
            Sys::Console::view(error);
        }
    }
}

void loadTxt()
{
    using namespace stbox;

    Interpreter interpreter;

    std::string errors;
    auto svg = interpreter.load("Resources/svg_commands.txt", errors);

    svg = Sketch::svg(100, 100, svg, Sketch::Metadata());
    Sys::IO::save(svg, "Resources/svgOutput2.svg");

    //Sys::Console::view(svg);
    Sys::Console::view(errors);

    std::cout << "Load Text File test finished!\n";
}

