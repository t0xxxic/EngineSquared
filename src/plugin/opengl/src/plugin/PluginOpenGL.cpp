#include "Input.hpp"
#include "OpenGL.hpp"
#include "PluginInput.hpp"
#include "PluginWindow.hpp"
#include "RenderingPipeline.hpp"
#include "Startup.hpp"
#include "Update.hpp"

// TODO: remove this as it's only used in lambda
#include "Window.hpp"

void ES::Plugin::OpenGL::Plugin::Bind()
{
    RequirePlugins<ES::Plugin::RenderingPipeline::Plugin, ES::Plugin::Window::Plugin, ES::Plugin::Input::Plugin>();

    RegisterSystems<ES::Plugin::RenderingPipeline::Init>(ES::Plugin::OpenGL::System::InitGLEW,
                                                         ES::Plugin::OpenGL::System::CheckGLEWVersion);

    RegisterSystems<ES::Plugin::RenderingPipeline::Setup>(
        ES::Plugin::OpenGL::System::SetupResizeViewport, ES::Plugin::OpenGL::System::LoadFontManager,
        ES::Plugin::OpenGL::System::LoadMaterialCache, ES::Plugin::OpenGL::System::LoadShaderManager,
        ES::Plugin::OpenGL::System::LoadDefaultShader, ES::Plugin::OpenGL::System::LoadDefaultTextShader,
        ES::Plugin::OpenGL::System::LoadDefaultSpriteShader, ES::Plugin::OpenGL::System::LoadTextureManager,
        ES::Plugin::OpenGL::System::CreateCamera, ES::Plugin::OpenGL::System::SetupShaderUniforms,
        ES::Plugin::OpenGL::System::SetupTextShaderUniforms, ES::Plugin::OpenGL::System::SetupSpriteShaderUniforms,
        ES::Plugin::OpenGL::System::LoadGLMeshBufferManager, ES::Plugin::OpenGL::System::LoadGLTextBufferManager,
        ES::Plugin::OpenGL::System::LoadGLSpriteBufferManager, ES::Plugin::OpenGL::System::SetupMouseDragging,
        [](ES::Engine::Core &core) {
            auto &window = core.GetResource<ES::Plugin::Window::Resource::Window>();
            auto size = window.GetSize();
            auto spec = ES::Plugin::OpenGL::Utils::FramebufferSpecification{size.x, size.y};
            core.RegisterResource<ES::Plugin::OpenGL::Utils::Framebuffer>(ES::Plugin::OpenGL::Utils::Framebuffer(spec))
                .Init();
        });

    RegisterSystems<ES::Plugin::RenderingPipeline::RenderSetup>(
        ES::Plugin::OpenGL::System::UpdateMatrices, ES::Plugin::OpenGL::System::SetupCamera,
        ES::Plugin::OpenGL::System::SetupLights, ES::Plugin::OpenGL::System::LoadGLMeshBuffer,
        ES::Plugin::OpenGL::System::LoadGLTextBuffer, ES::Plugin::OpenGL::System::LoadGLSpriteBuffer);

    RegisterSystems<ES::Plugin::RenderingPipeline::ToGPU>(
        [](ES::Engine::Core &core) {
            core.GetResource<ES::Plugin::OpenGL::Utils::Framebuffer>().Bind();
            glViewport(0, 0, core.GetResource<ES::Plugin::OpenGL::Utils::Framebuffer>().GetSpecification().width,
                       core.GetResource<ES::Plugin::OpenGL::Utils::Framebuffer>().GetSpecification().height);
        },
        ES::Plugin::OpenGL::System::GLClearColor, ES::Plugin::OpenGL::System::GLClearDepth,
        ES::Plugin::OpenGL::System::GLEnableDepth, ES::Plugin::OpenGL::System::GLEnableCullFace,
        ES::Plugin::OpenGL::System::RenderMeshes, ES::Plugin::OpenGL::System::RenderText,
        ES::Plugin::OpenGL::System::RenderSprites,
        [](ES::Engine::Core &core) { core.GetResource<ES::Plugin::OpenGL::Utils::Framebuffer>().UnBind(); });
}
