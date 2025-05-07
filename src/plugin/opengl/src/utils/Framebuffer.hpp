#pragma once
#include "Logger.hpp"
#include <GL/glew.h>
#include <glm/vec2.hpp>

namespace ES::Plugin::OpenGL::Utils {
struct FramebufferSpecification {
    int width;
    int height;
    int samples = 1;
    bool swapChainTarget = false;
};

class Framebuffer {
  public:
    Framebuffer(const FramebufferSpecification &specification) : _specification(specification){};
    virtual ~Framebuffer(){};

    void Init()
    {
        glCreateFramebuffers(1, &_rendererId);
        glBindFramebuffer(GL_FRAMEBUFFER, _rendererId);

        glCreateTextures(GL_TEXTURE_2D, 1, &_colorAttachment);
        glBindTexture(GL_TEXTURE_2D, _colorAttachment);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _specification.width, _specification.height, 0, GL_RGBA,
                     GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _colorAttachment, 0);

        glCreateTextures(GL_TEXTURE_2D, 1, &_depthAttachement);
        glBindTexture(GL_TEXTURE_2D, _depthAttachement);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, _specification.width, _specification.height, 0,
                     GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, _depthAttachement, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            ES::Utils::Log::Error("Framebuffer is not complete!");
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Resize(const glm::vec2 &size)
    {
        _specification.width = size.x;
        _specification.height = size.y;
        Deinit();
        Init();
    }

    void Deinit()
    {
        glDeleteTextures(1, &_colorAttachment);
        glDeleteTextures(1, &_depthAttachement);
        glDeleteFramebuffers(1, &_rendererId);
    }

    void Bind() { glBindFramebuffer(GL_FRAMEBUFFER, _rendererId); }

    void UnBind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

    const unsigned int &GetColorAttachment() { return _colorAttachment; }
    const unsigned int &GetDepthAttachment() { return _depthAttachement; }

    const FramebufferSpecification &GetSpecification() const { return _specification; }

  private:
    unsigned int _rendererId = 0;
    unsigned int _colorAttachment = 0;
    unsigned int _depthAttachement = 0;
    FramebufferSpecification _specification;
};
} // namespace ES::Plugin::OpenGL::Utils
