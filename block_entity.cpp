//
// Created by James Pickering on 3/16/25.
//

#include "block.hpp"
#include "lib/graphics/entity.hpp"

auto applyBlock(const Block& block, Entity& ent) -> void {
    ent.transform.size = block.getSize();
    ent.textures = block.getTextures();
}
