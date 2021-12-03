#include "lab_m1/tema1/tema1.h"

#include <vector>
#include <iostream>

#include "lab_m1/tema1/transform2D.h"
#include "lab_m1/tema1/object2D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    auto camera = GetSceneCamera();
    //camera->SetOrthographic(0, (float)window->GetResolution().x, 0, (float)window->GetResolution().y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    // Rotation variables
    angle = 0.f;

    // Initialize tx and ty (the translation steps)
    translateX = 0;
    translateY = 0;

    logicSpace.x = 0;       // logic x
    logicSpace.y = 0;       // logic y
    logicSpace.width = 12.8;   // logic width
    logicSpace.height = 7.2;  // logic height

    corner = glm::vec3(0, 0, 0);
    length = 0.5f;

    // initialize obstacles variables
    for (int i = 0; i < 3; i++) {
        obstacles[i].obstacleX = 3 * (i + 1);
        obstacles[i].obstacleY = 4 * i + 1;
        obstacles[i].obstacleLength = i + 2;
    }

    // collision with obstacles
    colObstacleDown = false;
    colObstacleUp = false;
    colObstacleLeft = false;
    colObstacleRight = false;
    isCollided = false;


    Mesh* mc = object2D::CreateChar("mc", corner, length, glm::vec3(0.75, 0.50, 1), true);
    AddMeshToList(mc);

    Mesh* map = object2D::CreateRectangle("map", corner, 30 * length, glm::vec3(0.25, 0, 0.50), true);
    AddMeshToList(map);

    Mesh* enemy = object2D::CreateEnemy("enemy", corner, length, glm::vec3(0.7, 0.9, 1), true);
    AddMeshToList(enemy);

    Mesh* proj = object2D::CreateProj("proj", corner, length / 3, glm::vec3(0, 0, 0), true);
    AddMeshToList(proj);

    Mesh* obstacle1 = object2D::CreateRectangle("obstacle1", corner, obstacles[0].obstacleLength * length, glm::vec3(1, 0.50, 0.67), true);
    AddMeshToList(obstacle1);

    Mesh* obstacle2 = object2D::CreateRectangle("obstacle2", corner, obstacles[1].obstacleLength * length, glm::vec3(1, 0.50, 0.67), true);
    AddMeshToList(obstacle2);

    Mesh* obstacle3 = object2D::CreateRectangle("obstacle3", corner, obstacles[2].obstacleLength * length, glm::vec3(1, 0.50, 0.67), true);
    AddMeshToList(obstacle3);

    Mesh* healthbar = object2D::CreateHealthbar("healthbar", corner, length, glm::vec3(1, 0.3, 0.42));
    AddMeshToList(healthbar);

    Mesh* life = object2D::CreateSquare("life", corner, length, glm::vec3(1, 0.3, 0.42), true);
    AddMeshToList(life);

    Mesh* extralife = object2D::CreateSquare("extralife", corner, length, glm::vec3(1, 0.3, 0.42), true);
    AddMeshToList(extralife);


}


// 2D visualization matrix
glm::mat3 Tema1::VisualizationTransf2D(const LogicSpace & logicSpace, const ViewportSpace & viewSpace)
{
    float sx, sy, tx, ty;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    tx = viewSpace.x - sx * logicSpace.x;
    ty = viewSpace.y - sy * logicSpace.y;

    return glm::transpose(glm::mat3(
        sx, 0.0f, tx,
        0.0f, sy, ty,
        0.0f, 0.0f, 1.0f));
}


// Uniform 2D visualization matrix (same scale factor on x and y axes)
glm::mat3 Tema1::VisualizationTransf2DUnif(const LogicSpace & logicSpace, const ViewportSpace & viewSpace)
{
    float sx, sy, tx, ty, smin;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    if (sx < sy)
        smin = sx;
    else
        smin = sy;
    tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
    ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;

    return glm::transpose(glm::mat3(
        smin, 0.0f, tx,
        0.0f, smin, ty,
        0.0f, 0.0f, 1.0f));
}


void Tema1::SetViewportArea(const ViewportSpace & viewSpace, glm::vec3 colorColor, bool clear)
{
    glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    glEnable(GL_SCISSOR_TEST);
    glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
    GetSceneCamera()->Update();
}


void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void Tema1::GenerateEnemy() {
    // generate enemies with random positions and speeds
    enemies.push_back(enemy(rand() % 11 + 1, rand() % 6 + 1, rand() % 3 + 1, false));
}


