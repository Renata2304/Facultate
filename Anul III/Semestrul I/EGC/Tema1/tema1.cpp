#include "lab_m1/Tema1/Tema1.h"

#include <vector>
#include <iostream>
#include <random>
#include <cmath>

#include "lab_m1/tema1/transform2D.h"
#include "lab_m1/tema1/object2D.h"
#include "core/engine.h"
#include "utils/gl_utils.h"

using namespace std;
using namespace m1;


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    glm::vec3 corner = glm::vec3(0, 0, 0);
    float squareSide = 100;

    // TODO(student): Compute coordinates of a square's center, and store
    // then in the `cx` and `cy` class variables (see the header). Use
    // `corner` and `squareSide`. These two class variables will be used
    // in the `Update()` function. Think about it, why do you need them?
    cx = corner.x + squareSide / 2;
    cy = corner.y + squareSide / 2;

    // Initialize tx and ty (the translation steps)
    translateX = 0;
    translateY = 0;

    // Initialize sx and sy (the scale factors)
    scaleX = 1;
    scaleY = 1;

    // Initialize angularStep
    angularStep = 0;

    Mesh* sqr1 = object2D::CreateSquare("sqr1", corner, squareSide, glm::vec3(0.3, 1, 0.4), true);
    AddMeshToList(sqr1);
    Mesh* sqr2 = object2D::CreateSquare("sqr2", corner, squareSide, glm::vec3(0.8, 0, 0), true);
    AddMeshToList(sqr2);
    Mesh* sqr31 = object2D::CreateSquare("sqr31", corner, squareSide, glm::vec3(0, 0, 0));
    AddMeshToList(sqr31);
    Mesh* sqr32 = object2D::CreateSquare("sqr32", corner, squareSide, glm::vec3(1, 0, 0));
    AddMeshToList(sqr32);
    Mesh* sqr4 = object2D::CreateSquare("sqr4", corner, squareSide, glm::vec3(1, 0, 0), true);
    AddMeshToList(sqr4);

    Mesh* diam1 = object2D::CreateDiamond("diam1", glm::vec3(0, 0, 1), squareSide, glm::vec3(1, 0.5, 0), true);
    AddMeshToList(diam1);
    Mesh* diam2 = object2D::CreateDiamond("diam2", glm::vec3(0, 0, 1), squareSide, glm::vec3(0.3, 0.6, 0.9), true);
    AddMeshToList(diam2);
    Mesh* diam3 = object2D::CreateDiamond("diam3", glm::vec3(0, 0, 1), squareSide, glm::vec3(1, 1, 0), true);
    AddMeshToList(diam3);
    Mesh* diam4 = object2D::CreateDiamond("diam4", glm::vec3(0, 0, 1), squareSide, glm::vec3(1, 0, 0.8), true);
    AddMeshToList(diam4);

    Mesh* hex11 = object2D::CreateHexagon("hex11", glm::vec3(0, 0, 2), squareSide, glm::vec3(1, 0.5, 0), true);
    AddMeshToList(hex11);
    Mesh* hex12 = object2D::CreateHexagon("hex12", glm::vec3(0, 0, 3), squareSide, glm::vec3(0.7, 0.2, 1), true);
    AddMeshToList(hex12);

    Mesh* hex21 = object2D::CreateHexagon("hex21", glm::vec3(0, 0, 2), squareSide, glm::vec3(0.3, 0.6, 0.9), true);
    AddMeshToList(hex21);
    Mesh* hex22 = object2D::CreateHexagon("hex22", glm::vec3(0, 0, 3), squareSide, glm::vec3(0.7, 0.2, 1), true);
    AddMeshToList(hex22);

    Mesh* hex31 = object2D::CreateHexagon("hex31", glm::vec3(0, 0, 2), squareSide, glm::vec3(1, 1, 0), true);
    AddMeshToList(hex31);
    Mesh* hex32 = object2D::CreateHexagon("hex32", glm::vec3(0, 0, 3), squareSide, glm::vec3(0.7, 0.2, 1), true);
    AddMeshToList(hex32);

    Mesh* hex41 = object2D::CreateHexagon("hex41", glm::vec3(0, 0, 2), squareSide, glm::vec3(1, 0, 0.8), true);
    AddMeshToList(hex41);
    Mesh* hex42 = object2D::CreateHexagon("hex42", glm::vec3(0, 0, 3), squareSide, glm::vec3(0.7, 0.2, 1), true);
    AddMeshToList(hex42);


    Mesh* star = object2D::CreateStar("star0", corner, squareSide, glm::vec3(0.5, 0.5, 0.5), true);
    AddMeshToList(star);
    Mesh* star1 = object2D::CreateStar("star1", glm::vec3(0, 0, 1), squareSide, glm::vec3(1, 0.5, 0), true);
    AddMeshToList(star1);
    Mesh* star2 = object2D::CreateStar("star2", glm::vec3(0, 0, 1), squareSide, glm::vec3(0.3, 0.6, 0.9), true);
    AddMeshToList(star2);
    Mesh* star3 = object2D::CreateStar("star3", glm::vec3(0, 0, 1), squareSide, glm::vec3(1, 1, 0), true);
    AddMeshToList(star3);
    Mesh* star4 = object2D::CreateStar("star4", glm::vec3(0, 0, 1), squareSide, glm::vec3(1, 0, 0.8), true);
    AddMeshToList(star4);
    Mesh* point = object2D::CreateStar("point", glm::vec3(0, 0, 3), squareSide, glm::vec3(1, 0.95, 0.6), true);
    AddMeshToList(point);
    Mesh* rndStars = object2D::CreateStar("rndStar", glm::vec3(0, 0, 3), squareSide, glm::vec3(1, 0, 1), true);
    AddMeshToList(rndStars);

    Mesh* heart = object2D::CreateHeart("heart1", corner, squareSide, glm::vec3(0.7, 0.1, 0.1), true);
    AddMeshToList(heart);
    

    for (int i = 0; i < 9; i++) {
        isOcupied[i] = 0;
    }
    for (int i = 0; i < 9; i++) {
        delDiam[i] = 1;
    }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            isHexCol[i][j] = 0;
        }
    }

    randHexTime = 2;
    randStarTime = 3;
    randStarShootTime = 1;
}


