#include "smallToolBox.h"

#include <cassert>
#include <chrono>
#include <memory>

// Tests
void basic();
void point();
void line();
void triangle();
void rectangle();
void circle();
void regularPolygons();
void irregularPolygon();
void color();

auto main() -> int
{
    std::cout << "Start tests ...\n";
    const auto start = std::chrono::steady_clock::now();

    // Sequence
    basic();
    point();
    line();
    triangle();
    rectangle();
    circle();
    regularPolygons();
    irregularPolygon();
    color();

    const auto end = std::chrono::steady_clock::now();
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "Finished all tests.\nElapsed: " << milliseconds << " miliseconds\n";

    return 0;
}

void basic()
{
    using namespace stbox;

    // Math
    std::vector<unsigned> angles {
        0, 15, 30, 45, 60, 90, 120, 135, 150, 180, 225, 270, 315, 360
    };
    for (auto &a : angles) {
        assert(std::sin(a * std::numbers::pi / 180.0) == Math::sin(0, 1, a));
        assert(std::cos(a * std::numbers::pi / 180.0) == Math::cos(0, 1, a));
    }

    assert(Math::radians(360) == 2 * std::numbers::pi);
    assert(Math::angle(2 * std::numbers::pi) == 360);

    assert(Math::angle(0.0, 0.0,  0.0,  0.0) == 0);
    assert(Math::angle(0.0, 0.0,  1.0,  0.0) == 0);
    assert(Math::angle(0.0, 0.0,  0.0,  1.0) == 90);
    assert(Math::angle(0.0, 0.0, -1.0,  0.0) == 180);
    assert(Math::angle(0.0, 0.0,  0.0, -1.0) == 270);
    assert(Math::angle(0.0, 0.0,  1.0,  1.0) == 45);
    assert(Math::angle(0.0, 0.0, -1.0,  1.0) == 90 + 45);
    assert(Math::angle(0.0, 0.0, -1.0, -1.0) == 180 + 45);
    assert(Math::angle(0.0, 0.0,  1.0, -1.0) == 270 + 45);
    assert(Math::angle(1.0, 1.0,  0.0,  0.0) == 180 + 45);

    assert(Math::cos(5, 10,  0) == (5 + 10 * 1));
    assert(Math::sin(5, 10,  0) == (5 + 10 * 0));
    assert(Math::sin(5, 10, 90) == (5 + 10 * 1));

    assert(Math::distance(0.0, 0.0, 5.0, 0.0) == 5);
    assert(Math::distance(0.0, 0.0, 0.0, 5.0) == 5);
    assert(Math::distance(0.0, 0.0, 5.0, 5.0) == std::sqrt(50));

    assert(round(0.011) == 0.000);
    assert(round(0.011) != 0.011);
    assert(std::round(0.011) != 0.011);
    assert(Math::round(0.0110, 2) == 0.01000000);
    assert(Math::round(0.0190, 2) == 0.02000000);
    assert(Math::round(0.0101, 3) == 0.0100);
    assert(Math::round(0.0109, 3) == 0.0110);

    assert(Math::triangleArea(0, 0, 5, 0, 0, 10) == 25.0);    // Triangle
    assert(Math::triangleHeight(0, 0, 5, 0, 0, 10) == 10.0);  // Triangle

    Numbers numbers {10, -1, 2};
    Numbers expected {-1, 2, 10};
    Numbers inOrder = Math::sort(numbers);
    assert(Check::equal(numbers, expected) == true);         // Order is not important.
    assert(Check::equal(inOrder, expected, true) == true);   // Order is important.

    // Strings
    assert(Text::join(std::vector<std::string> {"Hello", "World!"}, char(32)) == "Hello World!");
    assert(Check::equal(Text::split("Hello World!", char(32)),
                        std::vector<std::string> {"Hello", "World!"}) == true);
    assert(Check::equal(Text::trim(std::vector<std::string> {"100", "001"}, '0'), {"1", "1"}) == true);
    assert(Text::replace("010110", '1', "--") == "0--0----0");
    assert(Text::trim("0.100", '0') == ".1");
    assert(Text::ltrim("0001", '0') == "1");
    assert(Text::rtrim("1000", '0') == "1");

    assert(Text::trimZeros(1000) == "1000.0");
    assert(Text::trimZeros(1000.50) == "1000.50");
    //Console::view(1000.50); // 1000.500000

    assert(Text::strUpper("help") == "HELP");
    assert(Text::strLower("HelP") == "help");

    std::cout << "Basic test finished!\n";
}

