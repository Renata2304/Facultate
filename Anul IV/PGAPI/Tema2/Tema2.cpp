#include "lab_m2/Tema2/Tema2.h"

#include <vector>
#include <iostream>
#include "pfd/portable-file-dialogs.h"

using namespace std;
using namespace m2;

Tema2::Tema2()
{
    outputMode = 0;
    gpuProcessing = true;
    saveScreenToImage = false;
    window->SetSize(600, 600);
    color_texture1 = 0; color_texture2 = 0; color_texture3 = 0; color_texture4 = 0; color_texture5 = 0;
    framebuffer_object1 = 0; framebuffer_object2 = 0; framebuffer_object3 = 0; framebuffer_object4 = 0; framebuffer_object5 = 0;

    isFramebufferActive = false;
    isHasActive = false;
	isSobelActive = true;
}

Tema2::~Tema2() {}

void Tema2::Init()
{
    // Load default texture for image processing
    originalImage = TextureManager::LoadTexture(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES, "cube", "pos_x.png"), nullptr, "image", true, true);
    processedImage = TextureManager::LoadTexture(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES, "cube", "pos_x.png"), nullptr, "newImage", true, true);

    // Load the mesh
    Mesh* mesh = new Mesh("quad");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "quad.obj");
    mesh->UseMaterials(false);
    meshes[mesh->GetMeshID()] = mesh;

    // Load the shader
    std::string shaderPath = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema2", "shaders");
    Shader* shader = new Shader("ImageProcessing");
    shader->AddShader(PATH_JOIN(shaderPath, "VertexShader.glsl"), GL_VERTEX_SHADER);
    shader->AddShader(PATH_JOIN(shaderPath, "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
    shader->CreateAndLink();
    shaders[shader->GetName()] = shader;

    float aspectRatio = static_cast<float>(originalImage->GetWidth()) / originalImage->GetHeight();
    window->SetSize(static_cast<int>(600 * aspectRatio), 600);

    // Get the new resolution of the window
    glm::ivec2 resolution = window->GetResolution();
    CreateFramebuffer(framebuffer_object1, color_texture1, resolution.x, resolution.y);
    CreateFramebuffer(framebuffer_object2, color_texture2, resolution.x, resolution.y);
    CreateFramebuffer(framebuffer_object3, color_texture3, resolution.x, resolution.y);
    CreateFramebuffer(framebuffer_object4, color_texture4, resolution.x, resolution.y);
    CreateFramebuffer(framebuffer_object5, color_texture5, resolution.x, resolution.y);
}

void Tema2::FrameStart() {}

void Tema2::CreateFramebuffer(GLuint& framebuffer, GLuint& texture, int width, int height)
{
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    // Generate, bind, and initialize the color texture
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Attach the color texture to the framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    // Set the list of draw buffers
    GLenum draw_buffers[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, draw_buffers);

    // Check the framebuffer status
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        cerr << "Framebuffer is not complete!" << endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Tema2::Update(float deltaTimeSeconds)
{
    ClearScreen();

    auto shader = shaders["ImageProcessing"];
    shader->Use();

    if (saveScreenToImage) {
        window->SetSize(originalImage->GetWidth(), originalImage->GetHeight());
    }

    int flip_loc = shader->GetUniformLocation("flipVertical");
    glUniform1i(flip_loc, saveScreenToImage ? 0 : 1);

    int screenSize_loc = shader->GetUniformLocation("screenSize");
    glm::ivec2 resolution = window->GetResolution();
    glUniform2i(screenSize_loc, resolution.x, resolution.y);

    int outputMode_loc = shader->GetUniformLocation("outputMode");
    glUniform1i(outputMode_loc, outputMode);

    int locTexture = shader->GetUniformLocation("textureImage");
    glUniform1i(locTexture, 0);

    auto textureImage = gpuProcessing ? originalImage : processedImage;
    textureImage->BindToTextureUnit(GL_TEXTURE0);

    if (isFramebufferActive) {
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_object1);
        glClear(GL_COLOR_BUFFER_BIT);

        glUniform1i(outputMode_loc, 2);
        originalImage->BindToTextureUnit(GL_TEXTURE0);
        RenderMesh(meshes["quad"], shader, glm::mat4(1));

        // Vertical Blur Pass
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_object2);
        glClear(GL_COLOR_BUFFER_BIT);

        glUniform1i(outputMode_loc, 3);
        glBindTexture(GL_TEXTURE_2D, color_texture1);
        RenderMesh(meshes["quad"], shader, glm::mat4(1));

        glUniform1i(outputMode_loc, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, color_texture2);
        RenderMesh(meshes["quad"], shader, glm::mat4(1));
    }
    else if (isHasActive) {
        // Horizontal Blur Pass
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_object1);
        glClear(GL_COLOR_BUFFER_BIT);

        glUniform1i(outputMode_loc, 2);
        originalImage->BindToTextureUnit(GL_TEXTURE0); 
        RenderMesh(meshes["quad"], shader, glm::mat4(1));

        // Vertical Blur Pass
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_object2);
        glClear(GL_COLOR_BUFFER_BIT);

        glUniform1i(outputMode_loc, 3);
        glBindTexture(GL_TEXTURE_2D, color_texture1);
        RenderMesh(meshes["quad"], shader, glm::mat4(1));

        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_object5);
        glClear(GL_COLOR_BUFFER_BIT);

        glUniform1i(outputMode_loc, outputMode);
        glBindTexture(GL_TEXTURE_2D, color_texture2);
        RenderMesh(meshes["quad"], shader, glm::mat4(1));

        glUniform1i(outputMode_loc, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, color_texture5);
        RenderMesh(meshes["quad"], shader, glm::mat4(1));
    }
    else if (isSobelActive) {
        // Horizontal Blur Pass
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_object1);
        glClear(GL_COLOR_BUFFER_BIT);

        glUniform1i(outputMode_loc, 2);
        originalImage->BindToTextureUnit(GL_TEXTURE0);
        RenderMesh(meshes["quad"], shader, glm::mat4(1));

        // Vertical Blur Pass
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_object2);
        glClear(GL_COLOR_BUFFER_BIT);

        glUniform1i(outputMode_loc, 3);
        glBindTexture(GL_TEXTURE_2D, color_texture1);
        RenderMesh(meshes["quad"], shader, glm::mat4(1));

        // Hatching Pass
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_object4);
        glClear(GL_COLOR_BUFFER_BIT);

        glUniform1i(outputMode_loc, 7);
        glBindTexture(GL_TEXTURE_2D, color_texture2);
        RenderMesh(meshes["quad"], shader, glm::mat4(1));


        // Sobel Filter Pass
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_object3);
        glClear(GL_COLOR_BUFFER_BIT);

        glUniform1i(outputMode_loc, 1);
        originalImage->BindToTextureUnit(GL_TEXTURE0);
        RenderMesh(meshes["quad"], shader, glm::mat4(1));


        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        glUniform1i(outputMode_loc, 8);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, color_texture3); // Sobel result
        glUniform1i(shader->GetUniformLocation("textureSobel"), 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, color_texture4); // Hatching result
        glUniform1i(shader->GetUniformLocation("textureHatching"), 1);

        RenderMesh(meshes["quad"], shader, glm::mat4(1));
    }
    else {
        glUniform1i(outputMode_loc, outputMode);
        auto textureImage = gpuProcessing ? originalImage : processedImage;
        textureImage->BindToTextureUnit(GL_TEXTURE0);
        RenderMesh(meshes["quad"], shader, glm::mat4(1));
    }

}

