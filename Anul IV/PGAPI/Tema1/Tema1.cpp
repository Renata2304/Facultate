#include "lab_m2/Tema1/Tema1.h"

#include <vector>
#include <iostream>

#include "stb/stb_image.h"


using namespace std;
using namespace m2;


//Generates a random value between 0 and 1.
inline float Rand01()
{
    return rand() / static_cast<float>(RAND_MAX);
}

struct RainParticle
{
    glm::vec4 position;
    glm::vec4 speed;
    glm::vec4 initialPos;
    glm::vec4 initialSpeed;
    float delay;
    float initialDelay;
    float lifetime;
    float initialLifetime;

    RainParticle() {}

    RainParticle(const glm::vec4& pos, const glm::vec4& speed)
    {
        SetInitial(pos, speed);
    }

    void SetInitial(const glm::vec4& pos, const glm::vec4& speed,
        float delay = 0, float lifetime = 0)
    {
        position = pos;
        initialPos = pos;

        this->speed = speed;
        initialSpeed = speed;

        this->delay = delay;
        initialDelay = delay;

        this->lifetime = lifetime;
        initialLifetime = lifetime;
    }
};


ParticleEffect<RainParticle>* particleEffect;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema1::Tema1()
{

    frameBuffer = nullptr;

    lightBuffer = nullptr;

    outputType = -1;
}


Tema1::~Tema1()
{
}


glm::vec3 Tema1::generateBezierCurve(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, float t) {

    return  p0 * glm::pow((1.0f - t), 3.0f) +
        p1 * 3.0f * t * glm:: pow((1.0f - t), 2.0f) +
        p2 * 3.0f * glm::pow(t, 2.0f) * (1.0f - t) +
        p3 * glm::pow(t, 3.0f);
}

