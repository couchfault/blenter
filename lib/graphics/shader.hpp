//
// Created by James Pickering on 8/31/24.
//

#pragma once

#include <glm/glm.hpp>

#include <string>

struct Material;
struct Light;

enum class ShaderComponent : std::uint16_t {
    None = 0,
    DirectionalLight = 1,
    PointLight = 2,
    Flashlight = 4,
};

struct Shader {
    Shader(const char* vertexPath, const char* fragmentPath, ShaderComponent components = ShaderComponent::None);

    auto use() const -> void;
    auto setBool(const std::string& name, bool value) const -> void;
    auto setInt(const std::string& name, int value) const -> void;
    auto setFloat(const std::string& name, float value) const -> void;
    auto setVec2(const std::string& name, glm::vec2 value) const -> void;
    auto setVec3(const std::string& name, glm::vec3 value) const -> void;
    auto setVec4(const std::string& name, glm::vec4 value) const -> void;
    auto setMat4(const std::string& name, glm::mat4 value) const -> void;

    auto setMaterial(const Material& material) const -> void;
    auto setLight(const Light& light) const -> void;

    [[nodiscard]] auto components() const noexcept -> ShaderComponent;

    unsigned int id;

private:
    ShaderComponent _components;
};

auto setDefaults(Shader& shader) -> void;
