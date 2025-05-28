//
// Created by James Pickering on 9/1/24.
//

#include "camera.hpp"

#include "cursor.hpp"
#include "keyboard.hpp"
#include "window.hpp"

#include <iostream>

#include <GLFW/glfw3.h>
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <ostream>

Camera::Camera(ProjectionType type) :
    _projectionType{type} {
    update();
}

auto Camera::setSize(const glm::ivec2 size) -> void {
    _size = size;
    _aspectRatio = static_cast<float>(size.x) / static_cast<float>(size.y);
}

auto Camera::setSpeed(const float speed) noexcept -> void {
    _speed = speed;
}

auto Camera::view() const noexcept -> glm::mat4 {
    return glm::lookAt(_pos, _pos + _front, _up);
}

auto Camera::projection() const noexcept -> glm::mat4 {
    if (_projectionType == ProjectionType::Perspective) {
        return glm::perspective(glm::radians(_fov), _aspectRatio, Near, Far);
    }
    return glm::ortho(-_size.x / 2.f, _size.x / 2.f, -_size.y / 2.f, _size.y / 2.f, Near, Far);
}

auto Camera::invProjView() const noexcept -> glm::mat4 {
    return inverse(view()) * inverse(projection());
}

auto Camera::position() const noexcept -> glm::vec3 {
    return _pos;
}

auto Camera::front() const noexcept -> glm::vec3 {
    return _front;
}

auto Camera::getWorldPos(const float z) const noexcept -> glm::vec3 {
    const auto look = position() + (front() * z);
    return look;
}

auto Camera::lookAt(float x, float y) noexcept -> void {
    if (!_cursorMovedOnce) {
        _lastX = x;
        _lastY = y;
        _cursorMovedOnce = true;
    }

    constexpr auto sensitivity = static_cast<float>(0.1);
    const auto xOffset = (x - _lastX) * sensitivity;
    const auto yOffset = (_lastY - y) * sensitivity;

    _lastX = x;
    _lastY = y;
    _yaw += xOffset;
    _pitch += yOffset;

    if (_pitch > MaxPitch) {
        _pitch = MaxPitch;
    } else if (_pitch < -MaxPitch) {
        _pitch = -MaxPitch;
    }

    update();
}

auto Camera::move(const Window& window) noexcept -> void {
    if (flags & CF_Moveable) {
        const auto currentFrame = static_cast<float>(glfwGetTime());
        _deltafloatime = currentFrame - _lastFrame;
        _lastFrame = currentFrame;

        const auto cameraSpeed = _speed * _deltafloatime;

        auto w = false;
        if (keyStates.at(GLFW_KEY_W).pressed) {
            _pos += glm::vec3{cameraSpeed * _front.x, 0.f, cameraSpeed * _front.z};
            w = true;
        }
        if (keyStates.at(GLFW_KEY_S).pressed) {
            _pos -= glm::vec3{cameraSpeed * _front.x, 0.f, cameraSpeed * _front.z};
            w = true;
        }
        if (keyStates.at(GLFW_KEY_A).pressed) {
            _pos -= normalize(cross(_front, _worldUp)) * cameraSpeed;
            w = true;
        }
        if (keyStates.at(GLFW_KEY_D).pressed) {
            _pos += normalize(cross(_front, _worldUp)) * cameraSpeed;
            w = true;
        }

        if (flags & CF_AnimateOnWalk && w) {
            const auto baseSpeed = 6.f;
            const auto delta = std::sin(std::chrono::duration<float>(std::chrono::high_resolution_clock::now() - walkStart).count() * (baseSpeed + _speed)) * 1.f;
            const auto f2 = 0.6;
            const auto delta2 = std::min(std::sin(std::chrono::duration<float>(std::chrono::high_resolution_clock::now() - walkStart).count() * (baseSpeed + _speed) * 2.f) * f2, std::cos(0.f * (baseSpeed + _speed)) * f2);

            if (!walking) {
                walkStart = std::chrono::high_resolution_clock::now();
                realY = delta;
                startP = delta2;
            }

            _yaw += delta - realY;
            _pitch -= delta2 - startP;
            update();
            _yaw -= delta - realY;
            _pitch += delta2 - startP;
        }

        walking = w;

        if (keyStates.at(GLFW_KEY_SPACE).pressed) {
            _pos.y += cameraSpeed;
        }
        if (keyStates.at(GLFW_KEY_LEFT_SHIFT).pressed) {
            _pos.y = std::fmax(_pos.y - cameraSpeed, 0);
        }
    }
}

auto Camera::zoom(float amount) noexcept -> void {
    _fov -= amount;
    if (_fov < 1.f) {
        _fov = 1.f;
    } else if (_fov > 45.f) {
        _fov = 45.f;
    }
}

auto Camera::update() -> void {
    const auto direction = glm::vec<3, float>{
        cos(glm::radians(_yaw)) * cos(glm::radians(_pitch)),
        sin(glm::radians(_pitch)),
        sin(glm::radians(_yaw)) * cos(glm::radians(_pitch))};

    _front = normalize(direction);
    _right = normalize(cross(_front, _worldUp));
    _up = normalize(cross(_right, _front));
}

auto constrainPitch(Camera& cam) -> void {
    if (cam._pitch > Camera::MaxPitch) {
        cam._pitch = Camera::MaxPitch;
    } else if (cam._pitch < -Camera::MaxPitch) {
        cam._pitch = -Camera::MaxPitch;
    }
}

auto getForward(const Camera& cam) -> glm::vec3 {
    //auto adjPos = glm::vec3{cam._pos.z, cam._pos.y, cam._pos.x};
    const auto pos = cam._pos;
    //std::cout << "pos: " << pos.x << ", " << pos.y << ", " << pos.z << std::endl;
    return glm::normalize(cam.front() - pos);
}

auto getForward(const Camera& cam, glm::vec3 pos) -> glm::vec3 {
    //auto adjPos = glm::vec3{cam._pos.z, cam._pos.y, cam._pos.x};
    // const auto pos = cam._pos;
    //std::cout << "pos: " << pos.x << ", " << pos.y << ", " << pos.z << std::endl;
    return glm::normalize(cam.front() - pos);
}
