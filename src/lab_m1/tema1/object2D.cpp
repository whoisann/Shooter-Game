#include "object2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object2D::CreateChar(
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
        VertexFormat(corner + glm::vec3(0, length, 0), color),

        VertexFormat(corner + glm::vec3(length - 0.15, -0.15, 0), color), // 4
        VertexFormat(corner + glm::vec3(length + 0.15, -0.15, 0), color), // 5
        VertexFormat(corner + glm::vec3(length + 0.15, +0.15, 0), color), // 6
        VertexFormat(corner + glm::vec3(length - 0.15, +0.15, 0), color), // 7

        VertexFormat(corner + glm::vec3(length - 0.15, length - 0.15, 0), color), // 8
        VertexFormat(corner + glm::vec3(length + 0.15, length - 0.15, 0), color), // 9
        VertexFormat(corner + glm::vec3(length + 0.15, length + 0.15, 0), color), // 10
        VertexFormat(corner + glm::vec3(length - 0.15, length + 0.15, 0), color), // 11
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2,
        0, 3, 2,
        4, 5, 6,
        4, 7, 6,
        8, 9, 10,
        8, 11, 10,
    };

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

Mesh* object2D::CreateEnemy(
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
        VertexFormat(corner + glm::vec3(length, 0, 0), color), // 1
        VertexFormat(corner + glm::vec3(length, length, 0), color), // 2
        VertexFormat(corner + glm::vec3(0, length, 0), color), // 3

        VertexFormat(corner + glm::vec3(length - 0.15, -0.15, 0), color), // 4
        VertexFormat(corner + glm::vec3(length + 0.15, -0.15, 0), color), // 5
        VertexFormat(corner + glm::vec3(length + 0.15, +0.15, 0), color), // 6
        VertexFormat(corner + glm::vec3(length - 0.15, +0.15, 0), color), // 7

        VertexFormat(corner + glm::vec3(length - 0.15, length - 0.15, 0), color), // 8
        VertexFormat(corner + glm::vec3(length + 0.15, length - 0.15, 0), color), // 9
        VertexFormat(corner + glm::vec3(length + 0.15, length + 0.15, 0), color), // 10
        VertexFormat(corner + glm::vec3(length - 0.15, length + 0.15, 0), color), // 11

        VertexFormat(corner + glm::vec3(- 0.15, - 0.15, 0), color), // 12
        VertexFormat(corner + glm::vec3(+ 0.15, - 0.15, 0), color), // 13
        VertexFormat(corner + glm::vec3(+ 0.15, + 0.15, 0), color), // 14
        VertexFormat(corner + glm::vec3(- 0.15, + 0.15, 0), color), // 15

        VertexFormat(corner + glm::vec3(- 0.15, length - 0.15, 0), color), // 16
        VertexFormat(corner + glm::vec3(+ 0.15, length - 0.15, 0), color), // 17
        VertexFormat(corner + glm::vec3(+ 0.15, length + 0.15, 0), color), // 18
        VertexFormat(corner + glm::vec3(- 0.15, length + 0.15, 0), color), // 19
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2,
        0, 3, 2,
        4, 5, 6,
        4, 7, 6,
        8, 9, 10,
        8, 11, 10,
        12, 13, 14,
        12, 15, 14,
        16, 17, 18,
        16, 18, 19
    };

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

Mesh* object2D::CreateRectangle(
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
        VertexFormat(corner + glm::vec3(length, 2 * length, 0), color),
        VertexFormat(corner + glm::vec3(0, 2 * length, 0), color),
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3
    };

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
        VertexFormat(corner + glm::vec3(0, length, 0), color),
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3
    };

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

Mesh* object2D::CreateHealthbar(
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
        VertexFormat(corner + glm::vec3(length, 3 * length, 0), color),
        VertexFormat(corner + glm::vec3(0, 3 * length, 0), color),
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3
    };

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

Mesh* object2D::CreateProj(
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
        VertexFormat(corner + glm::vec3(2 * length, 0, 0), color),
        VertexFormat(corner + glm::vec3(2 * length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color),
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3
    };

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
