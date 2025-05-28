//
// Created by James Pickering on 3/18/25.
//

#pragma once

#include "block_entity.hpp"

#include "lib/graphics/scene.hpp"

struct Engine;
struct View;

struct ObjectsScene : Scene {
    ObjectsScene(View& view, Engine& engine);

    Engine& engine;
    Entity* blockEnt;
    // Entity* rayBlockEnt;
    // Entity* camEnt;
    Entity* xAxis;
    Entity* yAxis;
    Entity* zAxis;
    // Entity* rayEnt;
    Entity* xzPlane;
    // Entity* locOnPlane;

    glm::vec2 lookAt{-90, 0};
};