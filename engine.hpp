//
// Created by James Pickering on 3/18/25.
//

#pragma once

#include "lib/graphics/entity.hpp"

#include <filesystem>
#include <functional>

struct Block;

struct Engine {
    explicit Engine(std::filesystem::path path);
    ~Engine();

    auto selectBlock(unsigned int idx) -> void;

    [[nodiscard]] auto getBlocks() -> std::vector<std::unique_ptr<Block>>&;
    [[nodiscard]] auto getSelectedBlock() -> Block*;

    bool shiftPressed = false;
    Axis selAxis = Axis::None;
    Axis lastAxis = Axis::None;

    std::filesystem::path _path;
    std::vector<std::unique_ptr<Block>> _blocks;
    unsigned int _selectedBlock;
    unsigned int floorTex;
    unsigned int backWallTex;
    unsigned int leftWallTex;
    unsigned int rightWallTex;

    std::function<void(unsigned int)> onSelectBlock;
};
