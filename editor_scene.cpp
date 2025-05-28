//
// Created by James Pickering on 3/18/25.
//

#include "editor_scene.hpp"

#include "block.hpp"
#include "engine.hpp"
#include "lib/graphics/color.hpp"
#include "lib/graphics/keyboard.hpp"
#include "lib/graphics/mouse.hpp"
#include "lib/graphics/view.hpp"
#include "lib/graphics/window.hpp"
#include "texture.hpp"

#include <GLFW/glfw3.h>
#include <algorithm>

EditorScene::EditorScene(View& view, Engine& engine) :
    Scene{view},
    _engine{engine} {
    name = "level editor";
    flags |= SF_AcceptsAllMouseMoves;

    const auto center = glm::vec3{0.f, -0.6f, -3.f};
    const auto size = 3.f;
    const auto thick = 0.01f;

    std::cout << "Start : " << entities.size() << std::endl;

    floorEntity = createEntity(*this);
    floorEntity->vao = genVao(RenderMode::Mode3D);
    floorEntity->transform.pos = center;
    floorEntity->transform.size = {size, thick, size};
    floorEntity->textures = {engine.floorTex};
    applyTransform(floorEntity->model, floorEntity->transform);

    std::cout << "Start : " << entities.size() << std::endl;

    // backWallEntity = createEntity(*this);
    // backWallEntity->vao = genVao(RenderMode::Mode3D);
    // backWallEntity->transform.pos = {center.x, center.y + size / 2.f, center.z - size / 2.f};
    // backWallEntity->transform.size = {size, size, thick};
    // backWallEntity->textures = {engine.backWallTex};
    // applyTransform(backWallEntity->model, backWallEntity->transform);

    // leftWallEntity = createEntity(*this);
    // leftWallEntity->flags |= Invisible;
    // leftWallEntity->vao = genVao(RenderMode::Mode3D);
    // leftWallEntity->transform.pos = {center.x - size / 2.f, center.y + size / 2.f, center.z};
    // leftWallEntity->transform.size = {size, size, thick};
    // leftWallEntity->textures = {engine.leftWallTex};
    // applyTransform(leftWallEntity->model, leftWallEntity->transform);
    //
    // rightWallEntity = createEntity(*this);
    // rightWallEntity->flags |= Invisible;
    // rightWallEntity->vao = genVao(RenderMode::Mode3D);
    // rightWallEntity->transform.pos = {center.x + size / 2.f, center.y + size / 2.f, center.z};
    // rightWallEntity->transform.size = {size, size, thick};
    // rightWallEntity->textures = {engine.rightWallTex};
    // applyTransform(rightWallEntity->model, rightWallEntity->transform);

    cam = Camera{ProjectionType::Perspective};
    cam.flags |= CF_Moveable | CF_CursorControlsLookAt | CF_AnimateOnWalk;
    cam.setSize(view.window.size);

    onProcessInput = [this] {
        if (keyStates.at(GLFW_KEY_F).action == Action::Press) {
            added = true;
            const auto ent = createEntity(*this);
            ent->vao = genVao(RenderMode::Mode3D);
            ent->transform.pos = hits.front().pos;
            ent->transform.size = _engine.getSelectedBlock()->getSize();
            ent->transform.pos.y += ent->transform.size.y / 2.f;
            ent->textures = _engine.getSelectedBlock()->getTextures();
            applyTransform(ent->model, ent->transform);
            blockEntities.push_back(ent);
        }

        if (keyStates.at(GLFW_KEY_TAB).action == Action::Press) {
            _engine._selectedBlock++;
            if (_engine._selectedBlock == _engine._blocks.size()) {
                _engine._selectedBlock = 0;
            }
            if (_engine.onSelectBlock) {
                _engine.onSelectBlock(_engine._selectedBlock);
            }
        }

        if (cam.walking) {
        } else if (!cam.walking) {
            moving = false;
        }

        if (keyStates.at(GLFW_KEY_BACKSPACE).action == Action::Press) {
            if (!hits.empty()) {
                const auto range = std::ranges::remove_if(blockEntities, [this](const auto& lhs) {
                    return lhs == hits.front().ent;
                });
                blockEntities.erase(range.begin(), blockEntities.end());
                if (range.size()) {
                    entities.erase(std::ranges::remove_if(entities, [this](const auto& lhs) { return lhs.get() == hits.front().ent; }).begin(), entities.end());
                }
            }
        }
    };

    onTick = [this] {
        hits.clear();
        for (const auto& ent : entities) {
            ent->flags &= ~ShowPlaceQuad;
            ent->edge = {};
            if (auto res = ContainsRes{}; contains(*ent, cam, res)) {
                res.ent = ent.get();
                hits.emplace_back(res);
            }
        }
        if (hits.empty()) {
            return;
        }

        std::ranges::sort(hits, [](const ContainsRes& lhs, const ContainsRes& rhs) {
            return lhs.dist < rhs.dist;
        });

        auto hovEnt = hits.front().ent;

        hovEnt->placeQuadSize = {_engine.getSelectedBlock()->getSize().x, _engine.getSelectedBlock()->getSize().z};
        hovEnt->placeQuadPos = hits.front().pos;
        hovEnt->placeQuadPos.y = hits.front().pos.z;
        hovEnt->placeQuadPos.x -= hovEnt->transform.pos.x;
        hovEnt->placeQuadPos.x -= (hits.front().pos.x - hovEnt->transform.pos.x) * 2.f;
        hovEnt->placeQuadPos.y -= hovEnt->transform.pos.z;
        hovEnt->placeQuadPos.y -= (hits.front().pos.z - hovEnt->transform.pos.z) * 2.f;
        hovEnt->flags |= ShowPlaceQuad;
    };
}