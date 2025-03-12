#pragma once

#include <string>
#include <vector>

#include "components/simple_scene.h"
#include "components/transform.h"
#include "core/gpu/frame_buffer.h"
#include <glm/gtc/noise.hpp> 
#include <glm/glm.hpp>
#include <algorithm>
#include <glm/gtc/quaternion.hpp> 
#include "components/simple_scene.h"
#include "core/gpu/particle_effect.h"

namespace m2
{
    struct LightInfo
    {
        glm::vec3 position;
        glm::vec3 color;
        float radius;
    };

    struct Point {
        glm::vec3 position;
    };

    class Tema1 : public gfxc::SimpleScene
    {
     public:
         Tema1();
        ~Tema1();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        void LoadShader(const std::string &fileName);

        void InitPlane();
        void CreateCube();
        unsigned int UploadCubeTexture(const std::string& pos_x, const std::string& pos_y, const std::string& pos_z, const std::string& neg_x, const std::string& neg_y, const std::string& neg_z);
        void RenderCubemap();

        glm::vec3 generateBezierCurve(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, float t);
        void RenderWater();

        void LoadShaderRain(const std::string& name, const std::string& VS, const std::string& FS, const std::string& GS, bool hasGeomtery);
        void ResetParticlesRainSnow();


     private:
        FrameBuffer *frameBuffer;
        FrameBuffer *lightBuffer;
        FrameBuffer *reflectionBuffer;

        std::vector<LightInfo> lights;
        std::vector<LightInfo> stars;
        int outputType;
        float hmax = 15.5f;

        int cubeMapTextureID;
        unsigned int framebuffer_object;
        unsigned int color_texture;
        unsigned int depth_texture;
        unsigned int type;

        unsigned int reflectionTextureID;
        int reflectionFBO;

        glm::mat4 modelMatrix;
        glm::vec3 generator_position;
        GLenum polygonMode;
        int scene;
        float offset;
        int pause = 1;
        
    };
}   // namespace m2