void point()
{
    using namespace stbox;

    Point p1, p2; // (0, 0)

    assert(p1.equal(p2) == true);
    assert(p1 == Point(0, 0));
    assert(p1 + Point(1, 1) == Point(1,  1));
    assert(p1 - Point(1, 1) == Point(-1, -1));
    assert(p1 * Point(2, 2) == Point(0,  0));
    assert(Point(5, 5) * Point(0.5, 1 / 2)   == Point(2.5, 0));
    assert(Point(5, 5) * Point(0.5, 1 / 2.0) == Point(2.5, 2.5));

    p2 = Point(1, 1);
    p2 *= 3;
    p2 += 2;
    p2 -= 4;
    assert(p2 == Point(1, 1));

    p1 += Point(1, 1);
    assert(p1 == p2);

    p1 = Origin;
    assert(p1 == Point(0, 0));
    assert(p1.angle(Point(0, 1)) == 90);
    assert(p1.distance(Point(1, 0)) == 1.0);

    p1.sum(1, 1);
    assert(p1 == Point(1, 1));

    p1.multiply(0.5, 0.5);
    assert(p1 == Point(0.5, 0.5));

    p1.reset();
    assert(p1 == Origin);

    p1 = Zero;
    assert(p1 == Origin);

    p1 = Origin;
    p1 = p1.position(90, 10);
    assert(p1.round() == Point(0, 10));

    // Output
    //Console::view(1);
    //Console::view(0.5);
    //Console::view(1.5f);

    //Console::view(p1);
    //Console::view(p2);
    //Console::view(p1.XY());

    // Compare point vector.
    Point a(1, 1), b(2, 2), c(3, 3);
    assert(Check::equal(Points{a, b}, {b, a}) == true);
    assert(Check::equal({a, b, c}, Points{c, b, a}) == true);
    assert(Check::equal(Points{a + b, c}, {c, c}) == true);
    assert(Check::equal(Points{a, b}, Points{a, c}) == false);

    // Sum
    assert(Check::equal(Point::sum({ {1, 1},  {2, 2} }, 5), { {6, 6},  {7, 7} }) == true);
    assert(Check::equal(Point::sum({ {1, 1}, {2, 2} }, Point(1, 2)), { {2, 3}, {3, 4} }) == true);

    // Sum of distances
    assert(Point::sumDistances({Origin,  {10, 0},  {10, 20},  {-10, 20} }) == 50.0);

    // Sort X-axis
    auto points = Point::sort({Origin,  {-10, 0},  {10, 0},  {-5, -1} });
    Points expected = { {-10, 0},  {-5, -1}, Origin,  {10, 0} };
    assert(Check::equal(points, expected, true) == true);
    //Console::view(points);
    //Console::view(expected);

    // Sort Y-axis
    points = Point::sort({Origin,  {-10, 0},  {10, 0},  {-5, -1} }, false);
    expected = { {-5, -1},  {-10, 0}, Origin, {10, 0} };
    assert(Check::equal(points, expected, true) == true);
    //Console::view(points);
    //Console::view(expected);

    // Total
    assert(Point::total({ {1, 1},  {1, 1},  {1, 1} }) == Point(3, 3));

    // Average
    bool status;
    Point result;
    std::tie(result, status) = Point::average({});
    assert(status == false);
    assert(result == Point(0, 0));
    std::tie(result, status) = Point::average({ {1, 3},  {2, 1},  {3, 2} });
    assert(status == true);
    assert(result == Point(2, 2));

    // Organize
    assert(Point::organize({}).empty());
    assert(Check::equal(Point::organize({Origin}), {Point(0, 0)}) == true);
    assert(Check::equal(Point::organize({ {-1, -1}, {-1, 1}, {1, -1}, {1, 1} }),
    { {1, 1}, {-1, -1}, {1, -1}, {-1, 1} }) == true);
    assert(Check::equal(Point::organize({ {-1, -1}, {-1, 1}, {1, -1}, {1, 1} }),
    { {1, 1}, {-1, 1}, {-1, -1}, {1, -1} }, true) == true);

    // Angle between three points
    assert(Point::angle(Origin, Point(1, 1), Point(0, 1))       == 45);
    assert(Point::angle(Point(1, 1), Point(2, 2), Point(1, 2))  == 45);
    assert(Point::angle(Origin, Point(1, 1), Point(-1,  1))     == 90);
    assert(Point::angle(Origin, Point(1, 1), Point(-1, -1))     == 180);
    assert(Point::angle(Origin, Point(1, 1), Point(0, -1))      == 180 + 45);
    assert(Point::angle(Origin, Point(1, 1), Point(1, -1))      == 270);
    assert(Point::angle(Origin, Point(1, 0), Point(0, 1))       == 90);
    assert(Point::angle(Origin, Point(1, 0), Point(0, 1), true) == -90);
    assert(Point::angle(Origin, Origin, Origin)                 == 0);
    assert(Point::angle(Origin, Origin, Point(1, 1))            == 45);

    // String
    p1.X.value = 1.101;
    p1.Y.value = 1.202;
    assert(p1.toStr(true) == "1.1010,1.2020");
    //Console::view(p1.toStr());

    std::cout << "Point test finished!\n";
}

