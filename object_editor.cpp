//
// Created by James Pickering on 3/18/25.
//

#include "object_editor.hpp"

#include "block.hpp"
#include "engine.hpp"

#include "lib/graphics/color.hpp"
#include "lib/graphics/keyboard.hpp"
#include "lib/graphics/scene.hpp"
#include "lib/graphics/view.hpp"
#include "lib/graphics/window.hpp"

#include <GLFW/glfw3.h>

#define DEP_COL_FACTOR 0.3;
#define AXIS_WIDTH 0.003f;
#define AXIS_LENGTH 2.f;
#define MIN_HEIGHT 0.001f;

auto init(ObjectEditor& editor, Scene& scene) -> void {
    scene.name = "object editor";
    scene.flags |= SF_AcceptsAllMouseMoves | SF_AcceptsAllInputEvents;
    scene.fb.clear = glm::vec4{color::dark};
    scene.cam = Camera{ProjectionType::Perspective};
    scene.cam.setSize(glm::vec2{scene.view.window.size});
    scene.cam._yaw = 0;
    scene.cam.update();
    setStyleAttr(scene.display->style, Attr::BorderWidth, 0, 50.f);

    const auto sinYaw = std::sin(glm::radians(-scene.cam._yaw));
    const auto cosYaw = std::cos(glm::radians(-scene.cam._yaw));
    const auto sinPitch = std::sin(glm::radians(scene.cam._pitch));
    const auto cosPitch = std::cos(glm::radians(scene.cam._pitch));

    scene.cam._pos.x = -2 * cosYaw * cosPitch;
    scene.cam._pos.y = -2 * sinPitch;
    scene.cam._pos.z = 2 * sinYaw * cosPitch;

    const auto xAxis = createEntity(scene);
    xAxis->vao = genVao(RenderMode::Mode3D);
    xAxis->transform.size.x = AXIS_LENGTH;
    xAxis->transform.size.y = AXIS_WIDTH;
    xAxis->transform.size.z = AXIS_WIDTH;
    applyTransform(xAxis->model, xAxis->transform);
    setStyleAttr(xAxis->style, Attr::FillColor, Normal, color::green);
    editor.xAxis = xAxis;

    const auto yAxis = createEntity(scene);
    yAxis->vao = genVao(RenderMode::Mode3D);
    yAxis->transform.size.x = AXIS_WIDTH;
    yAxis->transform.size.y = AXIS_LENGTH;
    yAxis->transform.size.z = AXIS_WIDTH;
    applyTransform(yAxis->model, yAxis->transform);
    setStyleAttr(yAxis->style, Attr::FillColor, Normal, color::turquoise);
    editor.yAxis = yAxis;

    const auto zAxis = createEntity(scene);
    zAxis->vao = genVao(RenderMode::Mode3D);
    zAxis->transform.size.x = AXIS_WIDTH;
    zAxis->transform.size.y = AXIS_WIDTH;
    zAxis->transform.size.z = AXIS_LENGTH;
    applyTransform(zAxis->model, zAxis->transform);
    setStyleAttr(zAxis->style, Attr::FillColor, Normal, color::green);
    editor.zAxis = zAxis;

    const auto blockEnt = createEntity(scene);
    blockEnt->flags |= Hoverable | Depressable | ResizeOnFaceDrag | HoverUsesCursor;
    blockEnt->vao = genVao(RenderMode::Mode3D);
    blockEnt->transform.pos = glm::vec3{0, 0, 0};
    applyBlock(*editor.engine.getSelectedBlock(), *blockEnt);
    applyTransform(blockEnt->model, blockEnt->transform);
    setStyleAttr(blockEnt->style, Attr::CursorType, Hovered, CursorType::Expand);
    setStyleAttr(blockEnt->style, Attr::CursorType, Depressed, CursorType::Fist);
    editor.blockEnt = blockEnt;

    const auto xzPlane = createEntity(scene);
    xzPlane->flags |= ShowGrid | Invisible;
    xzPlane->vao = genVao(RenderMode::Mode3D);
    xzPlane->transform.size.x = AXIS_LENGTH;
    xzPlane->transform.size.y = MIN_HEIGHT;
    xzPlane->transform.size.z = AXIS_LENGTH;
    applyTransform(xzPlane->model, xzPlane->transform);
    setStyleAttr(xzPlane->style, Attr::FillColor, Normal, color::grey);
    editor.xzPlane = xzPlane;

    scene.onDrag = [&scene] {
        const auto& cur = Cursor::instance();
        const auto delta = glm::vec2{cur.screenSpacePos - cur.screenSpaceLastPos};

        scene.cam._yaw += delta.x;
        scene.cam._pitch += delta.y;
        constrainPitch(scene.cam);
        scene.cam.update();

        const auto sinYaw = std::sin(glm::radians(-scene.cam._yaw));
        const auto cosYaw = std::cos(glm::radians(-scene.cam._yaw));
        const auto sinPitch = std::sin(glm::radians(scene.cam._pitch));
        const auto cosPitch = std::cos(glm::radians(scene.cam._pitch));

        scene.cam._pos.x = -2 * cosYaw * cosPitch;
        scene.cam._pos.y = -2 * sinPitch;
        scene.cam._pos.z = 2 * sinYaw * cosPitch;
    };

    scene.onMousePressed = [&editor] {
        if (editor.blockEnt->edge == glm::vec3{}) {
            return;
        }
        if (!editor.blockEnt->edge.x) {
            auto color = getStyleAttr<glm::vec4>(editor.xAxis->style, Attr::FillColor, Normal);
            color *= DEP_COL_FACTOR;
            setStyleAttr(editor.xAxis->style, Attr::FillColor, Normal, color);
            color = getStyleAttr<glm::vec4>(editor.xAxis->style, Attr::FillColor, Normal);
        }
        if (!editor.blockEnt->edge.y) {
            auto color = getStyleAttr<glm::vec4>(editor.yAxis->style, Attr::FillColor, Normal);
            color *= DEP_COL_FACTOR;
            setStyleAttr(editor.yAxis->style, Attr::FillColor, Normal, color);
        }
        if (!editor.blockEnt->edge.z) {
            auto color = getStyleAttr<glm::vec4>(editor.zAxis->style, Attr::FillColor, Normal);
            color *= DEP_COL_FACTOR;
            setStyleAttr(editor.zAxis->style, Attr::FillColor, Normal, color);
        }
    };

    scene.onMouseReleased = [&editor] {
        if (editor.blockEnt->edge == glm::vec3{}) {
            return;
        }
        if (!editor.blockEnt->edge.x) {
            auto color = getStyleAttr<glm::vec4>(editor.xAxis->style, Attr::FillColor, Normal);
            color /= DEP_COL_FACTOR;
            setStyleAttr(editor.xAxis->style, Attr::FillColor, Normal, color);
        }
        if (!editor.blockEnt->edge.y) {
            auto color = getStyleAttr<glm::vec4>(editor.yAxis->style, Attr::FillColor, Normal);
            color /= DEP_COL_FACTOR;
            setStyleAttr(editor.yAxis->style, Attr::FillColor, Normal, color);
        }
        if (!editor.blockEnt->edge.z) {
            auto color = getStyleAttr<glm::vec4>(editor.zAxis->style, Attr::FillColor, Normal);
            color /= DEP_COL_FACTOR;
            setStyleAttr(editor.zAxis->style, Attr::FillColor, Normal, color);
        }

        const auto block = editor.engine.getSelectedBlock();
        block->_size = editor.blockEnt->transform.size;
    };

    scene.onMouseMoved = [&editor] {
        if (editor.engine.selAxis != Axis::None) {
            editor.engine.lastAxis = editor.engine.selAxis;
        }
        if (editor.blockEnt->edge.x != 0) {
            editor.engine.selAxis = Axis::X;
        }
        if (editor.blockEnt->edge.y != 0) {
            editor.engine.selAxis = Axis::Y;
        }
        if (editor.blockEnt->edge.z != 0) {
            editor.engine.selAxis = Axis::Z;
        } else {
            editor.engine.selAxis = Axis::None;
        }
    };

    scene.onProcessInput = [&editor] {
        if (keyStates.at(GLFW_KEY_G).action == Action::Press) {
            if (editor.xzPlane->flags & Invisible) {
                editor.xzPlane->flags &= ~Invisible;
            } else {
                editor.xzPlane->flags |= Invisible;
            }
        }
    };

    editor.engine.onSelectBlock = [&editor](auto) {
        applyBlock(*editor.engine.getSelectedBlock(), *editor.blockEnt);
        applyTransform(editor.blockEnt->model, editor.blockEnt->transform);
    };
}

auto applyBlock(const Block& block, Entity& ent) -> void {
    ent.transform.size = block.getSize();
    ent.textures = block.getTextures();
}
