#include "lab/lab02/lab02.h"

#include <glm/glm.hpp>

#include <vector>
#include <iostream>

using namespace std;
using namespace lab;

Lab02::Lab02()
{
    window->SetSize(1280, 720);
}


Lab02::~Lab02()
{
}

void Lab02::Initialize()
{
    image->Init(1280, 720, 3 /* channels */);
    depthImage->Init(1280, 720);

    {
#if 0
        vector<VertexFormat> vertices
        {
            VertexFormat(glm::vec3(290, 90,  0.5), glm::vec3(1, 0, 0)),
            VertexFormat(glm::vec3(1099, 450,  0.5), glm::vec3(0, 1, 0)),
            VertexFormat(glm::vec3(650, 719,  0.5), glm::vec3(0, 0, 1)),

            VertexFormat(glm::vec3(200, 450,  0), glm::vec3(0, 1, 1)),
            VertexFormat(glm::vec3(830, 719,  1), glm::vec3(1, 1, 0)),
            VertexFormat(glm::vec3(1099, 0,  1), glm::vec3(1, 0, 1)) };

        vector<unsigned int> indices
        {
            0, 1, 2,    // indices for first triangle
            3, 4, 5,    // indices for second triangle
        };
#else
        int circleX = 500;
        int circleY = 350;
        int radius = 200;

        std::vector<unsigned int> indices;
        std::vector<VertexFormat> vertices;

        const int VertexNo = 24;
        auto angleDelta = 2 * glm::pi<float>() / VertexNo;

        auto x0 = circleX + radius * cos(0 * angleDelta);
        auto y0 = circleY + radius * sin(0 * angleDelta);
        auto vertexPos0 = glm::vec3(x0, y0, 0);
        vertices.push_back({vertexPos0, glm::vec3(0.12f, 0.25f, 0.8f)});
        for (int i = 1; i < VertexNo; i++) {
            auto x = circleX + radius * cos(i * angleDelta);
            auto y = circleY + radius * sin(i * angleDelta);
            auto vertexPos = glm::vec3(x, y, 0);

            vertices.push_back({vertexPos, glm::vec3(0.12f, 0.25f, 0.8f)});
        }
        vertices.push_back({{circleX, circleY, 1}, glm::vec3(0.12f, 0.25f, 0.8f)});

        std::cout << (-1 % VertexNo) << '\n';
        for (int i = 1; i <= VertexNo; i++) {
            indices.push_back(VertexNo);
            indices.push_back(i % VertexNo);
            indices.push_back(glm::mod((i % VertexNo) - 1.f, (float)VertexNo));
        }
#endif

        Rasterize(vertices, indices);
    }
}

void Lab02::Rasterize(
    const vector<VertexFormat> &vertices,
    const vector<unsigned int> &indices)
{
    for (int i = 0; i < indices.size(); i += 3) {
        auto v1 = vertices[indices[i]];
        auto v2 = vertices[indices[i+1]];
        auto v3 = vertices[indices[i+2]];

        TriangleRasterizer::Rasterize(
            v1, v2, v3, image, depthImage
        );
    }
}