void line()
{
    using namespace stbox;

    Line line;

    assert(line.first == Origin);
    assert(line.second.equal(Origin) == true);

    line.setup({1, 1},  {2, 2});
    assert(line.first == Point(1, 1));
    assert(line.second.equal(Point(2, 2)) == true);

    line.setup(Origin,  {1, 0});
    assert(line.angle() == 0.0);
    assert(line.length() == 1.0);
    assert(line.middle() == Point(0.5, 0));

    line.setup(Origin,  {1, 1});
    assert(line.angle() == 45);
    assert(line.length() == std::sqrt(2));
    assert(line.middle().round().equal(Point(0.5, 0.5)) == true);

    line.setup(Origin, 90, 10);
    assert(line.second.round() == Point(0, 10));

    Line line1;
    line.setup({2, 2},  {2, 10});   // Line (2,2)(2,10)
    line1.setup(0, 4, 10, 4);       // Line (0,4)(10,4)
    //Console::view(line.points());
    //Console::view(line1.points());

    // tuple[Point, int]
    auto [result, status] = line.intersection(line1);
    assert(status != 0);            // Parallel or not intersect
    assert(status != 1);            // Coincident
    assert(status == 2);            // Intersect in range
    assert(status != 3);            // Intersect not in range
    assert(result == Point(2, 4));  // Intersection Point(2,4)

    Point a(1, 1), b(2, 2), c(3, 3);
    assert(Line(a, b).equal(Line(a, b)) == true);       // Equal
    assert(Line(b, a).equal(Line(a, b)) == true);       // Equal
    assert(Line(a, b).equal(Line(b, a)) == true);       // Equal
    assert(Line(a, a).equal(Line(a, a)) == true);       // Equal
    assert(Line(a + b, a).equal(Line(c, a)) == true);   // Equal
    assert(Line(a, b).equal(Line(c, b)) == false);      // Not equal

    assert(Line(0, 0, 0, 10).length() == 10.0);

    assert(Line({-1, 0}, {10, 0}).perpendicular({0, 10}).round()
           .equal(Line({0, 0},  {0, 10}).round()) == true);
    assert(Line({0, 1}, {0, 10}).perpendicular({5, 5}).round()
           .equal(Line({0, 5},  {5, 5}).round()) == true);

    // Angle : line direction
    assert(Line(0, 0, 1, 1).angle() == 45);
    assert(Line(1, 1, 0, 0).angle() == 180 + 45);
    assert(Line(0, 0, 1, 1).angle() != Line(1, 1, 0, 0).angle());

    // Intersection
    std::tie(result, status) = Point::lineIntersect(1, 2, 5, 5, 2, 1, 6, 4);
    assert(status == 0); // Parallel
    assert(result == Point(MAXNUMBER, MAXNUMBER));
    std::tie(result, status) = Point::lineIntersect(0, 0, 5, 5, 1, 1, 4, 4);
    assert(status == 1); // Coincident
    assert(result == Point(0, 0));
    std::tie(result, status) = Point::lineIntersect(1, 1, -1, -1, 2, -2, -2, 2);
    assert(status == 2); // Intersection at Origin
    assert(result == Point(0, 0));
    std::tie(result, status) = Point::lineIntersect(2, 2, 2, 10, 0, 4, 10, 4);
    assert(status == 2); // Intersection at (2,4)
    assert(result == Point(2, 4));
    std::tie(result, status) = Point::lineIntersect({2, 2}, {2, 10}, {0, 4}, {10, 4});
    assert(status == 2); // Intersection at (2,4)
    assert(result == Point(2, 4));
    std::tie(result, status) = Point::lineIntersect(Origin, {1, 1}, {2, 0}, {1.5, 1});
    assert(status < 0); // Not intersect
    assert(result == Point(MAXNUMBER, MAXNUMBER));

    line = Line(0, 0, 1, 1);
    std::tie(result, status) = line.intersection(Line(1, 0, 0, 1));
    assert(status == 2);
    std::tie(result, status) = Line::lineIntersect(Line(0, 0, 1, 1), Line(1, 0, 0, 1));
    assert(status == 2);
    assert(result == Point(0.5, 0.5));
    std::tie(result, status) = Line::lineIntersect(Line(0, 0, 1, 1), Line(0, 1, 1, 2));
    assert(status  == 0);
    std::tie(result, status) = Line::lineIntersect(Line(0, 0, 1, 1), Line(1, 1, 0, 0));
    assert(status  == 1);

    // Perpendicular
    assert(Line::perpendicular(Line(-1, 0, 1, 0), Point(0.5, 2)).round()
           .equal(Line(0.5, 0, 0.5, 2)) == true); // Between the two points.
    assert(Line::perpendicular(Line(-1, 0, 1, 0), Point(2.5, 2)).round()
           .equal(Line(2.5, 0, 2.5, 2)) == true); // Out of range.
    assert(Line::perpendicular(Line(-1, 0, 1, 0), Point(-1.5, 2)).round()
           .equal(Line(-0.5, 0, -1.5, 2)) == true); // Out of range.

    // Contains
    assert(line.contains(Origin) == true); // Line(0, 0, 1, 1);
    assert(line.contains({1, 1}) == true);
    assert(line.contains({0.5, 0.5}) == true);
    assert(line.contains({0.3, 0.2}) == false);
    assert(line.contains({0.1, 0.3}) == false);

    std::cout << "Line test finished!\n";
}