void Tema1::InitPlane() {
    float hmax = 4.0f;  // Înălțimea maximă a terenului
    float r = 7.9f;     // Raza de influență a curbei Bézier
    glm::vec2 center = glm::vec2(0.0f, 0.0f); // Centrul terenului
    float canalRadius = 1.3f;

    int m = 50, n = 50;

    float depth = 6.4f * r;
    float width = 6.4f * r;

    vector<VertexFormat> vertices;
    vector<unsigned int> indices;

    float stepX = width / (m - 1);
    float stepZ = depth / (n - 1);

    glm::vec3 p0(-14.0f, 10.3f, 10.0f);
    glm::vec3 p1(-11.0f, 6.0f, 8.0f);
    glm::vec3 p2(-7.0f, 1.8f, 4.5f);
    glm::vec3 p3(-4.7f, 0.1f, 2.7f);

    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            float x = -width / 2.0f + i * stepX;
            float z = -depth / 2.0f + j * stepZ;

            glm::vec3 position(x, 0.0f, z);
            glm::vec2 texCoords((float)i / (m - 1), (float)j / (n - 1));
            float h = 0.0f;
            float d = glm::length(glm::vec2(x, z)) / r;

            // Generăm terenul normal (munți, lacuri, zgomot)
            if (d < 1.0f) {
                h = (d * d / 2) * hmax;
            }
            else {
                h = (1.0f - ((2.0f - d) * (2.0f - d) / 2.0f)) * hmax;

                glm::vec2 noisePos = glm::vec2(x * 0.13f, z * 0.13f);
                float noiseValue = glm::perlin(noisePos) * 2.5f + 0.3f;
                float fineNoise = glm::perlin(noisePos * 1.5f) * 0.7f;
                h += (noiseValue + fineNoise + 0.5f) * hmax * 0.5f;
            }


            float minDist = FLT_MAX;
            glm::vec3 closestPoint;

            // Generăm curba Bézier pentru fiecare valoare de t
            for (float t = 0.0f; t <= 1.0f; t += 0.0001f) {
                glm::vec3 bezierPoint = generateBezierCurve(p0, p1, p2, p3, t);
                glm::vec2 bezier2D(bezierPoint.x, bezierPoint.z);
                glm::vec2 vertex2D(x, z);
                // Calculăm distanța de la punctul curent pe teren la curba Bézier
                float dist = glm::length(bezier2D - vertex2D);
                if (dist < minDist) {
                    minDist = dist;
                    closestPoint = bezierPoint;
                }
            }


            // Aplicăm efectul sinusoidal pentru tranziția lină
            float sinEffect = sin((glm::pi<float>() / 2.0f) - glm::clamp(minDist / canalRadius, 0.0f, 1.0f) * (glm::pi<float>() / 2.0f));

            // Calculăm înălțimea ajustată pe baza curbei Bézier
            float y_b_closest = closestPoint.y;
            h = glm::mix(y_b_closest, h, 1 - sinEffect);

            
            position.y = h;

            glm::vec3 normal(0.0f, 1.0f, 0.0f);
            glm::vec3 color = glm::vec3(0, 1, 0);

            vertices.push_back(VertexFormat(position, color));
        }
    }

    // Generăm indicii pentru triunghiuri
    for (int i = 0; i < m - 1; ++i) {
        for (int j = 0; j < n - 1; ++j) {
            int idx1 = i * n + j;
            int idx2 = i * n + j + 1;
            int idx3 = (i + 1) * n + j;
            int idx4 = (i + 1) * n + j + 1;

            indices.push_back(idx1);
            indices.push_back(idx3);
            indices.push_back(idx2);
            indices.push_back(idx2);
            indices.push_back(idx3);
            indices.push_back(idx4);
        }
    }

    // Calculăm normalele
    for (int i = 0; i < m - 1; ++i) {
        for (int j = 0; j < n - 1; ++j) {
            unsigned int idx1 = i * n + j;
            unsigned int idx2 = idx1 + 1;
            unsigned int idx3 = idx1 + n;

            glm::vec3 edge1 = vertices[idx2].position - vertices[idx1].position;
            glm::vec3 edge2 = vertices[idx3].position - vertices[idx1].position;

            glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));
            vertices[idx1].normal += normal;
            vertices[idx2].normal += normal;
            vertices[idx3].normal += normal;
        }
    }

    for (auto& vertex : vertices) {
        vertex.normal = glm::normalize(vertex.normal);
    }

    // Creăm mesh-ul
    Mesh* mesh = new Mesh("plane");
    mesh->InitFromData(vertices, indices);
    meshes[mesh->GetMeshID()] = mesh;
}


unsigned int Tema1::UploadCubeTexture(const std::string& pos_x, const std::string& pos_y, const std::string& pos_z, const std::string& neg_x, const std::string& neg_y, const std::string& neg_z)
{
    int width = 100, height = 100, chn;

    // Load texture data for each face of the cubemap
    unsigned char* data_pos_x = stbi_load(pos_x.c_str(), &width, &height, &chn, 0);
    unsigned char* data_pos_y = stbi_load(pos_y.c_str(), &width, &height, &chn, 0);
    unsigned char* data_pos_z = stbi_load(pos_z.c_str(), &width, &height, &chn, 0);
    unsigned char* data_neg_x = stbi_load(neg_x.c_str(), &width, &height, &chn, 0);
    unsigned char* data_neg_y = stbi_load(neg_y.c_str(), &width, &height, &chn, 0);
    unsigned char* data_neg_z = stbi_load(neg_z.c_str(), &width, &height, &chn, 0);

    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    // Specify each face of the cubemap
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_pos_x);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_neg_x);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_pos_y);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_neg_y);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_pos_z);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_neg_z);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    // Free image data after loading
    stbi_image_free(data_pos_x);
    stbi_image_free(data_pos_y);
    stbi_image_free(data_pos_z);
    stbi_image_free(data_neg_x);
    stbi_image_free(data_neg_y);
    stbi_image_free(data_neg_z);

    return textureID;
}

