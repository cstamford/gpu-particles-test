#pragma once

#include <cmath>

namespace br::math
{
    static constexpr float PI = 3.14159265f;

    inline constexpr float rad2deg(const float radians);
    inline constexpr float deg2rad(const float degrees);

    inline constexpr float rad2deg(const float radians)
    {
        return radians * (180.0f / PI);
    }

    inline constexpr float deg2rad(const float degrees)
    {
        return degrees * (PI / 180.0f);
    }

    struct Vec2
    {
        float x, y;

        static constexpr inline Vec2 zero() { return { 0.0f, 0.0f }; }
        static constexpr inline Vec2 one() { return { 1.0f, 1.0f }; }
        static constexpr inline Vec2 up() { return { 0.0f, -1.0f }; }
        static constexpr inline Vec2 right() { return { 1.0f, 0.0f }; }
    };

    using Vec2Ref = const Vec2&;

    inline Vec2 abs(Vec2Ref vec);
    inline float angle(Vec2Ref vec);
    inline Vec2 angle(const float radians);
    inline float len(Vec2Ref vec);
    inline float lensqr(Vec2Ref vec);
    inline Vec2 normalize(Vec2Ref vec);
    inline Vec2 rotate(Vec2Ref vec, const float radians);

    inline float dist(Vec2Ref lhs, Vec2Ref rhs);
    inline float distsqr(Vec2Ref lhs, Vec2Ref rhs);
    inline float dot(Vec2Ref lhs, Vec2Ref rhs);

    inline Vec2 operator+(Vec2Ref lhs, Vec2Ref rhs);
    inline Vec2 operator-(Vec2Ref lhs, Vec2Ref rhs);
    inline Vec2 operator*(Vec2Ref lhs, Vec2Ref rhs);
    inline Vec2 operator/(Vec2Ref lhs, Vec2Ref rhs);
    inline Vec2 operator+(Vec2Ref lhs, const float rhs);
    inline Vec2 operator-(Vec2Ref lhs, const float rhs);
    inline Vec2 operator*(Vec2Ref lhs, const float rhs);
    inline Vec2 operator/(Vec2Ref lhs, const float rhs);
    inline Vec2 operator-(Vec2Ref lhs);

    inline Vec2 abs(Vec2Ref __restrict vec)
    {
        return { std::abs(vec.x), std::abs(vec.y) };
    }

    inline float angle(Vec2Ref __restrict vec)
    {
        return std::atan2(vec.y, vec.x);
    }

    inline Vec2 angle(const float radians)
    {
        return { std::cos(radians), std::sin(radians) };
    }

    inline float len(Vec2Ref __restrict vec)
    {
        return sqrt(lensqr(vec));
    }

    inline float lensqr(Vec2Ref __restrict vec)
    {
        return vec.x * vec.x + vec.y * vec.y;
    }

    inline Vec2 normalize(Vec2Ref __restrict vec)
    {
        const float mag = len(vec);
        if (mag == 0.0f) return vec;
        const float factor = 1.0f / mag;
        return { vec.x * factor, vec.y * factor };
    }

    inline Vec2 rotate(Vec2Ref vec, const float radians)
    {
        const float c = std::cos(radians);
        const float s = std::sin(radians);
        return { vec.x * c - vec.y * s, vec.x * s + vec.y * c };
    }

    inline float dist(Vec2Ref __restrict lhs, Vec2Ref __restrict rhs)
    {
        return sqrt(distsqr(lhs, rhs));
    }

    inline float distsqr(Vec2Ref __restrict lhs, Vec2Ref __restrict rhs)
    {
        return lensqr(rhs - lhs);
    }

    inline float dot(Vec2Ref __restrict lhs, Vec2Ref __restrict rhs)
    {
        return lhs.x * rhs.x + lhs.y * rhs.y;
    }

    inline Vec2 operator+(Vec2Ref __restrict lhs, Vec2Ref __restrict rhs)
    {
        return { lhs.x + rhs.x, lhs.y + rhs.y };
    }

    inline Vec2 operator-(Vec2Ref __restrict lhs, Vec2Ref __restrict rhs)
    {
        return { lhs.x - rhs.x, lhs.y - rhs.y };
    }

    inline Vec2 operator*(Vec2Ref __restrict lhs, Vec2Ref __restrict rhs)
    {
        return { lhs.x * rhs.x, lhs.y * rhs.y };
    }

    inline Vec2 operator/(Vec2Ref __restrict lhs, Vec2Ref __restrict rhs)
    {
        return { lhs.x / rhs.x, lhs.y / rhs.y };
    }

    inline Vec2 operator+(Vec2Ref lhs, const float rhs)
    {
        return { lhs.x + rhs, lhs.y + rhs };
    }

    inline Vec2 operator-(Vec2Ref lhs, const float rhs)
    {
        return { lhs.x - rhs, lhs.y - rhs };
    }

    inline Vec2 operator*(Vec2Ref lhs, const float rhs)
    {
        return { lhs.x * rhs, lhs.y * rhs };
    }

    inline Vec2 operator/(Vec2Ref lhs, const float rhs)
    {
        return { lhs.x / rhs, lhs.y / rhs };
    }

