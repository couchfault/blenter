//
// Created by James Pickering on 3/18/25.
//

#include "scene.hpp"

#include "camera.hpp"
#include "color.hpp"
#include "entity.hpp"
#include "mouse.hpp"

#include "../../engine.hpp"
#include "view.hpp"
#include "window.hpp"

#include <glad/glad.h>

#include <GLFW/glfw3.h>

auto init(Framebuffer& fb) -> void {
    glGenFramebuffers(1, &fb.id);
    glGenTextures(1, &fb.tex);
    glGenRenderbuffers(1, &fb.rb);
}

auto resize(const Framebuffer& fb, const glm::vec2 size) -> void {
    // Resize the color attachment
    glBindFramebuffer(GL_FRAMEBUFFER, fb.id);
    glBindTexture(GL_TEXTURE_2D, fb.tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fb.tex, 0);

    // Resize the depth/stencil attachment
    glBindRenderbuffer(GL_RENDERBUFFER, fb.rb);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size.x, size.y);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, fb.rb);
}

Scene::Scene(View& view) :
    view{view},
    cam{ProjectionType::Orthographic},
    outCam{ProjectionType::Orthographic} {
    init(fb);

    display = std::make_unique<Entity>();
    display->vao = genVao(RenderMode::Mode2D);
    display->transform.size = glm::vec3{view.window.size, 1.f};
    display->transform.pos = glm::vec3{0.f, 0.f, -2.f};
    applyTransform(display->model, display->transform);
    setStyleAttr(display->style, Attr::BorderColor, 0, glm::vec4{0, 0, 0, 0.f});

    resize(fb, fb.size * (float)VIEWPORT_SCALE);
    outCam.setSize(fb.size);
}

Scene::~Scene() = default;

auto Scene::render(float z) const -> void {
    // ---------------------------------------------------------
    // Render layer to a fb
    // ---------------------------------------------------------
    glBindFramebuffer(GL_FRAMEBUFFER, fb.id);
    glClearColor(fb.clear.r, fb.clear.g, fb.clear.b, fb.clear.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const auto& shader = view.shader;
    shader.use();
    shader.setMat4("view", cam.view());
    shader.setMat4("proj", cam.projection());

    for (const auto& ent : entities) {
        renderEntity(*ent, shader, cam);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // ---------------------------------------------------------
    // Render fb texture
    // ---------------------------------------------------------
    shader.setMat4("view", outCam.view());
    shader.setMat4("proj", outCam.projection());
    shader.setFloat("radius", blur);
    shader.setFloat("desaturate", desat);

    display->transform.pos.z = z;
    display->transform.size = glm::vec3{fb.displaySize, 1.f};
    applyTransform(display->model, display->transform);
    display->textures.clear();
    display->textures.push_back(fb.tex);
    renderEntity(*display, shader, outCam);

    shader.setFloat("radius", 0.f);
    shader.setFloat("desaturate", 0.f);
}

auto createEntity(Scene& scene) -> Entity* {
    scene.entities.push_back(std::make_unique<Entity>());
    return scene.entities.back().get();
}

auto Scene::mouseMoved(Event& ev) -> void {
    if (cam.flags & CF_CursorControlsLookAt) {
        cam.lookAt(Cursor::instance().screenSpacePos.x, -Cursor::instance().screenSpacePos.y);
    }
    for (auto it = entities.rbegin(); it != entities.rend(); it = std::next(it)) {
        const auto& ent = *it;
        if (ev.isAccepted()) {
            break;
        }
        cursorMoved(*ent, Cursor::instance(), cam);
        handleMouseMoved(*ent, Cursor::instance(), ev);
    }
    if (!ev.isAccepted()) {
        if (Cursor::instance().pressed) {
            if (onDrag) {
                onDrag();
            }
            Cursor::instance().cursorType = CursorType::Fist;
        } else {
            Cursor::instance().cursorType = CursorType::Hand;
        }
    }
    if (onMouseMoved) {
        onMouseMoved();
    }
    if (flags & SF_AcceptsAllMouseMoves) {
        ev.accept();
    }
}

auto Scene::mousePressed(Event& ev) const -> void {
    for (auto it = entities.rbegin(); it != entities.rend(); it = std::next(it)) {
        const auto& ent = *it;
        if (ev.isAccepted()) {
            break;
        }
        handleMousePressed(*ent, Cursor::instance(), ev);
    }
    if (!ev.isAccepted()) {
        if (Cursor::instance().pressed) {
            Cursor::instance().cursorType = CursorType::Fist;
        } else {
            Cursor::instance().cursorType = CursorType::Hand;
        }
    }
    if (onMousePressed) {
        onMousePressed();
    }
}

auto Scene::mouseReleased(Event& ev) const -> void {
    for (auto it = entities.rbegin(); it != entities.rend(); it = std::next(it)) {
        const auto& ent = *it;
        if (ev.isAccepted()) {
            break;
        }
        handleMouseReleased(*ent, Cursor::instance(), ev);
    }
    if (!ev.isAccepted()) {
        if (Cursor::instance().pressed) {
            Cursor::instance().cursorType = CursorType::Fist;
        } else {
            Cursor::instance().cursorType = CursorType::Hand;
        }
    }
    if (onMouseReleased) {
        onMouseReleased();
    }
}

auto Scene::wheelScrolled(float offset) -> void {
    if (flags & CameraZoomsOnScroll) {
        cam.zoom(offset);
    }
}

auto Scene::windowResized(glm::ivec2 size) -> void {
    cam.setSize(size);
    resize(fb, fb.size * (float)VIEWPORT_SCALE);
    outCam.setSize(glm::vec2{size} / (float)VIEWPORT_SCALE);
}

auto Scene::processInput(Event& ev) -> void {
    if (cam._projectionType == ProjectionType::Perspective) {
        if (cam.flags & CF_Moveable) {
            cam.move(view.window);
        }
    }

    if (onProcessInput) {
        onProcessInput();
    }

    if (flags & SF_AcceptsAllInputEvents) {
        ev.accept();
    }
}
