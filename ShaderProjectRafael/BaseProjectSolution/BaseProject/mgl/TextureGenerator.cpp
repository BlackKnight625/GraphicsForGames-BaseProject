#include "TextureGenerator.hpp"

#include <cassert>

namespace mgl {

    inline float getRandomFloat() {
        return ((float)(rand()) / ((float)(RAND_MAX)));
    }


    ////////////////////////////////////////////////////////////////////////////////

    Sampler::Sampler() {
        glGenSamplers(1, &_samplerId);
        glSamplerParameteri(_samplerId, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glSamplerParameteri(_samplerId, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    Sampler::~Sampler() {}

    void Sampler::bind(GLuint unit) { glBindSampler(unit, _samplerId); }

    void Sampler::unbind(GLuint unit) { glBindSampler(unit, 0); }

    void NearestSampler::create() {
        glSamplerParameteri(_samplerId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glSamplerParameteri(_samplerId, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    }

    void LinearSampler::create() {
        glSamplerParameteri(_samplerId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glSamplerParameteri(_samplerId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }

    void NearestMimapNearestSampler::create() {
        glSamplerParameteri(_samplerId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glSamplerParameteri(_samplerId, GL_TEXTURE_MIN_FILTER,
            GL_NEAREST_MIPMAP_NEAREST);
    }

    void NearestMimapLinearSampler::create() {
        glSamplerParameteri(_samplerId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glSamplerParameteri(_samplerId, GL_TEXTURE_MIN_FILTER,
            GL_NEAREST_MIPMAP_LINEAR);
    }

    void LinearMimapNearestSampler::create() {
        glSamplerParameteri(_samplerId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glSamplerParameteri(_samplerId, GL_TEXTURE_MIN_FILTER,
            GL_LINEAR_MIPMAP_NEAREST);
    }

    void LinearMimapLinearSampler::create() {
        glSamplerParameteri(_samplerId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glSamplerParameteri(_samplerId, GL_TEXTURE_MIN_FILTER,
            GL_LINEAR_MIPMAP_LINEAR);
    }

    void LinearAnisotropicSampler::create() {
        glSamplerParameteri(_samplerId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glSamplerParameteri(_samplerId, GL_TEXTURE_MIN_FILTER,
            GL_LINEAR_MIPMAP_LINEAR);
        glSamplerParameterf(_samplerId, GL_TEXTURE_MAX_ANISOTROPY_EXT, Anisotropy);
    }

    void MaxAnisotropicSampler::create() {
        GLfloat max = 0.0f;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max);
        glSamplerParameteri(_samplerId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glSamplerParameteri(_samplerId, GL_TEXTURE_MIN_FILTER,
            GL_LINEAR_MIPMAP_LINEAR);
        glSamplerParameterf(_samplerId, GL_TEXTURE_MAX_ANISOTROPY_EXT, max);
        std::cout << "Sampler set to maximum anisotropy (" << max << ")."
            << std::endl;
    }

    ////////////////////////////////////////////////////////////////////////////////
    ///

    //////////////////////////////////////////////////////////////////// TextureInfo

    TextureInfo::TextureInfo(GLenum _unit, GLuint _index,
        const std::string& _uniform, Texture* _texture,
        Sampler* _sampler) {
        unit = _unit;
        uniform = _uniform;
        index = _index;
        texture = _texture;
        sampler = _sampler;
    }

    void TextureInfo::updateShader(ShaderProgram* shader) {
        glActiveTexture(unit);
        texture->bind();
        if (sampler)
            sampler->bind(index);
        glUniform1i(shader->Uniforms[uniform].index, index);
    }

    TextureInfo::TextureInfo() {
	    
    }


    //////////////////////////////////////////////////////////////////////// Texture

    Texture::Texture() : id(-1) {}

    Texture::~Texture() {}

    ////////////////////////////////////////////////////////////////////// Texture3D

    void Texture3D::bind() { glBindTexture(GL_TEXTURE_3D, id); }

    void Texture3D::unbind() { glBindTexture(GL_TEXTURE_3D, 0); }

    void Texture3D::load(ITextureGenerator* textureGenerator) {
        const int width = 128;
        const int height = 128;
        const int depth = 128;

		GLfloat* image = new GLfloat[width * height * depth];

        textureGenerator->generate(image, width, height, depth);

        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_3D, id);

        // Pré OpenGL v3.30 (still compatible with core)

        // glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        // glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

        // glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        // glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);

        // glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        // glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

        // glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        // glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER,
        //                GL_LINEAR_MIPMAP_LINEAR);
        // glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        glTexImage3D(GL_TEXTURE_3D, 0, GL_RED, width, height, depth, 0, GL_RED,
            GL_FLOAT, image);
        // syntax: glTexImage3D(target, level, internalformat, width, height, border,
        // format, type, data)

        glGenerateMipmap(GL_TEXTURE_3D);
        glBindTexture(GL_TEXTURE_3D, 0);

        delete image;
    }

    inline void ITextureGenerator::setRGBA(GLfloat* image, int pixelIndex, float red) {
        image[pixelIndex] = red;
    }

    void UniformTextureGenerator::generate(GLfloat* image, int width, int height, int depth) {
        int pixels = width * height * depth;

        for(int i = 0; i < pixels; i++) {
            setRGBA(image, i, _red);
        }
    }


    void RandomTextureGenerator::generate(GLfloat* image, int width, int height, int depth) {
        int pixels = width * height * depth;

        for (int i = 0; i < pixels; i++) {
			setRGBA(image, i, getRandomFloat() * 2.0f - 1.0f);
        }
    }

} // namespace mgl