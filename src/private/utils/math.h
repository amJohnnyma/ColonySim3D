#ifndef MATH_H
#define MATH_H
#include <SFML/Graphics.hpp>
#include <cmath>
struct Vec3
{
    float x, y, z;

    // ── ctors ───────────────────────────────────────
    constexpr Vec3()            : x(0), y(0), z(0) {}
    constexpr Vec3(float s)     : x(s), y(s), z(s) {}
    constexpr Vec3(float X,float Y,float Z) : x(X), y(Y), z(Z) {}

    // ── basic ops ──────────────────────────────────
    constexpr Vec3  operator+ (const Vec3& v) const { return {x+v.x, y+v.y, z+v.z}; }
    constexpr Vec3  operator- (const Vec3& v) const { return {x-v.x, y-v.y, z-v.z}; }
    constexpr Vec3  operator- ()             const { return {-x, -y, -z}; }
    constexpr Vec3  operator* (float s)      const { return {x*s, y*s, z*s}; }
    constexpr Vec3  operator/ (float s)      const { return {x/s, y/s, z/s}; }

    Vec3& operator+=(const Vec3& v){ x+=v.x; y+=v.y; z+=v.z; return *this; }
    Vec3& operator-=(const Vec3& v){ x-=v.x; y-=v.y; z-=v.z; return *this; }
    Vec3& operator*=(float s)     { x*=s;   y*=s;   z*=s;   return *this; }
    Vec3& operator/=(float s)     { x/=s;   y/=s;   z/=s;   return *this; }

    // ── length / normalize ─────────────────────────
    float lengthSquared() const  { return x*x + y*y + z*z; }
    float length()        const  { return std::sqrt(lengthSquared()); }

    Vec3  normalized()    const
    {
        float len = length();
        return len > 0 ? (*this)/len : Vec3();
    }

    // ── dot / cross ────────────────────────────────
    static float dot  (const Vec3& a,const Vec3& b)
    { return a.x*b.x + a.y*b.y + a.z*b.z; }

    static Vec3 cross(const Vec3& a,const Vec3& b)
    {
        return {
            a.y*b.z - a.z*b.y,
            a.z*b.x - a.x*b.z,
            a.x*b.y - a.y*b.x
        };
    }
};

// ── scalar on the left (s * v) ─────────────────────
inline Vec3 operator*(float s,const Vec3& v){ return v*s; }

// ── utility rotations matching your GLSL ───────────
inline Vec3 rotateX(const Vec3& v,float angle)
{
    float c = std::cos(angle), s = std::sin(angle);
    return { v.x,  c*v.y - s*v.z,  s*v.y + c*v.z };
}
inline Vec3 rotateY(const Vec3& v,float angle)
{
    float c = std::cos(angle), s = std::sin(angle);
    return {  c*v.x + s*v.z, v.y, -s*v.x + c*v.z };
}
class Math {
public:
    // Projects 2D screen coordinates to a unit hemisphere
    static sf::Vector3f projectToTrackball(const sf::Vector2i& pos, const sf::Vector2u& winSize);

    // Creates a 3x3 rotation matrix from an axis-angle representation
    static sf::Transform createRotationMatrix(const sf::Vector3f& axis, float angle);

    // Normalize a vector
    static sf::Vector3f normalize(const sf::Vector3f& v);

    // Dot product
    static float dot(const sf::Vector3f& a, const sf::Vector3f& b);

    // Cross product
    static sf::Vector3f cross(const sf::Vector3f& a, const sf::Vector3f& b);


};
#endif