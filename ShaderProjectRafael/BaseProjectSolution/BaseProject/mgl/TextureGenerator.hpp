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
	class Texture2D;
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
		void updateShader(ShaderProgram* shader);
	};

	/////////////////////////////////////////////////////////////////////// TEXTURES

	class Texture2D : public Texture {
	public:
		void bind() override;
		void unbind() override;
		void load(ITextureGenerator& textureGenerator);
	};

	////////////////////////////////////////////////////////////////////////////////

	class ITextureGenerator {
	public:
		virtual void generate(unsigned char* image, int width, int height) = 0;

		virtual ~ITextureGenerator() = default;

		static inline void setRGBA(unsigned char* image, int pixelIndex, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);
		static inline void setRGBA(unsigned char* image, int pixelIndex, glm::vec4 rgba);
	};

	class UniformTextureGenerator : ITextureGenerator {
	private:
		glm::vec4 _color;
	public:

		UniformTextureGenerator(glm::vec4 color) {
			_color = color;
		}

		void generate(unsigned char* image, int width, int height) override;

		~UniformTextureGenerator() override = default;
	};

	class PerlinNoiseTextureGenerator : ITextureGenerator {
	private:
		glm::vec4 _color1;
		glm::vec4 _color2;
		int _gridSize;
	public:

		PerlinNoiseTextureGenerator(glm::vec4 color1, glm::vec4 color2, int gridSize) {
			_color1 = color1;
			_color2 = color2;
			_gridSize = gridSize;
		}

		void generate(unsigned char* image, int width, int height) override;

		~PerlinNoiseTextureGenerator() override = default;
	};
} // namespace 


#endif /* MGL_TEXTURE_GENERATOR_HPP  */