//
// Created by James Pickering on 3/18/25.
//

#include "window.hpp"

#include "keyboard.hpp"
#include "view.hpp"

#include <glad/glad.h>
#include <glm/vec2.hpp>

#include <GLFW/glfw3.h>

inline auto framebufferSizeCallback(GLFWwindow* window, int width, int height) -> void;
inline auto cursorCallback(GLFWwindow* window, double xPos, double yPos) -> void;
inline auto scrollCallback(GLFWwindow*, double, double yOffset) -> void;

inline auto getWindow(GLFWwindow* window) {
    return static_cast<Window*>(glfwGetWindowUserPointer(window));
}

Window::Window(std::string title, const glm::ivec2 size) :
    title{std::move(title)},
    size{size} {
    // ---------------------------------------------------------------------------------------------------------------
    // Initialize GLFW
    //
    // - the first two hints require a certain OpenGL version
    // - the third hint specifies we want to use the core profile
    // - the final one is required for macOS (not sure why)
    // ---------------------------------------------------------------------------------------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    // ---------------------------------------------------------------------------------------------------------------
    // Create a window object
    //
    // First we request a window to be created. After that, we make the window we just created the current context.
    // In this project we want to hide the mouse like in Minecraft so we disable the cursor. Then we want to setup
    // all the relevant callbacks.
    // ---------------------------------------------------------------------------------------------------------------
    glfwWin = glfwCreateWindow(size.x, size.y, title.c_str(), nullptr, nullptr);
    if (!glfwWin) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(glfwWin);
    glfwSetWindowUserPointer(glfwWin, this);
    glfwSetInputMode(glfwWin, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetFramebufferSizeCallback(glfwWin, framebufferSizeCallback);
    glfwSetCursorPosCallback(glfwWin, cursorCallback);
    glfwSetScrollCallback(glfwWin, scrollCallback);
}

Window::~Window() {
    glfwDestroyWindow(glfwWin);
}

auto processInput(Window& win) -> void {
    updateKeyStates(win);
    if (win.view) {
        processInput(*win.view, win);
    }
}

inline auto framebufferSizeCallback(GLFWwindow* window, const int width, const int height) -> void {
    glViewport(0, 0, width, height);

    const auto win = getWindow(window);
    win->size = {width, height};

    if (win->view) {
        resizeEvent(*win->view, {width, height});
    }
}

inline auto cursorCallback(GLFWwindow* window, const double xPos, const double yPos) -> void {
    if (const auto win = getWindow(window); win->view) {
        mouseMoveEvent(*win->view, {xPos, yPos});
    }
}

inline auto scrollCallback(GLFWwindow* window, double, const double yOffset) -> void {
    if (const auto win = getWindow(window); win->view) {
        scrollEvent(*win->view, (float)yOffset);
    }
}