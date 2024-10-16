#include "lab/lab01/lab01.h"

#include <vector>

using namespace std;
using namespace lab;

Lab01::Lab01()
{
    window->SetSize(1280, 720);
}


Lab01::~Lab01()
{
}

void Lab01::Initialize()
{
    Exercise1();
    Exercise2();
    Exercise3();
    Exercise4();
    Exercise5();
    Exercise6();
}

void Lab01::Exercise1()
{
    SetExercise();

    //TODO(student): Ex. 1
    {
        SetPanel("1", { -3, -2, 100 }, { 0, 0, 1280, 720 });

        glm::vec2 v1(5, 2);
        glm::vec2 v2(2, 3);

        RenderVector(v1, glm::vec3(1, 0, 0), "v1");
        RenderVector(v2, glm::vec3(0, 1, 0), "v2");

        // a
        auto v3 = v1 + v2;
        RenderVector(v3, glm::vec3(0, 0, 1), "v3");

        // b
        RenderVector(v1, glm::vec3(1, 0, 0), "v1", v2);
        RenderVector(v2, glm::vec3(0, 1, 0), "v2", v1);
    }
}

void Lab01::Exercise2()
{
    SetExercise();

    //TODO(student): Ex. 2
    {
        SetPanel("2", { -7, -5, 75 }, { 0, 0, 1280, 720 });

        {
            glm::vec2 p1(1, 1);
            glm::vec2 p2(3, -3);

            RenderPoint(p1, glm::vec3(1, 0, 0), "P1");
            RenderPoint(p2, glm::vec3(0, 0, 1), "P2");

            auto v21 = p1 - p2;
            RenderVector(v21, glm::vec3(1, 0, 1), "v21", p2);
        }

        {
            glm::vec2 p3(-1, -2);
            glm::vec2 p4(-5, -4);

            RenderPoint(p3, glm::vec3(1, 0, 0), "P3");
            RenderPoint(p4, glm::vec3(0, 0, 1), "P4");

            auto v43 = p3 - p4;
            RenderVector(v43, glm::vec3(1, 0, 1), "v43", p4);
        }

        {
            glm::vec2 p5(6, 3);
            glm::vec2 p6(4, 1);

            RenderPoint(p5, glm::vec3(1, 0, 0), "P5");
            RenderPoint(p6, glm::vec3(0, 0, 1), "P6");

            auto v56 = p6 - p5;
            RenderVector(v56, glm::vec3(1, 0, 1), "v56", p5);
        }

        {
            glm::vec2 p7(5, 4);
            glm::vec2 p8(-3, 4);

            RenderPoint(p7, glm::vec3(1, 0, 0), "P7");
            RenderPoint(p8, glm::vec3(0, 0, 1), "P8");

            auto v87 = p7 - p8;
            RenderVector(v87, glm::vec3(1, 0, 1), "v87", p8);
        }
    }
}

void Lab01::Exercise3()
{
    SetExercise();

    //TODO(student): Ex. 3
    {
        SetPanel("3", { -7, -5, 75 }, { 0, 0, 1280, 720 });

        glm::vec2 v1(4, 3);
        glm::vec2 v2(-3, 3);
        glm::vec2 v3(-6, -2);
        glm::vec2 v4(-2, -4);
        glm::vec2 v5(3, -2);

        RenderVector(v1, glm::vec3(1, 0, 0), "v1");
        RenderVector(v2, glm::vec3(0, 1, 0), "v2");
        RenderVector(v3, glm::vec3(0, 1, 1), "v3");
        RenderVector(v4, glm::vec3(0, 0, 1), "v4");
        RenderVector(v5, glm::vec3(1, 0, 1), "v5");

        RenderVector(glm::normalize(v1), glm::vec3(0, 0, 0));
        RenderVector(glm::normalize(v2), glm::vec3(0, 0, 0));
        RenderVector(glm::normalize(v3), glm::vec3(0, 0, 0));
        RenderVector(glm::normalize(v4), glm::vec3(0, 0, 0));
        RenderVector(glm::normalize(v5), glm::vec3(0, 0, 0));
    }
}

void Lab01::Exercise4()
{
    SetExercise ();

    //TODO(student): Ex. 4
    {
        SetPanel("4", { -3, -2, 100 }, { 0, 0, 1280, 720 });

        glm::vec2 v1(4, 1.5);
        glm::vec2 v2(2, 3);

        RenderVector(v1, glm::vec3(1, 0, 0), "v1");
        RenderVector(v2, glm::vec3(0, 1, 0), "v2");

        float dot1 = glm::dot(glm::vec2(1, 0), glm::normalize(v1));
        float dot2 = glm::dot(glm::normalize(v1), glm::normalize(v2));
        auto start = acos(dot1);
        auto angle = acos(dot2);
        RenderArc(start, angle, glm::vec3(0, 0, 1));

        // bonus
        float dot3 = glm::dot(glm::vec2(1, 0), glm::normalize(v2));
        auto start2 = acos(dot3);
        RenderArc(start2, 2 * glm::pi<float>() - angle, glm::vec3(1, 0, 0), 1.f);
    }
}

