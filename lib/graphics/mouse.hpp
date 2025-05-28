//
// Created by James Pickering on 5/17/25.
//

#pragma once

#include "entity.hpp"

#include <glm/vec3.hpp>

struct Camera;
struct Cursor;
struct Entity;
struct Event;

auto cursorMoved(Entity& ent, Cursor& cur, const Camera& cam) -> void;
auto handleMouseMoved(Entity& ent, Cursor& cur, Event& ev) -> void;
auto handleMousePressed(Entity& ent, Cursor& cur, Event& ev) -> void;
auto handleMouseReleased(Entity& ent, Cursor& cur, Event& ev) -> void;

struct ContainsRes {
    glm::vec3 center;
    glm::vec3 pos;
    float dist;
    glm::vec3 edge;
    Entity* ent;
};

// auto bounds(const Transform& trans) -> glm::vec4;
auto contains(Entity& ent, const Camera& cam, ContainsRes& pos) -> bool;