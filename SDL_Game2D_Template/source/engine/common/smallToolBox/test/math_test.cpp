#include "../src/smalltoolbox_math.hpp"

#include <iostream>
#include <cassert>
#include <numbers>

#define MAXNUMBER   std::numeric_limits<float>::max()
#define PI          std::numbers::pi

using Numbers = std::vector<double>;
using Points  = std::vector<stbox::Math::Point>;
using Polygon = stbox::Math::RegularPolygon;
using Strings = std::vector<std::string>;


// Tests
void numbers();
void point();
void line();
void triangle();
void rectangle();
void circle();
void regularPolygons();
void irregularPolygon();

auto main() -> int
{
    // Sequence
    numbers();
    point();
    line();
    triangle();
    rectangle();
    circle();
    regularPolygons();
    irregularPolygon();

    std::cout << "Math test finished!\n";
}

void numbers()
{
    using namespace stbox;

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
    assert(Math::equal(numbers, expected) == true);         // Order is not important.
    assert(Math::equal(inOrder, expected, true) == true);   // Order is important.
}

void point()
{
    using namespace stbox;

    Math::Point p1, p2; // (0, 0)

    assert(p1.equal(p2) == true);
    assert(p1 == Math::Point(0, 0));
    assert(p1 + Math::Point(1, 1) == Math::Point(1,  1));
    assert(p1 - Math::Point(1, 1) == Math::Point(-1, -1));
    assert(p1 * Math::Point(2, 2) == Math::Point(0,  0));
    assert(Math::Point(5, 5) * Math::Point(0.5, 1 / 2)   == Math::Point(2.5, 0));
    assert(Math::Point(5, 5) * Math::Point(0.5, 1 / 2.0) == Math::Point(2.5, 2.5));

    p2 = Math::Point(1, 1);
    p2 *= 3;
    p2 += 2;
    p2 -= 4;
    assert(p2 == Math::Point(1, 1));

    p1 += Math::Point(1, 1);
    assert(p1 == p2);

    p1 = Math::Origin;
    assert(p1 == Math::Point(0, 0));
    assert(p1.angle(Math::Point(0, 1)) == 90);
    assert(p1.distance(Math::Point(1, 0)) == 1.0);

    p1.sum(1, 1);
    assert(p1 == Math::Point(1, 1));

    p1.multiply(0.5, 0.5);
    assert(p1 == Math::Point(0.5, 0.5));

    p1.reset();
    assert(p1 == Math::Origin);

    p1 = Math::Zero;
    assert(p1 == Math::Origin);

    p1 = Math::Origin;
    p1 = p1.position(90, 10);
    assert(p1.round() == Math::Point(0, 10));

    // Compare point vector.
    Math::Point a(1, 1), b(2, 2), c(3, 3);
    assert(Math::equal(Points{a, b}, {b, a}) == true);
    assert(Math::equal({a, b, c}, Points{c, b, a}) == true);
    assert(Math::equal(Points{a + b, c}, {c, c}) == true);
    assert(Math::equal(Points{a, b}, Points{a, c}) == false);

    // Sum
    assert(Math::equal(Math::Point::sum({ {1, 1},  {2, 2} }, 5), { {6, 6},  {7, 7} }) == true);
    assert(Math::equal(Math::Point::sum({ {1, 1}, {2, 2} }, Math::Point(1, 2)), { {2, 3}, {3, 4} }) == true);

    // Sum of distances
    assert(Math::Point::sumDistances({Math::Origin,  {10, 0},  {10, 20},  {-10, 20} }) == 50.0);

    // Sort X-axis
    auto points = Math::Point::sort({Math::Origin,  {-10, 0},  {10, 0},  {-5, -1} });
    Points expected = { {-10, 0},  {-5, -1}, Math::Origin,  {10, 0} };
    assert(Math::equal(points, expected, true) == true);

    // Sort Y-axis
    points = Math::Point::sort({Math::Origin,  {-10, 0},  {10, 0},  {-5, -1} }, false);
    expected = { {-5, -1},  {-10, 0}, Math::Origin, {10, 0} };
    assert(Math::equal(points, expected, true) == true);

    // Total
    assert(Math::Point::total({ {1, 1},  {1, 1},  {1, 1} }) == Math::Point(3, 3));

    // Average
    bool status;
    Math::Point result;
    std::tie(result, status) = Math::Point::average({});
    assert(status == false);
    assert(result == Math::Point(0, 0));
    std::tie(result, status) = Math::Point::average({ {1, 3},  {2, 1},  {3, 2} });
    assert(status == true);
    assert(result == Math::Point(2, 2));

    // Organize
    assert(Math::Point::organize({}).empty());
    assert(Math::equal(Math::Point::organize({Math::Origin}), {Math::Point(0, 0)}) == true);
    assert(Math::equal(Math::Point::organize({ {-1, -1}, {-1, 1}, {1, -1}, {1, 1} }),
    { {1, 1}, {-1, -1}, {1, -1}, {-1, 1} }) == true);
    assert(Math::equal(Math::Point::organize({ {-1, -1}, {-1, 1}, {1, -1}, {1, 1} }),
    { {1, 1}, {-1, 1}, {-1, -1}, {1, -1} }, true) == true);

    // Angle between three Points
    assert(Math::Point::angle(Math::Origin, Math::Point(1, 1), Math::Point(0, 1))       == 45);
    assert(Math::Point::angle(Math::Point(1, 1), Math::Point(2, 2), Math::Point(1, 2))  == 45);
    assert(Math::Point::angle(Math::Origin, Math::Point(1, 1), Math::Point(-1,  1))     == 90);
    assert(Math::Point::angle(Math::Origin, Math::Point(1, 1), Math::Point(-1, -1))     == 180);
    assert(Math::Point::angle(Math::Origin, Math::Point(1, 1), Math::Point(0, -1))      == 180 + 45);
    assert(Math::Point::angle(Math::Origin, Math::Point(1, 1), Math::Point(1, -1))      == 270);
    assert(Math::Point::angle(Math::Origin, Math::Point(1, 0), Math::Point(0, 1))       == 90);
    assert(Math::Point::angle(Math::Origin, Math::Point(1, 0), Math::Point(0, 1), true) == -90);
    assert(Math::Point::angle(Math::Origin, Math::Origin, Math::Origin)                 == 0);
    assert(Math::Point::angle(Math::Origin, Math::Origin, Math::Point(1, 1))            == 45);
}

