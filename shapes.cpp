//
// Created by James Pickering on 8/31/24.
//

#include "shapes.hpp"
#include "lib/graphics/camera.hpp"
#include "texture.hpp"

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>

// ---------------------------------------------------------------------------------------------------------------------
// Triangle
// ---------------------------------------------------------------------------------------------------------------------
Triangle::Triangle() {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer::triangle());
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) nullptr);
    glEnableVertexAttribArray(0);
}

auto Triangle::render() const noexcept -> void {
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

// ---------------------------------------------------------------------------------------------------------------------
// Rectangle
// ---------------------------------------------------------------------------------------------------------------------
Rectangle::Rectangle() {
    const auto &&indices = (unsigned int[]){
        0, 1, 3, // first triangle
        1, 2, 3 // second triangle
    };

    unsigned int ebo;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &ebo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer::rectangle());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) nullptr);
    glEnableVertexAttribArray(0);
}

auto Rectangle::render() const noexcept -> void {
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

// ---------------------------------------------------------------------------------------------------------------------
// Textured Rectangle
// ---------------------------------------------------------------------------------------------------------------------
TexturedRectangle::TexturedRectangle() {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // auto req = TextureLoadReq{};
    // req.path = "palette.png";
    // texture = Texture::fromPng(req);
    // req.path =
    // tex2 = Texture::fromPng("nav-play-inverse.png");

    const auto &&indices = (unsigned int[]){
        0, 1, 3, // first triangle
        1, 2, 3 // second triangle
    };

    unsigned int ebo;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &ebo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer::rectangle());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) nullptr);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
}

auto TexturedRectangle::render() const noexcept -> void {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, tex2);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

// ---------------------------------------------------------------------------------------------------------------------
// Textured Cube
// ---------------------------------------------------------------------------------------------------------------------
TexturedCube::TexturedCube(Shader shader)
    : _shader{shader},
      _cubePositions(std::array<glm::vec3, 9>{
          glm::vec3(5.f, 0.f, 0.f),
          glm::vec3(-5.f, 0.f, -0.f),
          glm::vec3(0.f, 0.f, 5.f),
          glm::vec3(0.f, -0.f, -5.f),
          glm::vec3(-2.7f, 3.0f, -3.5f),
          glm::vec3(2.3f, -4.0f, -2.5f),
          glm::vec3(1.5f, 2.0f, -2.5f),
          glm::vec3(4.5f, -3.2f, -1.5f),
          glm::vec3(-2.3f, 3.6f, 5.5f)
      }) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    const auto images = std::array<const char *, 9>{
        "palette.png",
        "nav-play-inverse.png",
        "large-logo.png",
        "btn-play.png",
        "gradient-logo.png",
        "face.png",
        "logo.png",
        "nav-play.png",
        "circle-logo.png"
    };

    // for (const auto img: images) {
    //     _textures.push_back(Texture::fromPng(img));
    // }

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer::cube());
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) nullptr);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    srand(static_cast<unsigned int>(glfwGetTime()));
    auto radius = 1.3f;
    auto offset = 0.17f;

    for (auto k = 0; k < _cubeCount; ++k) {
        for (auto i = 0; i < _asteroidCount; ++i) {
            auto model = glm::mat4{1.f};
            model = glm::translate(model, _cubePositions[k]);

            // Displace along circle with radius in rage [-offset, offset]
            const auto angle = static_cast<float>(i) / static_cast<float>(_asteroidCount) * 360.f;
            auto displacement = static_cast<float>((rand() % static_cast<int>(2 * offset * 100))) / 100.f - offset;
            const auto x = sin(angle) * radius + displacement;
            displacement = static_cast<float>((rand() % static_cast<int>(2 * offset * 100))) / 100.f - offset;
            const auto y = displacement;
            displacement = static_cast<float>((rand() % static_cast<int>(2 * offset * 100))) / 100.f - offset;
            const auto z = cos(angle) * radius + displacement;
            model = glm::translate(model, glm::vec3{x, y, z});

            // Scale between 0.05 and 0.25
            const auto scale = static_cast<float>((rand() % 4) / 100.f + 0.009);
            model = glm::scale(model, glm::vec3{scale});

            // Rotate
            const auto rotation = static_cast<float>(rand() % 360);
            model = glm::rotate(model, rotation, glm::vec3{0.4f, 0.6f, 0.8f});

            _asteroidModels[_asteroidCount * k + i] = model;
        }

        auto &i = _asteroidVaos[k];
        auto vbo = (unsigned int){};
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, _asteroidCount * sizeof(glm::mat4), &_asteroidModels[_asteroidCount * k],
                     GL_STATIC_DRAW);

        glGenVertexArrays(1, &i);
        glBindVertexArray(i);
        glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer::cube());
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) nullptr);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        auto vec4Size = sizeof(glm::vec4);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void *) 0);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void *) (1 * vec4Size));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void *) (2 * vec4Size));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void *) (3 * vec4Size));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glBindVertexArray(0);
    }
}