void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.2, 0.2, 0.2, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);

    
}

void Tema1::BuildSquare(float x, float y, int nr) {
    char name[6] = "sqr_";
    name[3] = nr + 48;

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(x, y);
    if (nr == 2) {
        modelMatrix *= transform2D::Scale(scaleX * 0.5, scaleY * 3.5);
    }
    if (nr == 31) {
        strcpy(name, "sqr31");
    }
    if (nr == 32) {
        strcpy(name, "sqr32");
    }
    RenderMesh2D(meshes[name], shaders["VertexColor"], modelMatrix);
}

void Tema1::BuildDiamond(float x, float y, int nr, int caseBD, float deltaTimeSeconds, int i) {
    char name[6] = "diam_";
    name[4] = nr + 48;

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(x, y);
    modelMatrix *= transform2D::Scale(scaleX * 0.75, scaleY * 0.75);
    if (caseBD == 1) {
        RenderMesh2D(meshes[name], shaders["VertexColor"], modelMatrix);
        return;
    }
    if (caseBD == -1) {
        delDiam[i] -= deltaTimeSeconds;
        if (delDiam[i] >= 0) {
            modelMatrix *= transform2D::Translate(cx, cy);
            modelMatrix *= transform2D::Scale(delDiam[i], delDiam[i]);
            modelMatrix *= transform2D::Translate(-cx, -cy);
            RenderMesh2D(meshes[name], shaders["VertexColor"], modelMatrix);
        }
    }
}

void Tema1::MoveDiamond(float x, float y, int nr) {
    char name[6] = "diam_";
    name[4] = nr + 48;

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(-31, -31);
    modelMatrix *= transform2D::Translate(x, y);
    modelMatrix *= transform2D::Scale(scaleX * 0.75, scaleY * 0.75);
    RenderMesh2D(meshes[name], shaders["VertexColor"], modelMatrix);
}

