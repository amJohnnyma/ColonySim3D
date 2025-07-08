#ifndef MATH_H
#define MATH_H
#include <SFML/Graphics.hpp>
#include <cmath>
#include <optional>

namespace math
{
    struct DebugVariables
    {
        float A = 0.f;
        float origin = 0.f;
    };
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
    inline bool operator==(const GridCoord &a, const GridCoord &b)
    {
        return a.face == b.face && a.i == b.i && a.j == b.j;
    }
    inline bool operator!=(const GridCoord &a, const GridCoord &b)
    {
        return !(a == b);
    }
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

    inline Ray screenToRayDirection(const sf::Vector2i &screen, const sf::Vector2f &center, float zoom, float rotationX, float rotationY, float distance, float worldRadius, float sliderRadius, DebugVariables debug)
    {
        sf::Vector2f mouseScreen(static_cast<float>(screen.x), static_cast<float>(screen.y));
        sf::Vector2f unzoomed = center + (mouseScreen - center) / zoom;
        sf::Vector2f projected = unzoomed - center;

        float frontZ = -worldRadius - sliderRadius;
        float scale = distance / (distance + frontZ);

        Vec3 viewDir(projected.x, -projected.y, scale);
        viewDir = viewDir.normalized();

        Vec3 dir = rotateX(rotateY(viewDir, -rotationY), -rotationX);
        Vec3 origin(0, 0, debug.origin);

        return {origin, dir};
    }

    inline std::optional<Vec3> intersectSphere(const Vec3 &origin, const Vec3 &dir, float radius, DebugVariables debug)
    {
        float A = debug.A;
        float B = 2.0f * Vec3::dot(origin, dir);
        float C = Vec3::dot(origin, origin) - radius * radius;

        float discriminant = B * B - 4 * A * C;
        if (discriminant < 0)
            return std::nullopt;

        float t = (-B - std::sqrt(discriminant)) / (2.0f * A);
        return Vec3(
            origin.x + t * dir.x,
            origin.y + t * dir.y,
            origin.z + t * dir.z);
    }

    inline GridCoord pointToCubeGrid(const Vec3 &hitPoint, int worldSize, DebugVariables debug)
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
    inline GridCoord screenToCubeCoord(
        const sf::Vector2i &mouse,
        const sf::Vector2f &screenCenter,
        float zoom,
        float rotX,
        float rotY,
        float distance,
        float radius,
        int gridSize,
        DebugVariables debug)
    {
        // screen to NDC -> Screen space
        sf::Vector2f mouseF(static_cast<float>(mouse.x), static_cast<float>(mouse.y));
        sf::Vector2f unzoomed = screenCenter + (mouseF - screenCenter) / zoom;

        // if we need to adjust the x and y we have vars
        sf::Vector2f delta = unzoomed - screenCenter;
        float adjustedDeltaY = delta.y;
        float adjustedDeltaX = -delta.x;

        // Compute the world-space camera direction
        Vec3 viewDir(0, 0, -1);
        Vec3 rotatedViewDir = rotateX(rotateY(viewDir, -rotY), -rotX);

        // Create the direction (into the 'sphere')
        Vec3 dir(adjustedDeltaX, adjustedDeltaY, distance);
        dir = dir.normalized();
        // we dont have rotated.z so emulate one
        float frontZ = -radius;
        // direction of the ray should have the inverse rotations of the raydir
        Vec3 rayDir = rotateX(rotateY(dir, -rotY), -rotX);
        // ray start from centre of the 'sphere'
        Vec3 rayOrigin(0, 0, 0);
        // The ray is defined as: R(t) = rayOrigin + t * rayDir
        // The sphere equation is: |P|^2 = radius^2, where P is a point on the sphere

        // quad coeff for ray-sphere intersection

        // A = dot product of the ray direction with itself.
        // Since rayDir is normalized, A should be 1.
        float A = Vec3::dot(rayDir, rayDir); // = 1
        // B = 2 times the dot product of ray origin and ray direction.
        // If ray origin is at (0,0,0), B will be zero.
        float B = 2.0f * Vec3::dot(rayOrigin, rayDir); // = 0 if origin is 0
        // C = squared length of ray origin minus squared radius of the sphere.
        // Represents how far ray origin is from sphere surface.
        float C = Vec3::dot(rayOrigin, rayOrigin) - radius * radius;

        // If discriminant is negative, ray misses the sphere (no real roots)
        float discriminant = B * B - 4 * A * C;
        if (discriminant < 0)
            return GridCoord(); // no intersection

        // Otherwise, compute the smallest positive root t of the quadratic:
        float t = (-B - std::sqrt(discriminant)) / (2.0f * A);

        // Calculate the intersection point on the sphere:
        Vec3 hitPoint = rayOrigin + rayDir * t;
        // Normalize hitPoint to get a point on the unit sphere (direction vector)
        Vec3 p = hitPoint.normalized();

        // Project the normalized point p on the sphere to one of the 6 cube faces:

        // Calculate absolute values of each component to determine dominant axis
        float absX = std::abs(p.x);
        float absY = std::abs(p.y);
        float absZ = std::abs(p.z);

        int face;   // Cube face index [0..5]
        float u, v; // 2D coordinates on the selected cube face in range [-1, 1]

        // Determine which cube face the point projects onto:
        // The face is chosen by the largest absolute component (dominant axis)
        if (absZ >= absX && absZ >= absY)
        {
            // Z-axis is dominant
            if (p.z > 0)
            {
                face = 0;       // Front face
                u = p.x / absZ; // Map x coordinate scaled by dominant axis
                v = p.y / absZ; // Map y coordinate scaled by dominant axis
            }
            else
            {
                face = 1;        // Back face
                u = -p.x / absZ; // Invert x to mirror correctly
                v = p.y / absZ;
            }
        }
        else if (absX >= absY)
        {
            // X-axis is dominant
            if (p.x > 0)
            {
                face = 3;        // Right face
                u = -p.z / absX; // Map z coordinate inverted
                v = p.y / absX;
            }
            else
            {
                face = 2; // Left face
                u = p.z / absX;
                v = p.y / absX;
            }
        }
        else
        {
            // Y-axis is dominant
            if (p.y > 0)
            {
                face = 4; // Top face
                u = p.x / absY;
                v = -p.z / absY; // Inverted z coordinate for correct orientation
            }
            else
            {
                face = 5; // Bottom face
                u = p.x / absY;
                v = p.z / absY;
            }
        }

        // At this point, u and v represent coordinates on the cube face in range [-1, 1].

        // Map these normalized face coordinates to discrete grid indices:

        // Calculate the size of one grid cell in normalized face space
        float step = 2.f / float(gridSize);

        // Convert continuous u, v coordinates to integer grid indices
        // floor maps coordinate to the lower grid cell
        // Clamp to ensure indices stay within valid [0, gridSize-1] bounds
        int i = std::clamp(int(std::floor((u + 1.f) / step)), 0, gridSize - 1);
        int j = std::clamp(int(std::floor((v + 1.f) / step)), 0, gridSize - 1);

        return GridCoord{face, i, j};
    }
}
#endif