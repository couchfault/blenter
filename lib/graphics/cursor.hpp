//
// Created by James Pickering on 3/16/25.
//

#pragma once

#include "camera.hpp"
#include "cursor_type.hpp"

#include <glm/glm.hpp>

#include <stdexcept>

constexpr auto VIEWPORT_SCALE = 2;

struct Cursor {
    static auto instance() -> Cursor& {
        return _instance;
    }

private:
    static Cursor _instance;

public:
    glm::vec2 viewportSize;
    glm::vec2 screenSpacePos;
    glm::vec2 screenSpaceLastPos;
    glm::vec2 clipSpacePos;
    glm::vec2 lastClipSpacePos;
    glm::vec2 screenSpaceMouseDownAt;
    glm::vec2 lastPosScreenSpace;

    CursorType cursorType = CursorType::Pointer;

    bool pressed = false;
    bool isFirstMove = true;
    bool visible = true;

    std::function<void()> onPress;
    std::function<void()> onMouseUp;
};

inline auto updateClipSpacePosition(Cursor& cur) {
    // Update the clip position

    cur.lastClipSpacePos = cur.clipSpacePos;
    // cur.clipSpacePos = glm::vec2{cur.screenSpacePos / cur.viewportSize * 2.f};

    cur.clipSpacePos = glm::vec2{cur.screenSpacePos / cur.viewportSize * 2.f * (float)VIEWPORT_SCALE};
    cur.clipSpacePos = {std::fmin(1.f, std::fmax(-1.f, cur.clipSpacePos.x)),
                        std::fmin(1.f, std::fmax(-1.f, cur.clipSpacePos.y))};
}

inline auto setScreenSpacePosition(Cursor& cur, glm::vec2 position) {
    // Set and update screen space position

    cur.screenSpacePos = position;
    updateClipSpacePosition(cur);
}

inline auto move(Cursor& cur, glm::vec2 posScreenSpace) {
    // Move the cursor to a position in screen space

    if (cur.isFirstMove) {
        cur.lastClipSpacePos = {};
        cur.clipSpacePos = {};
        cur.screenSpacePos = {};
        cur.lastPosScreenSpace = posScreenSpace;
        cur.screenSpaceLastPos = posScreenSpace;
        cur.isFirstMove = false;
    }

    const auto delta = posScreenSpace - cur.lastPosScreenSpace;

    cur.screenSpaceLastPos = cur.screenSpacePos;
    cur.screenSpacePos.x += delta.x;
    cur.screenSpacePos.y -= delta.y;
    cur.lastPosScreenSpace = posScreenSpace;

    updateClipSpacePosition(cur);
}

inline auto setPressed(Cursor& cur, const bool pressed) {
    // Set the cursor as pressed and handle calling any callbacks
    // that are set for the appropriate change

    if (!cur.pressed && pressed) {
        cur.screenSpaceMouseDownAt = cur.screenSpacePos;
        cur.pressed = true;
        if (cur.onPress) {
            cur.onPress();
        }
    } else if (cur.pressed && !pressed) {
        cur.pressed = false;
        if (cur.onMouseUp) {
            cur.onMouseUp();
        }
    }
}

inline auto toWorldSpace(const glm::vec2 clipSpace, const Camera& cam, float z = 0.f) {
    // Converts a position in clip space to a position in world space

    return cam.invProjView() * glm::vec4{clipSpace, z, 1.f};
}

inline auto delta(const glm::vec2 start, const glm::vec2 end) {
    // Gets the delta between two vectors

    return end - start;
}

inline auto updateCursor(Cursor& cur) {
    // Set the cursors last pos to the current pos

    cur.lastClipSpacePos = cur.clipSpacePos;
}

constexpr auto getImagePath(const CursorType type) -> const char* {
    switch (type) {
    case CursorType::Pointer:
        return "resources/cursor_pointer.png";
    case CursorType::Expand:
        return "resources/cursor_resize.png";
    case CursorType::Fist:
        return "resources/cursor_grabber.png";
    case CursorType::Hand:
        return "resources/cursor_hand.png";
    case CursorType::Pinch:
        return "resources/cursor_resizer.png";
    default:
        throw std::runtime_error("Cursor type not recognized");
    }
}
