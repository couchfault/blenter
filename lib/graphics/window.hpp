//
// Created by James Pickering on 3/17/25.
//

#pragma once

#include <glm/vec2.hpp>

#include <functional>
#include <string>

class GLFWwindow;
struct Window;
struct View;

struct Window {
    Window(std::string title, glm::ivec2 size);
    ~Window();

    std::string title;
    GLFWwindow* glfwWin;
    glm::ivec2 size;
    View* view = nullptr;
    bool mousePressedOnce = false;
};

auto processInput(Window& win) -> void;
