#ifndef MGL_TEXTURE_GENERATOR_HPP
#define MGL_TEXTURE_GENERATOR_HPP

#include <iostream>
#include <string>
#include <glm/glm.hpp>

#include "mglShader.hpp"
#include "TextureGenerator.hpp"

namespace mgl {
	class ITextureGenerator;

	class Sampler;
	class NearestSampler;
	class LinearSampler;
	class NearestMimapNearestSampler;
	class NearestMimapLinearSampler;
	class LinearMimapNearestSampler;
	class LinearMimapLinearSampler;
	class LinearAnisotropicSampler;
	class MaxAnisotropicSampler;

	////////////////////////////////////////////////////////////////////////////////

	class Sampler {
	protected:
		GLuint _samplerId;

	public:
		Sampler();
		~Sampler();
		virtual void create() = 0;
		void bind(GLuint unit);
		void unbind(GLuint unit);

		GLuint getSamplerId() {
			return _samplerId;
		}
	};

	class NearestSampler : public Sampler {
	public:
		void create() override;
	};

	class LinearSampler : public Sampler {
	public:
		void create() override;
	};

	class NearestMimapNearestSampler : public Sampler {
	public:
		void create() override;
	};

	class NearestMimapLinearSampler : public Sampler {
	public:
		void create() override;
	};

	class LinearMimapNearestSampler : public Sampler {
	public:
		void create() override;
	};

	class LinearMimapLinearSampler : public Sampler {
	public:
		void create() override;
	};

	class LinearAnisotropicSampler : public Sampler {
	public:
		float Anisotropy = 4.0f;
		void create() override;
	};

	class MaxAnisotropicSampler : public Sampler {
	public:
		void create() override;
	};

	class Texture;
	class Texture3D;
	struct TextureInfo;

	//////////////////////////////////////////////////////////////////////// TEXTURE

	class Texture {
	protected:
		GLuint id;

	public:
		Texture();
		~Texture();
		virtual void bind() = 0;
		virtual void unbind() = 0;
	};

	//////////////////////////////////////////////////////////////////// TextureInfo

	struct TextureInfo {
		GLenum unit;                // GL_TEXTUREi
		GLuint index;               // sampler index in shader
		std::string uniform;        // uniform name in shader
		Texture* texture = nullptr; // Texture (engine object)
		Sampler* sampler = nullptr; // Sampler (engine object)

		TextureInfo(GLenum textureunit, GLuint index, const std::string& uniform,
			Texture* texture, Sampler* sampler);
		TextureInfo();
		void updateShader(ShaderProgram* shader);
	};

	/////////////////////////////////////////////////////////////////////// TEXTURES

	class Texture3D : public Texture {
	public:
		void bind() override;
		void unbind() override;
		void load(ITextureGenerator* textureGenerator);
	};

	////////////////////////////////////////////////////////////////////////////////

	class ITextureGenerator {
	public:
		virtual void generate(GLfloat* image, int width, int height, int depth) = 0;

		virtual ~ITextureGenerator() = default;

		static inline void setRGBA(GLfloat* image, int pixelIndex, float red);
	};

	class UniformTextureGenerator : public ITextureGenerator {
	private:
		float _red;
	public:

		UniformTextureGenerator(float red) : ITextureGenerator() {
			_red = red;
		}

		void generate(GLfloat* image, int width, int height, int depth) override;

		~UniformTextureGenerator() override = default;
	};

	class RandomTextureGenerator : public ITextureGenerator {
	public:

		RandomTextureGenerator() {
		}

		void generate(GLfloat* image, int width, int height, int depth) override;

		~RandomTextureGenerator() override = default;
	};
} // namespace 


#endif /* MGL_TEXTURE_GENERATOR_HPP  */