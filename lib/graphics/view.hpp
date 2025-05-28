//
// Created by James Pickering on 3/17/25.
//

#pragma once

#include "shader.hpp"

#include <functional>
#include <vector>

struct Engine;
struct Scene;
struct Window;

struct View {
    ~View();

    Engine& engine;
    Window& window;
    Shader shader;// TODO: move

    std::vector<std::unique_ptr<Scene>> scenes;
    glm::vec<2, double> prevCurPos;
    glm::vec3 startPos;
    glm::vec4 clear;

    std::function<void()> onTick;
    std::function<void()> onProcessInput;
};

auto init(View& view) -> void;
auto tick(View& view) -> void;
auto render(const View& view) -> void;

auto scrollEvent(View& view, float distance) -> void;
auto mouseMoveEvent(View& view, glm::vec2 pos) -> void;
auto resizeEvent(View& view, glm::vec2 size) -> void;
auto processInput(View& view, Window& win) -> void;

template <typename T, typename... Args>
    requires std::is_base_of_v<Scene, T>
auto createScene(View& view, Args&&... args) -> T* {
    view.scenes.push_back(std::make_unique<T>(view, std::forward<Args>(args)...));
    return (T*)view.scenes.back().get();
}
