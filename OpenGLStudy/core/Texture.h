#pragma once

#include "glad/glad.h"
#include "glm/glm.hpp"
#include <vector>

struct TextureDefinition {
	unsigned int textureUnit;
	GLenum target;
	glm::ivec3 size;
	GLenum internalFormat{ GL_RGBA32F };
	GLenum format{ GL_RGBA };
	GLenum type{ GL_FLOAT };
	GLenum filter{ GL_LINEAR };
	GLenum wrap{ GL_CLAMP_TO_EDGE };
	bool image{ false };
	GLenum imageMode{ GL_READ_WRITE };
	glm::vec4 borderColor{ 0.0f };
};

class Texture
{
public:
	unsigned int ID;
	TextureDefinition defn{};

	Texture(TextureDefinition defn) : defn(defn)
	{
		glGenTextures(1, &ID);
		configure();
	}

	void bind(int textureUnit = -1)
	{
		if (textureUnit == -1)
			textureUnit = defn.textureUnit;
		glActiveTexture(GL_TEXTURE0 + textureUnit);
		glBindTexture(defn.target, ID);
		if (defn.image)
			glBindImageTexture(defn.textureUnit, ID, 0, GL_TRUE, 0, defn.imageMode, defn.internalFormat);
	}

	void bindImage(int imageUnit)
	{
		glBindImageTexture(imageUnit, ID, 0, GL_TRUE, 0, defn.imageMode, defn.internalFormat);
	}

	unsigned int dimensions()
	{
		switch (defn.target)
		{
		case GL_TEXTURE_1D:
		case GL_TEXTURE_1D_ARRAY:
		case GL_TEXTURE_BUFFER:
			return 1;
		case GL_TEXTURE_2D:
		case GL_TEXTURE_RECTANGLE:
			return 2;
		case GL_TEXTURE_3D:
			return 3;
		default:
			return 0;
		}
	}

	void clearImage(glm::vec4& values)
	{
		bind();
		glm::ivec3 dims = glm::max(glm::ivec3(1), defn.size);
		int size = dims.x * dims.y * dims.z;
		std::vector<glm::vec4> data(size, values);
		texImage((void*)data.data());
		configure();
	}

	void texImage(void* data=NULL)
	{
		switch (dimensions())
		{
		case 3:
			glTexImage3D(defn.target, 0, defn.internalFormat, defn.size.x, defn.size.y, defn.size.z, 0, defn.format, defn.type, data);
			break;
		case 2:
			glTexImage2D(defn.target, 0, defn.internalFormat, defn.size.x, defn.size.y, 0, defn.format, defn.type, data);
			break;
		case 1:
			glTexImage1D(defn.target, 0, defn.internalFormat, defn.size.x, 0, defn.format, defn.type, data);
			break;
		}
	}

	void configure()
	{
		bind();
		texImage();
		glTexParameteri(defn.target, GL_TEXTURE_MIN_FILTER, defn.filter);
		glTexParameteri(defn.target, GL_TEXTURE_MAG_FILTER, defn.filter);
		glTexParameteri(defn.target, GL_TEXTURE_WRAP_S, defn.wrap);
		if (dimensions() > 1)
			glTexParameteri(defn.target, GL_TEXTURE_WRAP_T, defn.wrap);
		if (dimensions() > 2)
			glTexParameteri(defn.target, GL_TEXTURE_WRAP_R, defn.wrap);
		if (defn.wrap == GL_CLAMP_TO_BORDER)
			glTexParameterfv(defn.target, GL_TEXTURE_BORDER_COLOR, (float*)&defn.borderColor);
	}
	void setSize(glm::ivec3 newSize)
	{
		defn.size = newSize;
		bind();
		texImage();
	}

	void setImageMode(GLenum imageMode)
	{
		defn.imageMode = imageMode;
		bind();
	}

	void setFormat(GLenum internalFormat, GLenum format, GLenum type)
	{
		defn.internalFormat = internalFormat;
		defn.format = format;
		defn.type = type;
		bind();
		texImage();
	}

	void setWrap(GLenum wrap, glm::vec4 borderColor = glm::vec4(0.0f))
	{
		defn.wrap = wrap;
		defn.borderColor = borderColor;
		glTexParameteri(defn.target, GL_TEXTURE_WRAP_S, defn.wrap);
		if (dimensions() > 1)
			glTexParameteri(defn.target, GL_TEXTURE_WRAP_T, defn.wrap);
		if (dimensions() > 2)
			glTexParameteri(defn.target, GL_TEXTURE_WRAP_R, defn.wrap);
		if (defn.wrap == GL_CLAMP_TO_BORDER)
			glTexParameterfv(defn.target, GL_TEXTURE_BORDER_COLOR, (float*)&defn.borderColor);
	}
};

class SlabTexture
{
public:
	Texture source, dest;
	TextureDefinition defn{};

	SlabTexture(TextureDefinition defn) : defn(defn), source(defn), dest(defn)
	{
	}

	void bind()
	{
		glActiveTexture(GL_TEXTURE0 + defn.textureUnit);
		glBindTexture(defn.target, source.ID);
		glBindImageTexture(defn.textureUnit, dest.ID, 0, GL_TRUE, 0, defn.imageMode, defn.internalFormat);
	}

	void swap()
	{
		std::swap(source, dest);
		bind();
	}

	void configure()
	{
		source.configure();
		dest.configure();
	}

	void clearImage(glm::vec4& data)
	{
		source.clearImage(data);
		dest.clearImage(data);
	}

	void setSize(glm::ivec3 newSize)
	{
		defn.size = newSize;
		source.setSize(newSize);
		dest.setSize(newSize);
	}

	void setFormat(GLenum internalFormat, GLenum format, GLenum type)
	{
		defn.internalFormat = internalFormat;
		defn.format = format;
		defn.type = type;
		source.setFormat(internalFormat, format, type);
		dest.setFormat(internalFormat, format, type);
	}

	void setImageMode(GLenum imageMode)
	{
		defn.imageMode = imageMode;
		source.setImageMode(imageMode);
		dest.setImageMode(imageMode);
	}

	void setWrap(GLenum wrap, glm::vec4 borderColor = glm::vec4(0.0f))
	{
		defn.wrap = wrap;
		defn.borderColor = borderColor;
		source.setWrap(wrap, borderColor);
		dest.setWrap(wrap, borderColor);
	}
};