void Tema1::BuildHexagon(float &x, float y, int nr, int caseBD, float deltaTimeSeconds, float &del, int nrHits) {
    // caseBD = 1 -> randez
    // caseBD = -1 -> sterg
    
    char name[6] = "hex__";
    name[3] = nr + 48;

    // outer hexagon
    glm::mat3 modelMatrixO = glm::mat3(1);
    modelMatrixO *= transform2D::Translate(x, y);
    modelMatrixO *= transform2D::Translate(cx, cy);
    modelMatrixO *= transform2D::Scale(scaleX * 0.3, scaleY * 0.3);
    modelMatrixO *= transform2D::Translate(-cx, -cy);

    float zoomOut = 0.2 - nrHits * 0.05;
    int pozx = 5 +  2.5 * nrHits, pozy = 5 + 2.5 * nrHits;
    // inner hegagon
    glm::mat3 modelMatrixI = glm::mat3(1);
    modelMatrixI *= transform2D::Translate(x - pozx, y - pozy);
    modelMatrixI *= transform2D::Translate(cx, cy);
    modelMatrixI *= transform2D::Scale(scaleX * zoomOut, scaleY * zoomOut);
    modelMatrixI *= transform2D::Translate(-cx, -cy);
    
    if (caseBD == 1) {
        x -= deltaTimeSeconds * 85;
        name[4] = 49;
        RenderMesh2D(meshes[name], shaders["VertexColor"], modelMatrixO);
        name[4] = 50;
        RenderMesh2D(meshes[name], shaders["VertexColor"], modelMatrixI);
        return;
    }
    if (caseBD == -1) { // delete
        del -= deltaTimeSeconds;
        
        name[4] = 49;
        modelMatrixO *= transform2D::Scale(del, del);
        RenderMesh2D(meshes[name], shaders["VertexColor"], modelMatrixO);
        name[4] = 50;
        modelMatrixI *= transform2D::Scale(del, del);
        RenderMesh2D(meshes[name], shaders["VertexColor"], modelMatrixI);
    }
}

void Tema1::BuildPoints(float x, float y, int starCase) {
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(x, y);

    char name[6];
    if (starCase == 0) {
        strcpy(name, "star0");
        modelMatrix *= transform2D::Scale(scaleX * 0.2, scaleY * 0.2);
    }
    else {
        strcpy(name, "point");
        modelMatrix *= transform2D::Scale(scaleX * 0.3, scaleY * 0.3);
    }
    
    RenderMesh2D(meshes[name], shaders["VertexColor"], modelMatrix);
}


void Tema1::BuildStar(float &x, float y, int nr, int caseBD, float deltaTimeSeconds, float& del) {
    char name[6] = "star_";
    name[4] = nr + 48;
    
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(x - 60, y);
    modelMatrix *= transform2D::Translate(cx, cy);
    modelMatrix *= transform2D::Scale(scaleX * 0.25, scaleY * 0.25);
    modelMatrix *= transform2D::Translate(-cx, -cy);
    
    if (caseBD == 1) {
        modelMatrix *= transform2D::Rotate(angularStep);
        RenderMesh2D(meshes[name], shaders["VertexColor"], modelMatrix);
        return;
    }
    if (caseBD == -1) { // delete
        del -= deltaTimeSeconds;
        modelMatrix *= transform2D::Scale(del, del);
        RenderMesh2D(meshes[name], shaders["VertexColor"], modelMatrix);
    }
}


void Tema1::BuildHeart(float x, float y) {
    char name[7] = "heart1";
    
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(x, y);
    modelMatrix *= transform2D::Scale(scaleX * 4, scaleY * 4);
    RenderMesh2D(meshes[name], shaders["VertexColor"], modelMatrix);
}


