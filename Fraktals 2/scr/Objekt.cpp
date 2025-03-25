#include "Objekt.h"




Objekt::Objekt()
{
}

Objekt::~Objekt()
{
}

void Objekt::Bind()
{
	va.Bind();
	ib.Bind();
}

void Objekt::Unbind()
{
	va.Unbind();
	vb.Unbind();
	ib.Unbind();
}

Tex_Fraktal2D::Tex_Fraktal2D()
{
	positions = {
		-1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, 1.0f, 1.0f,
		-1.0f,  1.0f, 0.0f, 1.0f
	};
	indecies = {
			0,1,2,
			2,3,0
	};

	vb.AddData(positions.data(), 4 * 4 * sizeof(float));


	layout.Push<float>(2);
	layout.Push<float>(2);
	va.AddBuffer(vb, layout);


	ib.AddData(indecies.data(), 6);
}

Tex_Fraktal2D::~Tex_Fraktal2D()
{
}

Box2D::Box2D(Math::Vec2 center, float width)
{
	float r = width / 2;
	float y = center.y;
	float x = center.x;
	positions = 
	{
		r+x,-r+y,
		-r+x,-r+y,
		-r+x,r+y,
		r+x,r+y
	};
	indecies = 
	{
		0,1,2,
		2,3,0
	};
	vb.AddData(positions.data(), positions.size() * sizeof(float));

	layout.Push<float>(2);

	va.AddBuffer(vb, layout);
	ib.AddData(indecies.data(), indecies.size());
}

Line::Line()
{
}

Line::Line(std::vector<Math::Vec2> Positions)
{
	ASSERT(Positions.size() > 1);

	std::vector<Math::Vec2> buf;
	for (size_t i = 0; i < Positions.size(); i++)
	{
		
		buf.push_back(Positions[i]);
		buf.push_back(Positions[i]+ move);
	}

	Positions = buf;
	for (size_t i = 0; i < Positions.size(); i+= 2)
	{
		indecies.push_back(i);
		indecies.push_back(i+1);
		indecies.push_back(i+2);
		
		indecies.push_back(i + 2);
		indecies.push_back(i + 3);
		indecies.push_back(i);
	}

	for (size_t i = 0; i < Positions.size(); i++)
	{
		positions.push_back(Positions[i].x);
		positions.push_back(Positions[i].y);
	}

	

	vb.AddData(positions.data(), positions.size()*sizeof(float));

	layout.Push<float>(2);

	va.AddBuffer(vb,layout);
	ib.AddData(indecies.data(), indecies.size());
}

void Line::AddData(std::vector<Math::Vec2> Positions)
{
	ASSERT(Positions.size() > 1);

	std::vector<Math::Vec2> buf;
	for (size_t i = 0; i < Positions.size(); i++)
	{
		
		buf.push_back(Positions[i]);
		buf.push_back((Positions[i] + move));
	}

	Positions = buf;
	for (size_t i = 0; i < Positions.size()-2; i += 2)
	{
		indecies.push_back(i);
		indecies.push_back(i + 1);
		indecies.push_back(i + 2);

		indecies.push_back(i + 2);
		indecies.push_back(i + 3);
		indecies.push_back(i+1 );
	}

	for (size_t i = 0; i < Positions.size(); i++)
	{
		positions.push_back(Positions[i].x);
		positions.push_back(Positions[i].y);
	}



	vb.AddData(positions.data(), positions.size() * sizeof(float));

	layout.Push<float>(2);

	va.AddBuffer(vb, layout);
	ib.AddData(indecies.data(), indecies.size());
}

Line::~Line()
{
}
