#include <glad/glad.h>
#include <glfw/glfw3.h>
#define GLM_ENABLE_EXPERIMENTAL

#include "lib/graphics/animate.hpp"
#include "lib/graphics/color.hpp"
#include "lib/graphics/cursor.hpp"
#include "lib/graphics/keyboard.hpp"
#include "lib/graphics/view.hpp"
#include "lib/graphics/window.hpp"

#include "engine.hpp"
#include "level_editor.hpp"
#include "object_editor.hpp"
#include "shapes.hpp"
#include "ui_scene.hpp"

auto setupOpenGl() {
    // ---------------------------------------------------------------------------------------------------------------
    // Initialize GLAD
    //
    // This must be done before any OpenGL functions are called. GLFW provides a function to get the address of OpenGL
    // function pointers, and GLAD then loads them all. This address is OS-specific.
    // ---------------------------------------------------------------------------------------------------------------
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        throw std::runtime_error("Failed to initialize GLAD");
    }

    // --------------------------------------------------------------------------------------------------------------
    // Setup viewport
    //
    // Let OpenGL know how we want to display data with respect to the window. My MBP w/ retina display has a 2:1
    // viewport to window ratio.
    // --------------------------------------------------------------------------------------------------------------
    glViewport(0, 0, WIN_WIDTH * VIEWPORT_SCALE, WIN_HEIGHT * VIEWPORT_SCALE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_MULTISAMPLE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_ALPHA_TEST);
}

UiScene* ui;

Animation showEditorAnim;// TODO: move

auto main() -> int {
    const auto path = std::filesystem::path{
        "example/"};

    // ---------------------------------------------------------------
    // Setup the window, cursor, keyboard
    // ---------------------------------------------------------------
    auto window = Window{"Blenter", {WIN_WIDTH, WIN_HEIGHT}};
    setupOpenGl();
    VertexBuffer::init();// This is dumb and ugly
    initKeyboard();
    Cursor::instance().viewportSize = {WIN_WIDTH * VIEWPORT_SCALE,
                                       WIN_HEIGHT * VIEWPORT_SCALE};

    // ---------------------------------------------------------------
    // Setup the view
    // ---------------------------------------------------------------
    auto engine = Engine{path};
    auto shader = Shader{"main_shader.vert", "main_shader.frag"};
    setDefaults(shader);
    auto view = View{engine, window, shader};
    view.clear = color::creme;
    init(view);
    window.view = &view;

    // ---------------------------------------------------------------
    // Create scenes
    // ---------------------------------------------------------------
    auto scene = createScene<Scene>(view);
    auto levelEditor = LevelEditor{engine, *scene};
    init(levelEditor, *scene);

    scene = createScene<Scene>(view);
    scene->flags |= SF_Hidden;
    auto objectEditor = ObjectEditor{engine, *scene};
    init(objectEditor, *scene);

    ui = createScene<UiScene>(view, engine);

    // ---------------------------------------------------------------
    // Callbacks
    // ---------------------------------------------------------------
    view.onTick = [&view, &levelEditor] {
        if (showEditorAnim.running) {
            const auto now = high_resolution_clock::now();
            const auto t = (float)duration_cast<milliseconds>(now - showEditorAnim.start).count() / showEditorAnim.duration + showEditorAnim.startT;
            showEditorAnim.t = t;
            const auto res = callFn(showEditorAnim.fn, t, showEditorAnim.forward);
            if (t >= 1.0f) {
                showEditorAnim.running = false;
            }
            const auto val = res * 0.25f * levelEditor.scene.cam._front;
            levelEditor.scene.cam._pos = view.startPos - val;
            levelEditor.scene.blur = res * 10.f;
            levelEditor.scene.desat = res * 0.5f;
        }
    };

    view.onProcessInput = [&view] {
        if (keyStates.at(GLFW_KEY_E).action == Action::Press) {
            if (view.scenes[1]->flags & SF_Hidden) {
                view.scenes[1]->flags &= ~SF_Hidden;
                view.scenes[2]->entities.at(view.scenes[2]->entities.size() - 2)->flags &= ~Invisible;

                glfwGetCursorPos(view.window.glfwWin, &view.prevCurPos.x, &view.prevCurPos.y);
                glfwSetCursorPos(view.window.glfwWin, 0, 0.0);
                Cursor::instance().isFirstMove = true;
                mouseMoveEvent(view, {0, 0});

                if (showEditorAnim.running) {
                    showEditorAnim.startT = 1.f - showEditorAnim.t;
                    showEditorAnim.startT = 0.f;
                } else {
                    showEditorAnim.running = true;
                    showEditorAnim.startT = 0.f;
                    view.startPos = view.scenes.at(0)->cam._pos;
                }
                showEditorAnim.forward = true;
                showEditorAnim.start = high_resolution_clock::now();
            }
        }

        if (keyStates.at(GLFW_KEY_ESCAPE).action == Action::Press) {
            if (!(view.scenes[1]->flags & SF_Hidden)) {
                view.scenes[1]->flags |= SF_Hidden;
                view.scenes[2]->entities.at(view.scenes[2]->entities.size() - 2)->flags |= Invisible;

                glfwSetCursorPos(view.window.glfwWin, view.prevCurPos.x, view.prevCurPos.y);

                if (showEditorAnim.running) {
                    showEditorAnim.startT = 1.f - showEditorAnim.t;
                    showEditorAnim.startT = 0.f;
                } else {
                    showEditorAnim.running = true;
                    showEditorAnim.startT = 0.f;
                }
                showEditorAnim.forward = false;
                showEditorAnim.start = high_resolution_clock::now();
            }
        }
    };

    // ---------------------------------------------------------------
    // Render loop
    // ---------------------------------------------------------------
    while (!glfwWindowShouldClose(window.glfwWin)) {
        processInput(window);
        tick(view);
        render(view);

        glfwSwapBuffers(window.glfwWin);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

// LIGHTING STUFF THAT COULD BE HELPFUL
//
// const auto light = Light{
//     {0.2f, 0.2f, 0.2f},
//     {0.5f, 0.5f, 0.5f},
//     {1.0f, 1.0f, 1.0f},
//     lightSource.pos()};
//
// const auto directionalLight = DirectionalLight{
//     {-0.2f, -1.f, -0.3f},
//     {0.2f, 0.2f, 0.2f},
//     {0.5f, 0.5f, 0.5f},
//     {1.0f, 1.0f, 1.0f}};
//
// const auto pointLight = PointLight{
//     {0.f, 0.f, 0.f},
//     {0.2f, 0.2f, 0.2f},
//     {0.5f, 0.5f, 0.5f},
//     {1.0f, 1.0f, 1.0f},
//     Distance::D50};
//
// auto flashlight = Flashlight{
//     {0.f, 0.f, 0.f},
//     {0.f, 0.f, 0.f},
//     {0.05f, 0.05f, 0.05f},
//     {0.5f, 0.5f, 0.5f},
//     {1.0f, 1.0f, 1.0f},
//     glm::cos(glm::radians(12.5f)),
//     glm::cos(glm::radians(17.5f))};
//
// shader.use();
// directionalLight.applyToShader(shader);
// pointLight.applyToShader(shader);
// flashlight.applyToShader(shader);
//
// instancedShader.use();
// directionalLight.applyToShader(instancedShader);
// pointLight.applyToShader(instancedShader);
// flashlight.applyToShader(instancedShader);