//
// Created by James Pickering on 3/17/25.
//

#include "view.hpp"

#include "color.hpp"
#include "event.hpp"
#include "scene.hpp"
#include "window.hpp"

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <ranges>

#define LOOP_BREAK true;
#define LOOP_CONTINUE false;

static auto forEachVisible(const std::vector<std::unique_ptr<Scene>>& scenes, auto&& callback) -> void {
    for (const auto& scene : std::ranges::views::reverse(scenes)) {
        if (scene->flags & SF_Hidden) {
            continue;
        }
        if (callback(scene.get())) {
            break;
        }
    }
}

auto scrollEvent(View& view, float distance) -> void {
    auto ev = Event{};
    forEachVisible(view.scenes, [&ev, &distance](Scene* scene) {
        scene->wheelScrolled(distance);
        if (ev.isAccepted()) {
            return LOOP_BREAK;
        }
        return LOOP_CONTINUE;
    });
}

auto mouseMoveEvent(View& view, glm::vec2 pos) -> void {
    move(Cursor::instance(), pos);

    auto ev = Event{};
    forEachVisible(view.scenes, [&ev](Scene* scene) {
        scene->mouseMoved(ev);
        if (ev.isAccepted()) {
            return LOOP_BREAK;
        }
        return LOOP_CONTINUE;
    });
}

auto resizeEvent(View& view, glm::vec2 size) -> void {
    setScreenSpacePosition(Cursor::instance(), size);
    for (const auto& scene : std::ranges::views::reverse(view.scenes)) {
        scene->windowResized(size);
    }
}

auto processInput(View& view, Window& win) -> void {
    setPressed(Cursor::instance(), glfwGetMouseButton(view.window.glfwWin, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);

    if (view.onProcessInput) {
        view.onProcessInput();
    }

    auto ev = Event{};
    forEachVisible(view.scenes, [&ev](Scene* scene) {
        scene->processInput(ev);
        if (ev.isAccepted()) {
            return LOOP_BREAK;
        }
        return LOOP_CONTINUE;
    });
}

auto init(View& view) -> void {
    Cursor::instance().onPress = [&view] {
        auto ev = Event{};
        for (const auto& scene : std::ranges::views::reverse(view.scenes)) {
            if (scene->flags & SF_Hidden) {
                continue;
            }
            scene->mousePressed(ev);
            if (ev.isAccepted()) {
                break;
            }
        }
    };

    Cursor::instance().onMouseUp = [&view] {
        auto ev = Event{};
        for (const auto& scene : std::ranges::views::reverse(view.scenes)) {
            if (scene->flags & SF_Hidden) {
                continue;
            }
            scene->mouseReleased(ev);
            if (ev.isAccepted()) {
                break;
            }
        }
    };
}

View::~View() = default;

auto tick(View& view) -> void {
    if (view.onTick) {
        view.onTick();
    }

    updateCursor(Cursor::instance());
    forEachVisible(view.scenes, [&](const Scene* scene) {
        if (scene->onTick) {
            scene->onTick();
        }
        return LOOP_CONTINUE;
    });
}

auto render(const View& view) -> void {
    glClearColor(view.clear.r, view.clear.g, view.clear.b, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // -------------------------------------------------------------
    // Render layers
    // -------------------------------------------------------------
    auto z = -2.f;
    for (const auto& scene : view.scenes) {
        if (scene->flags & SF_Hidden) {
            continue;
        }
        scene->render(z);
        z += 0.1f;
    }
}