static float triangleArea(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3) {
    auto s1 = glm::length(p1 - p2);
    auto s2 = glm::length(p2 - p3);
    auto s3 = glm::length(p3 - p1);
    auto s = (s1 + s2 + s3) / 2;
    return std::sqrt(s * (s - s1) * (s - s2) * (s - s3));
}

void Lab01::Exercise5()
{
    SetExercise();

    //TODO(student): Ex. 5
    {
        SetPanel("5", { -7, -5, 75 }, { 0, 0, 1280, 720 });

        {
            glm::vec2 p1(1, 1);
            glm::vec2 p2(3, 1);
            glm::vec2 p3(2, 4);

            RenderTriangle(p1, p2, p3, glm::vec3(0, 1, 1));

            RenderPoint(p1, glm::vec3(1, 0, 0), "P1");
            RenderPoint(p2, glm::vec3(0, 0, 1), "P2");
            RenderPoint(p3, glm::vec3(0, 1, 0), "P3");

            char text[16];
            sprintf(text, "%f", triangleArea(p1, p2, p3));
            auto center = glm::vec2((p1.x + p2.x + p3.x) / 3, (p1.y + p2.y + p3.y) / 3);

            RenderText(center, text, glm::vec3(0, 0, 0));
        }

        {
            glm::vec2 p1(-1.5, -3.7);
            glm::vec2 p2(-2.5, 3.2);
            glm::vec2 p3(-6.1, 2.9);

            RenderTriangle(p1, p2, p3, glm::vec3(0, 1, 1));

            RenderPoint(p1, glm::vec3(1, 0, 0), "P1");
            RenderPoint(p2, glm::vec3(0, 0, 1), "P2");
            RenderPoint(p3, glm::vec3(0, 1, 0), "P3");

            char text[16];
            sprintf(text, "%f", triangleArea(p1, p2, p3));
            auto center = glm::vec2((p1.x + p2.x + p3.x) / 3, (p1.y + p2.y + p3.y) / 3);

            RenderText(center, text, glm::vec3(0, 0, 0));
        }

        {
            glm::vec2 p1(2.3, -2.1);
            glm::vec2 p2(7.1, -3.5);
            glm::vec2 p3(7.5, 3.7);

            RenderTriangle(p1, p2, p3, glm::vec3(0, 1, 1));

            RenderPoint(p1, glm::vec3(1, 0, 0), "P1");
            RenderPoint(p2, glm::vec3(0, 0, 1), "P2");
            RenderPoint(p3, glm::vec3(0, 1, 0), "P3");

            char text[16];
            sprintf(text, "%f", triangleArea(p1, p2, p3));
            auto center = glm::vec2((p1.x + p2.x + p3.x) / 3, (p1.y + p2.y + p3.y) / 3);

            RenderText(center, text, glm::vec3(0, 0, 0));
        }
    }
}


void Lab01::Exercise6()
{
    SetExercise();

    //TODO(student): Ex. 6
    {
        SetPanel("6", { -7, -5, 75 }, { 0, 0, 1280, 720 });

        glm::vec2 p1(-4, 3);
        glm::vec2 p2(6, 2);
        glm::vec2 p3(1, -4);

        RenderTriangle(p1, p2, p3, glm::vec3(0, 1, 1));

        RenderPoint(p1, glm::vec3(0, 0, 1), "P1");
        RenderPoint(p2, glm::vec3(0, 0, 1), "P2");
        RenderPoint(p3, glm::vec3(0, 0, 1), "P3");

        std::vector<glm::vec2> check_points{
            glm::vec2(1, 1), glm::vec2(3.21, 3.2),
            glm::vec2(-2.16, 2.9), glm::vec2(2, -2),
            glm::vec2(-1.1, -0.9), glm::vec2(-1.9, 2.1),
            glm::vec2(-4, 0), glm::vec2(-4, -2.5),
            glm::vec2(4.5, 1.1), glm::vec2(5.9, -3.1),
            glm::vec2(7.1, 1.9), glm::vec2(-5.9, 1.5)
        };

        auto testArea = triangleArea(p1, p2, p3);
        for (const glm::vec2 &check_point : check_points) {
            auto area1 = triangleArea(check_point, p2, p3);
            auto area2 = triangleArea(p1, check_point, p3);
            auto area3 = triangleArea(p1, p2, check_point);

            const float eps = 0.01f;
            std::string text = "out";
            glm::vec3 color = glm::vec3(1, 0, 0);
            if (std::abs(testArea - (area1 + area2 + area3)) < eps) {
                text = "in";
                color = glm::vec3(0, 1, 0);
            }
            RenderPoint(check_point, color, text);
        }
    }
}