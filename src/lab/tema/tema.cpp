#include "tema.h"

#include <iostream>
#include <utility>
#include <queue>
#include <vector>
#include <stb/stb_image.h>

inline std::vector<glm::vec3> treeCubeVertices = {
        glm::vec3(-0.125, 0.0, 0.125),
        glm::vec3(0.125, 0.0, 0.125),
        glm::vec3(-0.125, 2.0, 0.125),
        glm::vec3(0.125, 2.0, 0.125),
        glm::vec3(-0.125, 0.0, -0.125),
        glm::vec3(0.125, 0.0, -0.125),
        glm::vec3(-0.125, 2.0, -0.125),
        glm::vec3(0.125, 2.0, -0.125),
};

inline std::vector<unsigned int> treeCubeIndices
        {
                0, 1, 2,
                1, 3, 2,
                2, 3, 7,
                2, 7, 6,
                1, 7, 3,
                1, 5, 7,
                6, 7, 4,
                7, 5, 4,
                0, 4, 1,
                1, 4, 5,
                2, 6, 4,
                0, 2, 4
        };

std::vector<glm::vec3> cubeVertices
        {
                glm::vec3(-0.5, -0.5, 0.5),
                glm::vec3(0.5, -0.5, 0.5),
                glm::vec3(-0.5, 0.5, 0.5),
                glm::vec3(0.5, 0.5, 0.5),
                glm::vec3(-0.5, -0.5, -0.5),
                glm::vec3(0.5, -0.5, -0.5),
                glm::vec3(-0.5, 0.5, -0.5),
                glm::vec3(0.5, 0.5, -0.5),
        };

std::vector<unsigned int> cubeIndices
        {
                0, 1, 2,
                1, 3, 2,
                2, 3, 7,
                2, 7, 6,
                1, 7, 3,
                1, 5, 7,
                6, 7, 4,
                7, 5, 4,
                0, 4, 1,
                1, 4, 5,
                2, 6, 4,
                0, 2, 4
        };

namespace lab {

    void tema::CreateGround(std::vector<VertexFormat>& vertices, std::vector<unsigned int>& indices) {
        vertices.reserve(m_gridWidth * m_gridHeight);
        indices.reserve((m_gridWidth - 1) * 2 * m_gridHeight * 3);

        const auto floatWidth = static_cast<float>(m_gridWidth);
        const auto floatHeight = static_cast<float>(m_gridHeight);
        for (int y = 0; y < m_gridHeight; y++) {
            for (int x = 0; x < m_gridWidth; x++) {
                auto floatX = static_cast<float>(x);
                auto floatY = static_cast<float>(y);

                glm::vec3 position(floatX * m_offset, 0.0f, floatY * m_offset);
                glm::vec3 color(0.0f, 0.0f, 0.0f);
                glm::vec3 normal(0.0f, 1.0f, 0.0f);
                glm::vec2 textureCoord(floatX / floatWidth, floatY / floatHeight);

                VertexFormat v(position, color, normal, textureCoord);
                vertices.push_back(v);
            }
        }

        for (int y = 0; y < m_gridHeight - 1; y++) {
            for (int x = 0; x < m_gridWidth - 1; x++) {
                auto index = y * m_gridWidth + x;

                indices.push_back(index);
                indices.push_back(index + m_gridWidth);
                indices.push_back(index + m_gridWidth + 1);

                indices.push_back(index);
                indices.push_back(index + m_gridWidth + 1);
                indices.push_back(index + 1);
            }
        }
    }

