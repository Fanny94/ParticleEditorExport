#include <iostream>

#include "Gear.h"
#include "Window.h"
#include <thread>
#include "ParticleEditor.h"

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	ParticleEditor particleEditor;
	particleEditor.start();
	return 0;	
}

