#pragma once

#include "components/simple_scene.h"

namespace lab {
    class tema : public gfxc::SimpleScene {
    private:
        size_t m_gridWidth;
        size_t m_gridHeight;
        float m_offset;
        float m_treeHeight;

        glm::vec2 m_clickPos;
        glm::vec3 m_heliPos;
        glm::vec2 m_heliDirection;
        glm::vec3 m_camOffset;
        bool m_shouldMove;

        std::unordered_map<std::string, Texture2D*> mapTextures;

        unsigned int m_colorTexture;
        unsigned int m_depthTexture;
        unsigned int m_framebufferObject;

        std::vector<glm::mat4> m_treePositions;
        std::vector<int> m_treeIds;
        std::vector<bool> m_treeAlive;

    public:
        tema();
        ~tema();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;

        Texture2D* LoadTexture(const char* imagePath);
        Texture2D* CreateTexture(unsigned int width, unsigned int height, unsigned int channels, unsigned char* data);

        void CreateShader(const std::string& name);
        void CreateFramebuffer(int width, int height);

        void CreateGround(std::vector<VertexFormat>& vertices, std::vector<unsigned int>& indices);
        void CreateTree(std::vector<VertexFormat>& vertices, std::vector<unsigned int>& indices, glm::mat4 model, int depth, int& indexOffset);
        void CreateHelicopter(std::vector<VertexFormat>& vertices, std::vector<unsigned int>& indices);
        void CreateMark(std::vector<VertexFormat>& vertices, std::vector<unsigned int>& indices);

        void SpawnTree(glm::vec3 position);
    };
};

