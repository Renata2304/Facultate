#pragma once

#include <vector>

#include "components/simple_scene.h"
namespace m1
{
    struct HexStruct {
        glm::vec2 coords;   // x, y
        bool toBeDel;
        float del;
        int color;                    // transX, color, nrHits
        int nrHits;
        float r;
    };

    struct StarShotStruct {
        glm::vec2 coords;
        int color;
        float del;
        bool toBeDel;
        float r;
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

        void CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;


        // new
        void InitBoard();
        void BuildSquare(float x, float y, int nr);
        void BuildDiamond(float x, float y, int nr, int caseBD, float deltaTimeSeconds, int i);
        void BuildHexagon(float &x, float y, int nr, int caseBD, float deltaTimeSeconds, float &del, int nrHits);
        void BuildPoints(float x, float y, int starCase);
        void BuildHeart(float x, float y);
        void BuildStar(float &x, float y, int nr, int caseBD, float deltaTimeSeconds, float& del);
        void MoveDiamond(float x, float y, int nr);

        int GetSquare();

        void GeneratePoints();
        void ChechCollectPoints();

        void GenerateHex();

        void GenerateStars(int i);
        void StopShootingStars(int i);

        int WhichRow(int row);

     protected:
        float cx, cy;
        glm::mat3 modelMatrix;
        float myX, myY;
        float translateX, translateY;
        float scaleX, scaleY;
        float angularStep;

        float delDiam[9];
        float rDiam = 50;
        float timer[9];

        int lives = 3;
        int totalPoints = 5;
        int cost1 = 1, cost2 = 2, cost3 = 3;
        bool lftButtonPressed = false, rghButtonPressed = false;
        int nrCase;

        int square = -1;

        std::vector<glm::vec3> board; // x, y, color
        int isOcupied[9];

        std::vector<glm::vec3> points; // x, y, isCollected

        std::vector<HexStruct> hexagons; // ((x, y), color, nrHits)
        int isHexCol[3][4];

        std::vector<StarShotStruct> stars[9];
        float randStarShootTime;

        bool toBeDel = false;

        float randHexTime;
        float randStarTime;
        bool ok = 0;

    };
}
