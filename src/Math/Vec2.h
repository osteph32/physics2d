// Vec2.h

#pragma once
#include <cmath>

// 2D vector class
struct Vec2 {
    float x = 0.f, y = 0.f;

    constexpr Vec2() = default;
    constexpr Vec2(float x, float y) : x(x), y(y) {}

    // Vector Arithmetic
    constexpr Vec2 operator+(const Vec2& o) const { return {x+o.x, y+o.y}; }
    constexpr Vec2 operator-(const Vec2& o) const { return {x-o.x, y-o.y}; }
    constexpr Vec2 operator*(float s) const { return {x*s, y*s}; }
    constexpr Vec2 operator/(float s) const { return {x/s, y/s}; }
    constexpr Vec2 operator-() const { return {-x, -y}; }

    Vec2& operator+=(const Vec2& o) { x+=o.x; y+=o.y; return *this; }
    Vec2& operator-=(const Vec2& o) { x-=o.x; y-=o.y; return *this; }
    Vec2& operator*=(float s) { x*=s;   y*=s;   return *this; }

    // Products
    //Dot product
    constexpr float Dot  (const Vec2& o) const { return x*o.x + y*o.y; }
    
    // Cross product (2D scalar)
    constexpr float Cross(const Vec2& o) const { return x*o.y - y*o.x; }

    // Cross with scalar
    constexpr Vec2 Cross(float s) const { return { s*y, -s*x }; }

    // Length and normalization
    float LenSq() const { return x*x + y*y; }
    float Len () const { return std::sqrt(LenSq()); }
    Vec2 Norm() const {
        float l = Len();
        return l > 1e-8f ? Vec2{x/l, y/l} : Vec2{};
    }

    // Perpendicular vector
    constexpr Vec2 Perp() const { return {-y, x}; }
};

inline constexpr Vec2 operator*(float s, const Vec2& v) { return v * s; }
inline constexpr Vec2 Cross(float s, const Vec2& v) { return {-s*v.y, s*v.x}; }