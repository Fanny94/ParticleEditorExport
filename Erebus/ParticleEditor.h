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
#include "ParticleSystem.h"

class ParticleEditor
{
public:
	ParticleEditor();
	~ParticleEditor();
	void start();

	struct Emitter
	{
		int numOfParticles;
		float posX, posY, posZ;
		float lifeTime;
		float speed;
		float particleRate;
		int partPerRate;
		float gravity;
		float focusSpread;
		float particleSize;
		float dirX, dirY, dirZ;
		float shrinkage;
		char textureName[32];
	};

	static void TW_CALL newTexture(void*);
	static void TW_CALL newParticleFile(void*);
	static void TW_CALL start(void*);
	static void TW_CALL reset(void*);
	static void TW_CALL save(void*);
	static void TW_CALL addEmitter(void*);
	void setBar();
	void writeToFile();
	void readFromFile(std::string path);
	void update();
	void updateSystem();

private:
	float lifeTime;
	int selectedEmitter;
	int nrOfEmitters;
	bool running;
	Window window;
	
	Gear::ParticleSystem* ps;
	std::vector<Gear::ParticleEmitter*> particleEmitters;
	Gear::ParticleEmitter* pEmitter;
	TwBar * editorBar;
	Emitter emitter;
	TextureAsset* textureAsset;
	void copyOverVariables();
};