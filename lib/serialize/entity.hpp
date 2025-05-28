//
// Created by Henry Pitcairn on 5/28/25.
//

#pragma once
#include "../graphics/entity.hpp"
#include "../graphics/scene.hpp"
#include "cursor_type.hpp"
#include "glm_mat.hpp"
#include "glm_vec.hpp"
#include "render_mode.hpp"
#include "style.hpp"
#include "tranform.hpp"
#include <istream>
#include <ostream>
#include <ranges>


auto operator<<(std::ostream& os, const Entity& entity) -> std::ostream& {
    // Serialize Style
    os << entity.style;

    // Serialize Transform
    os << entity.transform;

    // Serialize render mode
    os << entity.vao.mode;

    // Serialize model matrix
    os << entity.model;

    // Serialize edge vector
    os << entity.edge;

    // Serialize textures vector
    auto size = static_cast<std::uint32_t>(entity.textures.size());
    os.write(reinterpret_cast<const char*>(&size), sizeof(std::uint32_t));
    for (const auto& texture : entity.textures) {
        os.write(reinterpret_cast<const char*>(&texture), sizeof(unsigned int));
    }

    // Serialize flags and state
    os.write(reinterpret_cast<const char*>(&entity.flags), sizeof(int));
    os.write(reinterpret_cast<const char*>(&entity.state), sizeof(int));

    // Serialize intersection vector
    os << entity.intersection;

    // Serialize placeQuadSize and placeQuadPos
    os << entity.placeQuadSize << entity.placeQuadPos;

    return os;
}

auto operator>>(std::istream& is, Entity& entity) -> std::istream& {
    // Deserialize Style
    is >> entity.style;

    // Deserialize Transform
    is >> entity.transform;

    // Deserialize render mode
    is >> entity.vao.mode;

    // Deserialize model matrix
    is >> entity.model;

    // Deserialize edge vector
    is >> entity.edge;

    // Deserialize textures vector
    std::uint32_t size;
    is.read(reinterpret_cast<char*>(&size), sizeof(std::uint32_t));
    entity.textures.resize(size);
    for (auto& texture : entity.textures) {
        is.read(reinterpret_cast<char*>(&texture), sizeof(unsigned int));
    }

    // Deserialize flags and state
    is.read(reinterpret_cast<char*>(&entity.flags), sizeof(int));
    is.read(reinterpret_cast<char*>(&entity.state), sizeof(int));

    // Deserialize intersection vector
    is >> entity.intersection;

    // Deserialize placeQuadSize and placeQuadPos
    is >> entity.placeQuadSize >> entity.placeQuadPos;

    return is;
}

auto writeEntities(std::ostream& os, const std::vector<Entity*>& entities) -> void {
    os << entities.size();

    for (auto entity : entities) {
        os << *entity;
    }
}

auto readEntities(std::istream& is, Scene& scene, std::vector<Entity*>& entities) -> size_t {
    size_t numEntities;
    is >> numEntities;
    auto entity = std::make_unique<Entity>();

    for (auto i = 0; i < numEntities; ++i) {
        is >> *entity;
        scene.entities.push_back(std::move(entity));
        entities.push_back(scene.entities.back().get());
    }

    return numEntities;
}
