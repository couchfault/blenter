//
// Created by James Pickering on 3/18/25.
//

#pragma once

#include "camera.hpp"
#include "entity.hpp"
#include "event.hpp"

#include <glm/vec2.hpp>

#include <memory>
#include <vector>

struct View;

constexpr auto WIN_WIDTH = 900;
constexpr auto WIN_HEIGHT = 700;

enum SceneFlags {
    CameraZoomsOnScroll = 1 << 0,
    SF_Hidden = 1 << 1,
    SF_AcceptsAllMouseMoves = 1 << 2,
    SF_AcceptsAllInputEvents = 1 << 3,
};

struct Framebuffer {
    unsigned int id;
    unsigned int tex;
    unsigned int rb;
    glm::vec4 clear{0.f};
    glm::vec2 scale{1.f};
    glm::vec2 size{WIN_WIDTH, WIN_HEIGHT};
    glm::vec2 displaySize{WIN_WIDTH, WIN_HEIGHT};
};

auto init(Framebuffer& fb) -> void;
auto resize(const Framebuffer& fb, glm::vec2 size) -> void;

struct Scene {
    explicit Scene(View& view);
    virtual ~Scene();

    auto render(float z) const -> void;
    auto wheelScrolled(float offset) -> void;
    auto mousePressed(Event& ev) const -> void;
    auto mouseReleased(Event& ev) const -> void;
    auto mouseMoved(Event& ev) -> void;
    auto windowResized(glm::ivec2 size) -> void;
    auto processInput(Event& ev) -> void;

    int flags = 0;
    View& view;
    Camera cam;
    Camera outCam;

    float blur = 0.f;
    float desat = 0.f;

    Framebuffer fb;

    std::string name;

    std::vector<std::unique_ptr<Entity>> entities;
    std::unique_ptr<Entity> display;

    std::function<void()> onTick;
    std::function<void()> onDrag;
    std::function<void()> onMousePressed;
    std::function<void()> onMouseReleased;
    std::function<void()> onMouseMoved;
    std::function<void()> onProcessInput;
};

auto createEntity(Scene& scene) -> Entity*;
