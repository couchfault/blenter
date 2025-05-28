//
// Created by James Pickering on 3/18/25.
//

#include "ui_scene.hpp"

#include "lib/graphics/view.hpp"

#include "block.hpp"
#include "engine.hpp"
#include "lib/graphics/window.hpp"
#include "texture.hpp"

UiScene::UiScene(View& view, Engine& eng) :
    Scene{view},
    engine{eng} {
    name = "ui layer";

    cam = Camera{ProjectionType::Orthographic};
    cam.setSize(view.window.size);

    auto offset = 0;
    auto i = 0;
    for (const auto& block : engine.getBlocks()) {
        auto blockEntity = createEntity(*this);
        blockEntity->flags |= Hoverable | Selectable | Depressable;
        blockEntity->vao = genVao(RenderMode::Mode2D);
        blockEntity->textures.push_back(block->getTextures().front());
        blockEntity->transform.pos = {-350, 270 - offset, -2};
        blockEntity->transform.size = {50, 50, 10};
        blockEntity->onClick = [this, idx = i++](auto& sender) {
            for (const auto& b : blockBtns) {
                if (b == &sender) {
                    continue;
                }
                b->state &= ~Selected;
            }
            engine.selectBlock(idx);
        };

        applyStyle(blockEntity->style, StyleClass::Default);
        applyTransform(blockEntity->model, blockEntity->transform);

        blockBtns.push_back(blockEntity);
        offset += 80;
    }

    auto type = CursorType::Pointer;
    while (type != CursorType::None) {
        curTexs[type] = Texture::fromPng({getImagePath(type)});
        type = (CursorType)((int)type + 1);
    }

    entCursor = createEntity(*this);
    entCursor->vao = genVao(RenderMode::Mode2D);
    entCursor->textures.push_back(curTexs[CursorType::Pointer]);
    entCursor->flags |= FollowsMouse | Invisible;
    entCursor->transform.pos.z = 0;
    entCursor->transform.pos.x = 0;
    entCursor->transform.size = {50, 50, 1};
    applyTransform(entCursor->model, entCursor->transform);

    crosshair = createEntity(*this);
    crosshair->vao = genVao(RenderMode::Mode2D);
    crosshair->textures.push_back(Texture::fromPng({"resources/crosshair.png"}));
    crosshair->transform.pos = {0, 0, -2};
    crosshair->transform.size = {20, 20, 1};
    applyTransform(crosshair->model, crosshair->transform);

    onTick = [this] {
        // Update cursor texture
        const auto& cur = Cursor::instance();
        if (entCursor->textures.front() != curTexs.at(cur.cursorType)) {
            entCursor->textures.clear();
            entCursor->textures.push_back(curTexs.at(cur.cursorType));
        }

        // Update selected box
        for (const auto& b : blockBtns) {
            if (b->textures.front() == engine.getSelectedBlock()->getTextures().front()) {
                b->state |= Selected;
            } else {
                b->state &= ~Selected;
            }
        }
    };
}
