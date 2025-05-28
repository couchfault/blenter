//
// Created by James Pickering on 3/18/25.
//

#pragma once

#include "lib/graphics/camera.hpp"
#include "lib/graphics/scene.hpp"
#include "texture.hpp"

#include <memory>

struct Engine;
struct CursorEntity;

struct UiScene : Scene {
    UiScene(View& view, Engine& engine);

    Entity* entCursor;
    Entity* crosshair;

    std::vector<Entity*> blockBtns;
    std::unordered_map<CursorType, unsigned int> curTexs;

    Engine& engine;
};