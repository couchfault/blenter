//
// Created by James Pickering on 9/1/24.
//

#pragma once

#include <chrono>
#include <glm/glm.hpp>

#include <functional>

struct Window;

enum class ProjectionType {
    Perspective,
    Orthographic
};

enum CameraFlags {
    CF_Moveable = 1 << 0,
    CF_CursorControlsLookAt = 1 << 1,
    CF_AnimateOnWalk = 1 << 2,
};

struct Camera {
    Camera(ProjectionType type);

    auto setSize(glm::ivec2 size) -> void;
    auto setSpeed(float speed) noexcept -> void;

    [[nodiscard]] auto view() const noexcept -> glm::mat4;
    [[nodiscard]] auto projection() const noexcept -> glm::mat4;
    [[nodiscard]] auto invProjView() const noexcept -> glm::mat4;
    [[nodiscard]] auto position() const noexcept -> glm::vec3;
    [[nodiscard]] auto front() const noexcept -> glm::vec3;
    [[nodiscard]] auto getWorldPos(float z) const noexcept -> glm::vec3;

    auto lookAt(float x, float y) noexcept -> void;
    auto move(const Window& window) noexcept -> void;
    auto zoom(float amount) noexcept -> void;

    std::function<void(bool)> onMove;

    auto update() -> void;

    int flags = 0;

    ProjectionType _projectionType;
    glm::vec3 _pos{0.f, 0.f, 0.f};
    glm::vec3 _front{0.f, 0.f, -1.f};
    glm::vec3 _up;
    glm::vec3 _right;
    glm::vec3 _worldUp{0, 1, 0};
    glm::ivec2 _size;

    glm::mat4 _proj;
    glm::mat4 _lookAt;

    float _deltafloatime = 0.f;
    float _lastFrame = 0.f;
    float _aspectRatio;
    float _fov{45};
    float _lastX;
    float _lastY;
    float _pitch{0};
    float _yaw{-90};

    bool _cursorMovedOnce{false};
    float _speed{1.6f};

    float sinYaw;
    float cosYaw;
    float sinPitch;
    float cosPitch;

    bool walking = false;
    std::chrono::time_point<std::chrono::high_resolution_clock> walkStart;
    float realY;
    float startP;

    static constexpr float MaxPitch{89};
    static constexpr float Near{0.1};
    static constexpr float Far{100.f};
};

auto constrainPitch(Camera& cam) -> void;
auto getForward(const Camera& cam) -> glm::vec3;
auto getForward(const Camera& cam, glm::vec3 pos) -> glm::vec3;
