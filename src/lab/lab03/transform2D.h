#pragma once

#include "utils/glm_utils.h"

#include <iostream>

namespace transform2D
{
    // Translation matrix
    inline glm::mat3 Translate(float translateX, float translateY)
    {
        // TODO(student): Ex. 1
        auto mat = glm::mat3(1);
        mat[2][0] = translateX;
        mat[2][1] = translateY;

        return mat;
    }

    // Scaling matrix
    inline glm::mat3 Scale(float scaleX, float scaleY)
    {
        // TODO(student): Ex. 1
        auto mat = glm::mat3(1);
        mat[0][0] = scaleX;
        mat[1][1] = scaleY;
        return mat;
    }

    // Rotation matrix
    inline glm::mat3 Rotate(float radians)
    {
        auto mat = glm::mat3(1);
        mat[0][0] = cos(radians);
        mat[0][1] = sin(radians);
        mat[1][0] = -sin(radians);
        mat[1][1] = cos(radians);
        // TODO(student): Ex. 1
        return mat;
    }

    struct ViewportSpace
    {
        ViewportSpace() : x(0), y(0), width(1), height(1) {}
        ViewportSpace(int x, int y, int width, int height)
            : x(x), y(y), width(width), height(height) {}
        int x;
        int y;
        int width;
        int height;
    };

    struct LogicSpace
    {
        LogicSpace() : x(0), y(0), width(1), height(1) {}
        LogicSpace(float x, float y, float width, float height)
            : x(x), y(y), width(width), height(height) {}
        float x;
        float y;
        float width;
        float height;
    };

    // Viewport transformation
    inline glm::mat3 Viewport(
        const LogicSpace& logic_space,
        const ViewportSpace& viewport_space)
    {
        // TODO(student): Ex. 1
        //
        // You can use the translation and scaling transformations
        auto originTranslate = Translate(-logic_space.x, -logic_space.y);
        auto viewTranslate = Translate(viewport_space.x, viewport_space.y);
        auto viewScale = Scale(viewport_space.width / logic_space.width, viewport_space.height / logic_space.height);

        return viewTranslate * viewScale * originTranslate;
    }

}   // namespace transform2D