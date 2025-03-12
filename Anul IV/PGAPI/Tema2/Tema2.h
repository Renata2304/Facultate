#pragma once

#include <string>

#include "components/simple_scene.h"
#include "core/gpu/frame_buffer.h"


namespace m2
{
    class Tema2 : public gfxc::SimpleScene
    {
     public:
         Tema2();
        ~Tema2();

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

        void OpenDialog();
        void OnFileSelected(const std::string &fileName);

        // Processing effects
        void GrayScale();
        void SaveImage(const std::string &fileName);

        void CreateFramebuffer(GLuint& framebuffer, GLuint& texture, int width, int height);

     private:
        Texture2D *originalImage;
        Texture2D *processedImage;

        int outputMode;
        bool gpuProcessing;
        bool saveScreenToImage;

        unsigned int framebuffer_object;
        unsigned int color_texture;
        unsigned int depth_texture;

        bool isFramebufferActive;
        bool isSobelActive;
        bool isHasActive;

        GLuint framebuffer_object1, framebuffer_object2, framebuffer_object3, framebuffer_object4, framebuffer_object5;
        GLuint color_texture1, color_texture2, color_texture3, color_texture4, color_texture5;


    };
}   // namespace m2
