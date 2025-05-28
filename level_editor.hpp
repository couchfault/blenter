//
// Created by James Pickering on 3/18/25.
//

#pragma once

#include "lib/graphics/mouse.hpp"

#include <glm/vec3.hpp>

#include <chrono>
#include <vector>

struct Engine;
struct Entity;
struct Scene;

struct LevelEditor {
    Engine& engine;
    Scene& scene;
    const std::string& saveFile;
    Entity* floor;
    std::vector<Entity*> blockEntities;
    glm::vec3 placePos;
    std::chrono::high_resolution_clock::time_point walkStart;
    std::vector<ContainsRes> hits = {};
};

auto init(LevelEditor& editor, Scene& scene) -> void;
