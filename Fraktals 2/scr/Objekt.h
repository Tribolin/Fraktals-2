#pragma once

#include <vector>

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"





class Objekt
{
private:

public:
	Objekt();
	~Objekt();

	void Bind();
	void Unbind();

	std::vector<float> positions;
	std::vector<unsigned int> indecies;
	
	VertexArray va;
	VertexBuffer vb;
	VertexBufferLayout layout;
	IndexBuffer ib;
};

