//
// Created by James Pickering on 3/18/25.
//

#pragma once

#include <glm/vec2.hpp>

struct Block;
struct Engine;
struct Entity;
struct Scene;

struct ObjectEditor {
    Engine& engine;
    Scene& scene;
    Entity* blockEnt;
    Entity* xAxis;
    Entity* yAxis;
    Entity* zAxis;
    Entity* xzPlane;
    glm::vec2 lookAt{-90, 0};
};

auto init(ObjectEditor& editor, Scene& scene) -> void;
auto applyBlock(const Block& block, Entity& ent) -> void;