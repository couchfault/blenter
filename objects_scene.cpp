//
// Created by James Pickering on 3/18/25.
//

#include "objects_scene.hpp"

#include "engine.hpp"

#include "lib/graphics/color.hpp"
#include "lib/graphics/keyboard.hpp"
#include "lib/graphics/view.hpp"
#include "lib/graphics/window.hpp"

#include <GLFW/glfw3.h>

constexpr float factor = 0.3;

ObjectsScene::ObjectsScene(View& view, Engine& eng) :
    Scene{view},
    engine{eng} {
    name = "object editor";

    // fb.size = glm::vec2{view.getWindow().getSize()};
    // fb.displaySize = {WIN_WIDTH / 2.f, WIN_HEIGHT / 2.f};
    fb.clear = glm::vec4{color::dark};

    setStyleAttr(display->style, Attr::BorderWidth, 0, 50.f);

    // resize(fb, fb.size);

    cam = Camera{ProjectionType::Perspective};
    cam.setSize(glm::vec2{view.window.size});
    cam._yaw = 0;
    cam.update();
    //cam.zoom(20.f);

    flags |= SF_AcceptsAllMouseMoves | SF_AcceptsAllInputEvents;

    const auto sinYaw = std::sin(glm::radians(-cam._yaw));
    const auto cosYaw = std::cos(glm::radians(-cam._yaw));
    const auto sinPitch = std::sin(glm::radians(cam._pitch));
    const auto cosPitch = std::cos(glm::radians(cam._pitch));

    cam._pos.x = -2 * cosYaw * cosPitch;
    cam._pos.y = -2 * sinPitch;
    cam._pos.z = 2 * sinYaw * cosPitch;

    const auto width = 0.003f;
    const auto len = 2.f;
    zAxis = createEntity(*this);
    zAxis->vao = genVao(RenderMode::Mode3D);
    zAxis->transform.size.x = width;
    zAxis->transform.size.y = width;
    zAxis->transform.size.z = len;
    applyTransform(zAxis->model, zAxis->transform);
    setStyleAttr(zAxis->style, Attr::FillColor, Normal, color::green);

    yAxis = createEntity(*this);
    yAxis->vao = genVao(RenderMode::Mode3D);
    yAxis->transform.size.x = width;
    yAxis->transform.size.y = len;
    yAxis->transform.size.z = width;
    applyTransform(yAxis->model, yAxis->transform);
    setStyleAttr(yAxis->style, Attr::FillColor, Normal, color::turquoise);

    xAxis = createEntity(*this);
    xAxis->vao = genVao(RenderMode::Mode3D);
    xAxis->transform.size.x = len;
    xAxis->transform.size.y = width;
    xAxis->transform.size.z = width;
    applyTransform(xAxis->model, xAxis->transform);
    setStyleAttr(xAxis->style, Attr::FillColor, Normal, color::green);

    blockEnt = createEntity(*this);
    blockEnt->flags |= Hoverable | Depressable | ResizeOnFaceDrag | HoverUsesCursor;
    blockEnt->vao = genVao(RenderMode::Mode3D);
    blockEnt->transform.pos = glm::vec3{0, 0, 0};
    applyBlock(*engine.getSelectedBlock(), *blockEnt);
    applyTransform(blockEnt->model, blockEnt->transform);
    setStyleAttr(blockEnt->style, Attr::CursorType, Hovered, CursorType::Expand);
    setStyleAttr(blockEnt->style, Attr::CursorType, Depressed, CursorType::Fist);

    xzPlane = createEntity(*this);
    xzPlane->flags |= ShowGrid | Invisible;
    xzPlane->vao = genVao(RenderMode::Mode3D);
    xzPlane->transform.size.x = len;
    xzPlane->transform.size.y = 0.001;
    xzPlane->transform.size.z = len;
    applyTransform(xzPlane->model, xzPlane->transform);
    setStyleAttr(xzPlane->style, Attr::FillColor, Normal, color::grey);

    onDrag = [this] {
        const auto& cur = Cursor::instance();
        const auto delta = glm::vec2{cur.screenSpacePos - cur.screenSpaceLastPos};

        cam._yaw += delta.x;
        cam._pitch += delta.y;
        constrainPitch(cam);
        cam.update();

        const auto sinYaw = std::sin(glm::radians(-cam._yaw));
        const auto cosYaw = std::cos(glm::radians(-cam._yaw));
        const auto sinPitch = std::sin(glm::radians(cam._pitch));
        const auto cosPitch = std::cos(glm::radians(cam._pitch));

        cam._pos.x = -2 * cosYaw * cosPitch;
        cam._pos.y = -2 * sinPitch;
        cam._pos.z = 2 * sinYaw * cosPitch;
    };

    onMousePressed = [this] {
        if (blockEnt->edge == glm::vec3{}) {
            return;
        }
        if (!blockEnt->edge.x) {
            auto color = getStyleAttr<glm::vec4>(xAxis->style, Attr::FillColor, Normal);
            color *= factor;
            setStyleAttr(xAxis->style, Attr::FillColor, Normal, color);
            color = getStyleAttr<glm::vec4>(xAxis->style, Attr::FillColor, Normal);
        }
        if (!blockEnt->edge.y) {
            auto color = getStyleAttr<glm::vec4>(yAxis->style, Attr::FillColor, Normal);
            color *= factor;
            setStyleAttr(yAxis->style, Attr::FillColor, Normal, color);
        }
        if (!blockEnt->edge.z) {
            auto color = getStyleAttr<glm::vec4>(zAxis->style, Attr::FillColor, Normal);
            color *= factor;
            setStyleAttr(zAxis->style, Attr::FillColor, Normal, color);
        }
    };

    onMouseReleased = [this] {
        if (blockEnt->edge == glm::vec3{}) {
            return;
        }
        if (!blockEnt->edge.x) {
            auto color = getStyleAttr<glm::vec4>(xAxis->style, Attr::FillColor, Normal);
            color /= factor;
            setStyleAttr(xAxis->style, Attr::FillColor, Normal, color);
        }
        if (!blockEnt->edge.y) {
            auto color = getStyleAttr<glm::vec4>(yAxis->style, Attr::FillColor, Normal);
            color /= factor;
            setStyleAttr(yAxis->style, Attr::FillColor, Normal, color);
        }
        if (!blockEnt->edge.z) {
            auto color = getStyleAttr<glm::vec4>(zAxis->style, Attr::FillColor, Normal);
            color /= factor;
            setStyleAttr(zAxis->style, Attr::FillColor, Normal, color);
        }
    };

    onMouseMoved = [this] {
        if (engine.selAxis != Axis::None) {
            engine.lastAxis = engine.selAxis;
        }
        if (blockEnt->edge.x != 0) {
            engine.selAxis = Axis::X;
        }
        if (blockEnt->edge.y != 0) {
            engine.selAxis = Axis::Y;
        }
        if (blockEnt->edge.z != 0) {
            engine.selAxis = Axis::Z;
        } else {
            engine.selAxis = Axis::None;
        }
    };

    onProcessInput = [this] {
        if (keyStates.at(GLFW_KEY_G).action == Action::Press) {
            if (xzPlane->flags & Invisible) {
                xzPlane->flags &= ~Invisible;
            } else {
                xzPlane->flags |= Invisible;
            }
        }
    };

    engine.onSelectBlock = [this](auto) {
        applyBlock(*engine.getSelectedBlock(), *blockEnt);
        applyTransform(blockEnt->model, blockEnt->transform);
    };
}
