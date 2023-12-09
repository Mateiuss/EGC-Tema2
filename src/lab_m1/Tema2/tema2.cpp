#include "lab_m1/Tema2/tema2.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}

void Tema2::Init()
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    camera = new implemented::Camera();
    camera->Set(glm::vec3(0, 1.f, 3.5f), glm::vec3(0, -1, 0), glm::vec3(0, 1, 0));

    {
        myTank = Tanc(camera->GetTargetPosition(), glm::vec3(0, 0, -1), -90);
        myTank.set_colors(glm::vec3(0.88f, 0.88f, 0.88f), glm::vec3(0.06f, 0.25f, 0.02f), glm::vec3(0.16f, 0.28f, 0.06f), glm::vec3(0.88f, 0.88f, 0.88f));
        camera->SetTank(&myTank);
    }

    {
        enemyTankModel = Tanc(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1), -90);
        enemyTankModel.set_colors(glm::vec3(0.88f, 0.88f, 0.88f), glm::vec3(0.369f, 0.067f, 0.239f), glm::vec3(0.671f, 0.129f, 0.435f), glm::vec3(0.88f, 0.88f, 0.88f));
    }

    {
        for (int i = 0; i < 15; i++) {
            glm::vec3 position = glm::vec3(rand() % 50 - rand() % 50, 0, rand() % 50 - rand() % 50);
			Tanc enemyTank = enemyTankModel;
			enemyTank.position = position;
			enemyTanks.push_back(enemyTank);
        }
    }

    {
        buildingModels.push_back(Building(glm::vec3(0, 0, 0), 2, 6, 2));
        buildingModels.push_back(Building(glm::vec3(0, 0, 0), 3, 9, 6));
        buildingModels.push_back(Building(glm::vec3(0, 0, 0), 3, 6, 9));
        buildingModels.push_back(Building(glm::vec3(0, 0, 0), 12, 3, 3));

        for (int i = 0; i < 30; i++) {
            // get random building model
            int model_index = rand() % buildingModels.size();
            Building building = buildingModels[model_index];

            // get random position for building
            float x = rand() % 75 - rand() % 75;
            float z = rand() % 75 - rand() % 75;
            glm::vec3 center = glm::vec3(x, building.height / 2, z);

            building.center = center;
            buildings.push_back(building);
        }
    }

    {
        Mesh* senile = new Mesh("senile");
        senile->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tema"), "senile.obj");
        meshes[senile->GetMeshID()] = senile;
    }

    {
        Mesh* corp = new Mesh("corp");
        corp->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tema"), "corp.obj");
        meshes[corp->GetMeshID()] = corp;
    }

    {
        Mesh* turela = new Mesh("turela");
        turela->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tema"), "turela.obj");
        meshes[turela->GetMeshID()] = turela;
    }

    {
        Mesh* tun = new Mesh("tun");
        tun->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tema"), "tun.obj");
        meshes[tun->GetMeshID()] = tun;
    }

    {
        Mesh* mesh = new Mesh("plane");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Shader* shader = new Shader("TemaShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        lightPosition = glm::vec3(0, 1, 1);
        materialShininess = 30;
        materialKd = 0.5;
        materialKs = 0.5;
    }

    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
}


void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.67f, 0.53f, 0.17f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(3));
        RenderMesh(meshes["plane"], shaders["TemaShader"], modelMatrix, glm::vec3(0.5f));
    }

    // Render the tank
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, myTank.position);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(myTank.body_angle), glm::vec3(0, 1, 0));
        RenderMesh(meshes["senile"], shaders["TemaShader"], modelMatrix, myTank.senile_color);
        RenderMesh(meshes["corp"], shaders["TemaShader"], modelMatrix, myTank.corp_color);

        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, myTank.position);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(myTank.turret_angle), glm::vec3(0, 1, 0));
        RenderMesh(meshes["turela"], shaders["TemaShader"], modelMatrix, myTank.turela_color);
        RenderMesh(meshes["tun"], shaders["TemaShader"], modelMatrix, myTank.tun_color);
    }

    // Render enemy tank
    {
        for (int i = 0; i < enemyTanks.size(); i++) {
            glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, enemyTanks[i].position);
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(enemyTanks[i].body_angle), glm::vec3(0, 1, 0));
			RenderMesh(meshes["senile"], shaders["TemaShader"], modelMatrix, enemyTanks[i].senile_color);
			RenderMesh(meshes["corp"], shaders["TemaShader"], modelMatrix, enemyTanks[i].corp_color);

			modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, enemyTanks[i].position);
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(enemyTanks[i].turret_angle), glm::vec3(0, 1, 0));
			RenderMesh(meshes["turela"], shaders["TemaShader"], modelMatrix, enemyTanks[i].turela_color);
			RenderMesh(meshes["tun"], shaders["TemaShader"], modelMatrix, enemyTanks[i].tun_color);
        }
    }

    // Render the projectiles
    {
        for (int i = 0; i < projectiles.size(); i++) {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, projectiles[i].position);
            modelMatrix = glm::scale(modelMatrix, glm::vec3(projectiles[i].scale));
            RenderMesh(meshes["sphere"], shaders["TemaShader"], modelMatrix, glm::vec3(0.88f, 0.88f, 0.88f));
        }
    }

    {
        for (int i = 0; i < buildings.size(); i++) {
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, buildings[i].center);
			modelMatrix = glm::scale(modelMatrix, glm::vec3(buildings[i].width, buildings[i].height, buildings[i].depth));
			RenderMesh(meshes["box"], shaders["TemaShader"], modelMatrix, glm::vec3(0.88f, 0.88f, 0.88f));
		}
    }
}


