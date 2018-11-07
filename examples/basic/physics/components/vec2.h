#pragma once

struct vec2
{
    vec2(int x, int y)
        : x(x)
        , y(y)
    {
    }

    vec2() = default;

    vec2  operator + (const vec2 & rhs)
    {
        vec2 temp(*this);
        temp.x += rhs.x;
        temp.y += rhs.y;

        return temp;
    }

    vec2 operator - (const vec2 & rhs)
    {
        vec2 temp(*this);
        temp.x -= rhs.x;
        temp.y -= rhs.y;

        return temp;
    }

    vec2 & operator += (const vec2 & rhs)
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    vec2 & operator -= (const vec2 & rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    int x = 0;
    int y = 0;
};
