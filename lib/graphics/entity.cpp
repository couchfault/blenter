//
// Created by James Pickering on 3/19/25.
//

#include "entity.hpp"
#include "../../shapes.hpp"
#include "camera.hpp"

#include <glad/glad.h>
#include <glm/ext/matrix_transform.hpp>

#include <iostream>

auto renderEntity(const Entity& entity, const Shader shader, const Camera& camera) -> void {
    if (!entity.vao.id || entity.flags & Invisible) {
        return;
    }

    const auto useTex = (int)entity.textures.size() && !(entity.flags & IgnoreTexture);
    const auto mousePressed = Cursor::instance().pressed && entity.state & Depressed;

    const auto borderColor = getStyleAttr<glm::vec4>(entity.style, Attr::BorderColor, entity.state);
    const auto fillColor = getStyleAttr<glm::vec4>(entity.style, Attr::FillColor, entity.state);
    const auto borderWidth = getStyleAttr<float>(entity.style, Attr::BorderWidth, entity.state);
    const auto showGrid = entity.flags & ShowGrid;
    const auto showPlaceQuad = entity.flags & ShowPlaceQuad;

    shader.use();
    shader.setBool("useTexture", useTex);
    shader.setMat4("model", entity.model);
    shader.setVec3("viewPos", camera.position());
    shader.setBool("mouseDown", mousePressed);
    shader.setVec4("fillColor", fillColor);
    shader.setVec4("borderColor", borderColor);
    shader.setFloat("borderWidth", borderWidth);
    shader.setBool("showGrid", showGrid);
    shader.setBool("showPlaceQuad", showPlaceQuad);
    shader.setVec2("placeQuadSize", entity.placeQuadSize);
    shader.setVec2("placeQuadPos", entity.placeQuadPos);
    shader.setVec3("worldPos", entity.transform.pos);
    shader.setVec3("worldSize", entity.transform.size);

    if (useTex) {
        bindTextures(entity.textures);
    } else {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    switch (entity.vao.mode) {
    case RenderMode::Mode3D:
        shader.setBool("useMultiTextures", (int)entity.textures.size() > 1);
        shader.setVec3("edge", entity.edge);

        glBindVertexArray(entity.vao.id);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        logError();

        break;
    case RenderMode::Mode2D:
        shader.setBool("useMultiTextures", false);
        shader.setBool("focused", entity.state & Hovered);
        shader.setBool("selected", entity.state & Selected);

        bindTextures(entity.textures);
        glBindVertexArray(entity.vao.id);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        glBindTexture(GL_TEXTURE_2D, 0);
        logError();

        break;
    }

    // if (useTex) {
    //     for (auto i = 0; i < entity.textures.size(); i++) {
    //         glBindTexture(GL_TEXTURE_2D + i, 0);
    //     }
    // }
}

auto bindTextures(const std::vector<unsigned int>& textures) -> void {
    for (auto i = 0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, textures.at(i));
    }
}

auto applyTransform(glm::mat4& model, const Transform& trans) -> void {
    model = glm::mat4{1.f};

    model = translate(model, trans.pos);
    model = rotate(model, glm::radians(trans.rot.z), glm::vec3(0, 0, 1));
    model = rotate(model, glm::radians(trans.rot.y), glm::vec3(0, 1, 0));
    model = scale(model, trans.size);

    // model = rotate(model, glm::radians(trans.rot.x), glm::vec3(1, 0, 0));
}

// auto setPosition(Entity& ent, glm::vec3 pos) -> void {
//     ent.transform.pos = pos;
//     applyTransform(ent.model, ent.transform);
// }
//
// auto setSize(Entity& ent, glm::vec3 size) -> void {
//     ent.transform.size = size;
//     applyTransform(ent.model, ent.transform);
// }

// auto setRotation(Entity& ent, const glm::vec2 rot) -> void {
//     ent.transform.rot = rot;
//     applyTransform(ent.model, ent.transform);
// }

auto containsPoint(glm::vec2 pos, glm::vec2 size, glm::vec2 point) -> bool {
    return point.x > pos.x - size.x / 2.f &&
           point.x < pos.x + size.x / 2.f &&
           point.y > pos.y - size.y / 2.f &&
           point.y < pos.y + size.y / 2.f;
}

auto containsPoint(const std::vector<glm::vec4>& polygon, const glm::vec2& point) -> bool {
    int numIntersections = 0;
    const size_t n = polygon.size();

    for (size_t i = 0; i < n; i++) {
        const glm::vec2 v1 = polygon[i];
        const glm::vec2 v2 = polygon[(i + 1) % n];// Wrap around last vertex to first

        // Check if the ray from the point horizontally crosses the edge (v1, v2)
        if ((v1.y > point.y) != (v2.y > point.y)) {
            float xIntersection = v1.x + (point.y - v1.y) * (v2.x - v1.x) / (v2.y - v1.y);
            if (point.x < xIntersection) {
                numIntersections++;
            }
        }
    }
    // If intersections are odd, the point is inside the polygon
    return (numIntersections % 2) == 1;
}

auto genVao(const RenderMode mode) -> Vao {
    auto id = 0u;
    switch (mode) {
    case RenderMode::Mode3D:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glGenVertexArrays(1, &id);
        glBindVertexArray(id);
        glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer::cube());
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)nullptr);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        break;
    case RenderMode::Mode2D:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        const auto&& indices = (unsigned int[]){
            0, 1, 3,// first triangle
            1,
            2,
            3// second triangle
        };

        unsigned int ebo;

        glGenVertexArrays(1, &id);
        glGenBuffers(1, &ebo);
        glBindVertexArray(id);
        glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer::rectangle());
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)nullptr);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        break;
    }

    return {id, mode};
}

auto logError() -> void {
    switch (glGetError()) {
    case GL_INVALID_ENUM:
        std::cout << "Invalid enum" << std::endl;
        break;
    case GL_INVALID_VALUE:
        std::cout << "Invalid value" << std::endl;
        break;
    case GL_INVALID_OPERATION:
        std::cout << "Invalid operation" << std::endl;
        break;
    case 0:
        break;
    default:
        std::cout << "Unknown error" << std::endl;
        break;
    }
}