void Tema1::CreateCube() {
    // Calea către resursele pentru cubemap-ul de noapte
    std::string texturePath = PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES, "cubemap_night");

    // Încarcă texturile cubemap-ului pentru fiecare față
    cubeMapTextureID = UploadCubeTexture(
        PATH_JOIN(texturePath, "neg_x.png"),
        PATH_JOIN(texturePath, "neg_y.png"),
        PATH_JOIN(texturePath, "pos_z.png"),
        PATH_JOIN(texturePath, "pos_x.png"),
        PATH_JOIN(texturePath, "pos_y.png"),
        PATH_JOIN(texturePath, "neg_z.png")
    );

    // Crează un cub pentru a afișa cubemap-ul
    {
        Mesh* cubeMesh = new Mesh("cube");
        cubeMesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        cubeMesh->UseMaterials(false);
        meshes[cubeMesh->GetMeshID()] = cubeMesh;
    }

    // Shader pentru cubemap, dacă nu este deja creat
    if (shaders.find("CubeMap") == shaders.end()) {
        std::string shaderPath = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders");
        Shader* shader = new Shader("CubeMap");
        shader->AddShader(PATH_JOIN(shaderPath, "CubeMap.VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(shaderPath, "CubeMap.FS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
}


void Tema1::ResetParticlesRainSnow() {
    unsigned int nrParticles = 15000;

    particleEffect = new ParticleEffect<RainParticle>();
    particleEffect->Generate(nrParticles, true);

    auto particleSSBO = particleEffect->GetParticleBuffer();
    RainParticle* data = const_cast<RainParticle*>(particleSSBO->GetBuffer());

    glm::vec4 generatorPosition = glm::vec4(-10.0, 10.3, 10.0, 1.0f); // Poziția generatorului
    float spread = 15.0f; // Variabilă pentru dispersie

    for (unsigned int i = 0; i < nrParticles; i++) {
        // Poziționăm particulele în jurul generatorului pe axele X și Z, dar menținem Y fix
        glm::vec4 pos = generatorPosition + glm::vec4(
            2.0f,      // Dispersie pe X
            0.0f,                                    // Y fix (nu o schimbăm, menținem valoarea Y a generatorului)
            (rand() / (float)RAND_MAX - 0.3f) * spread,      // Dispersie pe Z
            0.0f
        );

        // Generează un delay aleator pentru fiecare particulă
        float delay = (rand() % 100 / 100.0f) * 1.0f;

        // Setează poziția inițială și alte atribute ale particulei
        data[i].SetInitial(pos, glm::vec4(0.0f), delay);
    }


    particleSSBO->SetBufferData(data);
}


void Tema1::Init() {
    outputType = 0;

    // Configurarea camerei
    auto camera = GetSceneCamera();
    camera->SetPositionAndRotation(glm::vec3(3, 2, -3), glm::quat(glm::vec3(-10 * TO_RADIANS, 140 * TO_RADIANS, 0)));
    camera->Update();

    // Încărcarea texturilor
    TextureManager::LoadTexture(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES), "ground.jpg");
    TextureManager::LoadTexture(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES), "rain.png");

    if (!TextureManager::GetTexture("rain.png")) {
        std::cerr << "ERROR: Texture 'rain.png' not found or failed to load!" << std::endl;
        return;
    }

    // Inițializarea plane-ului
    InitPlane();

    // Încărcăm mesh-uri
    {
        Mesh* mesh = new Mesh("water");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        mesh->UseMaterials(false);
        meshes[mesh->GetMeshID()] = mesh;

        TextureManager::LoadTexture(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES), "white.png");
    }

    {
        CreateCube();
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        mesh->UseMaterials(false);
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("quad");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "quad.obj");
        mesh->UseMaterials(false);
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        TextureManager::LoadTexture(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES), "rain.png");
        LoadShaderRain("RainSnow", "Particle_rain_snow", "Particle_simple", "Particle", true);
        ResetParticlesRainSnow();
    }

    // Încărcăm shader-ele
    LoadShader("Render2Texture");
    LoadShader("Composition");
    LoadShader("LightPass");
    LoadShader("CubeMap");

    generator_position = glm::vec3(-17.0, 8.0, 12.0);
    offset = 0.1f;
    pause = 0;

    // Inițializăm frame buffer-urile
    auto resolution = window->GetResolution();
    frameBuffer = new FrameBuffer();
    frameBuffer->Generate(resolution.x, resolution.y, 3);

    lightBuffer = new FrameBuffer();
    lightBuffer->Generate(resolution.x, resolution.y, 1, false);

    reflectionBuffer = new FrameBuffer();
    reflectionBuffer->Generate(resolution.x, resolution.y, 1, false);

    // Generăm surse de lumină
    for (int i = 0; i < 1000; ++i) {
        LightInfo lightInfo = { glm::vec3(0), glm::vec3(0), 0 };

        lightInfo.position = glm::vec3(
            Rand01() * 70.0f - 35.0f,  // X: între -50 și 50
            Rand01() * 5.0f + 7.0f,          // Y: între 7 și 12
            Rand01() * 70.0f - 35.0f   // Z: între -50 și 50
        );
        lightInfo.color = glm::vec3(Rand01(), Rand01(), Rand01());
        lightInfo.radius = (2.0f + Rand01());

        lights.push_back(lightInfo);
    }

    for (int i = 0; i < 500; ++i) {
        LightInfo starsInfo = { glm::vec3(0), glm::vec3(0), 0 };

        starsInfo.position = glm::vec3(
            Rand01() * 100.0f - 50.0f,
            20,
            Rand01() * 100.0f - 50.0f
        );
        starsInfo.color = glm::vec3(1, 1, 0);
        starsInfo.radius = (2.0f + Rand01());

        stars.push_back(starsInfo);
    }
}