void line()
{
    using namespace stbox;

    Math::Line line;

    assert(line.first == Math::Origin);
    assert(line.second.equal(Math::Origin) == true);

    line.setup({1, 1},  {2, 2});
    assert(line.first == Math::Point(1, 1));
    assert(line.second.equal(Math::Point(2, 2)) == true);

    line.setup(Math::Origin,  {1, 0});
    assert(line.angle() == 0.0);
    assert(line.length() == 1.0);
    assert(line.middle() == Math::Point(0.5, 0));

    line.setup(Math::Origin,  {1, 1});
    assert(line.angle() == 45);
    assert(line.length() == std::sqrt(2));
    assert(line.middle().round().equal(Math::Point(0.5, 0.5)) == true);

    line.setup(Math::Origin, 90, 10);
    assert(line.second.round() == Math::Point(0, 10));

    Math::Line line1;
    line.setup({2, 2},  {2, 10});   // Line (2,2)(2,10)
    line1.setup(0, 4, 10, 4);       // Line (0,4)(10,4)

    // tuple[Point, int]
    auto [result, status] = line.intersection(line1);
    assert(status != 0);            // Parallel or not intersect
    assert(status != 1);            // Coincident
    assert(status == 2);            // Intersect in range
    assert(status != 3);            // Intersect not in range
    assert(result == Math::Point(2, 4));  // Intersection Point(2,4)

    Math::Point a(1, 1), b(2, 2), c(3, 3);
    assert(Math::Line(a, b).equal(Math::Line(a, b)) == true);       // Equal
    assert(Math::Line(b, a).equal(Math::Line(a, b)) == true);       // Equal
    assert(Math::Line(a, b).equal(Math::Line(b, a)) == true);       // Equal
    assert(Math::Line(a, a).equal(Math::Line(a, a)) == true);       // Equal
    assert(Math::Line(a + b, a).equal(Math::Line(c, a)) == true);   // Equal
    assert(Math::Line(a, b).equal(Math::Line(c, b)) == false);      // Not equal

    assert(Math::Line(0, 0, 0, 10).length() == 10.0);

    assert(Math::Line({-1, 0}, {10, 0}).perpendicular({0, 10}).round()
           .equal(Math::Line({0, 0},  {0, 10}).round()) == true);
    assert(Math::Line({0, 1}, {0, 10}).perpendicular({5, 5}).round()
           .equal(Math::Line({0, 5},  {5, 5}).round()) == true);

    // Angle : line direction
    assert(Math::Line(0, 0, 1, 1).angle() == 45);
    assert(Math::Line(1, 1, 0, 0).angle() == 180 + 45);
    assert(Math::Line(0, 0, 1, 1).angle() != Math::Line(1, 1, 0, 0).angle());

    // Intersection
    std::tie(result, status) = Math::Point::lineIntersect(1, 2, 5, 5, 2, 1, 6, 4);
    assert(status == 0); // Parallel
    assert(result == Math::Point(MAXNUMBER, MAXNUMBER));
    std::tie(result, status) = Math::Point::lineIntersect(0, 0, 5, 5, 1, 1, 4, 4);
    assert(status == 1); // Coincident
    assert(result == Math::Point(0, 0));
    std::tie(result, status) = Math::Point::lineIntersect(1, 1, -1, -1, 2, -2, -2, 2);
    assert(status == 2); // Intersection at Origin
    assert(result == Math::Point(0, 0));
    std::tie(result, status) = Math::Point::lineIntersect(2, 2, 2, 10, 0, 4, 10, 4);
    assert(status == 2); // Intersection at (2,4)
    assert(result == Math::Point(2, 4));
    std::tie(result, status) = Math::Point::lineIntersect({2, 2}, {2, 10}, {0, 4}, {10, 4});
    assert(status == 2); // Intersection at (2,4)
    assert(result == Math::Point(2, 4));
    std::tie(result, status) = Math::Point::lineIntersect(Math::Origin, {1, 1}, {2, 0}, {1.5, 1});
    assert(status < 0); // Not intersect
    assert(result == Math::Point(MAXNUMBER, MAXNUMBER));

    line = Math::Line(0, 0, 1, 1);
    std::tie(result, status) = line.intersection(Math::Line(1, 0, 0, 1));
    assert(status == 2);
    std::tie(result, status) = Math::Line::lineIntersect(Math::Line(0, 0, 1, 1), Math::Line(1, 0, 0, 1));
    assert(status == 2);
    assert(result == Math::Point(0.5, 0.5));
    std::tie(result, status) = Math::Line::lineIntersect(Math::Line(0, 0, 1, 1), Math::Line(0, 1, 1, 2));
    assert(status  == 0);
    std::tie(result, status) = Math::Line::lineIntersect(Math::Line(0, 0, 1, 1), Math::Line(1, 1, 0, 0));
    assert(status  == 1);

    // Perpendicular
    assert(Math::Line::perpendicular(Math::Line(-1, 0, 1, 0), Math::Point(0.5, 2)).round()
           .equal(Math::Line(0.5, 0, 0.5, 2)) == true); // Between the two points.
    assert(Math::Line::perpendicular(Math::Line(-1, 0, 1, 0), Math::Point(2.5, 2)).round()
           .equal(Math::Line(2.5, 0, 2.5, 2)) == true); // Out of range.
    assert(Math::Line::perpendicular(Math::Line(-1, 0, 1, 0), Math::Point(-1.5, 2)).round()
           .equal(Math::Line(-0.5, 0, -1.5, 2)) == true); // Out of range.

    // Contains
    assert(line.contains(Math::Origin) == true); // Line(0, 0, 1, 1);
    assert(line.contains({1, 1}) == true);
    assert(line.contains({0.5, 0.5}) == true);
    assert(line.contains({0.3, 0.2}) == false);
    assert(line.contains({0.1, 0.3}) == false);
}