void triangle()
{
    using namespace stbox;

    Triangle triangle;

    assert(triangle.first == Origin);
    assert(triangle.second == Origin);
    assert(triangle.third == Origin);

    triangle.setup(Point(-1, 0), Point(1, 0), 10);
    assert(triangle.third.round() == Point(0, 10));

    Line side;
    side.setup(Point(-1, 0), 0, 2);
    triangle.setup(side, 10);
    assert(triangle.third.round() == Point(0, 10));

    // Base 2, height 10 : Area (2 * 10 / 2)
    triangle.setup({-2, 0},  {0, 0},  {0, 10});
    assert(triangle.area() == (2 * 10 / 2.0));
    assert(triangle.height() == 10.0);

    triangle.setup(Origin,  {10, 0},  {0, 10});
    assert(triangle.perimeter() == 10 + 10 + std::sqrt(200));

    Point a(1, 1), b(2, 2), c(3, 3), d(4, 4);
    assert(Triangle(a, b, c).equal(Triangle(b, a, c)) == true);     // Equal
    assert(Triangle(a, b, a + b).equal(Triangle(c, b, a)) == true); // Equal
    assert(Triangle(a, b, c).equal(Triangle(a, b, d)) == false);    // Not Equal
    //Console::view(Triangle(a, b, c).points());
    //Console::view(Triangle(a, b, c).lengthOfSides());

    // Area, height
    assert(Point::triangleArea(Origin, {20, 0}, {0, 30}) == 300.0);
    assert(Point::triangleHeight(Origin, {20, 0}, {0, 30}) == 30.0);

    // Contains
    assert(triangle.contains(Origin) == true);      // Origin, {10, 0}, {0, 10}
    assert(triangle.contains({10,  0}) == true);
    assert(triangle.contains({ 0, 10}) == true);
    assert(triangle.contains({ 5,  5}) == true);
    assert(triangle.contains({-1,  5}) == false);
    assert(triangle.contains({ 6,  5}) == false);
    assert(triangle.contains({ 5, -1}) == false);

    std::cout << "Triangle test finished!\n";
}

