//
// Created by James Pickering on 3/18/25.
//

#include "level_editor.hpp"

#include "block.hpp"
#include "engine.hpp"

#include "lib/graphics/color.hpp"
#include "lib/graphics/keyboard.hpp"
#include "lib/graphics/mouse.hpp"
#include "lib/graphics/scene.hpp"
#include "lib/graphics/view.hpp"
#include "lib/graphics/window.hpp"

#include <GLFW/glfw3.h>
#include <algorithm>

constexpr auto FLOOR_POS = glm::vec3{0.f, -0.6f, -3.f};
constexpr auto FLOOR_SIZE = glm::vec3{3.f, 0.01f, 3.f};

auto init(LevelEditor& editor, Scene& scene) -> void {
    scene.name = "level editor";
    scene.flags |= SF_AcceptsAllMouseMoves;

    scene.cam = Camera{ProjectionType::Perspective};
    scene.cam.flags |= CF_Moveable | CF_CursorControlsLookAt | CF_AnimateOnWalk;
    scene.cam.setSize(scene.view.window.size);

    const auto floor = createEntity(scene);
    floor->vao = genVao(RenderMode::Mode3D);
    floor->transform.pos = FLOOR_POS;
    floor->transform.size = FLOOR_SIZE;
    floor->textures = {editor.engine.floorTex};
    applyTransform(floor->model, floor->transform);
    editor.floor = floor;

    scene.onProcessInput = [&editor, &scene] {
        if (keyStates.at(GLFW_KEY_F).action == Action::Press) {
            const auto ent = createEntity(scene);
            ent->vao = genVao(RenderMode::Mode3D);
            ent->transform.pos = editor.hits.front().pos;
            ent->transform.size = editor.engine.getSelectedBlock()->getSize();
            ent->transform.pos.y += ent->transform.size.y / 2.f;
            ent->textures = editor.engine.getSelectedBlock()->getTextures();
            applyTransform(ent->model, ent->transform);
            editor.blockEntities.push_back(ent);
        }

        if (keyStates.at(GLFW_KEY_TAB).action == Action::Press) {
            editor.engine._selectedBlock++;
            if (editor.engine._selectedBlock == editor.engine._blocks.size()) {
                editor.engine._selectedBlock = 0;
            }
            if (editor.engine.onSelectBlock) {
                editor.engine.onSelectBlock(editor.engine._selectedBlock);
            }
        }

        if (keyStates.at(GLFW_KEY_BACKSPACE).action == Action::Press) {
            if (!editor.hits.empty()) {
                const auto range = std::ranges::remove_if(editor.blockEntities, [editor](const auto& lhs) {
                    return lhs == editor.hits.front().ent;
                });
                editor.blockEntities.erase(range.begin(), editor.blockEntities.end());
                if (!range.empty()) {
                    scene.entities.erase(
                        std::ranges::remove_if(scene.entities, [&editor](const auto& lhs) {
                            return lhs.get() == editor.hits.front().ent;
                        }).begin(),
                        scene.entities.end());
                }
            }
        }
    };

    scene.onTick = [&editor, &scene] {
        editor.hits.clear();
        for (const auto& ent : scene.entities) {
            ent->flags &= ~ShowPlaceQuad;
            ent->edge = {};
            if (auto res = ContainsRes{}; contains(*ent, scene.cam, res)) {
                res.ent = ent.get();
                editor.hits.emplace_back(res);
            }
        }
        if (editor.hits.empty()) {
            return;
        }

        std::ranges::sort(editor.hits, [](const ContainsRes& lhs, const ContainsRes& rhs) {
            return lhs.dist < rhs.dist;
        });

        auto hovEnt = editor.hits.front().ent;

        hovEnt->placeQuadSize = {editor.engine.getSelectedBlock()->getSize().x, editor.engine.getSelectedBlock()->getSize().z};
        hovEnt->placeQuadPos = editor.hits.front().pos;
        hovEnt->placeQuadPos.y = editor.hits.front().pos.z;
        hovEnt->placeQuadPos.x -= hovEnt->transform.pos.x;
        hovEnt->placeQuadPos.x -= (editor.hits.front().pos.x - hovEnt->transform.pos.x) * 2.f;
        hovEnt->placeQuadPos.y -= hovEnt->transform.pos.z;
        hovEnt->placeQuadPos.y -= (editor.hits.front().pos.z - hovEnt->transform.pos.z) * 2.f;
        hovEnt->flags |= ShowPlaceQuad;
    };
}