void Tema1::UpdateEnemy(float deltaTimeSeconds) {

    float speed = 0.8 * deltaTimeSeconds;

    // eliminate hit enemies
    for (int i = 0; i < enemies.size(); i++) {
        if (enemies[i].isHit == false) {
            modelMatrix = visMatrix * transform2D::Translate(enemies[i].enemyX, enemies[i].enemyY);
            RenderMesh2D(meshes["enemy"], shaders["VertexColor"], modelMatrix);
        }
        else {
            enemies.erase(enemies.begin() + i);
        }

    }

    // generate enemy every 3 seconds
    this_time = clock();
    time_counter += (double)(this_time - last_time);
    last_time = this_time;
    if (time_counter > (double)(3 * CLOCKS_PER_SEC))
    {
        time_counter -= (double)(3 * CLOCKS_PER_SEC);
        GenerateEnemy();
    }

    // make enemy follow you
    for (int i = 0; i < enemies.size(); i++) {

        if (enemies[i].enemyX > translateX - length / 2 + logicSpace.width / 2) {
            (float)enemies[i].enemyX -= speed * enemies[i].enemySpeed;
        }
        if (enemies[i].enemyX + length / 2 < translateX + logicSpace.width / 2) {
            (float)enemies[i].enemyX += speed * enemies[i].enemySpeed;
        }
        if (enemies[i].enemyY + length < translateY + logicSpace.height / 2) {
            (float)enemies[i].enemyY += speed * enemies[i].enemySpeed;
        }
        if (enemies[i].enemyY > translateY  + logicSpace.height / 2) {
            (float)enemies[i].enemyY -= speed * enemies[i].enemySpeed;
        }
    }

    // check collision enemy-player
    for (int i = 0; i < enemies.size(); i++) {

        if (CheckCollision(translateX + logicSpace.width/2, translateY + logicSpace.height / 2, length,
            length, enemies[i].enemyX, enemies[i].enemyY, length + 0.15, length + 0.15)) {
            enemies[i].isHit = true;
            lives--;
        }
    }

}

void Tema1::GenerateProj() {

    projs.push_back(proj(translateX, translateY, angle, false));

}

void Tema1::UpdateProj(float deltaTimeSeconds) {

    // calculate projectile trajectory
    float startX = 0;
    float startY = 0;
    for (int i = 0; i < projs.size(); i++) {
        if (projMoving == false) {
            projs[i].projX = translateX;
            projs[i].projY = translateY;
            projs[i].projAngle = angle;
        }
        else {
            projs[i].projX += projSpeed * cos(projs[i].projAngle);
            projs[i].projY += projSpeed * sin(projs[i].projAngle);
        }
    }

    // check collision enemy-projectile
    for (int i = 0; i < enemies.size(); i++) {
        for (int j = 0; j < projs.size(); j++) {
            if (CheckCollision(projs[j].projX + logicSpace.width/2, projs[j].projY + logicSpace.height/2, length / 3,
                2 * length / 3, enemies[i].enemyX, enemies[i].enemyY, length, length)) {
                enemies[i].isHit = true;
                projs[j].projHit = true;
                score++;
                cout << "Score: " << score << endl;
            }
        }
    }

    // check collision obstacle-projectile
    for (int j = 0; j < 3; j++) {
        for (int i = 0; i < projs.size(); i++) {
            if (CheckCollision(projs[i].projX + logicSpace.width / 2, projs[i].projY + logicSpace.height / 2, length / 3, 2 * length / 3,
                obstacles[j].obstacleX, obstacles[j].obstacleY, obstacles[j].obstacleLength * length, 2 * length * obstacles[j].obstacleLength)) {
                projs[i].projHit = true;
            }
        }
    }

    // check collision projectile map
    float mapW, mapH;
    mapW = 30 * length;
    mapH = 60 * length;

    for (int i = 0; i < projs.size(); i++) {
        if (projs[i].projX > corner.x + mapW / 2) {
            projs[i].projHit = true;
        }

        if (projs[i].projX < corner.x - mapW / 2) {
            projs[i].projHit = true;
        }

        if (projs[i].projY > corner.y + mapH / 2) {
            projs[i].projHit = true;
        }

        if (projs[i].projY < corner.y - mapH / 2) {
            projs[i].projHit = true;
        }
    }

}

bool Tema1::CheckCollision(float oneX, float oneY, float oneW, float oneH,
    float twoX, float twoY, float twoW, float twoH) {

    bool collisionX = oneX + oneW >= twoX &&
        twoX + twoW >= oneX;
    bool collisionY = oneY + oneH >= twoY &&
        twoY + twoH >= oneY;
    return collisionX && collisionY;
}

