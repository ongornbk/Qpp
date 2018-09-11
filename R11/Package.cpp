#include "pch.h"
#include "Package.h"


Package::Package()
{
}


Package::~Package()
{
}

void Package::Import()
{
	m_initializer();
}
