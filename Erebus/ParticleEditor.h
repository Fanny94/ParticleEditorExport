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
	void update(float dt);
	void setAlive();
	void die(const float & dt);

	static void TW_CALL addParticle(void*);
	void setBar();
	void writeToFile();

	struct particle
	{
		int numOfParticles;
		float lifeTime;
		float speed;
		float emitPerSecond;
		int nrOfParticlesPerEmit;
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