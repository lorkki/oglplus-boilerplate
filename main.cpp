#include <iostream>
#include <memory>
#include <stdexcept>

#define SDL_MAIN_HANDLED
#include <SDL.h>

#include <GL/glew.h>
#include <oglplus/all.hpp>

#include <glm/glm.hpp>

namespace
{

/*
 * Minimal RAII-friendly utilities for SDL 2
 */

class SDL
{
public:
    using Error = std::runtime_error;

    /*
     * Init
     */

    using InitError = Error;

    SDL (void)
    {
        if (SDL_Init(SDL_INIT_VIDEO) != 0)
            throw InitError(SDL_GetError());
    }

    ~SDL (void)
    {
        SDL_Quit();
    }

    /*
     * Window
     */

    using CreateWindowError = Error;

    class _WindowDeleter
    {
    public:
        void operator() (SDL_Window* window)
        {
            SDL_DestroyWindow(window);
        }
    };

    using Window = std::unique_ptr<SDL_Window, _WindowDeleter>;

    Window createWindow (const std::string& title, int width, int height) const
    {
        SDL_Window* sdlWindow = SDL_CreateWindow(
            title.c_str(),
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            width, height,
            SDL_WINDOW_OPENGL
            );

        if (!sdlWindow)
            throw CreateWindowError(SDL_GetError());

        return Window(sdlWindow);
    }

    /*
     * OpenGL context
     */

    class GLContext
    {
    public:
        using CreateError = SDL::Error;
        using GLEWError = std::runtime_error;

        explicit GLContext (const Window& window)
        {
            const SDL_GLContext sdlContext = SDL_GL_CreateContext(window.get());

            if (!sdlContext)
                throw CreateError(SDL_GetError());

            glewExperimental = GL_TRUE;

            const GLenum err = glewInit();
            if (err != GLEW_OK)
                throw GLEWError(reinterpret_cast<const char*>(glewGetErrorString(err)));

            glGetError(); // Clear error state after GLEW

            m_sdlContext = sdlContext;
        }

        ~GLContext (void)
        {
            SDL_GL_DeleteContext(m_sdlContext);
        }

        GLContext (GLContext&&) = default;
        GLContext& operator= (GLContext&&) = default;

    private:
        SDL_GLContext m_sdlContext;
    };

    GLContext createGLContext (const Window& window) const
    {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);

        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        return GLContext(window);
    }

    void swapWindow (const Window& window) const
    {
        SDL_GL_SwapWindow(window.get());
    }
};

} // namespace

int main (int argc, char* argv[])
{
    try
    {
        SDL sdl;

        SDL::Window window = sdl.createWindow("OGLplus Boilerplate", 800, 600);
        SDL::GLContext context = sdl.createGLContext(window);

        oglplus::Context gl;

        gl.ClearColor(0.0f, 1.0f, 0.0f, 0.0f);
        gl.ClearDepth(1.0f);

        gl.Clear().ColorBuffer().DepthBuffer();

        // ...

        sdl.swapWindow(window);

        return 0;
    }
    catch (oglplus::Error& err)
    {
        std::cerr
            << "Error (in " << err.GLFunc() << "'): "
            << err.what()
            << " [" << err.SourceFile() << ":" << err.SourceLine() << "]"
            << std::endl;
    }
    catch (std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 1;
}