void rectangle()
{
    using namespace stbox;

    Rectangle rectangle;

    assert(rectangle.first == Origin);
    assert(rectangle.second == Origin);
    assert(rectangle.third == Origin);
    assert(rectangle.fourth == Origin);

    rectangle.setup(Point(0, 0), 10, 10);
    assert(rectangle.second.round() == Point(10, 0));
    assert(rectangle.third.round() == Point(10, 10));
    assert(rectangle.fourth.round() == Point(0, 10));
    assert(rectangle.area() == 100);
    assert(rectangle.perimeter() == 40);
    assert(rectangle.width() == 10);
    assert(rectangle.height() == 10);

    rectangle.setup(Point(0, 0), 20, 10);
    //Console::view(rectangle.points());
    //Console::view(rectangle.lengthOfSides());
    assert(rectangle.width() == 20);
    assert(rectangle.height() == 10);

    Point a(1, 1), b(5, 1), c(5, 5), d(1, 5), e(5, 5);
    assert(Rectangle(a, b, c, d).equal(Rectangle(d, a, b, c)) == true);             // Equal
    assert(Rectangle(a, b, c, e).equal(Rectangle(a, b, c, a + a + c)) == true);     // Equal
    assert(Rectangle(a, b, c, e).equal(Rectangle(a, b, c, a * 4.0 + 1.0)) == true); // Equal
    assert(Rectangle(a, b, c, d).round().area() == Rectangle(d, a, b, c).round().area());
    assert(Rectangle(a, b, c, d).round().perimeter() == Rectangle(d, a, b, c).round().perimeter());
    //Console::view(Rectangle(a, b, c, d).points());
    //Console::view(Rectangle(d, a, b, c).points());

    rectangle.setup(Origin, {0, 10}, {10, 10}, {0, 10});
    assert(Check::equal(rectangle.intersect(Rectangle({5, 5}, {15, 5}, {15, 15}, {5, 15}).points()), {{5, 10}, {10, 5}}) == true);
    assert(Check::equal(rectangle.intersect(Rectangle({5, 5}, {10, 5}, {10, 15}, {5, 15}).points()),
    {{5, 10}, {10, 5}, {10, 10}, {10, 10}}) == true);   // Point(10, 10) is the intersection of two lines.

    std::cout << "Rectangle test finished!\n";
}

