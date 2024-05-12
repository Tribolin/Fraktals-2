#include "Objekt.h"




Objekt::Objekt()
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

	vb.AddData(positions.data(),4*4*sizeof(float));
	

	layout.Push<float>(2);
	layout.Push<float>(2);
	va.AddBuffer(vb, layout);


	ib.AddData(indecies.data(),6);
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