    void tema::CreateTree(std::vector<VertexFormat>& vertices, std::vector<unsigned int>& indices, glm::mat4 model, int depth, int& indexOffset) {
        if (depth >= 6) {
            return;
        }

        auto scale = glm::scale(glm::mat4(1.f), glm::vec3(glm::pow(0.66f, depth)));
        for (int i = 0; i < treeCubeVertices.size(); i++) {
            auto pos = model * scale * glm::vec4(treeCubeVertices[i], 1.0f);
            if (m_treeHeight < pos.y) {
                m_treeHeight = pos.y;
            }

            VertexFormat v(pos);
            v.color = glm::vec3((float)depth);
            vertices.push_back(v);
        }

        for (int i = 0; i < treeCubeIndices.size(); i++) {
            indices.push_back(treeCubeIndices[i] + indexOffset);
        }
        indexOffset += treeCubeVertices.size();

        constexpr int childCount = 5;
        for (int i = 0; i < childCount; i++) {
            auto childModel = model;

            auto offset = glm::vec3(scale * glm::vec4(0, 2.f, 0, 1.f));
            childModel = glm::translate(childModel, offset);

            auto spinAngleRad = glm::radians((float)i * 360.f / (float)childCount);
            childModel = glm::rotate(childModel, spinAngleRad, glm::vec3(0, 1, 0));

            auto tiltAngleRad = glm::radians(45.f);
            childModel = glm::rotate(childModel, tiltAngleRad, glm::vec3(1, 0, 0));

            CreateTree(vertices, indices, childModel, depth + 1, indexOffset);
        }
    }

