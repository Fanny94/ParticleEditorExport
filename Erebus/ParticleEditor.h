#pragma once
#include <iostream>
#include "Gear.h"
#include "Inputs.h"
#include "Assets.h"
#include "TextureAsset.h"
#include "Controls.h"
#include "Window.h"
#include <ctime>
#include "PerformanceCounter.h"
#include <String>
#include <thread>
#include <AntTweakBar.h>


class ParticleEditor
{
public:
	ParticleEditor();
	~ParticleEditor();
	void start();

	static void TW_CALL newTexture1(void*);
	static void TW_CALL newTexture2(void*);
	static void TW_CALL reset(void*);
	void setBar();
	void writeToFile();
	void update();

	struct particle
	{
		int numOfParticles;
		float lifeTime;
		float speed;
		float emitPerSecond;
		int nrOfParticlesPerEmit;
		float gravity;
		float focusSpread;
		/*float dirX, dirY, dirZ;*/
		char textureName[32];
	};

private:
	float lifeTime;
	bool running;
	Controls controls;
	Window window;
	std::vector<Gear::ParticleSystem*> ps;
	TwBar * editorBar;
	particle p;
	TextureAsset* textureAsset;
};