void Tema1::InitBoard() {
    
    // the 9 green squares
    BuildSquare(90, 30, 1);
    board.push_back(glm::vec3(100, 40, -1));
    BuildSquare(90, 155, 1);
    board.push_back(glm::vec3(100, 165, -1));
    BuildSquare(90, 280, 1);
    board.push_back(glm::vec3(100, 290, -1));
    BuildSquare(215, 30, 1);
    board.push_back(glm::vec3(225, 40, -1));
    BuildSquare(215, 155, 1);
    board.push_back(glm::vec3(225, 165, -1));
    BuildSquare(215, 280, 1);
    board.push_back(glm::vec3(225, 290, -1));
    BuildSquare(340, 30, 1);
    board.push_back(glm::vec3(350, 40, -1));
    BuildSquare(340, 155, 1);
    board.push_back(glm::vec3(350, 165, -1));
    BuildSquare(340, 280, 1);
    board.push_back(glm::vec3(350, 290, -1));


    // the red rectagle
    BuildSquare(15, 30, 2);

    // top squeres and diamonds
    if (totalPoints >= cost1)
        BuildSquare(50, 600, 31);
    else 
        BuildSquare(50, 600, 32);
    BuildDiamond(80, 610, 1, 1, 0, -1);

    if (totalPoints >= cost2)
        BuildSquare(180, 600, 31);
    else
        BuildSquare(180, 600, 32);
    BuildDiamond(210, 610, 2, 1, 0, -1);

    if (totalPoints >= cost2)
        BuildSquare(310, 600, 31);
    else
        BuildSquare(310, 600, 32);
    BuildDiamond(340, 610, 3, 1, 0, -1);

    if (totalPoints >= cost3)
        BuildSquare(440, 600, 31);
    else    
        BuildSquare(440, 600, 32);
    BuildDiamond(460, 610, 4, 1, 0, -1);
    
    // Stars
    BuildPoints(70, 570, 0);
    BuildPoints(200, 570, 0);
    BuildPoints(240, 570, 0);
    BuildPoints(330, 570, 0);
    BuildPoints(370, 570, 0);
    BuildPoints(450, 570, 0);
    BuildPoints(490, 570, 0);
    BuildPoints(530, 570, 0);

}


int Tema1::GetSquare() {
    if (myX >= 90 && myX <= 185 && myY >= 40 && myY <= 150)
        return 0;
    if (myX >= 90 && myX <= 185 && myY >= 165 && myY <= 275)
        return 1;
    if (myX >= 90 && myX <= 185 && myY >= 290 && myY <= 400)
        return 2;

    if (myX >= 215 && myX <= 310 && myY >= 40 && myY <= 150)
        return 3;
    if (myX >= 215 && myX <= 310 && myY >= 165 && myY <= 275)
        return 4;
    if (myX >= 215 && myX <= 310 && myY >= 290 && myY <= 400)
        return 5;

    if (myX >= 340 && myX <= 435 && myY >= 40 && myY <= 150)
        return 6;
    if (myX >= 340 && myX <= 435 && myY >= 165 && myY <= 275)
        return 7;
    if (myX >= 340 && myX <= 435 && myY >= 290 && myY <= 400)
        return 8;


    return 10;
}


void Tema1::GeneratePoints() {
    int pointsMax = 3;
    if (20 - totalPoints <= 3)
        pointsMax = 20 - totalPoints;
    for (int i = 0; i < pointsMax; i++) {
        glm::vec3 newStar = glm::vec3(0);
        glm::ivec2 resolution = window->GetResolution();
        newStar.x = rand() % (int)resolution.x;
        newStar.y = rand() % (int)resolution.y;
        newStar.z = 0; // not collected
        points.emplace_back(newStar);
    }
}

void Tema1::ChechCollectPoints() {
    for (int i = 0; i < points.size(); i++) {
        int centreX = points[i].x;
        int centreY = points[i].y;

        if (myX >= centreX - 30 && myX <= centreX + 30 && myY >= centreY - 30 && myY <= centreY + 30) {
            points[i].z = 1;
            totalPoints++;
        }
    }
}