void Tema1::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->GetResolution();

    //// Sets the screen area where to draw - the left half of the window

    viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
    SetViewportArea(viewSpace, glm::vec3(0.2), true);

    // Compute uniform 2D visualization matrix
    visMatrix = glm::mat3(1);
    visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);

    UpdateEnemy(deltaTimeSeconds);
    UpdateProj(deltaTimeSeconds);

    DrawScene(visMatrix);

    // collision with map
    colUp = false;
    colDown = false;
    colLeft = false;
    colRight = false;

    float mapW, mapH;
    mapW = 30 * length;
    mapH = 60 * length;

    if (translateX > corner.x + mapW / 2 - length / 2) {
        colRight = true;
    }

    if (translateX < corner.x - mapW / 2 + length / 2) {
        colLeft = true;
    }

    if (translateY > corner.y + mapH / 2 - length / 2) {
        colUp = true;
    }

    if (translateY < corner.y - mapH / 2 + length / 2) {
        colDown = true;
    }

    if (CheckCollision(translateX + logicSpace.width / 2, translateY + logicSpace.height / 2, length,
        length, 3, 4, length + 0.15, length + 0.15) && lives < 3) {
        extralife = false;
        lives += 1;
    }
    
    // exit game if you die
    if (lives == 0) {
        exit(0);
    }

}


void Tema1::FrameEnd()
{
}


