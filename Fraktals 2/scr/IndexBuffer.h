#pragma once

class IndexBuffer
{
private:
	unsigned int m_RendererID;
	unsigned int m_Count;
public:
	IndexBuffer();
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void AddData(const unsigned int* data, unsigned int count);

	void Bind() const;
	void Unbind() const;

	inline unsigned int  GetCount() const { return m_Count; };
};

