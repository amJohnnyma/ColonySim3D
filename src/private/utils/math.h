#ifndef MATH_H
#define MATH_H
#include <SFML/Graphics.hpp>
#include <cmath>
#include <optional>

namespace math
{

    struct Vec3
    {
        float x, y, z;

        // ctors 
        constexpr Vec3() : x(0), y(0), z(0) {}
        constexpr Vec3(float s) : x(s), y(s), z(s) {}
        constexpr Vec3(float X, float Y, float Z) : x(X), y(Y), z(Z) {}

        // basic ops 
        constexpr Vec3 operator+(const Vec3 &v) const { return {x + v.x, y + v.y, z + v.z}; }
        constexpr Vec3 operator-(const Vec3 &v) const { return {x - v.x, y - v.y, z - v.z}; }
        constexpr Vec3 operator-() const { return {-x, -y, -z}; }
        constexpr Vec3 operator*(float s) const { return {x * s, y * s, z * s}; }
        constexpr Vec3 operator/(float s) const { return {x / s, y / s, z / s}; }

        Vec3 &operator+=(const Vec3 &v)
        {
            x += v.x;
            y += v.y;
            z += v.z;
            return *this;
        }
        Vec3 &operator-=(const Vec3 &v)
        {
            x -= v.x;
            y -= v.y;
            z -= v.z;
            return *this;
        }
        Vec3 &operator*=(float s)
        {
            x *= s;
            y *= s;
            z *= s;
            return *this;
        }
        Vec3 &operator/=(float s)
        {
            x /= s;
            y /= s;
            z /= s;
            return *this;
        }

        // length / normalize 
        float lengthSquared() const { return x * x + y * y + z * z; }
        float length() const { return std::sqrt(lengthSquared()); }

        Vec3 normalized() const
        {
            float len = length();
            return len > 0 ? (*this) / len : Vec3();
        }

        // dot / cross 
        static float dot(const Vec3 &a, const Vec3 &b)
        {
            return a.x * b.x + a.y * b.y + a.z * b.z;
        }

        static Vec3 cross(const Vec3 &a, const Vec3 &b)
        {
            return {
                a.y * b.z - a.z * b.y,
                a.z * b.x - a.x * b.z,
                a.x * b.y - a.y * b.x};
        }
    };

    struct Ray
    {
        Vec3 origin;
        Vec3 direction;
    };

    struct GridCoord
    {
        int face;
        int i;
        int j;
    };
    // scalar on the left (s * v)
    inline Vec3 operator*(float s, const Vec3 &v) { return v * s; }

    // utility rotations
    inline Vec3 rotateX(const Vec3 &v, float angle)
    {
        float c = std::cos(angle), s = std::sin(angle);
        return {v.x, c * v.y - s * v.z, s * v.y + c * v.z};
    }
    inline Vec3 rotateY(const Vec3 &v, float angle)
    {
        float c = std::cos(angle), s = std::sin(angle);
        return {c * v.x + s * v.z, v.y, -s * v.x + c * v.z};
    }

    inline Ray screenToRayDirection(const sf::Vector2i &screen, const sf::Vector2f &center, float zoom, float rotationX, float rotationY, float distance, float worldRadius, float sliderRadius)
    {
        sf::Vector2f mouseScreen(static_cast<float>(screen.x), static_cast<float>(screen.y));
        sf::Vector2f unzoomed = center + (mouseScreen - center) / zoom;
        sf::Vector2f projected = unzoomed - center;

        float frontZ = -worldRadius - sliderRadius;
        float scale = distance / (distance + frontZ);

        Vec3 viewDir(projected.x, -projected.y, scale);
        viewDir = viewDir.normalized();

        Vec3 dir = rotateX(rotateY(viewDir, -rotationY), -rotationX);
        Vec3 origin(0.f, 0.f, 0.f);

        return {origin, dir};
    }

    inline std::optional<Vec3> intersectSphere(const Vec3 &origin, const Vec3 &dir, float radius)
    {
        float A = 1.0f;
        float B = 2.0f * (origin.x * dir.x + origin.y * dir.y + origin.z * dir.z);
        float C = origin.x * origin.x + origin.y * origin.y + origin.z * origin.z - radius * radius;

        float discriminant = B * B - 4 * A * C;
        if (discriminant < 0)
            return std::nullopt;

        float t = (-B - std::sqrt(discriminant)) / (2.0f * A);
        return Vec3(
            origin.x + t * dir.x,
            origin.y + t * dir.y,
            origin.z + t * dir.z);
    }

    inline GridCoord pointToCubeGrid(const Vec3 &hitPoint, int worldSize)
    {
        Vec3 p = hitPoint.normalized();

        float absX = std::abs(p.x);
        float absY = std::abs(p.y);
        float absZ = std::abs(p.z);

        int face;
        float u, v;

        if (absZ >= absX && absZ >= absY)
        {
            if (p.z > 0)
            {
                face = 0;
                u = p.x / absZ;
                v = p.y / absZ;
            }
            else
            {
                face = 1;
                u = -p.x / absZ;
                v = p.y / absZ;
            }
        }
        else if (absX >= absY)
        {
            if (p.x > 0)
            {
                face = 3;
                u = -p.z / absX;
                v = p.y / absX;
            }
            else
            {
                face = 2;
                u = p.z / absX;
                v = p.y / absX;
            }
        }
        else
        {
            if (p.y > 0)
            {
                face = 4;
                u = p.x / absY;
                v = -p.z / absY;
            }
            else
            {
                face = 5;
                u = p.x / absY;
                v = p.z / absY;
            }
        }

        float step = 2.f / float(worldSize);
        int i = std::clamp(int((u + 1.f) / step), 0, worldSize - 1);
        int j = std::clamp(int((v + 1.f) / step), 0, worldSize - 1);

        return {face, i, j};
    }
}
#endif