void circle()
{
    using namespace stbox;

    Ellipse ellipse(Origin, 8, 2);
    Circle circle(Origin, 4);
    //Console::view(circle.sides);
    //Console::view(ellipse.sides);
    assert(circle.area() == ellipse.area());
    assert(circle.horizontalRadius != ellipse.horizontalRadius);
    assert(circle.verticalRadius != ellipse.verticalRadius);

    assert(Math::round(Ellipse(Origin, 10, 10).perimeter(), 3) == 62.832); // Circle
    assert(Math::round(Ellipse(Origin, 10,  5).perimeter(), 3) == 48.442);
    assert(Math::round(Ellipse(Origin, 10,  3).perimeter(), 3) == 43.859);
    assert(Math::round(Ellipse(Origin, 10,  1).perimeter(), 3) == 40.639);

    // Contains
    assert(ellipse.contains(Origin) == true);
    assert(ellipse.contains(Origin + Point(8, 0)) == true);
    assert(ellipse.contains(Origin + Point(0, 2)) == true);
    assert(ellipse.contains(Origin + Point(-8, 0)) == true);
    assert(ellipse.contains(Origin + Point(0, -2)) == true);
    assert(ellipse.contains(Point(8.1, 0)) == false);
    assert(ellipse.contains(Point(0, 2.1)) == false);

    assert(circle.contains(Point(4, 0)) == true);
    assert(circle.contains(Point(0, 4)) == true);
    assert(circle.contains(Point(-4, 0)) == true);
    assert(circle.contains(Point(0, -4)) == true);
    assert(circle.contains(Point(5, 1)) == false);
    assert(circle.contains(Point(4.001, 0)) == false);

    std::cout << "Circle test finished!\n";
}

void regularPolygons()
{
    using namespace stbox;

    // Polygon
    RegularPolygon polygon;
    polygon.setup(Point(0, 0), -1,  0,  0); // Return empty
    assert(polygon.points().empty() == true);
    polygon.setup(Point(0, 0),  0,  0, -1); // Return empty
    assert(polygon.points().empty() == true);
    polygon.setup(Point(1, 1),  0,  0,  0); // Return empty
    assert(polygon.points().empty() == true);
    polygon.setup(Point(0, 0),  1,  0,  1); // Return empty
    assert(polygon.points().empty() == true);

    //Console::view(polygon.setup(Point(0, 0),  1,    0,  3)); // Triangle
    //Console::view(polygon.setup(Point(0, 0),  1,   45,  3)); // Triangle
    //Console::view(polygon.setup(Point(0, 0),  1,   90,  3)); // Triangle
    //Console::view(polygon.setup(Point(0, 0),  1,  180,  3)); // Triangle
    //Console::view(polygon.setup(Point(0, 0),  1,  270,  3)); // Triangle
    //Console::view(polygon.setup(Point(0, 0),  1,    0,  4)); // Rectangle
    //Console::view(polygon.setup(Point(0, 0),  2,   45,  4)); // Rectangle
    //Console::view(polygon.setup(Point(0, 0),  1,    0,  5)); // Pentagon
    //Console::view(polygon.setup(Point(0, 0),  1,   45,  5)); // Pentagon

    // Rectangle (1,0)(0,1)(-1,0)(0,-1)
    polygon.setup(Point(0, 0),  1,  0,  4);
    assert(round(polygon.sideLength()) == round(Point(1, 0).distance({0, 1})));

    RegularPolygon rPolygon1, rPolygon2;
    rPolygon1.setup(Origin, std::sqrt(2), 0, 4);
    rPolygon2.setup(Origin, std::sqrt(2), 90, 4);
    //Console::view(rPolygon1.points());
    //Console::view(rPolygon1.round().points());
    //Console::view(rPolygon2.points());
    //Console::view(rPolygon2.round().points());
    assert(rPolygon1.round().equal(rPolygon2.round()) == true);   // Equal
    assert(rPolygon1.equal(rPolygon2) == false);                  // Same but different precision.

    // Rectangle 2 x 2 : (1,1)(-1,1)(-1,-1)(1,-1)
    polygon = RegularPolygon(Origin, std::sqrt(2), 45, 4);
    //Console::view(polygon.points());
    assert(polygon.isConvex() == true);
    assert(round(polygon.area()) == 4.0);
    assert(round(polygon.perimeter()) == 8.0);
    assert(round(polygon.sideLength()) == 2.0);
    polygon.sides = 4;
    assert(polygon.state() == false);   // Unmodified data!
    polygon.sides = 5;
    assert(polygon.state() == true);    // Modified data!

    // Polygon
    polygon = RegularPolygon(Origin, 1, 0, 5);
    assert(polygon.isConvex() == true);
    //Console::view(polygon.averageLength());
    //Console::view(polygon.sideLength());
    //Console::view(polygon.lengthOfSides());

    polygon = RegularPolygon(Origin, 1, 0, 6);
    assert(polygon.isConvex() == true);

    polygon = RegularPolygon(Origin, 1, 0, 360);
    assert(polygon.isConvex() == true);

    std::cout << "Polygon test finished!\n";
}

