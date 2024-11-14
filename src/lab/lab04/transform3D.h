#pragma once

#include "utils/glm_utils.h"
#include <glm/gtx/string_cast.hpp>

namespace transform3D
{
    // Translation matrix
    inline glm::mat4 Translate(float translateX, float translateY, float translateZ)
    {
        // TODO(student): Ex. 1
        auto mat = glm::mat4(1.f);
        mat = glm::translate(mat, {translateX, translateY, translateZ});
        //mat[3][0] = translateX;
        //mat[3][1] = translateY;
        //mat[3][2] = translateZ;
        return mat;
    }

    // Scaling matrix
    inline glm::mat4 Scale(float scaleX, float scaleY, float scaleZ)
    {
        // TODO(student): Ex. 1
        auto mat = glm::mat4(1.f);
        mat[0][0] = scaleX;
        mat[1][1] = scaleY;
        mat[2][2] = scaleZ;
        return mat;
    }

    // Rotation matrix relative to the OZ axis
    inline glm::mat4 RotateOZ(float radians)
    {
        // TODO(student): Ex. 1
        auto mat = glm::mat4(1.f);
        mat[0][0] = std::cos(radians);
        mat[0][1] = std::sin(radians);
        mat[1][0] = -std::sin(radians);
        mat[1][1] = std::cos(radians);
        return mat;
    }

    // Rotation matrix relative to the OY axis
    inline glm::mat4 RotateOY(float radians)
    {
        // TODO(student): Ex. 1
        auto mat = glm::mat4(1.f);
        mat[0][0] = std::cos(radians);
        mat[0][2] = -std::sin(radians);
        mat[2][0] = std::sin(radians);
        mat[2][2] = std::cos(radians);
        return mat;
    }

    // Rotate matrix relative to the OX axis
    inline glm::mat4 RotateOX(float radians)
    {
        // TODO(student): Ex. 1
        auto mat = glm::mat4(1.f);
        mat[1][1] = std::cos(radians);
        mat[1][2] = std::sin(radians);
        mat[2][1] = -std::sin(radians);
        mat[2][2] = std::cos(radians);
        return mat;
    }

    // Perspective projection matrix
    inline glm::mat4 Perspective(
        float fovy, float aspect, float zNear, float zFar)
    {
        // TODO(student): Ex. 2
        auto mat = glm::mat4(1.f);
        mat[0][0] = 1.f / (std::tan(fovy / 2.f) * aspect);
        mat[1][1] = 1.f / std::tan(fovy / 2.f);
        mat[2][2] = (-1.f * (zFar + zNear)) / (zFar - zNear);
        mat[3][2] = (-2.f * zFar * zNear) / (zFar - zNear);
        mat[2][3] = -1.f;

        return mat;
    }

    // View matrix
    inline glm::mat4 View(
        const glm::vec3& position,
        const glm::vec3& forward,
        const glm::vec3& right,
        const glm::vec3& up)
    {
        // TODO(student): Ex. 4
        auto mat = glm::mat4(1.f);
        mat[0][0] = right.x;
        mat[1][0] = right.y;
        mat[2][0] = right.z;

        mat[0][1] = up.x;
        mat[1][1] = up.y;
        mat[2][1] = up.z;

        mat[0][2] = forward.x;
        mat[1][2] = forward.y;
        mat[2][2] = forward.z;

        return mat * Translate(-position.x, -position.y, -position.z);
    }

}   // namespace transform3D