    inline Vec2 operator-(Vec2Ref __restrict lhs)
    {
        return { -lhs.x, -lhs.y };
    }

    struct Rect
    {
        Vec2 centre;
        Vec2 half_extents;
    };

    using RectRef = const Rect&;

    inline bool overlap(RectRef __restrict a, RectRef __restrict b)
    {
        Vec2 delta = abs(a.centre - b.centre);
        Vec2 size = a.half_extents + b.half_extents;
        return delta.x <= size.x && delta.y <= size.y;
    }

    struct Mat3
    {
        float t[9];

        static constexpr inline Mat3 identity() { return { 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f }; }
    };

    using Mat3Ref = const Mat3&;

    inline Mat3 ortho(const float width, const float height);
    inline Mat3 camera(Vec2Ref pos, float zoom);

    inline Mat3 translate(Mat3Ref mat, Vec2Ref pos);
    inline Mat3 rotate(Mat3Ref mat, const float radians);
    inline Mat3 scale(Mat3Ref mat, const float sx, const float sy);

    inline Mat3 operator+(Mat3Ref lhs, Mat3Ref rhs);
    inline Mat3 operator-(Mat3Ref lhs, Mat3Ref rhs);
    inline Mat3 operator*(Mat3Ref lhs, Mat3Ref rhs);

    inline Mat3 ortho(const float width, const float height)
    {
        return { 2.0f / width, 0.0f, 0.0f, 0.0f, -(2.0f / height), 0.0f, -1.0f, 1.0f, 1.0f };
    }

    inline Mat3 camera(Vec2Ref __restrict pos, float zoom)
    {
        return { zoom, 0.0f, 0.0f, 0.0f, zoom, 0.0f, -pos.x, -pos.y, 1.0f };
    }

    inline Mat3 translate(Mat3Ref __restrict mat, Vec2Ref __restrict pos)
    {
        Mat3 trans = Mat3::identity();
        trans.t[6] = pos.x;
        trans.t[7] = pos.y;
        return mat * trans;
    }

    inline Mat3 rotate(Mat3Ref __restrict mat, const float radians)
    {
        const float c = cos(radians);
        const float s = sin(radians);

        Mat3 rot = Mat3::identity();
        rot.t[0] = c;
        rot.t[1] = s;
        rot.t[3] = -s;
        rot.t[4] = c;

        return mat * rot;
    }

    inline Mat3 scale(Mat3Ref mat, const float sx, const float sy)
    {
        Mat3 scale = Mat3::identity();
        scale.t[0] = sx;
        scale.t[4] = sy;
        return mat * scale;
    }

    inline Mat3 operator+(Mat3Ref __restrict lhs, Mat3Ref __restrict rhs)
    {
        return
        {
            lhs.t[0] + rhs.t[0], lhs.t[1] + rhs.t[1], lhs.t[2] + rhs.t[2],
            lhs.t[3] + rhs.t[3], lhs.t[4] + rhs.t[4], lhs.t[5] + rhs.t[5],
            lhs.t[6] + rhs.t[6], lhs.t[7] + rhs.t[7], lhs.t[8] + rhs.t[8],
        };
    }

    inline Mat3 operator-(Mat3Ref __restrict lhs, Mat3Ref __restrict rhs)
    {
        return
        {
            lhs.t[0] - rhs.t[0], lhs.t[1] - rhs.t[1], lhs.t[2] - rhs.t[2],
            lhs.t[3] - rhs.t[3], lhs.t[4] - rhs.t[4], lhs.t[5] - rhs.t[5],
            lhs.t[6] - rhs.t[6], lhs.t[7] - rhs.t[7], lhs.t[8] - rhs.t[8],
        };
    }

    inline Mat3 operator*(Mat3Ref __restrict lhs, Mat3Ref __restrict rhs)
    {
        return
        {
            (lhs.t[0] * rhs.t[0]) + (lhs.t[3] * rhs.t[1]) + (lhs.t[6] * rhs.t[2]),
            (lhs.t[1] * rhs.t[0]) + (lhs.t[4] * rhs.t[1]) + (lhs.t[7] * rhs.t[2]),
            (lhs.t[2] * rhs.t[0]) + (lhs.t[5] * rhs.t[1]) + (lhs.t[8] * rhs.t[2]),
            (lhs.t[0] * rhs.t[3]) + (lhs.t[3] * rhs.t[4]) + (lhs.t[6] * rhs.t[5]),
            (lhs.t[1] * rhs.t[3]) + (lhs.t[4] * rhs.t[4]) + (lhs.t[7] * rhs.t[5]),
            (lhs.t[2] * rhs.t[3]) + (lhs.t[5] * rhs.t[4]) + (lhs.t[8] * rhs.t[5]),
            (lhs.t[0] * rhs.t[6]) + (lhs.t[3] * rhs.t[7]) + (lhs.t[6] * rhs.t[8]),
            (lhs.t[1] * rhs.t[6]) + (lhs.t[4] * rhs.t[7]) + (lhs.t[7] * rhs.t[8]),
            (lhs.t[2] * rhs.t[6]) + (lhs.t[5] * rhs.t[7]) + (lhs.t[8] * rhs.t[8]),
        };
    }
}
