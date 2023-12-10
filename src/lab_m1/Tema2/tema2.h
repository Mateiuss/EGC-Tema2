#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema2/camera.h"
#include "components/transform.h"
#include "lab_m1/Tema2/Tanc.h"
#include "lab_m1/Tema2/Projectile.h"
#include "lab_m1/Tema2/Building.h"
#include <vector>

namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
     public:
         struct ViewportArea
         {
             ViewportArea() : x(0), y(0), width(1), height(1) {}
             ViewportArea(int x, int y, int width, int height)
                 : x(x), y(y), width(width), height(height) {}
             int x;
             int y;
             int width;
             int height;
         };

        Tema2();
        ~Tema2();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color, int hp, int obj_type);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        bool tanks_intersect(Tanc *tank1, Tanc *tank2);
        void decollision_tanks(Tanc *tank1, Tanc *tank2);
        bool tank_building_intersect(Tanc *tank, Building *building);
        void decollision_tank_building(Tanc *tank, Building *building);
        bool tank_projectile_intersect(Tanc *tank, Projectile *projectile);
        bool projectile_building_intersect(Projectile *projectile, Building *building);
        void render_scene();

        void finish_game();

     protected:
        implemented::Camera *camera, copyCamera;
        glm::mat4 projectionMatrix;
        glm::mat4 miniMapProjectionMatrix;
        Tanc myTank;
        Tanc enemyTankModel;

        std::vector<Building> buildingModels;
        std::vector<Building> buildings;

        std::vector<Tanc> enemyTanks;
        std::vector<Tanc> destroyedEnemyTanks;

        glm::vec3 lightPosition;
        unsigned int materialShininess;
        float materialKd;
        float materialKs;

        std::vector<Projectile> projectiles;

        float playTime;
        float maxPlayTime;
        float points;

        bool gameOver;

        ViewportArea miniViewportArea;
    };
}   // namespace m1