void triangle()
{
    using namespace stbox;

    Math::Triangle triangle;

    assert(triangle.first == Math::Origin);
    assert(triangle.second == Math::Origin);
    assert(triangle.third == Math::Origin);

    triangle.setup(Math::Point(-1, 0), Math::Point(1, 0), 10);
    assert(triangle.third.round() == Math::Point(0, 10));

    Math::Line side;
    side.setup(Math::Point(-1, 0), 0, 2);
    triangle.setup(side, 10);
    assert(triangle.third.round() == Math::Point(0, 10));

    // Base 2, height 10 : Area (2 * 10 / 2)
    triangle.setup({-2, 0},  {0, 0},  {0, 10});
    assert(triangle.area() == (2 * 10 / 2.0));
    assert(triangle.height() == 10.0);

    triangle.setup(Math::Origin,  {10, 0},  {0, 10});
    assert(triangle.perimeter() == 10 + 10 + std::sqrt(200));

    Math::Point a(1, 1), b(2, 2), c(3, 3), d(4, 4);
    assert(Math::Triangle(a, b, c).equal(Math::Triangle(b, a, c)) == true);     // Equal
    assert(Math::Triangle(a, b, a + b).equal(Math::Triangle(c, b, a)) == true); // Equal
    assert(Math::Triangle(a, b, c).equal(Math::Triangle(a, b, d)) == false);    // Not Equal

    // Area, height
    assert(Math::Point::triangleArea(Math::Origin, {20, 0}, {0, 30}) == 300.0);
    assert(Math::Point::triangleHeight(Math::Origin, {20, 0}, {0, 30}) == 30.0);

    // Contains
    assert(triangle.contains(Math::Origin) == true);      // Origin, {10, 0}, {0, 10}
    assert(triangle.contains({10,  0}) == true);
    assert(triangle.contains({ 0, 10}) == true);
    assert(triangle.contains({ 5,  5}) == true);
    assert(triangle.contains({-1,  5}) == false);
    assert(triangle.contains({ 6,  5}) == false);
    assert(triangle.contains({ 5, -1}) == false);
}

