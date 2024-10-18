#include "lab/lab02/triangle_rasterizer.h"

#include <vector>
#include <iostream>

using namespace std;
using namespace lab;

void TriangleRasterizer::Rasterize(
        const VertexFormat &v1,
        const VertexFormat &v2,
        const VertexFormat &v3,
        Image *image,
        DepthImage *depthImage) {
    float xMin = min(v1.position.x, min(v2.position.x, v3.position.x));
    float yMin = min(v1.position.y, min(v2.position.y, v3.position.y));
    float xMax = max(v1.position.x, max(v2.position.x, v3.position.x));
    float yMax = max(v1.position.y, max(v2.position.y, v3.position.y));

    for (int row = yMin; row < yMax; row++) {
        for (int column = xMin; column < xMax; column++) {

            if (row < 0 || row >= image->GetHeight() ||
                column < 0 || column >= image->GetWidth()) {
                continue;
            }

            glm::vec2 p(column + 0.5f, row + 0.5f);

            if (CheckPointInsideTriangle(p, v1, v2, v3)) {

                float depth = ComputePixelDepth(p, v1, v2, v3);
                float pixelDepth = depthImage->Get(row, column).x;

                if (pixelDepth > depth) {
                    image->Set(row, column, ComputePixelColor(p, v1, v2, v3));
                    depthImage->Set(row, column, glm::vec3(depth, 0, 0));
                }
            }
        }
    }
}

float TriangleRasterizer::ComputeTriangleArea(
        const glm::vec2 &v1,
        const glm::vec2 &v2,
        const glm::vec2 &v3) {
    auto s1 = glm::length(v1 - v2);
    auto s2 = glm::length(v2 - v3);
    auto s3 = glm::length(v3 - v1);
    auto s = (s1 + s2 + s3) / 2;
    return std::sqrt(s * (s - s1) * (s - s2) * (s - s3));

    // de ce nu merge?
    //return 0.5f * (v1.x * (v2.y - v3.y) + v2.x * (v3.y - v1.y) + v3.x * (v1.y - v2.y));
}

bool TriangleRasterizer::CheckPointInsideTriangle(
        const glm::vec2 &p,
        const VertexFormat &v1,
        const VertexFormat &v2,
        const VertexFormat &v3) {
    const float EPSILON = 12.0f;

    auto area = ComputeTriangleArea(v1.position, v2.position, v3.position);

    auto subArea1 = ComputeTriangleArea(p, v2.position, v3.position);
    auto subArea2 = ComputeTriangleArea(v1.position, p, v3.position);
    auto subArea3 = ComputeTriangleArea(v1.position, v2.position, p);

    if (std::abs(area - (subArea1 + subArea2 + subArea3)) < EPSILON) {
        return true;
    }

    return false;
}

glm::vec3 TriangleRasterizer::ComputePixelColor(
        const glm::vec2 &p,
        const VertexFormat &v1,
        const VertexFormat &v2,
        const VertexFormat &v3) {
    auto area = ComputeTriangleArea(v1.position, v2.position, v3.position);

    auto subArea1 = ComputeTriangleArea(p, v2.position, v3.position);
    auto subArea2 = ComputeTriangleArea(v1.position, p, v3.position);
    auto subArea3 = ComputeTriangleArea(v1.position, v2.position, p);

    auto u = subArea1 / area;
    auto v = subArea2 / area;
    auto w = subArea3 / area;

    return u * v1.color + v * v2.color + w * v3.color;
}

float TriangleRasterizer::ComputePixelDepth(
        const glm::vec2 &p,
        const VertexFormat &v1,
        const VertexFormat &v2,
        const VertexFormat &v3) {
    auto area = ComputeTriangleArea(v1.position, v2.position, v3.position);

    auto subArea1 = ComputeTriangleArea(p, v2.position, v3.position);
    auto subArea2 = ComputeTriangleArea(v1.position, p, v3.position);
    auto subArea3 = ComputeTriangleArea(v1.position, v2.position, p);

    auto u = subArea1 / area;
    auto v = subArea2 / area;
    auto w = subArea3 / area;

    return u * v1.position.z + v * v2.position.z + w * v3.position.z;
}
