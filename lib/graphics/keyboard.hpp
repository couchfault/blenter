//
// Created by James Pickering on 5/27/25.
//

#pragma once

#include <unordered_map>

struct Window;

enum class Action {
    Press,
    Release,
    None
};

struct KeyState {
    Action action;
    bool pressed;
};


extern std::unordered_map<unsigned int, KeyState> keyStates;

auto initKeyboard() -> void;
auto updateKeyStates(const Window& win) -> void;