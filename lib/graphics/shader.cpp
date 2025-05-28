//
// Created by James Pickering on 8/31/24.
//

#include "shader.hpp"
#include "light.hpp"
#include "material.hpp"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <iostream>
#include <sstream>

Shader::Shader(const char* vertexPath, const char* fragmentPath, ShaderComponent components) :
    _components{components} {
    auto vCode = std::string{};
    auto fCode = std::string{};
    auto vFile = std::ifstream{};
    auto fFile = std::ifstream{};

    try {
        vFile.open(vertexPath);
        fFile.open(fragmentPath);

        auto vStream = std::stringstream{};
        auto fStream = std::stringstream{};

        vStream << vFile.rdbuf();
        fStream << fFile.rdbuf();

        vCode = vStream.str();
        fCode = fStream.str();
    } catch (const std::ifstream::failure& e) {
        std::cout << "Shader(): " << e.what() << std::endl;
        throw e;
    }

    const auto vCodeCStr = vCode.c_str();
    const auto fCodeCStr = fCode.c_str();

    auto success = int{};
    auto&& log = (char[512]){};

    const auto vId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vId, 1, &vCodeCStr, nullptr);
    glCompileShader(vId);
    glGetShaderiv(vId, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vId, 512, nullptr, log);
        std::cout << "Shader(): " << log << std::endl;
        throw std::runtime_error("Failed to compile vertex shader");
    }

    const auto fId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fId, 1, &fCodeCStr, nullptr);
    glCompileShader(fId);
    glGetShaderiv(fId, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(fId, 512, nullptr, log);
        std::cout << "Shader(): " << log << std::endl;
        throw std::runtime_error("Failed to compile fragment shader");
    }

    id = glCreateProgram();
    glAttachShader(id, vId);
    glAttachShader(id, fId);
    glLinkProgram(id);
    glGetProgramiv(id, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(id, 512, nullptr, log);
        std::cout << "Shader(): " << log << std::endl;
        throw std::runtime_error("Failed to link shaders");
    }

    glDeleteShader(vId);
    glDeleteShader(fId);
}

auto Shader::use() const -> void {
    glUseProgram(id);
}

auto Shader::components() const noexcept -> ShaderComponent {
    return _components;
}

auto Shader::setBool(const std::string& name, bool value) const -> void {
    glUniform1i(glGetUniformLocation(id, name.c_str()), static_cast<int>(value));
}

auto Shader::setInt(const std::string& name, int value) const -> void {
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

auto Shader::setFloat(const std::string& name, float value) const -> void {
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

auto Shader::setVec2(const std::string& name, glm::vec2 value) const -> void {
    glUniform2f(glGetUniformLocation(id, name.c_str()), value.x, value.y);
}

auto Shader::setVec3(const std::string& name, glm::vec3 value) const -> void {
    glUniform3f(glGetUniformLocation(id, name.c_str()), value.x, value.y, value.z);
}

auto Shader::setVec4(const std::string& name, glm::vec4 value) const -> void {
    glUniform4f(glGetUniformLocation(id, name.c_str()), value.x, value.y, value.z, value.w);
}

auto Shader::setMat4(const std::string& name, glm::mat4 value) const -> void {
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

auto Shader::setMaterial(const Material& material) const -> void {
    setVec3("material.ambient", material.ambient);
    setVec3("material.diffuse", material.diffuse);
    setVec3("material.specular", material.specular);
    setFloat("material.shininess", material.shininess);
}

auto Shader::setLight(const Light& light) const -> void {
    setVec3("light.ambient", light.ambient);
    setVec3("light.diffuse", light.diffuse);
    setVec3("light.specular", light.specular);
    setVec3("light.position", light.position);
}

auto setDefaults(Shader& shader) -> void {
    shader.use();
    shader.setInt("tex1", 0);
    shader.setInt("tex2", 1);
    shader.setInt("tex3", 2);
}
