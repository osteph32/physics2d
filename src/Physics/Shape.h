// Shape.h

#pragma once
#include "../Math/Vec2.h"
#include "../Math/Mat2.h"
#include <vector>
#include <cmath>

// Shape type tag
enum class ShapeType { Circle, Polygon };

// Shape base
struct Shape {
    ShapeType type;
    virtual ~Shape() = default;
    virtual float ComputeInertia(float mass) const = 0;
};

// Circle
struct Circle : Shape {
    float radius;
    explicit Circle(float r) : radius(r) {
        type = ShapeType::Circle;
    }
    float ComputeInertia(float mass) const override {
        return 0.5f * mass * radius * radius;
    }
};

// Polygon
struct Polygon : Shape {
    std::vector<Vec2> vertices;
    std::vector<Vec2> normals;

    Polygon() { type = ShapeType::Polygon; }

    void SetBox(float hw, float hh) {
        vertices = {
            {-hw, -hh},
            {hw, -hh},
            {hw, hh},
            {-hw, hh}
        };
        ComputeNormals();
    }

    void ComputeNormals() {
        int n = (int)vertices.size();
        normals.resize(n);
        for (int i = 0; i < n; ++i) {
            int j = (i + 1) % n;
            Vec2 edge = vertices[j] - vertices[i];
            normals[i] = Vec2{edge.y, -edge.x}.Norm();
        }
    }

    Vec2 GetSupport(Vec2 d) const {
        float best = -1e30f;
        Vec2  bestV;
        for (const auto& v : vertices) {
            float proj = v.Dot(d);
            if (proj > best) { best = proj; bestV = v; }
        }
        return bestV;
    }

    float ComputeInertia(float mass) const override {
        float hw = vertices[1].x;
        float hh = vertices[2].y;
        float w  = hw * 2.f, h = hh * 2.f;
        return mass * (w*w + h*h) / 12.f;
    }
};