auto TexturedCube::render(const Camera &camera, Shader instancedShader) const noexcept -> void {
    for (auto i = 0; i < _cubeCount; ++i) {
        _shader.use();
        glBindVertexArray(vao);

        for (auto j = 0; j < 2; ++j) {
            glActiveTexture(GL_TEXTURE0 + j);
            glBindTexture(GL_TEXTURE_2D, _textures[(j + i) % _textures.size()]);
        }

        auto model = glm::mat4{1.f};
        model = glm::translate(model, _cubePositions[i]);
        model = glm::rotate(model, (float) glfwGetTime() * glm::radians(20.f * static_cast<float>(i + 1)),
                            glm::vec3(1.f, 0.3f, 0.f));

        _shader.setMat4("model", model);
        _shader.setVec3("viewPos", camera.position());

        glDrawArrays(GL_TRIANGLES, 0, 36);

        instancedShader.use();

        //for (auto k = 0; k < _asteroidCount; ++k) {
        glBindVertexArray(_asteroidVaos[i]);

        for (auto j = 0; j < 2; ++j) {
            glActiveTexture(GL_TEXTURE0 + j);
            glBindTexture(GL_TEXTURE_2D, _textures[(j + i) % _textures.size()]);
        }
        //_shader.setMat4("model", _asteroidModels[_asteroidCount * i + k]);

        //if (i == 0) continue;
        //glDrawArrays(GL_TRIANGLES, 16 + 36 + 2 + 3, 36);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 36, _asteroidCount);
        auto error = glGetError();

        switch (error) {
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
                std::cout << "Unkown error" << std::endl;
                break;
        }
        //}
    }
}

// ---------------------------------------------------------------------------------------------------------------------
// Textured Cube
// ---------------------------------------------------------------------------------------------------------------------
LightSource::LightSource()
    : _pos{1.2f, 1.0f, 2.0f} {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer::cube());
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) nullptr);
    glEnableVertexAttribArray(0);
}

auto LightSource::render(Shader shader) const noexcept -> void {
    auto model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(0.2f));
    model = glm::translate(model, _pos);

    //shader.setMat4("model", model);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

auto LightSource::pos() const noexcept -> glm::vec3 {
    return _pos;
}

// ---------------------------------------------------------------------------------------------------------------------
// Container
// ---------------------------------------------------------------------------------------------------------------------
Container::Container() {
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer::cube());
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) nullptr);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    // _wall = Texture::fromPng("win-bg.png");
    // _face = Texture::fromPng("face5.png", false);
}

auto Container::render(Shader shader) const noexcept -> void {
    glBindVertexArray(_vao);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _wall);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _face);

    auto model = glm::mat4(1.f);
    model = glm::scale(model, glm::vec3{15.f});

    shader.setMat4("model", model);

    glDrawArrays(GL_TRIANGLES, 0, 36);
}

// ---------------------------------------------------------------------------------------------------------------------
// Vertex Buffer
// ---------------------------------------------------------------------------------------------------------------------
unsigned int VertexBuffer::_triangleVbo = {};
unsigned int VertexBuffer::_rectangleVbo = {};
unsigned int VertexBuffer::_cubeVbo = {};

auto VertexBuffer::init() noexcept -> void {
    /**
     * Triangle
     */
    {
        const auto &&vertices = (float[]){
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f, 0.5f, 0.0f
        };

        _triangleVbo = (unsigned int){};

        glGenBuffers(1, &_triangleVbo);
        glBindBuffer(GL_ARRAY_BUFFER, _triangleVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    }

    /**
     * Rectangle
     */
    {
        const auto &&vertices = (float[]){
            0.5f, 0.5f, 0.0f, 0.f, 0.f, 1.f, 1.f, 1.f, // top right
            0.5f, -0.5f, 0.0f, 0.f, 0.f, 1.f, 1.f, 0.f, // bottom right
            -0.5f, -0.5f, 0.0f, 0.f, 0.f, 1.f, 0.f, 0.f, // bottom left
            -0.5f, 0.5f, 0.0f, 0.f, 0.f, 1.f, 0.f, 1.f // top left
        };

        _rectangleVbo = (unsigned int){};

        glGenBuffers(1, &_rectangleVbo);
        glBindBuffer(GL_ARRAY_BUFFER, _rectangleVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    }

    /**
     * Cube
     */
    {
        const auto &&vertices = (float[]){
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
            -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,

            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,

            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
        };

        _cubeVbo = (unsigned int){};

        glGenBuffers(1, &_cubeVbo);
        glBindBuffer(GL_ARRAY_BUFFER, _cubeVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    }
}

auto VertexBuffer::triangle() noexcept -> unsigned int {
    return _triangleVbo;
}

auto VertexBuffer::rectangle() noexcept -> unsigned int {
    return _rectangleVbo;
}

auto VertexBuffer::cube() noexcept -> unsigned int {
    return _cubeVbo;
}
