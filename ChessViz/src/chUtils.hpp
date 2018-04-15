// chUtils.hpp
// Callum Howard, 2017

#ifndef CHUTILS_H
#define CHUTILS_H

#include <cmath>                // fabs
#include "cinder/gl/gl.h"
#include "cinder/CinderMath.h"
#include "cinder/Rand.h"
#include "Cinder/app/App.h"

namespace ch {
using namespace ci;
using namespace ci::app;


enum Orientation {
    COLINEAR,
    CLOCKWISE,
    COUNTERCLOCKWISE
};


// prototypes
void setMagnitude(vec2& v, float len);
vec2 safeNormalize(const vec2& v);
void limit(vec2& v, float maxLength);
bool intersects(const vec2& p1, const vec2& q1, const vec2& p2, const vec2& q2);

inline vec2 midpoint(const vec2& first, const vec2& second) {
    return (second - first) * 0.5f;
};

// for intersects: check if point q lies on line segment 'pr'
inline bool onSegment(const vec2& p, const vec2& q, const vec2& r) {
    return q.x <= fmax(p.x, r.x) and q.x >= fmin(p.x, r.x) and
            q.y <= fmax(p.y, r.y) and q.y >= fmin(p.y, r.y);
}

// for intersects: To find orientation of ordered triplet (p, q, r)
inline Orientation orientation(const vec2& p, const vec2& q, const vec2& r) {
    // See http://www.geeksforgeeks.org/orientation-3-ordered-points/
    // for details of below formula.
    const int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (val == 0) { return COLINEAR; }
    return (val > 0) ? CLOCKWISE : COUNTERCLOCKWISE;
}


template<typename T>
inline bool between(T a, T lower, T upper) { return a >= lower and a <= upper; }

inline float lengthSquared(const vec2& v) { return v.x * v.x + v.y * v.y; }
inline float length(const vec2& v) { return glm::sqrt(lengthSquared(v)); }
inline float distance(const vec2& a, const vec2& b) { return length(a - b); }
inline float heading(const vec2& v) { return atan2(v.y, v.x); }

vec2 hOrV(const vec2& v) {
    return (std::fabs(v.x) > std::fabs(v.y)) ? vec2{v.x, 0} : vec2{0, v.y};
}

vec2 safeNormalize(const vec2& v) {
    const auto s = lengthSquared(v);
    return (s <= 0) ? vec2{} : v * glm::inversesqrt(s);
}

void limit(vec2& v, float maxLength) {
    const auto ls = lengthSquared(v);
    if ((ls > maxLength * maxLength) and (ls > 0)) {
        v *= maxLength / glm::sqrt(ls);
    }
}

void setMagnitude(vec2& v, float len) { v = safeNormalize(v) * len; }

inline vec2 makeRandPoint() {
    return vec2{randFloat(getWindowWidth()), randFloat(getWindowHeight())};
}

inline vec2 addNoise(const vec2& p, float noise) {
    return p + vec2{randFloat(-noise, noise), randFloat(-noise, noise)};
}

// larger bias (<1) favours smaller values
inline int biasRandInt(int min, int max, float bias) {
    return min + ((max - min) * (1 - (1.0f / bias) * pow(randFloat(0.0f, 1.0f), bias)));
}

inline vec2 normal(const vec2& a, const vec2& b) {
    return vec2{b.y - a.y, -b.x + a.x};
}

// returns true if line segment 'p1q1' and 'p2q2' intersect
bool intersects(const vec2& p1, const vec2& q1, const vec2& p2, const vec2& q2) {
    // Find the 4 orientations needed for general and special cases
    Orientation o1 = orientation(p1, q1, p2);
    Orientation o2 = orientation(p1, q1, q2);
    Orientation o3 = orientation(p2, q2, p1);
    Orientation o4 = orientation(p2, q2, q1);

    // General case
    if (o1 != o2 and o3 != o4) { return true; };

    // Special Cases
    // p1, q1 and p2 are colinear and p2 lies on segment p1q1
    if (o1 == COLINEAR and onSegment(p1, p2, q1)) { return true; }

    // p1, q1 and p2 are colinear and q2 lies on segment p1q1
    if (o2 == COLINEAR and onSegment(p1, q2, q1)) { return true; }

    // p2, q2 and p1 are colinear and p1 lies on segment p2q2
    if (o3 == COLINEAR and onSegment(p2, p1, q2)) { return true; }

    // p2, q2 and q1 are colinear and q1 lies on segment p2q2
    if (o4 == COLINEAR and onSegment(p2, q1, q2)) { return true; }

    return false;  // Doesn't fall in any of the above cases
}

vec2 getIntersection(const vec2& a, const vec2& b, const vec2& c, const vec2& d) {
    // http://www.geeksforgeeks.org/program-for-point-of-intersection-of-two-lines/
    // assumes that lines are intersecting

    // Line AB represented as a1x + b1y = c1
    const auto a1 = b.y - a.y;
    const auto b1 = a.x - b.x;
    const auto c1 = a1 * (a.x) + b1 * (a.y);

    // Line CD represented as a2x + b2y = c2
    const auto a2 = d.y - c.y;
    const auto b2 = c.x - d.x;
    const auto c2 = a2 * (c.x) + b2 * (c.y);

    const auto determinant = a1 * b2 - a2 * b1;
    const auto x = (b2 * c1 - b1 * c2) / determinant;
    const auto y = (a1 * c2 - a2 * c1) / determinant;

    return vec2{x, y};
}

template <typename T>
bool within(T target, T lower, T upper) {
    return lower <= target and target < upper;
}

}

#endif /* chUtils_h */