void irregularPolygon()
{
    using namespace stbox;

    IrregularPolygon iPolygon;

    iPolygon.setup({ {0, 0}, {0, 1}, {1, 1} });                // Triangle
    assert(iPolygon.isConvex() == true);
    assert(Math::round(iPolygon.area(), 2) == 0.5);
    assert(iPolygon.perimeter() == 2 + std::sqrt(2));

    iPolygon.setup(Triangle(Origin, {0, 1}, {1, 1}).points()); // Triangle
    assert(iPolygon.isConvex() == true);
    assert(Math::round(iPolygon.area(), 2) == 0.5);
    assert(iPolygon.perimeter() == 2 + std::sqrt(2));

    iPolygon.setup({Origin, {1, 0}, {1, 1}, {0, 1} });         // Rectangle
    assert(iPolygon.isConvex() == true);
    assert(round(iPolygon.area()) == 1.0);
    assert(round(iPolygon.perimeter()) == 4.0);

    iPolygon.setup(Rectangle(Origin, 50, 20).points());        // Rectangle
    assert(iPolygon.isConvex() == true);
    assert(round(iPolygon.area()) == 50 * 20);
    assert(round(iPolygon.perimeter()) == 2 * 50 + 2 * 20);

    // Point type behavior. Returning empty.
    assert(IrregularPolygon({ {1, 1} }).points().empty() == true);

    // Line type behavior.
    assert(IrregularPolygon(Line(Origin, {1, 1}).points()).points().empty() == false);
    assert(IrregularPolygon(Line(Origin, {1, 1}).points()).points().size() == 2);

    // Convex polygon
    iPolygon = IrregularPolygon({ {0, 2}, {-2, 1}, {-1, -1}, {1, -1}, {2, 1} });
    assert(iPolygon.isConvex() == true);
    assert(Math::round(iPolygon.area(), 0) == 8.0);
    assert(Math::round(iPolygon.averageLength(), 1) == 2.2);

    // Concave polygon
    iPolygon = IrregularPolygon({ {0, 2}, {-2, 1}, {1, 0}, {-1, -1}, {1, -1}, {2, 1} });
    assert(iPolygon.isConvex() == false);
    assert(iPolygon.area() == 5.5);

    // Check if polygon contains point.
    iPolygon = IrregularPolygon({ {0, 0}, {10, 0}, {10, 10}, {0, 10} });
    //Console::view(iPolygon);
    assert(iPolygon.isConvex() == true);
    assert(iPolygon.area() == 100.0);                   // 100
    assert(iPolygon.contains({ 0,  0}) == true);
    assert(iPolygon.contains({10,  0}) == true);
    assert(iPolygon.contains({10, 10}) == true);
    assert(iPolygon.contains({ 0, 10}) == true);
    assert(iPolygon.contains({ 5,  5}) == true);
    assert(iPolygon.contains({ 0,  5}) == true);
    assert(iPolygon.contains({ 5,  0}) == true);
    assert(iPolygon.contains({ 5, 10}) == true);
    assert(iPolygon.contains({10,  5}) == true);
    assert(iPolygon.contains({-1,  5}) == false);
    assert(iPolygon.contains({ 5, -1}) == false);
    assert(iPolygon.contains({11,  5}) == false);
    assert(iPolygon.contains({ 5, 11}) == false);

    iPolygon = IrregularPolygon({ {-10, 0}, {10, 0}, {10, 0}, {0, -10} });
    //Console::view(iPolygon);
    assert(iPolygon.isConvex() == true);
    assert(Math::round(iPolygon.area(), 2) == 100.0);   // 100.00000000000001
    assert(iPolygon.contains({  6,   5}) == false);
    assert(iPolygon.contains({  6,  -5}) == false);
    assert(iPolygon.contains({ 11,   0}) == false);
    assert(iPolygon.contains({ 11,  10}) == false);
    assert(iPolygon.contains({ 11, -10}) == false);
    assert(iPolygon.contains({ -6,   5}) == false);
    assert(iPolygon.contains({ -6,  -5}) == false);
    assert(iPolygon.contains({-11,   0}) == false);
    assert(iPolygon.contains({-11,  10}) == false);
    assert(iPolygon.contains({-11, -10}) == false);

    iPolygon = IrregularPolygon({ {-3, -2}, {-1, 4}, {6, 1}, {3, 10}, {-4, 9} });
    //Console::view(iPolygon);
    assert(iPolygon.isConvex() == false);
    assert(iPolygon.area() == 60.0);
    assert(iPolygon.contains({ 0, 5}) == true);
    assert(iPolygon.contains({ 5, 3}) == true);
    assert(iPolygon.contains({-3, 5}) == true);
    assert(iPolygon.contains({-2, 2}) == true);
    assert(iPolygon.contains({-3, 2}) == true);
    assert(iPolygon.contains({ 0, 3}) == false);
    assert(iPolygon.contains({ 2, 2}) == false);
    assert(iPolygon.contains({ 5, 5}) == false);
    assert(iPolygon.contains({-4, 2}) == false);
    assert(iPolygon.contains({-4, 5}) == false);
    assert(iPolygon.contains({10, 0}) == false);

    // Check if polygon contains points.
    auto iPolygon1 = IrregularPolygon({ {0, 5}, {-5, 0}, {0, -5}, {5, 0} });
    auto iPolygon2 = IrregularPolygon({ {0, 1}, {-1, 0}, {0, -1}, {1, 0} });
    auto [result, status] = iPolygon1.contains(iPolygon2);
    assert(status == true);
    std::tie(result, status) = iPolygon2.contains(iPolygon1);
    assert(status == false);

    std::cout << "Irregular Polygon test finished!\n";
}

