#pragma once
#include <iostream>
#include "Gear.h"
#include "Inputs.h"
#include "Assets.h"
#include "TextureAsset.h"
#include "ModelAsset.h"
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

	static void TW_CALL newTexture(void*);
	static void TW_CALL start(void*);
	static void TW_CALL reset(void*);
	static void TW_CALL save(void*);
	void setBar();
	void writeToFile();
	void update();
	void updateSystem();

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
	Window window;
	std::vector<Gear::ParticleSystem*> ps;
	TwBar * editorBar;
	particle p;
	TextureAsset* textureAsset;
};