void Tema2::FrameEnd()
{
    DrawCoordinateSystem();
}

void Tema2::OnFileSelected(const string& fileName)
{
    if (!fileName.empty()) {
        cout << fileName << endl;

        // Load the new textures
        originalImage = TextureManager::LoadTexture(fileName, nullptr, "image", true, true);
        processedImage = TextureManager::LoadTexture(fileName, nullptr, "newImage", true, true);

        // Calculate the new aspect ratio and update the window size
        float aspectRatio = static_cast<float>(originalImage->GetWidth()) / originalImage->GetHeight();
        window->SetSize(static_cast<int>(600 * aspectRatio), 600);

        // Get the new resolution of the window
        glm::ivec2 resolution = window->GetResolution();

        // Recreate framebuffers with the new resolution
        CreateFramebuffer(framebuffer_object1, color_texture1, resolution.x, resolution.y);
        CreateFramebuffer(framebuffer_object2, color_texture2, resolution.x, resolution.y);
        CreateFramebuffer(framebuffer_object3, color_texture3, resolution.x, resolution.y);
        CreateFramebuffer(framebuffer_object4, color_texture4, resolution.x, resolution.y);
        CreateFramebuffer(framebuffer_object5, color_texture5, resolution.x, resolution.y);
    }
}

void Tema2::GrayScale()
{
    unsigned int channels = originalImage->GetNrChannels();
    unsigned char* data = originalImage->GetImageData();
    unsigned char* newData = processedImage->GetImageData();

    if (channels < 3) return;

    glm::ivec2 imageSize = glm::ivec2(originalImage->GetWidth(), originalImage->GetHeight());

    for (int i = 0; i < imageSize.y; i++) {
        for (int j = 0; j < imageSize.x; j++) {
            int offset = channels * (i * imageSize.x + j);
            char value = static_cast<char>(data[offset + 0] * 0.2f + data[offset + 1] * 0.71f + data[offset + 2] * 0.07f);
            memset(&newData[offset], value, 3);
        }
    }
    processedImage->UploadNewData(newData);
}

void Tema2::OpenDialog()
{
    vector<string> filters = { "Image Files", "*.png *.jpg *.jpeg *.bmp", "All Files", "*" };
    auto selection = pfd::open_file("Select a file", ".", filters).result();
    if (!selection.empty()) {
        cout << "User selected file " << selection[0] << "\n";
        OnFileSelected(selection[0]);
    }
}

void Tema2::OnInputUpdate(float deltaTime, int mods) {}

void Tema2::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_F || key == GLFW_KEY_ENTER || key == GLFW_KEY_SPACE) {
        OpenDialog();
    }
    if (key == GLFW_KEY_E) {
        gpuProcessing = !gpuProcessing;
        if (!gpuProcessing) outputMode = 0;
        cout << "Processing on GPU: " << (gpuProcessing ? "true" : "false") << endl;
    }
    if (key >= GLFW_KEY_0 && key <= GLFW_KEY_9) {
        outputMode = key - GLFW_KEY_0;
        if (!gpuProcessing) {
            outputMode = 0;
            GrayScale();
        }
        isFramebufferActive = (key == GLFW_KEY_3);
        isSobelActive = (key == GLFW_KEY_8);
        isHasActive = (key == GLFW_KEY_4 || key == GLFW_KEY_5 || key == GLFW_KEY_6 || key == GLFW_KEY_7);
    }
}

void Tema2::OnKeyRelease(int key, int mods) {}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {}

void Tema2::OnWindowResize(int width, int height) {}