void Tema1::GenerateHex() {
    HexStruct newHex;
    newHex.coords.x = 1300;
    int row = rand() % 3;
    newHex.coords.y = 300 - row * 130 ;
    newHex.color = rand() % 4 + 1;
    newHex.nrHits = 0;
    newHex.r = 3;

    isHexCol[WhichRow(newHex.coords.y)][(int)newHex.color - 1]++;

    newHex.del = 1;
    newHex.toBeDel = false;

    hexagons.emplace(hexagons.end(), newHex);
}

void Tema1::GenerateStars(int i) {
    StarShotStruct newStar;
    newStar.coords.x = board.at(i).x + 100;
    newStar.coords.y = board.at(i).y + 50;
    newStar.color = board.at(i).z;
    newStar.del = 1;
    newStar.toBeDel = false;
    newStar.r = 2.5;

    stars[i].emplace(stars[i].end(), newStar);
}


void Tema1::StopShootingStars(int i) {
    for (auto starCrt = stars[i].begin(); starCrt != stars[i].end(); ++starCrt) {
        starCrt = stars[i].erase(starCrt);
        if (starCrt == stars[i].end())
            return;
    }
}


int Tema1::WhichRow(int row) {
    if (row == 300) // row3
        return 2;
    if (row == 170) // row2
        return 1;
    if (row == 40)  // row1
        return 0;
}


