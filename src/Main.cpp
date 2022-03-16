#include <borealis/Task.hpp>
#include <borealis/gfx/Camera.hpp>
#include <borealis/gfx/Canvas.hpp>
#include <borealis/gfx/Gfx.hpp>
#include <borealis/gfx/Material.hpp>
#include <borealis/gfx/Mesh.hpp>
#include <borealis/gfx/Particle.hpp>
#include <borealis/gfx/Shader.hpp>
#include <borealis/util/File.hpp>
#include <borealis/util/Math.hpp>
#include <borealis/util/Time.hpp>
#include <borealis/util/Trace.hpp>

#if defined(LIVEPP_ENABLED)
    #include <Windows.h>
    #include <LPP_API.h>
#endif

#define VMEM_OVERRIDE_NEW_DELETE
#include <VMem.h>

#include <SDL.h>
#include <glad/glad.h>
#include <filesystem>
#include <thread>

using namespace br;

math::Vec2 g_window_size = { 1920.0f, 1080.0f };
math::Vec2 g_render_size = g_window_size;

void update(float dt)
{
    BR_ASSERT(dt > 0.0f);
}

gfx::CanvasId _canvas;
gfx::CameraId _camera;
gfx::MaterialId _material;
gfx::MeshId _mesh;
gfx::ParticleId _particles;

void render(float dt)
{
    if (static bool once = false; !once)
    {
        _camera = gfx::camera_make();
        _canvas = gfx::canvas_make();

        gfx::canvas_set_camera(_canvas, _camera);

        static constexpr const char* vtx_path = "shaders/test.vert";
        static constexpr const char* frag_path = "shaders/test.frag";

        gfx::ShaderId vtx = gfx::shader_make();
        gfx::shader_set_code(vtx, gfx::Vertex, file::read_all_text(vtx_path).c_str());
        gfx::shader_set_reload_from_disk(vtx, vtx_path);

        gfx::ShaderId frag = gfx::shader_make();
        gfx::shader_set_code(frag, gfx::Fragment, file::read_all_text(frag_path).c_str());
        gfx::shader_set_reload_from_disk(frag, frag_path);

        _material = gfx::material_make();
        gfx::material_set_shader(_material, vtx);
        gfx::material_set_shader(_material, frag);
        gfx::material_link(_material);

        math::Vec2 vertices[] =
        {
            { 0.0f, 0.0f },
            { 100.0f, 0.0f },
            { 100.0f, 100.0f },
            { 100.0f, 100.0f },
            { 0.0f, 100.0f },
            { 0.0f, 0.0f },
        };

        _mesh = gfx::mesh_make();
        gfx::mesh_set_geometry(_mesh, vertices, _countof(vertices), false);

        _particles = gfx::particle_make();

        gfx::ParticleData data;
        data.lifetime = 10.0f;
        data.count = 1024 * 1024 * 8;
        data.velocity = 150.0f;
        data.rand_dir = math::deg2rad(360);
        data.rand_velocity = 1.0f;
        data.col_start = { 0.0f, 1.0f, 0.0f, 1.0f };
        data.col_end = { 1.0f, 0.0f, 0.0f, 1.0f };
        data.gravity = 98.1f;

        gfx::particle_set_data(_particles, data);

        once = true;
    }

    static float delta = 0.0f;
    delta += dt;

    gfx::update(dt);

    gfx::camera_set_transform(_camera, math::ortho(g_render_size.x, g_render_size.y) * math::camera({ 0.0f, 0.0f }, 1.0f));
    gfx::canvas_set_size(_canvas, (uint32_t)g_window_size.x, (uint32_t)g_window_size.y);

    gfx::canvas_begin(_canvas);
    gfx::canvas_clear(_canvas, { 0.2f, 0.2f, 0.2f, 1.0f });
    gfx::canvas_draw_particle(_particles, math::translate(math::Mat3::identity(), g_render_size / 2.0f));
}

int SDL_main(int, char*[])
{
    BR_TRACE("Working directory is '%s'.", std::filesystem::current_path().generic_string().c_str());

#if defined(LIVEPP_ENABLED)
    HMODULE lpp_handle = lpp::lppLoadAndRegister(LIVEPP_DIR, "Quickstart");
    void* lpp_token = lpp::lppEnableAllCallingModulesAsync(lpp_handle);
#endif

    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window* window = SDL_CreateWindow(
        "Borealis Engine",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        (int)g_window_size.x,
        (int)g_window_size.y,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );

    if (!window)
    {
        BR_TRACE("Could not create window: %s.", SDL_GetError());
        return 1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GLContext ctx = SDL_GL_CreateContext(window);

    if (!gladLoadGLLoader((GLADloadproc)&SDL_GL_GetProcAddress))
    {
        BR_TRACE("Failed to initialize GLAD.");
        return 1;
    }

    SDL_GL_SetSwapInterval(0);

    gfx::init();
    task::init(std::min(2u, std::thread::hardware_concurrency() - 2));

    {
        float last_frame_time = 1.0f / 60.0f;

        bool quit = false;
        while (!quit)
        {
            const uint64_t start_frame = time::current();

            SDL_Event event;

            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                {
                    quit = true;
                }
                else if (event.type == SDL_WINDOWEVENT)
                {
                    if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                    {
                        g_window_size = { (float)event.window.data1, (float)event.window.data2 };
                        g_render_size = g_window_size;
                    }
                }
                else if (event.type == SDL_KEYUP)
                {
                    if (event.key.keysym.sym == SDLK_F10)
                    {
                        gfx::reload_shaders();
                    }
                }
            }

#if defined(LIVEPP_ENABLED)
            lpp::lppSyncPoint(lpp_handle);
#endif

            update(last_frame_time);
            render(last_frame_time);

            SDL_GL_SwapWindow(window);

            last_frame_time = time::diff(time::current(), start_frame);
        }
    }

    task::free();
    gfx::free();

    SDL_GL_DeleteContext(ctx);
    SDL_DestroyWindow(window);
    SDL_Quit();

#if defined(LIVEPP_ENABLED)
    lpp::lppWaitForToken(lpp_handle, lpp_token);
    lpp::lppShutdown(lpp_handle);
    FreeLibrary(lpp_handle);
#endif

    return 0;
}