void Tema1::FrameStart()
{
    DrawCoordinateSystem();
}

void Tema1::RenderCubemap() {
    // Dezactivează scrierea adâncimii
    auto camera = GetSceneCamera();

    // Setează shader-ul pentru cubemap
    Shader* shader = shaders["CubeMap"];
    shader->Use();

    // Matricea model pentru cub
    glm::mat4 modelMatrix = glm::scale(glm::mat4(1), glm::vec3(30));

    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));

    // Legarea texturii cubemap
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTextureID);
    int loc_texture = shader->GetUniformLocation("texture_cubemap");
    glUniform1i(loc_texture, 0);

    // Redă cubul
    //meshes["cube"]->Render();
    RenderMesh(meshes["cube"], shader, glm::vec3(0, 0, 0), glm::vec3(50));
}

void Tema1::RenderWater() {
    Shader* shader = shaders["Render2Texture"];
    shader->Use();

    // Bind the reflection texture
    glActiveTexture(GL_TEXTURE0);
    reflectionBuffer->BindTexture(0, GL_TEXTURE0);
    glUniform1i(shader->GetUniformLocation("reflectionTexture"), 0);

    // Set up the water plane transformation
    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.5f, 0.0f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(20.0f, 1.0f, 20.0f));

    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(GetSceneCamera()->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(GetSceneCamera()->GetProjectionMatrix()));

    // Render the water mesh
    RenderMesh(meshes["water"], shader, glm::vec3(-0.9, 0.8, -1), glm::vec3(0.25f));
}