void Tema1::Update(float deltaTimeSeconds)
{
    //angularStep += deltaTimeSeconds;
    InitBoard();
    translateX -= deltaTimeSeconds * 100;
    translateY -= deltaTimeSeconds * 100;

    angularStep -= deltaTimeSeconds * 4;
    
    randHexTime -= deltaTimeSeconds;
    randStarTime -= deltaTimeSeconds;

    randStarShootTime -= deltaTimeSeconds;

    // detecting the square
    if (lftButtonPressed && nrCase != 0) {
        square = -1;
        if (GetSquare() < 10) {
            if (isOcupied[GetSquare()] == 0) {
                square = GetSquare();
            }
        }
    }

    // moving the diamonds
    if (lftButtonPressed && nrCase != 0) {
        if (nrCase == 1 && totalPoints >= cost1)
            MoveDiamond(myX, myY, nrCase); 
        if ((nrCase == 2 || nrCase == 3) && totalPoints >= cost2)
            MoveDiamond(myX, myY, nrCase);
        if (nrCase == 4  && totalPoints >= cost3)
            MoveDiamond(myX, myY, nrCase);
    }

    // placing diamonds
    if (!lftButtonPressed && nrCase != 0) {
        int costCrt;
        if (nrCase == 1 && totalPoints >= cost1) {
            ok = 1;
            costCrt = cost1;
        }
        else if ((nrCase == 2 || nrCase == 3) && totalPoints >= cost1) {
            ok = 1;
            costCrt = cost2;
        }
        else if (nrCase == 4 && totalPoints >= cost3) {
            ok = 1;
            costCrt = cost3;
        }
        else {
            ok = 0;
        }
        if (ok) {
            if (square != -1) {
                isOcupied[square] = 1;
                board.at(square).z = nrCase;
                totalPoints -= costCrt;
            }
        }
        square = -1;
    }

    // mark diamond as to be deleted
    if (rghButtonPressed) {
        square = -1;
        if (GetSquare() < 10) {
            if (isOcupied[GetSquare()] != 0) {
                square = GetSquare();
                toBeDel = true;
            }
        }
        rghButtonPressed = false;
    }

    if (!rghButtonPressed && toBeDel) {
        rghButtonPressed = false;
        isOcupied[square] = 2;
        square = -1;
        toBeDel = false;
    }

    // printing the diamonds
    for (int i = 0; i < 9; i++) {
        if (isOcupied[i] == 1) {
            BuildDiamond(board.at(i).x, board.at(i).y, board.at(i).z, 1, 0, -1);
        }
        if (isOcupied[i] == 2) {
            BuildDiamond(board.at(i).x, board.at(i).y, board.at(i).z, -1, deltaTimeSeconds, i);
            if (delDiam[i] <= 0) {
                isOcupied[i] = 0;
                
                delDiam[i] = 1;
            }
        }
    }

    // generate new hexagon
    if (randHexTime <= -1) {
        GenerateHex();
        randHexTime = 5 + rand() % 3 + 1;
    }

    // collision star - hexagon
    for (int sqrCrt = 0; sqrCrt < 9; sqrCrt++) {
        if (isOcupied[sqrCrt] == 1) {
            for (auto starCrt = stars[sqrCrt].begin(); starCrt != stars[sqrCrt].end(); ++starCrt) {
                for (auto hexCrt = hexagons.begin(); hexCrt != hexagons.end(); ++hexCrt) {
                    if (hexCrt->toBeDel == false && starCrt->toBeDel == false) {
                        if (WhichRow((int)hexCrt->coords.y) == sqrCrt % 3 &&
                            (int)hexCrt->color == starCrt->color &&
                            hexCrt->toBeDel == false) {
                            if (hexCrt->coords.x - starCrt->coords.x < (hexCrt->r + starCrt->r) &&
                                starCrt->coords.x < hexCrt->coords.x) {
                                starCrt->toBeDel = true;
                                hexCrt->nrHits++;
                                
                            }
                        }
                    }
                    if (hexCrt->nrHits >= 3) {
                        hexCrt->toBeDel = true;
                        break;
                    }
                }
            }
        }
    }

    // collision diamond - hexagon
    for (auto hexCrt = hexagons.begin(); hexCrt != hexagons.end(); ++hexCrt) {
        int r = WhichRow((int)hexCrt->coords.y);

        for (int i = 0; i < 3; i++) {
            int c = 3 * i + r;
            if (isOcupied[c] == 1) {
                if (hexCrt->coords.x - board.at(c).x <= (hexCrt->r + rDiam) && board.at(c).x < hexCrt->coords.x) {
                    isOcupied[c] = 2;
                }
            }
        }
    }

    // detect the hexagons that need to be deleted
    for (auto hexCrt = hexagons.begin(); hexCrt != hexagons.end(); ++hexCrt) {
        if (hexCrt->coords.x <= 35 && hexCrt->toBeDel == false) {
            hexCrt->toBeDel = true;
            lives--;
        }
    }

    // print hexagons
    for (auto hexCrt = hexagons.begin(); hexCrt != hexagons.end(); ++hexCrt) {
        if (hexCrt->toBeDel == true) {
            BuildHexagon(hexCrt->coords.x, hexCrt->coords.y, hexCrt->color, -1, deltaTimeSeconds, hexCrt->del, hexCrt->nrHits);
            if (hexCrt->del <= 0) {
                isHexCol[WhichRow((int)hexCrt->coords.y)][(int)(hexCrt->color) - 1]--;
                hexCrt = hexagons.erase(hexCrt);
                if (hexCrt == hexagons.end()) {
                    break;
                }
            }
        }
        else {
            BuildHexagon(hexCrt->coords.x, hexCrt->coords.y, hexCrt->color, 1, deltaTimeSeconds, hexCrt->del, hexCrt->nrHits);
        }
    }

    for (int i = 0; i < 9; i++) {
        timer[i] -= deltaTimeSeconds;
    }
    

    // shooting stars
    for (int i = 0; i < 9; i++) {
        int r = i % 3, c = i / 3;
        
        if (isOcupied[i] == 1 && isHexCol[r][(int)board.at(i).z - 1] > 0) {
            if (timer[i] <= -1) {
                GenerateStars(i);
                timer[i] = 2;
            }        
        }
    }

    // drawing the stars
    for (int i = 0; i < 9; i++) {
        for (auto starCrt = stars[i].begin(); starCrt != stars[i].end(); ++starCrt) {
            if (starCrt->toBeDel == false) {
                starCrt->coords.x += deltaTimeSeconds * 100;
                BuildStar(starCrt->coords.x, starCrt->coords.y - 50, starCrt->color, 1, deltaTimeSeconds, starCrt->del);
            }
            else {
                BuildStar(starCrt->coords.x, starCrt->coords.y - 50, starCrt->color, -1, deltaTimeSeconds, starCrt->del);
                if (starCrt->del <= 0) {
                    starCrt = stars[i].erase(starCrt);
                    if (starCrt == stars[i].end()) {
                        break;
                    }
                }
            }
        }
    }
    
    // print points
    int x = 800, y = 570;
    for (int i = 0; i < totalPoints; i++) {
        if (i > 0 && i % 10 == 0) {
            x = 800;
            y -= 40;
        }
        BuildPoints(x, y, 0);
        x += 40;
    }

    // add new stars for points
    if (randStarTime <= 0) {
        GeneratePoints();
        randStarTime = 6;
    }

    // print random stars for points
    for (auto starCrt = points.begin(); starCrt != points.end(); ++starCrt) {
        if (starCrt->z == 1) {
            starCrt = points.erase(starCrt);
            if (starCrt == points.end()) {
                break;
            }
        }
        if (totalPoints < 20)
            BuildPoints(starCrt->x, starCrt->y, 1);
    }
    
    // print lives
    x = 830, y = 660;
    for (int i = 0; i < lives; i++) {
        BuildHeart(x + 150 * i, y);
    }
    if (lives <= 0)
        exit(0);
}