void rectangle()
{
    using namespace stbox;

    Math::Rectangle rectangle;

    assert(rectangle.first == Math::Origin);
    assert(rectangle.second == Math::Origin);
    assert(rectangle.third == Math::Origin);
    assert(rectangle.fourth == Math::Origin);

    rectangle.setup(Math::Point(0, 0), 10, 10);
    assert(rectangle.second.round() == Math::Point(10, 0));
    assert(rectangle.third.round() == Math::Point(10, 10));
    assert(rectangle.fourth.round() == Math::Point(0, 10));
    assert(rectangle.area() == 100);
    assert(rectangle.perimeter() == 40);
    assert(rectangle.width() == 10);
    assert(rectangle.height() == 10);

    rectangle.setup(Math::Point(0, 0), 20, 10);
    assert(rectangle.width() == 20);
    assert(rectangle.height() == 10);

    Math::Point a(1, 1), b(5, 1), c(5, 5), d(1, 5), e(5, 5);
    assert(Math::Rectangle(a, b, c, d).equal(Math::Rectangle(d, a, b, c)) == true);             // Equal
    assert(Math::Rectangle(a, b, c, e).equal(Math::Rectangle(a, b, c, a + a + c)) == true);     // Equal
    assert(Math::Rectangle(a, b, c, e).equal(Math::Rectangle(a, b, c, a * 4.0 + 1.0)) == true); // Equal
    assert(Math::Rectangle(a, b, c, d).round().area() == Math::Rectangle(d, a, b, c).round().area());
    assert(Math::Rectangle(a, b, c, d).round().perimeter() == Math::Rectangle(d, a, b, c).round().perimeter());

    rectangle.setup(Math::Origin, {0, 10}, {10, 10}, {0, 10});
    assert(Math::equal(rectangle.intersect(Math::Rectangle({5, 5}, {15, 5}, {15, 15}, {5, 15}).points()), {{5, 10}, {10, 5}}) == true);
    assert(Math::equal(rectangle.intersect(Math::Rectangle({5, 5}, {10, 5}, {10, 15}, {5, 15}).points()),
    {{5, 10}, {10, 5}, {10, 10}, {10, 10}}) == true);   // Point(10, 10) is the intersection of two lines.
}

