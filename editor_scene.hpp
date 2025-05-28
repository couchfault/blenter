//
// Created by James Pickering on 3/18/25.
//

#pragma once

#include "lib/graphics/mouse.hpp"
#include "lib/graphics/scene.hpp"

#include <chrono>

struct Engine;
struct View;

struct EditorScene : Scene {
    EditorScene(View& view, Engine& engine);

    std::vector<Entity*> blockEntities;
    Entity* floorEntity;
    // Entity* backWallEntity;
    // Entity* leftWallEntity;
    // Entity* rightWallEntity;

    glm::vec3 placePos;

    bool added = false;
    bool moving = false;

    float realY = 0.f;
    float startP = 0.f;
    std::chrono::high_resolution_clock::time_point walkStart;
    std::vector<ContainsRes> hits = {};

private:
    Engine& _engine;
};