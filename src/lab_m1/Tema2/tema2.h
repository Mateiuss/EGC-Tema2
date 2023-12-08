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
        Tema2();
        ~Tema2();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color);

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

     protected:
        implemented::Camera *camera;
        glm::mat4 projectionMatrix;
        Tanc myTank;
        Tanc enemyTankModel;

        std::vector<Building> buildingModels;
        std::vector<Building> buildings;

        std::vector<Tanc> enemyTanks;

        glm::vec3 lightPosition;
        unsigned int materialShininess;
        float materialKd;
        float materialKs;

        std::vector<Projectile> projectiles;
    };
}   // namespace m1
