#pragma once

#include <vector>

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "Math.h"


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

class Tex_Fraktal2D : public Objekt
{
public:
	Tex_Fraktal2D();
	~Tex_Fraktal2D();

private:

};

class Line : public Objekt
{
private:
	Math::Vec2 move  {0.0,0.005};
public:
	Line();
	Line(std::vector<Math::Vec2> Positions);
	void AddData(std::vector<Math::Vec2> Positions);
	~Line();
};