    void tema::CreateHelicopter(std::vector<VertexFormat> &vertices, std::vector<unsigned int> &indices) {
        vertices.reserve(cubeVertices.size() * 6);
        indices.reserve(cubeIndices.size() * 6);

        // Body
        auto scale = glm::scale(glm::mat4(1.f), glm::vec3(1.f, 1.f, 2.f));
        for (int i = 0; i < cubeVertices.size(); i++) {
            auto pos = scale * glm::vec4(cubeVertices[i], 1.0f);

            VertexFormat v(pos);
            v.color = glm::vec3(0.27, 0.29, 0.1);
            vertices.push_back(v);
        }

        // Tail
        scale = glm::scale(glm::mat4(1.f), glm::vec3(0.5f, 0.5f, 2.0f));
        auto translate = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, 1.0f));
        for (int i = 0; i < cubeVertices.size(); i++) {
            auto pos = scale * translate * glm::vec4(cubeVertices[i], 1.0f);

            VertexFormat v(pos);
            v.color = glm::vec3(0.27, 0.29, 0.1);
            vertices.push_back(v);
        }

        // Back rotor
        scale = glm::scale(glm::mat4(1.f), glm::vec3(0.075f, 0.25f, 1.25f));
        translate = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, 3.0f));
        for (int i = 0; i < cubeVertices.size(); i++) {
            auto pos = translate * scale * glm::vec4(cubeVertices[i], 1.0f);

            VertexFormat v(pos);
            v.color = glm::vec3(0.29, 0.29, 0.3);
            vertices.push_back(v);
        }

        scale = glm::scale(glm::mat4(1.f), glm::vec3(0.075f, 1.25f, 0.25f));
        for (int i = 0; i < cubeVertices.size(); i++) {
            auto pos = translate * scale * glm::vec4(cubeVertices[i], 1.0f);

            VertexFormat v(pos);
            v.color = glm::vec3(0.29, 0.29, 0.3);
            vertices.push_back(v);
        }

        // Main rotor
        scale = glm::scale(glm::mat4(1.f), glm::vec3(0.25f, 0.075f, 4.0f));
        translate = glm::translate(glm::mat4(1.f), glm::vec3(0, 0.575f, 0));
        for (int i = 0; i < cubeVertices.size(); i++) {
            auto pos = translate * scale * glm::vec4(cubeVertices[i], 1.0f);

            VertexFormat v(pos);
            v.color = glm::vec3(0.3, 0.3, 0.3);
            vertices.push_back(v);
        }

        scale = glm::scale(glm::mat4(1.f), glm::vec3(4.0f, 0.075f, 0.25f));
        for (int i = 0; i < cubeVertices.size(); i++) {
            auto pos = translate * scale * glm::vec4(cubeVertices[i], 1.0f);

            VertexFormat v(pos);
            v.color = glm::vec3(0.3, 0.3, 0.3);
            vertices.push_back(v);
        }

        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < cubeIndices.size(); j++) {
                indices.push_back(cubeIndices[j] + i * 8);
            }
        }
    }

    void tema::CreateMark(std::vector<VertexFormat>& vertices, std::vector<unsigned int>& indices) {
        vertices.reserve(18 + cubeVertices.size());
        indices.reserve(17 * 3 + cubeIndices.size());

        VertexFormat v({0, 0, 0});
        v.color = glm::vec3(0.90, 0.64, 0.70);
        vertices.push_back(v);
        const float angleOffset = 360.f / 16.f;
        for (int i = 0; i <= 16; i++) {
            auto x = 0.5f * glm::cos(glm::radians((float)i * angleOffset));
            auto y = 0.5f * glm::sin(glm::radians((float)i * angleOffset));

            v.position = glm::vec3(x, 0, y);
            v.color = glm::vec3(0.90, 0.64, 0.70);
            vertices.push_back(v);
        }

        for (int i = 1; i < 17; i++) {
            indices.push_back(0);
            indices.push_back(i);
            indices.push_back(i + 1);
        }

        auto offset = vertices.size();
        auto scale = glm::scale(glm::mat4(1.f), glm::vec3(0.25f, 0.5f, 0.25f));
        auto translate = glm::translate(glm::mat4(1.f), glm::vec3(0, 1, 0));
        for (int i = 0; i < cubeVertices.size(); i++) {
            auto pos = translate * scale * glm::vec4(cubeVertices[i], 1.0f);

            v.position = glm::vec3(pos);
            v.color = glm::vec3(0.89, 0.64, 0.70);
            vertices.push_back(v);
        }

        for (int i = 0; i < cubeIndices.size(); i++) {
            indices.push_back(cubeIndices[i] + offset);
        }
    }

    tema::tema() {
        srand(time(nullptr));

        m_gridWidth = 100;
        m_gridHeight = 100;

        m_offset = 1.f;
        m_treeHeight = 0.f;

        for (int i = 0; i < 10; i++) {
            float x = (float)(rand() % 100 - 50);
            float z = (float)(rand() % 100 - 50);
            SpawnTree({x, 0, z});
        }

        m_heliPos = glm::vec3(2, 0, 2);
        m_heliDirection = glm::vec2(1, 0);
        m_shouldMove = false;

        auto cam = GetSceneCamera();

        m_camOffset = glm::vec3(0, 10, 12);
        cam->SetPositionAndRotation(
                m_heliPos + m_camOffset,
                glm::quatLookAt(-glm::normalize(m_camOffset), glm::vec3(0, 1, 0))
        );
    }

    tema::~tema() {

    }

    void tema::Init() {

        // Ground mesh
        {
            std::vector<VertexFormat> vertices;
            std::vector<unsigned int> indices;

            CreateGround(vertices, indices);

            auto *mesh = new Mesh("ground");
            mesh->InitFromData(vertices, indices);
            meshes[mesh->GetMeshID()] = mesh;
        }

        // Tree mesh
        {
            std::vector<VertexFormat> vertices;
            std::vector<unsigned int> indices;

            int indexOffset = 0;
            CreateTree(vertices, indices, glm::mat4(1.f), 0, indexOffset);

            Mesh* mesh = new Mesh("tree");
            mesh->InitFromData(vertices, indices);
            meshes[mesh->GetMeshID()] = mesh;
        }

        // Helicopter mesh
        {
            std::vector<VertexFormat> vertices;
            std::vector<unsigned int> indices;

            CreateHelicopter(vertices, indices);

            Mesh* mesh = new Mesh("helicopter");
            mesh->InitFromData(vertices, indices);
            meshes[mesh->GetMeshID()] = mesh;
        }

        // Mark mesh
        {
            std::vector<VertexFormat> vertices;
            std::vector<unsigned int> indices;

            CreateMark(vertices, indices);

            Mesh* mesh = new Mesh("mark");
            mesh->InitFromData(vertices, indices);
            meshes[mesh->GetMeshID()] = mesh;
        }


        {
            auto* texture = LoadTexture("assets\\textures\\ground.jpg");
            mapTextures["ground"] = texture;
        }

        {
            auto* texture = LoadTexture("src\\lab\\lab08\\images\\snow.jpg");
            mapTextures["snow"] = texture;
        }

        {
            auto* texture = LoadTexture("src\\lab\\lab08\\images\\noise.png");
            mapTextures["noise"] = texture;
        }

        {
            auto* texture = LoadTexture("assets\\textures\\leaves.png");
            mapTextures["leaves"] = texture;
        }

        {
            auto* texture = LoadTexture("assets\\textures\\bark.jpg");
            mapTextures["bark"] = texture;
        }

        CreateShader("ground");
        CreateShader("tree");
        CreateShader("helicopter");
        CreateShader("ground_position");
        CreateShader("mark");
        CreateShader("tree_id");

        auto resolution = window->GetResolution();
        CreateFramebuffer(resolution.x, resolution.y);
    }

    void tema::FrameStart() {
        glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferObject);
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::ivec2 resolution = window->GetResolution();

        glViewport(0, 0, resolution.x, resolution.y);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glViewport(0, 0, resolution.x, resolution.y);
    }

    void tema::Update(float deltaTimeSeconds) {
        if (m_shouldMove) {
            m_heliDirection = glm::normalize(m_clickPos - glm::vec2(m_heliPos.x, m_heliPos.z));
            m_heliPos = glm::mix(m_heliPos, glm::vec3(m_clickPos.x, 0, m_clickPos.y), deltaTimeSeconds * 3.f);

            auto cam = GetSceneCamera();

            cam->SetPositionAndRotation(
                    m_heliPos + m_camOffset,
                    glm::quatLookAt(-glm::normalize(m_camOffset), glm::vec3(0, 1, 0))
            );

            auto comp = [](float a, float b) -> bool {
                return std::abs(a - b) <= 0.001f;
            };
            auto clickPos = glm::vec3(m_clickPos.x, 0, m_clickPos.y);
            if (comp(m_heliPos.x, clickPos.x) && comp(m_heliPos.z, clickPos.z)) {
                m_shouldMove = false;
            }
        }

        auto program = shaders["ground"]->program;
        glUseProgram(program);

        auto model = glm::mat4(1.f);
        const auto floatWidth = static_cast<float>(m_gridWidth);
        const auto floatHeight = static_cast<float>(m_gridHeight);
        model = glm::translate(model, glm::vec3(-floatWidth * m_offset / 2.f, 0.f, -floatHeight * m_offset / 2.f));
        glUniformMatrix4fv(glGetUniformLocation(program, "Model"), 1, GL_FALSE, glm::value_ptr(model));

        auto view = GetSceneCamera()->GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(program, "View"), 1, GL_FALSE, glm::value_ptr(view));

        auto projection = GetSceneCamera()->GetProjectionMatrix();
        glUniformMatrix4fv(glGetUniformLocation(program, "Projection"), 1, GL_FALSE, glm::value_ptr(projection));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mapTextures["noise"]->GetTextureID());
        glUniform1i(glGetUniformLocation(program, "noiseTex"), 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, mapTextures["ground"]->GetTextureID());
        glUniform1i(glGetUniformLocation(program, "groundTex"), 1);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, mapTextures["snow"]->GetTextureID());
        glUniform1i(glGetUniformLocation(program, "snowTex"), 2);

        glUniform3fv(glGetUniformLocation(program, "heliPos"), 1, glm::value_ptr(m_heliPos));

        auto* mesh = meshes["ground"];
        glBindVertexArray(mesh->GetBuffers()->m_VAO);
        glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferObject);
        program = shaders["ground_position"]->program;
        glUseProgram(program);

        glUniformMatrix4fv(glGetUniformLocation(program, "Model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(program, "View"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(program, "Projection"), 1, GL_FALSE, glm::value_ptr(projection));

        glUniform3fv(glGetUniformLocation(program, "heliPos"), 1, glm::value_ptr(m_heliPos));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mapTextures["noise"]->GetTextureID());
        glUniform1i(glGetUniformLocation(program, "noiseTex"), 0);

        glBindVertexArray(mesh->GetBuffers()->m_VAO);
        glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        for (int i = 0; i < m_treePositions.size(); i++) {
            glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferObject);
            program = shaders["tree_id"]->program;
            glUseProgram(program);

            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, mapTextures["noise"]->GetTextureID());
            glUniform1i(glGetUniformLocation(program, "noiseTex"), 2);

            glUniformMatrix4fv(glGetUniformLocation(program, "View"), 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(glGetUniformLocation(program, "Projection"), 1, GL_FALSE, glm::value_ptr(projection));

            glUniform1f(glGetUniformLocation(program, "treeHeight"), m_treeHeight);
            if (!m_treeAlive[i]) {
                continue;
            }

            glUniformMatrix4fv(glGetUniformLocation(program, "Model"), 1, GL_FALSE, glm::value_ptr(m_treePositions[i]));
            glUniform1i(glGetUniformLocation(program, "treeId"), m_treeIds[i]);

            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, mapTextures["noise"]->GetTextureID());
            glUniform1i(glGetUniformLocation(program, "noiseTex"), 2);

            glUniform3fv(glGetUniformLocation(program, "heliPos"), 1, glm::value_ptr(m_heliPos));

            mesh = meshes["tree"];
            glBindVertexArray(mesh->GetBuffers()->m_VAO);
            glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);

            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            program = shaders["tree"]->program;
            glUseProgram(program);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, mapTextures["bark"]->GetTextureID());
            glUniform1i(glGetUniformLocation(program, "barkTex"), 0);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, mapTextures["leaves"]->GetTextureID());
            glUniform1i(glGetUniformLocation(program, "leavesTex"), 1);

            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, mapTextures["noise"]->GetTextureID());
            glUniform1i(glGetUniformLocation(program, "noiseTex"), 2);

            glUniformMatrix4fv(glGetUniformLocation(program, "View"), 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(glGetUniformLocation(program, "Projection"), 1, GL_FALSE, glm::value_ptr(projection));

            glUniform1f(glGetUniformLocation(program, "treeHeight"), m_treeHeight);
            glUniform3fv(glGetUniformLocation(program, "heliPos"), 1, glm::value_ptr(m_heliPos));
            if (!m_treeAlive[i]) {
                continue;
            }

            glUniformMatrix4fv(glGetUniformLocation(program, "Model"), 1, GL_FALSE, glm::value_ptr(m_treePositions[i]));

            mesh = meshes["tree"];
            glBindVertexArray(mesh->GetBuffers()->m_VAO);
            glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
        }

        program = shaders["helicopter"]->program;
        glUseProgram(program);

        auto translate = glm::translate(glm::mat4(1.f), m_heliPos);
        auto angle = atan2(m_heliDirection.y, m_heliDirection.x);
        auto rotation = glm::rotate(glm::mat4(1.f), -angle, glm::vec3(0, 1, 0));
        rotation = glm::rotate(rotation, glm::radians(-90.f), glm::vec3(0, 1, 0));
        glUniformMatrix4fv(glGetUniformLocation(program, "rotation"), 1, GL_FALSE, glm::value_ptr(rotation));
        glUniformMatrix4fv(glGetUniformLocation(program, "translation"), 1, GL_FALSE, glm::value_ptr(translate));
        glUniformMatrix4fv(glGetUniformLocation(program, "View"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(program, "Projection"), 1, GL_FALSE, glm::value_ptr(projection));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mapTextures["noise"]->GetTextureID());
        glUniform1i(glGetUniformLocation(program, "noiseTex"), 0);

        glUniform1f(glGetUniformLocation(program, "time"), Engine::GetElapsedTime());
        glUniform3fv(glGetUniformLocation(program, "heliPos"), 1, glm::value_ptr(m_heliPos));

        mesh = meshes["helicopter"];
        glBindVertexArray(mesh->GetBuffers()->m_VAO);
        glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);

        if (m_shouldMove) {
            program = shaders["mark"]->program;
            glUseProgram(program);

            model = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, 0));
            glUniformMatrix4fv(glGetUniformLocation(program, "Model"), 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(glGetUniformLocation(program, "View"), 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(glGetUniformLocation(program, "Projection"), 1, GL_FALSE, glm::value_ptr(projection));

            glUniform1f(glGetUniformLocation(program, "time"), Engine::GetElapsedTime());
            glUniform2fv(glGetUniformLocation(program, "markPos"), 1, glm::value_ptr(m_clickPos));
            glUniform3fv(glGetUniformLocation(program, "heliPos"), 1, glm::value_ptr(m_heliPos));

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, mapTextures["noise"]->GetTextureID());
            glUniform1i(glGetUniformLocation(program, "noiseTex"), 0);

            mesh = meshes["mark"];
            glBindVertexArray(mesh->GetBuffers()->m_VAO);
            glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
        }
    }

    void tema::FrameEnd() {
        //DrawCoordinateSystem();
    }

    Texture2D* tema::LoadTexture(const char* imagePath)
    {
        int width, height, channels;
        unsigned char* imageData = stbi_load(imagePath, &width, &height, &channels, 0);

        return CreateTexture(width, height, channels, imageData);
    }

    Texture2D *tema::CreateTexture(unsigned int width, unsigned int height,
                                    unsigned int channels, unsigned char* data)
    {
        GLuint textureID = 0;
        unsigned int size = width * height * channels;

        // TODO(student): Generate and bind the new texture ID
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        if (GLEW_EXT_texture_filter_anisotropic) {
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
        }
        // TODO(student): Set the texture parameters (MIN_FILTER and MAG_FILTER) using glTexParameteri
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        CheckOpenGLError();

        // TODO(student): Use the "glTexImage2D" directive to load the information
        // into the graphics processor's memory. Use the correct format based on
        // the number of components:
        //   - 1 color channel - GL_RED
        //   - 2 color channels - GL_RG
        //   - 3 color channels - GL_RGB
        //   - 4 color channels - GL_RGBA
        int glChannels;
        if (channels == 1) {
            glChannels = GL_RED;
        } else if (channels == 2) {
            glChannels = GL_RG;
        } else if (channels == 3) {
            glChannels = GL_RGB;
        } else if (channels == 4) {
            glChannels = GL_RGBA;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, glChannels, width, height, 0, glChannels, GL_UNSIGNED_BYTE, data);

        // TODO(student): Generate texture mip-maps
        glGenerateMipmap(GL_TEXTURE_2D);

        CheckOpenGLError();

        // Save the texture into a wrapper Texture2D class for using easier later during rendering phase
        Texture2D* texture = new Texture2D();
        texture->Init(textureID, width, height, channels);

        SAFE_FREE_ARRAY(data);
        return texture;
    }

    void tema::CreateFramebuffer(int width, int height)
    {
        glGenFramebuffers(1, &m_framebufferObject);
        glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferObject);

        glGenTextures(1, &m_colorTexture);
        glBindTexture(GL_TEXTURE_2D, m_colorTexture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);

        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_colorTexture, 0);

        glGenTextures(1, &m_depthTexture);
        glBindTexture(GL_TEXTURE_2D, m_depthTexture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_depthTexture, 0);

        std::vector<GLenum> draw_textures;
        draw_textures.push_back(GL_COLOR_ATTACHMENT0);
        glDrawBuffers(draw_textures.size(), &draw_textures[0]);

        glCheckFramebufferStatus(GL_FRAMEBUFFER);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void tema::CreateShader(const std::string& name) {
        auto path = PATH_JOIN(window->props.selfDir, "src/lab", "tema", "shaders");
        auto shader = new Shader(name);
        shader->AddShader(PATH_JOIN(path, name + ".VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(path, name + ".FS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    void tema::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {
        if (!IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT)) {
            return;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferObject);
        glReadBuffer(GL_COLOR_ATTACHMENT0);

        int x = mouseX;
        int y = window->GetResolution().y - mouseY;

        float data[4];
        glReadPixels(x, y, 1, 1, GL_RGBA, GL_FLOAT, data);

        if (data[3] == 1.0) {
            m_clickPos.x = data[0];
            m_clickPos.y = data[2];

            m_shouldMove = true;
        } else {
            for (int i = 0; i < m_treeAlive.size(); i++) {
                if (m_treeIds[i] == (int)(data[3] * 255.f)) {
                    m_treeAlive[i] = false;

                    float xPos = (float)(rand() % 100 - 50);
                    float yPos = (float)(rand() % 100 - 50);
                    SpawnTree({xPos, 0, yPos});
                    break;
                }
            }
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void tema::SpawnTree(glm::vec3 position) {
        static int id = 0;
        auto translate = glm::translate(glm::mat4(1.f), position);

        m_treePositions.push_back(translate);
        m_treeIds.push_back(id++);
        m_treeAlive.push_back(true);
    }
};