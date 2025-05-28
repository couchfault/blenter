//
// Created by James Pickering on 3/19/25.
//

#pragma once

#include "cursor.hpp"
#include "cursor_type.hpp"
#include "event.hpp"
#include "shader.hpp"
#include "style.hpp"

#include <variant>

struct Camera;

enum EntityFlag {
    Draggable = 1 << 0,
    Dragging = 1 << 1,
    Hoverable = 1 << 3,
    Selectable = 1 << 4,
    IgnoreTexture = 1 << 5,
    FollowsMouse = 1 << 6,
    Depressable = 1 << 7,
    ResizeOnFaceDrag = 1 << 9,
    ShowGrid = 1 << 10,
    Invisible = 1 << 11,
    HoverUsesCursor = 1 << 12,
    ShowPlaceQuad = 1 << 13,
};

enum StateFlag {
    Normal = 0,
    Hovered = 1 << 0,
    Depressed = 1 << 1,
    Selected = 1 << 2,
    All = 1 << 3
};

struct Transform {
    glm::vec3 pos;
    glm::vec3 size;
    glm::vec3 rot;
};

enum class RenderMode {
    Mode2D,
    Mode3D
};

struct Vao {
    unsigned int id = 0;
    RenderMode mode = RenderMode::Mode3D;
};

enum class Axis {
    X,
    Y,
    Z,
    None
};

struct Face {
    glm::vec3 center;
    Axis axis;
};

struct Entity {
    Style style;
    Transform transform;

    glm::mat4 model;
    glm::vec3 edge;// TODO: rename to face
    std::vector<unsigned int> textures;
    Vao vao;
    int flags = 0;
    int state = 0;

    glm::vec3 intersection;
    glm::vec2 placeQuadSize;
    glm::vec2 placeQuadPos;

    std::function<void(Entity&)> onClick{nullptr};
    std::function<void(Entity&)> onDrag{nullptr};
};

// render.hpp
auto renderEntity(const Entity& entity, Shader shader, const Camera& camera) -> void;
auto bindTextures(const std::vector<unsigned int>& textures) -> void;

// transform.hpp
// auto setPosition(Entity& ent, glm::vec3 pos) -> void;
// auto setSize(Entity& ent, glm::vec3 size) -> void;
// auto setRotation(Entity& ent, glm::vec2 rot) -> void;
auto applyTransform(glm::mat4& model, const Transform& trans) -> void;
auto containsPoint(glm::vec2 pos, glm::vec2 size, glm::vec2 point) -> bool;
bool containsPoint(const std::vector<glm::vec4>& polygon, const glm::vec2& point);

// cursor events.hpp

// graphics.hpp
auto genVao(RenderMode mode) -> Vao;
auto logError() -> void;