//
// Created by James Pickering on 3/16/25.
//

#pragma once

#include "texture.hpp"

#include <glm/vec3.hpp>
#include <rapidjson/document.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>
#include <vector>

struct Block {
    auto loadInfo() -> void {
        const auto infoPath = _path / "info.json";
        if (!exists(infoPath)) {
            throw std::runtime_error("Info not found");
        }

        auto doc = rapidjson::Document{};

        std::ifstream file(infoPath);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file");
        }
        std::stringstream buffer;
        buffer << file.rdbuf();

        doc.Parse(buffer.str().c_str());
        _size = {doc["width"].GetFloat(),
                 doc["height"].GetFloat(),
                 doc["length"].GetFloat()};
    }

    explicit Block(std::filesystem::path path) :
        _path{std::move(path)} {
        if (!exists(_path)) {
            throw std::runtime_error("Dir not found");
        }
        if (!is_directory(_path)) {
            return;
        }

        for (const auto& entry : std::filesystem::directory_iterator(_path)) {
            if (is_directory(entry.path())) {
                continue;
            }
            if (entry.path().extension() == ".png") {
                auto req = TextureLoadReq{};
                req.path = entry.path().string();
                req.hasAlpha = false;
                req.repeat = false;

                //std::cout << "Loading " << req.path << std::endl;
                _textures.push_back(Texture::fromPng(req));
            }
        }

        loadInfo();
    }

    [[nodiscard]] auto getTextures() const -> const auto& {
        return _textures;
    }

    [[nodiscard]] auto getSize() const -> const auto& {
        return _size;
    }

    std::filesystem::path _path;
    std::vector<unsigned int> _textures;

    glm::vec3 _size;
};