void Tema1::Update(float deltaTimeSeconds)
{
    ClearScreen();

    for (auto& l : lights)
    {
        constexpr float rotationSpeed = glm::radians(6.0f); // 6 degrees per second
        float rotationRadians = rotationSpeed * deltaTimeSeconds;

        glm::mat4 rotateMatrix = glm::rotate(glm::mat4(1.0f), rotationRadians, glm::vec3(0, 1, 0));
        l.position = rotateMatrix * glm::vec4(l.position, 1.0f);
    }
    // ------------------------------------------------------------------------
    // Deferred rendering pass
    {
        frameBuffer->Bind();

        RenderCubemap();
        
        {
            glEnable(GL_BLEND);
            glEnable(GL_DEPTH_TEST);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            auto shader = shaders["RainSnow"];
            if (shader->GetProgramID())
            {
                shader->Use();

                // Bind the correct texture for snow
                TextureManager::GetTexture("rain.png")->BindToTextureUnit(GL_TEXTURE0);

                // Send uniforms to the shader
                glUniform3fv(glGetUniformLocation(shader->program, "generator_position"), 1, glm::value_ptr(generator_position));
                glUniform1f(glGetUniformLocation(shader->program, "deltaTime"), deltaTimeSeconds);
                glUniform1f(glGetUniformLocation(shader->program, "offset"), offset);
                glUniform1i(glGetUniformLocation(shader->program, "pause"), pause);
                
                
                particleEffect->Render(GetSceneCamera(), shader);
            }
            
        }

        glDisable(GL_BLEND);
        auto shader = shaders["Render2Texture"];

        TextureManager::GetTexture("default.png")->BindToTextureUnit(GL_TEXTURE0);
        

        TextureManager::GetTexture("ground.jpg")->BindToTextureUnit(GL_TEXTURE0);
        RenderMesh(meshes["plane"], shader, glm::vec3(0, 0, 0), glm::vec3(1.0f));

        RenderWater();

        if (outputType == 7) {
            for (auto& l : stars)
            {
                auto model = glm::translate(glm::mat4(1), l.position);
                model = glm::scale(model, glm::vec3(0.05f)); // Scalarea pentru fiecare sferă

                // Setăm culoarea albă înainte de a randa
                TextureManager::GetTexture("white.png")->BindToTextureUnit(GL_TEXTURE0);

                RenderMesh(meshes["sphere"], shader, model);
            }
        }

    }

    // ------------------------------------------------------------------------
    // Lighting pass
    {
        glm::vec3 ambientLight(0.2f);
        //Set the initial light accumulation in each pixel to be equal to the ambient light.
        lightBuffer->SetClearColor(glm::vec4(ambientLight.x, ambientLight.y, ambientLight.z, 1.0f));
        lightBuffer->Bind();
        glClearColor(0, 0, 0, 1);

        // Enable buffer color accumulation
        glDepthMask(GL_FALSE);
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_ONE, GL_ONE);

        auto shader = shaders["LightPass"];
        shader->Use();

        {
            int texturePositionsLoc = shader->GetUniformLocation("texture_position");
            glUniform1i(texturePositionsLoc, 0);
            frameBuffer->BindTexture(0, GL_TEXTURE0);
        }

        {
            int textureNormalsLoc = shader->GetUniformLocation("texture_normal");
            glUniform1i(textureNormalsLoc, 1);
            frameBuffer->BindTexture(1, GL_TEXTURE0 + 1);
        }

        auto camera = GetSceneCamera();
        glm::vec3 cameraPos = camera->m_transform->GetWorldPosition();
        int loc_eyePosition = shader->GetUniformLocation("eye_position");
        glUniform3fv(loc_eyePosition, 1, glm::value_ptr(cameraPos));

        auto resolution = window->GetResolution();
        int loc_resolution = shader->GetUniformLocation("resolution");
        glUniform2i(loc_resolution, resolution.x, resolution.y);

        //Front face culling
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);

        for (auto& lightInfo : lights)
        {
            // TODO(student): Set the shader uniforms 'light_position', 'light_color' and 'light_radius'
            // with the values from the light source. Use shader 'shader'.
            glUniform3fv(shader->GetUniformLocation("light_position"), 1, glm::value_ptr(lightInfo.position));
            glUniform3fv(shader->GetUniformLocation("light_color"), 1, glm::value_ptr(lightInfo.color));
            glUniform1f(shader->GetUniformLocation("light_radius"), lightInfo.radius);


            // TODO(student): Draw the mesh "sphere" at the position of the light source
            // and scaled 2 times the light source radius.
            // Use RenderMesh(mesh, shader, position, scale). Use shader 'shader'.
            RenderMesh(meshes["sphere"], shader, lightInfo.position, glm::vec3(2.0f * lightInfo.radius));
        }

        glDisable(GL_CULL_FACE);

        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
    }

    // ------------------------------------------------------------------------
    // Composition pass
    {
        FrameBuffer::BindDefault();

        auto shader = shaders["Composition"];
        shader->Use();

        int outputTypeLoc = shader->GetUniformLocation("output_type");
        glUniform1i(outputTypeLoc, outputType);

        {
            int texturePositionsLoc = shader->GetUniformLocation("texture_position");
            glUniform1i(texturePositionsLoc, 1);
            frameBuffer->BindTexture(0, GL_TEXTURE0 + 1);
        }

        {
            int textureNormalsLoc = shader->GetUniformLocation("texture_normal");
            glUniform1i(textureNormalsLoc, 2);
            frameBuffer->BindTexture(1, GL_TEXTURE0 + 2);
        }

        {
            int textureColorLoc = shader->GetUniformLocation("texture_color");
            glUniform1i(textureColorLoc, 3);
            frameBuffer->BindTexture(2, GL_TEXTURE0 + 3);
        }

        {
            int textureDepthLoc = shader->GetUniformLocation("texture_depth");
            glUniform1i(textureDepthLoc, 4);
            frameBuffer->BindDepthTexture(GL_TEXTURE0 + 4);
        }

        {
            int textureLightLoc = shader->GetUniformLocation("texture_light");
            glUniform1i(textureLightLoc, 5);
            lightBuffer->BindTexture(0, GL_TEXTURE0 + 5);
        }

        // Render the object again but with different properties
        RenderMesh(meshes["quad"], shader, glm::vec3(0, 0, 0));
    }
}


