//
// Created by James Pickering on 9/2/24.
//

#pragma once

#include <glm/glm.hpp>

struct Shader;

struct Light {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 position;
};

struct DirectionalLight {
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    auto applyToShader(const Shader& shader) const noexcept -> void;
};

struct Attenuation {
    float constant;
    float linear;
    float quadratic;
};

struct Distance {
    static const Attenuation D7;
    static const Attenuation D13;
    static const Attenuation D20;
    static const Attenuation D32;
    static const Attenuation D50;
    static const Attenuation D65;
    static const Attenuation D100;
    static const Attenuation D160;
    static const Attenuation D200;
    static const Attenuation D325;
    static const Attenuation D600;
    static const Attenuation D3250;
};

struct PointLight {
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    Attenuation attenuation;

    auto applyToShader(const Shader& shader) const noexcept -> void;
};

struct Flashlight {
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float cutoff;
    float outerCutoff;

    auto applyToShader(const Shader& shader) const noexcept -> void;
    auto update(const Shader& shader, glm::vec3 position, glm::vec3 direction) noexcept -> void;
};
