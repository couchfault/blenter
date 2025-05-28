//
// Created by James Pickering on 5/27/25.
//

#include "keyboard.hpp"
#include "../graphics/window.hpp"

#include <GLFW/glfw3.h>

std::unordered_map<unsigned int, KeyState> keyStates = {};

auto initKeyboard() -> void {
    for (auto i = GLFW_KEY_SPACE; i < GLFW_KEY_LAST; i++) {
        keyStates[i] = {Action::None, false};
    }
}

auto updateKeyStates(const Window& win) -> void {
    const auto w = win.glfwWin;
    for (auto i = GLFW_KEY_SPACE; i < GLFW_KEY_LAST; i++) {
        if (auto& [action, pressed] = keyStates.at(i); !pressed && glfwGetKey(w, i) == GLFW_PRESS) {
            action = Action::Press;
            pressed = true;
        } else if (pressed && glfwGetKey(w, i) == GLFW_RELEASE) {
            action = Action::Release;
            pressed = false;
        } else {
            action = Action::None;
        }
    }
}