void Tema2::Update(float deltaTimeSeconds)
{
    myTank.timeFromLastShot += deltaTimeSeconds;

    for (int i = 0; i < buildings.size(); i++) {
        if (tank_building_intersect(&myTank, &buildings[i])) {
            decollision_tank_building(&myTank, &buildings[i]);
        }
    }

    for (int i = 0; i < projectiles.size(); i++) {
        if (tank_projectile_intersect(&myTank, &projectiles[i])) {
            if (myTank.hp <= 0) {
                cout << "GAME OVER" << endl;
                exit(0);
            }
        }

        bool erase = false;

        for (int j = 0; j < buildings.size(); j++) {
            if (projectile_building_intersect(&projectiles[i], &buildings[j])) {
				projectiles.erase(projectiles.begin() + i);
                i--;
                erase = true;
                break;
			}
		}

        if (erase) {
			continue;
		}
        
		projectiles[i].position += glm::normalize(projectiles[i].forward) * projectiles[i].speed * deltaTimeSeconds;
		projectiles[i].lifeTime += deltaTimeSeconds;
        if (projectiles[i].lifeTime > 10) {
			projectiles.erase(projectiles.begin() + i);
			i--;
		}
	}

    for (int i = 0; i < enemyTanks.size(); i++) {
        bool erase = false;

        for (int j = 0; j < projectiles.size(); j++) {
            if (tank_projectile_intersect(&enemyTanks[i], &projectiles[j])) {
                if (enemyTanks[i].hp <= 0) {
					enemyTanks.erase(enemyTanks.begin() + i);
					i--;
				}
                erase = true;
				projectiles.erase(projectiles.begin() + j);
                break;
            }
        }

        if (erase) {
			continue;
		}

        if (tanks_intersect(&myTank, &enemyTanks[i])) {
            decollision_tanks(&myTank, &enemyTanks[i]);
        }

        for (int j = 0; j < buildings.size(); j++) {
            if (tank_building_intersect(&enemyTanks[i], &buildings[j])) {
				decollision_tank_building(&enemyTanks[i], &buildings[j]);
			}
		}

        for (int j = i + 1; j < enemyTanks.size(); j++) {
			if (tanks_intersect(&enemyTanks[i], &enemyTanks[j])) {
                decollision_tanks(&enemyTanks[i], &enemyTanks[j]);
            }
        }

        if (enemyTanks[i].timeFromLastMove > 5) {
            enemyTanks[i].get_random_moving_state();
        }

        enemyTanks[i].move(deltaTimeSeconds);

        enemyTanks[i].attack(deltaTimeSeconds, myTank.position, projectiles);
	}
}

void Tema2::FrameEnd()
{
    //DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    float tankForwardSpeed = 1.5f;
    float tankRotatingSpeed = 30.0f;

    if (window->KeyHold(GLFW_KEY_W)) {
        camera->MoveForward(tankForwardSpeed * deltaTime);
    }

    if (window->KeyHold(GLFW_KEY_A)) {
        camera->RotateRight(tankRotatingSpeed * deltaTime);
    }

    if (window->KeyHold(GLFW_KEY_S)) {
        camera->MoveForward(-tankForwardSpeed * deltaTime);
    }

    if (window->KeyHold(GLFW_KEY_D)) {
        camera->RotateRight(-tankRotatingSpeed * deltaTime);
    }
}


