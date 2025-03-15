#include "object2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"
#include <math.h>


Mesh* object2D::CreateSquare(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* object2D::CreateDiamond(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        // diamond shape
        VertexFormat(corner + glm::vec3(length / 4, 0, 0), color), //C
        VertexFormat(corner + glm::vec3(0, length / 2, 0), color), // B
        VertexFormat(corner + glm::vec3(length / 4, length, 0), color), // A

        // square
        VertexFormat(corner + glm::vec3(length * 7 / 16, length * 5 / 8, 0), color), // I
        VertexFormat(corner + glm::vec3(length * 3 / 4, length * 5 / 8, 0), color), // F
        VertexFormat(corner + glm::vec3(length * 3 / 4, length * 3 / 8, 0), color), // G
        VertexFormat(corner + glm::vec3(length * 7 / 16, length * 3 / 8, 0), color), // J

        VertexFormat(corner + glm::vec3(length / 2, length / 2, 0), color), // D

    };

    Mesh* diamond = new Mesh(name);
    std::vector<unsigned int> indices = { 
        0, 1, 7,
        2, 7, 1,
        1, 0, 6,
        3, 4, 5, 
        6, 4, 5,
        3, 6, 5,
        3, 6, 4
    };

        if (!fill) {
            diamond->SetDrawMode(GL_LINE_LOOP);
        }
        else {
            // Draw 2 triangles. Add the remaining 2 indices
            indices.push_back(0);
            indices.push_back(2);
        }

        diamond->InitFromData(vertices, indices);
        return diamond;
 }


Mesh* object2D::CreateHexagon(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices
    {
        VertexFormat(corner + glm::vec3(0, 0, 0), color), // O
        VertexFormat(corner + glm::vec3(-length, 0, 0), color), // A
        VertexFormat(corner + glm::vec3(-length / 2, length * sqrt(3) / 2, 0), color), // B
        VertexFormat(corner + glm::vec3(length / 2, length * sqrt(3) / 2, 0), color), // C
        VertexFormat(corner + glm::vec3(length, 0, 0), color), // D
        VertexFormat(corner + glm::vec3(length / 2, -length * sqrt(3) / 2, 0), color), // E
        VertexFormat(corner + glm::vec3(-length / 2, -length * sqrt(3) / 2, 0), color), // F
    };

    Mesh* hex = new Mesh(name);
    std::vector<unsigned int> indices = {
        0, 1, 2,
        0, 2, 3,
        0, 3, 4,
        0, 4, 5,
        0, 5, 6,
        0, 6, 1

    };

    if (!fill) {
        hex->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }
    hex->InitFromData(vertices, indices);
    return hex;
}


Mesh* object2D::CreateStar(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;
    float cos1 = (sqrt(5) - 1) / 4;
    float cos2 = (sqrt(5) + 1) / 4;
    float sin1 = (sqrt(10 + 2 * sqrt(5))) / 4;
    float sin2 = (sqrt(10 - 2 * sqrt(5))) / 4;

    std::vector<VertexFormat> vertices
    {
        VertexFormat(corner + glm::vec3(0, 0, 0), color), // O
        VertexFormat(corner + glm::vec3(0,  length, 0), color), // A
        VertexFormat(corner + glm::vec3(-length * sin1, length * cos1, 0), color), // B
        VertexFormat(corner + glm::vec3(-length * sin2, -length * cos2, 0), color), // C
        VertexFormat(corner + glm::vec3(length * sin2, -length * cos2, 0), color), // D
        VertexFormat(corner + glm::vec3(length * sin1, length * cos1, 0), color), // E
        
    };

    Mesh* star = new Mesh(name);
    std::vector<unsigned int> indices = {
        0, 1, 4,
        0, 1, 3,
        0, 2, 4,
        0, 2, 5,
        0, 3, 5
    };

    if (!fill) {
        star->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }
    star->InitFromData(vertices, indices);
    return star;
}


Mesh* object2D::CreateHeart(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;
    glm::vec3 vertexPosition;
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    for (float t = 0; t <= 2 * M_PI; t += 0.1)
    {
        vertexPosition.x = 16 * pow(sin(t), 3);
        vertexPosition.y = 13 * cos(t) - 5 * cos(2 * t) - 2 * cos(3 * t) - cos(4 * t);
        vertexPosition.z = 0;  // Assuming a 2D shape, set z-coordinate to 0

        vertices.emplace_back(corner + vertexPosition, color);
        indices.push_back(vertices.size() - 1);
    }

    Mesh* heart = new Mesh(name);
    if (!fill) {
        heart->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        heart->SetDrawMode(GL_TRIANGLE_FAN);
    }
    heart->InitFromData(vertices, indices);

    return heart;
}
