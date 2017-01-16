#pragma once
#include "ShaderProgram.h"
#include "Camera.h"
#include "ParticleSystem.h"
#include "ModelAsset.h"
#include "TextureAsset.h"
#include "Material.h"
#include <vector>
#include <numeric>
#include <algorithm>
#include "Animation.h"

using namespace Importer;

class RenderQueue
{
public:
	RenderQueue();
	~RenderQueue();

	void init();
	void updateUniforms(Camera* camera);

	// TEMP:
	std::vector<Gear::ParticleSystem*> particleSystem;

	void particlePass(std::vector<Gear::ParticleSystem*>* particleSystems);

private:
	ShaderProgram* allShaders[1];
};