#pragma once

#include "Renderer.h"

class Texture
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP;
public:
	Texture(const std::string& path);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
	



};

class CubeMap
{
public:
	CubeMap(std::vector<std::string> faces);
	~CubeMap();

	void Bind(unsigned int slot = 0);
private:
	int m_Width, m_Height, m_BPP;
	unsigned int m_RendererID;
	std::vector<std::string> faces;
};

class ComputeTexture
{
public:
	ComputeTexture(int width,int height);
	~ComputeTexture();
	void Delete();
	GLuint  m_RendererID = 0;
	int width, height;
private:
	
	
};
class Framebuffer
{
public:
	Framebuffer(const ComputeTexture texture);
	~Framebuffer();
	void BlitFramebufferToSwapchain();
	bool AttachTexture(const ComputeTexture texture);
	ComputeTexture ColorAttachment;
private:
	
	GLuint  m_RendererID = 0;
};


