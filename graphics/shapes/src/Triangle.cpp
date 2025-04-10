#include "Triangle.h"

#include <algorithm>

triangle_t triangle_t::sortByHeight() const
{

    triangle_t sortedTriangle = *this;
    std::sort(sortedTriangle._points.begin(), sortedTriangle._points.end(),
              [](const auto& point1, const auto& point2) {
                  return point1.y < point2.y; // Sort in descending order by y
              });
    return sortedTriangle;

}

vect2_t<float> triangle_t::getMidPoint() const
{

    // The formula for calculating the x-coordinate of the midpoint of the base of the triangle
    // is derived using the symmetry of the triangle. We are essentially projecting the
    // point where the line from (x0, y0) to (x2, y2) intersects the horizontal line at y = y1.
    //
    // Consider the following triangle diagram:
    //
    //          (x0, y0)
    //            / \
    //           /   \
    //          /     \
    //         /       \
    //        /         \
    //   (x1, y1)------(Mx, My)
    //       \_           \
    //          \_         \
    //             \_       \
    //                \_     \
    //                   \    \
    //                     \_  \
    //                        \_\
    //                           \
    //                         (x2, y2)
    //
    // From the symmetry of the triangle, the midpoint (Mx, My) lies along the line
    // between (x0, y0) and (x2, y2) at the same y-level as (x1, y1). The formula for
    // calculating the x-coordinate of this midpoint is:
    // (Mx - x0) / (x2 - x0) = (y1 - y0) / (y2 - y0)
    //
    // Rearranging for Mx:
    //
    // Mx = x0 + ((x2 - x0) * (y1 - y0)) / (y2 - y0)

    auto [point0, point1, point2] = _points;
    auto midPointX = point0.x + ((point1.y- point0.y)*(point2.x - point0.x) / (point2.y - point0.y) );
    return {midPointX,point1.y};
}