void color()
{
    using namespace stbox;

    assert(Color::RGBA(1, 0, 1, 0).equal(Color::RGBA(0, 1, 0, 1)) == false);

    //Console::view(Color::RGBA(255, 255, 0, 0).toStr());
    //Console::view(Color::RGBA(255, 255, 0, 0).toStr(false));
    //Console::view(Color::RGBA(std::vector<unsigned char> {1}).toStr(true));
    //Console::view(Color::RGBA(std::vector<unsigned char> {1, 2}).toStr(true));
    //Console::view(Color::RGBA(std::vector<unsigned char> {1, 2, 3}).toStr(true));
    //Console::view(Color::RGBA(std::vector<unsigned char> {1, 2, 3, 4}).toStr(true));

    assert(Color::int2hex(0) == "00");
    assert(Color::int2hex(10) == "0A");
    assert(Color::int2hex(255) == "FF");

    assert(Color::hex2int("#")   ==   0);
    assert(Color::hex2int("##")  ==   0);
    assert(Color::hex2int("#0#") ==   0);
    assert(Color::hex2int("#0")  ==   0);
    assert(Color::hex2int("0A")  ==  10);
    assert(Color::hex2int("ff")  == 255);
    assert(Color::hex2int("FF")  == 255);

    assert(Color::hex2RGB("#").empty());
    assert(Color::hex2RGB("#1234567").empty());
    assert(Check::equal(Color::hex2RGB("#0102"), {1, 2}));
    assert(Check::equal(Color::hex2RGB("#01020304"), {1, 2, 3, 4}));
    assert(Check::equal(Color::hex2RGB("#7F7F7F"), {127, 127, 127}));        // RGB
    assert(Check::equal(Color::hex2RGB("#7F7F7FFF"), {127, 127, 127, 255})); // RGBA
    //Console::view(Color::hex2RGB("#7F7F7FFF"));
    //Console::view(Color::RGBA(Color::hex2RGB("#7F7F7FFF")).toStr());

    std::cout << "Color test finished!\n";
}
