#pragma once

class VertexBuffer
{
private:
	unsigned int m_RendererID;
public:
	VertexBuffer();
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void AddData(const void* data, unsigned int size);//Only call once

	void Bind() const;
	void Unbind() const;
};