void circle()
{
    using namespace stbox;

    Math::Ellipse ellipse(Math::Origin, 8, 2);
    Math::Circle circle(Math::Origin, 4);
    assert(circle.area() == ellipse.area());
    assert(circle.horizontalRadius != ellipse.horizontalRadius);
    assert(circle.verticalRadius != ellipse.verticalRadius);

    assert(Math::round(Math::Ellipse(Math::Origin, 10, 10).perimeter(), 3) == 62.832); // Circle
    assert(Math::round(Math::Ellipse(Math::Origin, 10,  5).perimeter(), 3) == 48.442);
    assert(Math::round(Math::Ellipse(Math::Origin, 10,  3).perimeter(), 3) == 43.859);
    assert(Math::round(Math::Ellipse(Math::Origin, 10,  1).perimeter(), 3) == 40.639);

    // Contains
    assert(ellipse.contains(Math::Origin) == true);
    assert(ellipse.contains(Math::Origin + Math::Point(8, 0)) == true);
    assert(ellipse.contains(Math::Origin + Math::Point(0, 2)) == true);
    assert(ellipse.contains(Math::Origin + Math::Point(-8, 0)) == true);
    assert(ellipse.contains(Math::Origin + Math::Point(0, -2)) == true);
    assert(ellipse.contains(Math::Point(8.1, 0)) == false);
    assert(ellipse.contains(Math::Point(0, 2.1)) == false);

    assert(circle.contains(Math::Point(4, 0)) == true);
    assert(circle.contains(Math::Point(0, 4)) == true);
    assert(circle.contains(Math::Point(-4, 0)) == true);
    assert(circle.contains(Math::Point(0, -4)) == true);
    assert(circle.contains(Math::Point(5, 1)) == false);
    assert(circle.contains(Math::Point(4.001, 0)) == false);
}

void regularPolygons()
{
    using namespace stbox;

    Math::RegularPolygon polygon;
    polygon.setup(Math::Point(0, 0), -1,  0,  0); // Return empty
    assert(polygon.points().empty() == true);
    polygon.setup(Math::Point(0, 0),  0,  0, -1); // Return empty
    assert(polygon.points().empty() == true);
    polygon.setup(Math::Point(1, 1),  0,  0,  0); // Return empty
    assert(polygon.points().empty() == true);
    polygon.setup(Math::Point(0, 0),  1,  0,  1); // Return empty
    assert(polygon.points().empty() == true);

    // Rectangle (1,0)(0,1)(-1,0)(0,-1)
    polygon.setup(Math::Point(0, 0),  1,  0,  4);
    assert(round(polygon.sideLength()) == round(Math::Point(1, 0).distance({0, 1})));

    Math::RegularPolygon rPolygon1, rPolygon2;
    rPolygon1.setup(Math::Origin, std::sqrt(2), 0, 4);
    rPolygon2.setup(Math::Origin, std::sqrt(2), 90, 4);
    assert(rPolygon1.round().equal(rPolygon2.round()) == true);   // Equal
    assert(rPolygon1.equal(rPolygon2) == false);                  // Same but different precision.

    // Rectangle 2 x 2 : (1,1)(-1,1)(-1,-1)(1,-1)
    polygon = Math::RegularPolygon(Math::Origin, std::sqrt(2), 45, 4);
    assert(polygon.isConvex() == true);
    assert(round(polygon.area()) == 4.0);
    assert(round(polygon.perimeter()) == 8.0);
    assert(round(polygon.sideLength()) == 2.0);
    polygon.sides = 4;
    assert(polygon.state() == false);   // Unmodified data!
    polygon.sides = 5;
    assert(polygon.state() == true);    // Modified data!

    // Polygon
    polygon = Math::RegularPolygon(Math::Origin, 1, 0, 5);
    assert(polygon.isConvex() == true);

    polygon = Math::RegularPolygon(Math::Origin, 1, 0, 6);
    assert(polygon.isConvex() == true);

    polygon = Math::RegularPolygon(Math::Origin, 1, 0, 360);
    assert(polygon.isConvex() == true);
}

