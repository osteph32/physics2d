// Mat2.h

#pragma once
#include "Vec2.h"
#include <cmath>

// 2x2 rotation matrix
struct Mat2 {
    float m00=1, m01=0;
    float m10=0, m11=1;

    Mat2() = default;

    explicit Mat2(float radians) {
        float c = std::cos(radians);
        float s = std::sin(radians);
        m00= c; m01=-s;
        m10= s; m11= c;
    }

    Vec2 operator*(const Vec2& v) const {
        return { m00*v.x + m01*v.y, m10*v.x + m11*v.y };
    }

    Mat2 Transpose() const {
        Mat2 t;
        t.m00=m00; t.m01=m10;
        t.m10=m01; t.m11=m11;
        return t;
    }
};