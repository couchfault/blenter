//
// Created by James Pickering on 9/2/24.
//

#include "light.hpp"

#include "enum_math.hpp"
#include "shader.hpp"

#include <iostream>

auto DirectionalLight::applyToShader(const Shader& shader) const noexcept -> void {
    if ((shader.components() & ShaderComponent::DirectionalLight) == ShaderComponent::None) {
        std::cout << "Error: shader does not support DirectionalLight" << std::endl;
        return;
    }

    shader.setVec3("dirLight.direction", direction);
    shader.setVec3("dirLight.ambient", ambient);
    shader.setVec3("dirLight.diffuse", diffuse);
    shader.setVec3("dirLight.specular", specular);
}

auto PointLight::applyToShader(const Shader& shader) const noexcept -> void {
    if ((shader.components() & ShaderComponent::PointLight) == ShaderComponent::None) {
        std::cout << "Error: shader does not support PointLight" << std::endl;
        return;
    }

    shader.setVec3("posLight.position", position);
    shader.setVec3("posLight.ambient", ambient);
    shader.setVec3("posLight.diffuse", diffuse);
    shader.setVec3("posLight.specular", specular);
    shader.setFloat("posLight.constant", attenuation.constant);
    shader.setFloat("posLight.linear", attenuation.linear);
    shader.setFloat("posLight.quadratic", attenuation.quadratic);
}

auto Flashlight::applyToShader(const Shader& shader) const noexcept -> void {
    if ((shader.components() & ShaderComponent::PointLight) == ShaderComponent::None) {
        std::cout << "Error: shader does not support Flashlight" << std::endl;
        return;
    }

    shader.setVec3("flashlight.position", position);
    shader.setVec3("flashlight.direction", direction);
    shader.setVec3("flashlight.ambient", ambient);
    shader.setVec3("flashlight.diffuse", diffuse);
    shader.setVec3("flashlight.specular", specular);
    shader.setFloat("flashlight.cutoff", cutoff);
    shader.setFloat("flashlight.outerCutoff", outerCutoff);
}

auto Flashlight::update(const Shader& shader, glm::vec3 pos, glm::vec3 dir) noexcept -> void {
    position = pos;
    direction = dir;

    shader.setVec3("flashlight.position", position);
    shader.setVec3("flashlight.direction", direction);
}

const Attenuation Distance::D7 = {1.0f, 0.7f, 1.8f};
const Attenuation Distance::D13 = {1.0f, 0.35f, 0.44f};
const Attenuation Distance::D20 = {1.0f, 0.22f, 0.20f};
const Attenuation Distance::D32 = {1.0f, 0.14f, 0.07f};
const Attenuation Distance::D50 = {1.0f, 0.09f, 0.032f};
const Attenuation Distance::D65 = {1.0f, 0.07f, 0.017f};
const Attenuation Distance::D100 = {1.0f, 0.045f, 0.0075f};
const Attenuation Distance::D160 = {1.0f, 0.027f, 0.0028f};
const Attenuation Distance::D200 = {1.0f, 0.022f, 0.0019f};
const Attenuation Distance::D325 = {1.0f, 0.014f, 0.0007f};
const Attenuation Distance::D600 = {1.0f, 0.007f, 0.0002f};
const Attenuation Distance::D3250 = {1.0f, 0.0014f, 0.000007f};