void irregularPolygon()
{
    using namespace stbox;

    Math::IrregularPolygon iPolygon;

    iPolygon.setup({ {0, 0}, {0, 1}, {1, 1} });                // Triangle
    assert(iPolygon.isConvex() == true);
    assert(Math::round(iPolygon.area(), 2) == 0.5);
    assert(iPolygon.perimeter() == 2 + std::sqrt(2));

    iPolygon.setup(Math::Triangle(Math::Origin, {0, 1}, {1, 1}).points()); // Triangle
    assert(iPolygon.isConvex() == true);
    assert(Math::round(iPolygon.area(), 2) == 0.5);
    assert(iPolygon.perimeter() == 2 + std::sqrt(2));

    iPolygon.setup({Math::Origin, {1, 0}, {1, 1}, {0, 1} });         // Rectangle
    assert(iPolygon.isConvex() == true);
    assert(round(iPolygon.area()) == 1.0);
    assert(round(iPolygon.perimeter()) == 4.0);

    iPolygon.setup(Math::Rectangle(Math::Origin, 50, 20).points());        // Rectangle
    assert(iPolygon.isConvex() == true);
    assert(round(iPolygon.area()) == 50 * 20);
    assert(round(iPolygon.perimeter()) == 2 * 50 + 2 * 20);

    // Point type behavior. Returning empty.
    assert(Math::IrregularPolygon({ {1, 1} }).points().empty() == true);

    // Line type behavior.
    assert(Math::IrregularPolygon(Math::Line(Math::Origin, {1, 1}).points()).points().empty() == false);
    assert(Math::IrregularPolygon(Math::Line(Math::Origin, {1, 1}).points()).points().size() == 2);

    // Convex polygon
    iPolygon = Math::IrregularPolygon({ {0, 2}, {-2, 1}, {-1, -1}, {1, -1}, {2, 1} });
    assert(iPolygon.isConvex() == true);
    assert(Math::round(iPolygon.area(), 0) == 8.0);
    assert(Math::round(iPolygon.averageLength(), 1) == 2.2);

    // Concave polygon
    iPolygon = Math::IrregularPolygon({ {0, 2}, {-2, 1}, {1, 0}, {-1, -1}, {1, -1}, {2, 1} });
    assert(iPolygon.isConvex() == false);
    assert(iPolygon.area() == 5.5);

    // Check if polygon contains point.
    iPolygon = Math::IrregularPolygon({ {0, 0}, {10, 0}, {10, 10}, {0, 10} });
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

    iPolygon = Math::IrregularPolygon({ {-10, 0}, {10, 0}, {10, 0}, {0, -10} });
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

    iPolygon = Math::IrregularPolygon({ {-3, -2}, {-1, 4}, {6, 1}, {3, 10}, {-4, 9} });
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
    auto iPolygon1 = Math::IrregularPolygon({ {0, 5}, {-5, 0}, {0, -5}, {5, 0} });
    auto iPolygon2 = Math::IrregularPolygon({ {0, 1}, {-1, 0}, {0, -1}, {1, 0} });
    auto [result, status] = iPolygon1.contains(iPolygon2);
    assert(status == true);
    std::tie(result, status) = iPolygon2.contains(iPolygon1);
    assert(status == false);
}