void Tema1::FrameEnd()
{   
    
}


void m1::Tema1::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices)
{
    unsigned int VAO = 0;
    // TODO(student): Create the VAO and bind it
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO = 0;
    // TODO(student): Create the VBO and bind it
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // TODO(student): Send vertices data into the VBO buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    unsigned int IBO = 0;
    // TODO(student): Create the IBO and bind it
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    // TODO(student): Send indices data into the IBO buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    // ========================================================================
    // This section demonstrates how the GPU vertex shader program
    // receives data. It will be learned later, when GLSL shaders will be
    // introduced. For the moment, just think that each property value from
    // our vertex format needs to be sent to a certain channel, in order to
    // know how to receive it in the GLSL vertex shader.

    // Set vertex position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

    // Set vertex normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

    // Set texture coordinate attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

    // Set vertex color attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
    // ========================================================================

    // TODO(student): Unbind the VAO
    glBindVertexArray(0);
    // Check for OpenGL errors
    if (GetOpenGLError() == GL_INVALID_OPERATION)
    {
        cout << "\t[NOTE] : For students : DON'T PANIC! This error should go away when completing the tasks." << std::endl;
        cout << "\t[NOTE] : For developers : This happens because OpenGL core spec >=3.1 forbids null VAOs." << std::endl;
    }

    // Mesh information is saved into a Mesh object
    meshes[name] = new Mesh(name);
    meshes[name]->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
}


void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
    myX = mouseX;
    myY = window->GetResolution().y - mouseY;
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT)) {
        ChechCollectPoints();
        lftButtonPressed = true;
        if (mouseX >= 75 && mouseX <= 135  && mouseY >= 30 && mouseY <= 115) {
            //printf("caz1\n");
            nrCase = 1;
        } else if (mouseX >= 205 && mouseX <= 265 && mouseY >= 30 && mouseY <= 115) {
            //printf("caz2\n");
            nrCase = 2;
        } else if (mouseX >= 335 && mouseX <= 395 && mouseY >= 30 && mouseY <= 115) {
            //printf("caz3\n");
            nrCase = 3;
        } else if (mouseX >= 465 && mouseX <= 525 && mouseY >= 30 && mouseY <= 115) {
            //printf("caz4\n");
            nrCase = 4;
        }
        else {
            //lastCase = nrCase;
            nrCase = 0;
        }
    }
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
        rghButtonPressed = true;
    }
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
    if (!window->MouseHold(GLFW_MOUSE_BUTTON_LEFT)) {
        lftButtonPressed = false;
    }
    
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}
