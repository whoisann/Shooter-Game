#pragma once

#include "components/simple_scene.h"


namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        struct ViewportSpace
        {
            ViewportSpace() : x(0), y(0), width(1), height(1) {}
            ViewportSpace(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        };

        struct LogicSpace
        {
            LogicSpace() : x(0), y(0), width(1), height(1) {}
            LogicSpace(float x, float y, float width, float height)
                : x(x), y(y), width(width), height(height) {}
            float x;
            float y;
            float width;
            float height;
        };

        struct enemy {
            enemy() : enemyX(0), enemyY(0), enemySpeed(0), isHit(false) {}
            enemy(float enemyX, float enemyY, float enemySpeed, bool isHit)
                : enemyX(enemyX), enemyY(enemyY), enemySpeed(enemySpeed), isHit(isHit) {}
            float enemyX, enemyY, enemySpeed;
            bool isHit = false;
        };

        struct proj {
            proj() : projX(0), projY(0), projAngle(0), projHit(false) {}
            proj(float projX, float projY, float projAngle, bool projHit)
                : projX(projX), projY(projY), projAngle(projAngle), projHit(projHit) {}
            float projX, projY, projAngle;
            bool projHit;
        };

        struct obstacle {
            obstacle() : obstacleX(0), obstacleY(0), obstacleLength(0) {}
            obstacle(float obstacleX, float obstacleY, float obstacleLength)
                : obstacleX(obstacleX), obstacleY(obstacleY), obstacleLength(obstacleLength) {}
            float obstacleX, obstacleY, obstacleLength;
        };

    public:
        Tema1();
        ~Tema1();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void DrawScene(glm::mat3 visMatrix);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;

        void UpdateEnemy(float deltaTimeSeconds);
        void UpdateProj(float deltaTimeSeconds);
        void GenerateProj();
        void GenerateEnemy();
        void GenerateObstacle();
        bool CheckCollision(float oneX, float oneY, float oneW, float oneH,
            float twoX, float twoY, float twoW, float twoH);

        // Sets the logic space and view space
        // logicSpace: { x, y, width, height }
        // viewSpace: { x, y, width, height }
        glm::mat3 VisualizationTransf2D(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);
        glm::mat3 VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);

        void SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor = glm::vec3(0), bool clear = true);

    protected:
        // mc variables
        float length, angle;
        bool colUp, colDown, colLeft, colRight;
        bool colObstacleUp, colObstacleDown, colObstacleLeft, colObstacleRight, isCollided;
        float translateX, translateY;
        float x1, x2, y1, y2, w1, w2, h1, h2;
        ViewportSpace viewSpace;
        LogicSpace logicSpace;
        glm::mat3 modelMatrix, visMatrix;
        glm::vec3 corner;
        int lives = 3;
        int score = 0;
        bool extralife = true;

        // projectile variables
        glm::vector<proj> projs;
        bool projMoving = false;
        float projSpeed;

        // time variables
        double time_counter = 0;
        clock_t this_time = clock();
        clock_t last_time = this_time;

        // enemy variables
        int nr_enemies = 0;
        glm::vector<enemy> enemies;
        obstacle obstacles[4];
    };
}   // namespace m1
