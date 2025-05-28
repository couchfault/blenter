//
// Created by James Pickering on 8/31/24.
//

#pragma once

#include "lib/graphics/shader.hpp"

#include <array>
#include <vector>

struct Camera;

struct Shape {
    virtual auto render() const noexcept -> void = 0;
};

struct Triangle : Shape {
    Triangle();

    auto render() const noexcept -> void override;

private:
    unsigned int vao{};
};

struct Rectangle : Shape {
    Rectangle();

    auto render() const noexcept -> void override;

private:
    unsigned int vao{};
};

struct TexturedRectangle : Shape {
    TexturedRectangle();

    auto render() const noexcept -> void override;

private:
    unsigned int vao{};
    unsigned int texture{};
    unsigned int tex2{};
};

struct TexturedCube {
    explicit TexturedCube(Shader shader);

    auto render(const Camera &camera, Shader instancedShader) const noexcept -> void;

private:
    unsigned int vao{};

    constexpr static unsigned int _asteroidCount = 2000;
    constexpr static unsigned int _cubeCount = 9;

    Shader _shader;
    std::array<glm::vec3, 9> _cubePositions;
    std::array<glm::mat4, _asteroidCount * _cubeCount> _asteroidModels{};
    std::vector<unsigned int> _textures{};
    std::array<unsigned int, _cubeCount> _asteroidVaos{};
};

struct LightSource {
    LightSource();

    auto render(Shader shader) const noexcept -> void;

    [[nodiscard]] auto pos() const noexcept -> glm::vec3;

private:
    unsigned int vao{};
    glm::vec3 _pos;
};

struct Container {
    Container();

    auto render(Shader shader) const noexcept -> void;

private:
    unsigned int _vao{};
    unsigned int _wall{};
    unsigned int _face{};
};

struct VertexBuffer {
    static auto init() noexcept -> void;

    [[nodiscard]] static auto triangle() noexcept -> unsigned int;
    [[nodiscard]] static auto rectangle() noexcept -> unsigned int;
    [[nodiscard]] static auto cube() noexcept -> unsigned int;

private:
    static unsigned int _triangleVbo;
    static unsigned int _rectangleVbo;
    static unsigned int _cubeVbo;
};

