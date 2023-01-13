#include "TextureGenerator.hpp"

#include <cassert>

namespace mgl {

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

    ////////////////////////////////////////////////////////////////////// Texture2D

    void Texture2D::bind() { glBindTexture(GL_TEXTURE_2D, id); }

    void Texture2D::unbind() { glBindTexture(GL_TEXTURE_2D, 0); }

    void Texture2D::load(ITextureGenerator* textureGenerator) {
        const int width = 1024;
        const int height = 1024;

        unsigned char* image = new unsigned char[width * height * 4];

        textureGenerator->generate(image, width, height);

        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);

        // Pré OpenGL v3.30 (still compatible with core)

        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
        //                GL_LINEAR_MIPMAP_LINEAR);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, image);
        // syntax: glTexImage2D(target, level, internalformat, width, height, border,
        // format, type, data)

        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);

        delete image;
    }

    inline void ITextureGenerator::setRGBA(unsigned char* image, int pixelIndex, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) {
        int index = pixelIndex * 4;

        image[index] = red;
        image[index + 1] = green;
        image[index + 2] = blue;
        image[index + 3] = alpha;
    }

    inline void ITextureGenerator::setRGBA(unsigned char* image, int pixelIndex, glm::vec4 rgba) {
        setRGBA(image, pixelIndex, 
            ((unsigned char)rgba[0] * 255), 
            ((unsigned char)rgba[1] * 255), 
            ((unsigned char)rgba[2] * 255), 
            ((unsigned char)rgba[3] * 255));
    }

    void UniformTextureGenerator::generate(unsigned char* image, int width, int height) {
        int pixels = width * height;

        for(int i = 0; i < pixels; i++) {
            setRGBA(image, i, _color);
        }
    }


    void PerlinNoiseTextureGenerator::generate(unsigned char* image, int width, int height) {
        // Assuming _gridSize is a divisor of width and height

	    for(int x = 0; x < width; x++) {
		    for(int y = 0; y < height; y++) {
			    
		    }
	    }
    }

} // namespace mgl