void Tema2::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

        camera->RotateFirstPerson_OX(-sensivityOY * deltaY);
        camera->RotateFirstPerson_OY(-sensivityOX * deltaX);
        camera->position = myTank.position - camera->distanceToTarget * camera->forward;
    }
    else {
        float sensivity = 0.35f;

		myTank.rotate_turret(-sensivity * deltaX);
    }
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    if (button == GLFW_MOUSE_BUTTON_2 && myTank.timeFromLastShot >= 2) {
		projectiles.push_back(myTank.shoot());
    }
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}


void Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Set shader uniforms for light & material properties
    GLint locLightPos = glGetUniformLocation(shader->program, "light_position");
    glUniform3fv(locLightPos, 1, glm::value_ptr(lightPosition));


    glm::vec3 eyePosition = camera->position;
    GLint loc_eye_position = glGetUniformLocation(shader->program, "eye_position");
    glUniform3fv(loc_eye_position, 1, glm::value_ptr(eyePosition));

    GLint loc_material_shininess = glGetUniformLocation(shader->program, "material_shininess");
    glUniform1i(loc_material_shininess, materialShininess);

    GLint loc_material_kd = glGetUniformLocation(shader->program, "material_kd");
    glUniform1f(loc_material_kd, materialKd);

    GLint loc_material_ks = glGetUniformLocation(shader->program, "material_ks");
    glUniform1f(loc_material_ks, materialKs);

    GLint loc_object_color = glGetUniformLocation(shader->program, "object_color");
    glUniform3fv(loc_object_color, 1, glm::value_ptr(color));

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = camera->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

bool Tema2::tanks_intersect(Tanc *tank1, Tanc *tank2) {
	float distance = glm::distance(tank1->position, tank2->position);
	return distance < tank1->radius + tank2->radius;
}

void Tema2::decollision_tanks(Tanc* tank1, Tanc* tank2) {
	glm::vec3 diff = tank2->position - tank1->position;
	float P = (tank1->radius + tank2->radius) - glm::distance(tank1->position, tank2->position);
	glm::vec3 new_P = glm::normalize(diff) * P;
	new_P = glm::vec3(new_P.x, 0, new_P.z);
	tank1->position -= new_P * 0.5f;
	tank2->position += new_P * 0.5f;

    if (tank1 == &myTank) {
        camera->position -= new_P * 0.5f;
    }
    else if (tank2 == &myTank) {
		camera->position += new_P * 0.5f;
	}
}

glm::vec3 closestPointOnPrism(Building& building, glm::vec3& spherePos) {
    glm::vec3 closestPoint;

    closestPoint.x = std::max(building.center.x - building.width / 2.0f, std::min(spherePos.x, building.center.x + building.width / 2.0f));
    closestPoint.y = std::max(building.center.y - building.height / 2.0f, std::min(spherePos.y, building.center.y + building.height / 2.0f));
    closestPoint.z = std::max(building.center.z - building.depth / 2.0f, std::min(spherePos.z, building.center.z + building.depth / 2.0f));

    return closestPoint;
}

bool Tema2::tank_building_intersect(Tanc* tank, Building* building) {
    glm::vec3 closestPoint = closestPointOnPrism(*building, tank->position);

    // Check if the distance between the closest point and the sphere center is less than the sphere radius
    float distance = glm::distance(tank->position, closestPoint);
    return distance < tank->radius;
}

void Tema2::decollision_tank_building(Tanc* tank, Building* building) {
    glm::vec3 closestPoint = closestPointOnPrism(*building, tank->position);

    // Calculate the displacement vector from the sphere center to the closest point
    glm::vec3 displacement = tank->position - closestPoint;

    if (glm::length(displacement) < 0.0001f) {
		// Sphere center and closest point coincide
		// Choose random (but consistent) values
		displacement = glm::vec3(1, 0, 0);
	}

    // Normalize the displacement vector
    glm::vec3 collisionNormal = glm::normalize(displacement);

    // Move the tank outside the building by adjusting its position
    tank->position = closestPoint + collisionNormal * tank->radius;

    if (tank == &myTank) {
		camera->position = tank->position - camera->distanceToTarget * camera->forward;
	}
}

bool Tema2::tank_projectile_intersect(Tanc* tank, Projectile* projectile) {
	float distance = glm::distance(tank->position, projectile->position);

    if (distance < tank->radius + projectile->scale) {
		tank->hp -= projectile->damage;
		projectile->lifeTime = 10;

        return true;
	}

    return false;
}

bool Tema2::projectile_building_intersect(Projectile* projectile, Building* building) {
	glm::vec3 closestPoint = closestPointOnPrism(*building, projectile->position);

	// Check if the distance between the closest point and the sphere center is less than the sphere radius
	float distance = glm::distance(projectile->position, closestPoint);
    if (distance < projectile->scale) {
		projectile->lifeTime = 10;

        return true;
	}

    return false;
}