void Tema1::DrawScene(glm::mat3 visMatrix)
{

    modelMatrix = visMatrix * transform2D::Translate(translateX, translateY);
    modelMatrix *= transform2D::Translate(logicSpace.width / 3, logicSpace.height / 2) * transform2D::Translate(length, length);
    RenderMesh2D(meshes["healthbar"], shaders["VertexColor"], modelMatrix);

    if (lives == 3) {
        modelMatrix = visMatrix * transform2D::Translate(translateX, translateY);
        modelMatrix *= transform2D::Translate(logicSpace.width / 3, logicSpace.height / 2) * transform2D::Translate(length, length);
        RenderMesh2D(meshes["life"], shaders["VertexColor"], modelMatrix);

        modelMatrix = visMatrix * transform2D::Translate(translateX, translateY);
        modelMatrix *= transform2D::Translate(logicSpace.width / 3, logicSpace.height / 2 + length) * transform2D::Translate(length, length);
        RenderMesh2D(meshes["life"], shaders["VertexColor"], modelMatrix);

        modelMatrix = visMatrix * transform2D::Translate(translateX, translateY);
        modelMatrix *= transform2D::Translate(logicSpace.width / 3, logicSpace.height / 2 + 2 * length) * transform2D::Translate(length, length);
        RenderMesh2D(meshes["life"], shaders["VertexColor"], modelMatrix);
    }

    if (lives == 2) {
        modelMatrix = visMatrix * transform2D::Translate(translateX, translateY);
        modelMatrix *= transform2D::Translate(logicSpace.width / 3, logicSpace.height / 2) * transform2D::Translate(length, length);
        RenderMesh2D(meshes["life"], shaders["VertexColor"], modelMatrix);

        modelMatrix = visMatrix * transform2D::Translate(translateX, translateY);
        modelMatrix *= transform2D::Translate(logicSpace.width / 3, logicSpace.height / 2 + length) * transform2D::Translate(length, length);
        RenderMesh2D(meshes["life"], shaders["VertexColor"], modelMatrix);
    }

    if (lives == 1) {
        modelMatrix = visMatrix * transform2D::Translate(translateX, translateY);
        modelMatrix *= transform2D::Translate(logicSpace.width / 3, logicSpace.height / 2) * transform2D::Translate(length, length);
        RenderMesh2D(meshes["life"], shaders["VertexColor"], modelMatrix);
    }

    if (extralife == true) {
        modelMatrix = visMatrix * transform2D::Translate(3, 4);
        RenderMesh2D(meshes["extralife"], shaders["VertexColor"], modelMatrix);
    }

    modelMatrix = visMatrix * transform2D::Translate(translateX, translateY);
    modelMatrix *= transform2D::Translate(logicSpace.width/2, logicSpace.height/2) * transform2D::Translate(-length / 2, -length / 2) *
        transform2D::Translate(length / 2, length / 2) * transform2D::Rotate(angle) * transform2D::Translate(-length / 2, -length / 2);
    RenderMesh2D(meshes["mc"], shaders["VertexColor"], modelMatrix);


    for (int i = 0; i < projs.size(); i++) {
        if (projs[i].projHit == false) {
            modelMatrix = visMatrix * transform2D::Translate(projs[i].projX, projs[i].projY);
            modelMatrix *= transform2D::Translate(logicSpace.width / 2, logicSpace.height / 2) * transform2D::Translate(-length / 2, -length / 6) *
                transform2D::Translate(length / 2, length / 6) * transform2D::Rotate(projs[i].projAngle) * transform2D::Translate(-length / 2, -length / 6);
            RenderMesh2D(meshes["proj"], shaders["VertexColor"], modelMatrix);
        }
        else {
            projs.erase(projs.begin() + i);
        }
    }

    modelMatrix = visMatrix * transform2D::Translate(obstacles[0].obstacleX, obstacles[0].obstacleY);
    RenderMesh2D(meshes["obstacle1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix * transform2D::Translate(obstacles[1].obstacleX, obstacles[1].obstacleY);
    RenderMesh2D(meshes["obstacle2"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix * transform2D::Translate(obstacles[2].obstacleX, obstacles[2].obstacleY);
    RenderMesh2D(meshes["obstacle3"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix * transform2D::Translate(logicSpace.width / 2, logicSpace.height / 2) * transform2D::Translate(-30 * length / 2, -60 * length / 2);
    RenderMesh2D(meshes["map"], shaders["VertexColor"], modelMatrix);


}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    // Move the logic window with W, A, S, D (up, left, down, right)

    if (window->KeyHold(GLFW_KEY_W) && colUp == false && colObstacleDown == false)
    {
        logicSpace.y += deltaTime * 10;
        translateY += deltaTime * 10;
        colDown = false;
        for (int i = 0; i < 3; i++) {
            if (CheckCollision(obstacles[i].obstacleX, obstacles[i].obstacleY, obstacles[i].obstacleLength * length,
                2 * obstacles[i].obstacleLength * length, translateX + logicSpace.width / 2, translateY + logicSpace.height / 2,
                length, length)) {
                colObstacleDown = true;
                colObstacleRight = false;
                colObstacleLeft = false;
                colObstacleUp = false;
                break;
            }
            else {
                colObstacleDown = false;
                colObstacleRight = false;
                colObstacleLeft = false;
                colObstacleUp = false;
            }
        }
    }
    if (window->KeyHold(GLFW_KEY_S) && colDown == false && colObstacleUp == false)
    { 
        logicSpace.y -= deltaTime * 10;
        translateY -= deltaTime * 10;
        colUp = false;
        colObstacleDown = false;
        for (int i = 0; i < 3; i++) {
            if (CheckCollision(obstacles[i].obstacleX, obstacles[i].obstacleY, obstacles[i].obstacleLength * length,
                2 * obstacles[i].obstacleLength * length, translateX + logicSpace.width / 2, translateY + logicSpace.height / 2,
                2*length, 2*length)) {
                colObstacleUp = true;
                colObstacleRight = false;
                colObstacleLeft = false;
                break;
            }
            else {
                colObstacleDown = false;
                colObstacleRight = false;
                colObstacleLeft = false;
                colObstacleUp = false;
            }
        }
    }
    if (window->KeyHold(GLFW_KEY_D) && colRight == false && colObstacleLeft == false)
    {
        logicSpace.x += deltaTime * 10;
        translateX += deltaTime * 10;
        colLeft = false;
        colObstacleRight = false;
        for (int i = 0; i < 3; i++) {
            if (CheckCollision(obstacles[i].obstacleX, obstacles[i].obstacleY, obstacles[i].obstacleLength * length,
                2 * obstacles[i].obstacleLength * length, translateX + logicSpace.width / 2, translateY + logicSpace.height / 2,
                length, length)) {
                colObstacleLeft = true;
                colObstacleUp = false;
                colObstacleDown = false;
                break;
            }
            else {
                colObstacleDown = false;
                colObstacleRight = false;
                colObstacleLeft = false;
                colObstacleUp = false;
            }
        }
    }
    if (window->KeyHold(GLFW_KEY_A) && colLeft == false && colObstacleRight == false)
    {
        logicSpace.x -= deltaTime * 10;
        translateX -= deltaTime * 10;
        colRight = false;
        colObstacleLeft = false;
        for (int i = 0; i < 3; i++) {
            if (CheckCollision(obstacles[i].obstacleX, obstacles[i].obstacleY, obstacles[i].obstacleLength * length,
                2 * obstacles[i].obstacleLength * length, translateX + logicSpace.width / 2, translateY + logicSpace.height / 2,
                length, length)) {
                colObstacleRight = true;
                colObstacleUp = false;
                colObstacleDown = false;
                break;
            }
            else {
                colObstacleDown = false;
                colObstacleRight = false;
                colObstacleLeft = false;
                colObstacleUp = false;
            }
        }
    }
        
    if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT) == false) {
        projMoving = true;
        projSpeed = deltaTime * 12;
    }
}


void Tema1::OnKeyPress(int key, int mods)
{
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // player rotation angle
    angle = atan2f((viewSpace.height - mouseY * viewSpace.height/window->GetResolution().y - viewSpace.height/2),
        (mouseX * viewSpace.width/window->GetResolution().x - viewSpace.width / 2));
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT) == true) {
        this_time = clock();
        time_counter += (double)(this_time - last_time);
        last_time = this_time;
        if (time_counter > (double)(0.7 * CLOCKS_PER_SEC))
        {
            time_counter -= (double)(0.7 * CLOCKS_PER_SEC);
            GenerateProj();
        }
    }
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}