void Tema1::FrameEnd()
{
    DrawCoordinateSystem();
}


void Tema1::LoadShader(const std::string &name)
{
    std::string shaderPath = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders");

    // Create a shader program for particle system
    {
        Shader *shader = new Shader(name);
        shader->AddShader(PATH_JOIN(shaderPath, name + ".VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(shaderPath, name + ".FS.glsl"), GL_FRAGMENT_SHADER);
        if (name == "Particle")
		{
			shader->AddShader(PATH_JOIN(shaderPath, name + ".GS.glsl"), GL_GEOMETRY_SHADER);
		}
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;

    }
}


void Tema1::LoadShaderRain(const std::string& name, const std::string& VS, const std::string& FS, const std::string& GS, bool hasGeomtery)
{
    std::string shaderPath = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders");

    // Create a shader program for particle system
    {
        Shader* shader = new Shader(name);
        shader->AddShader(PATH_JOIN(shaderPath, VS + ".VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(shaderPath, FS + ".FS.glsl"), GL_FRAGMENT_SHADER);
        if (hasGeomtery)
        {
            shader->AddShader(PATH_JOIN(shaderPath, GS + ".GS.glsl"), GL_GEOMETRY_SHADER);
        }

        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    // Treat continuous update based on input
}


void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event

    // These are the key mappings for compositing different passes.
    // What does each key seem to activate? Where can you find the
    // answer? Examine the source code to find out!
    int index = key - GLFW_KEY_0;
    if (index >= 0 && index <= 9)
    {
        outputType = index;
    }
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
    // Treat mouse scroll event
}


void Tema1::OnWindowResize(int width, int height)
{
    // Treat window resize event
    frameBuffer->Resize(width, height, 32);
    lightBuffer->Resize(width, height, 32);
}
