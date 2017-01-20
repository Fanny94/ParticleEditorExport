#pragma once
#include "ShaderProgram.h"
#include "Camera.h"
#include "ParticleEmitter.h"
#include "ModelAsset.h"
#include "TextureAsset.h"
#include "Material.h"
#include <vector>
#include <numeric>
#include <algorithm>
#include "Animation.h"

using namespace Importer;
struct ModelInstance {
	ModelAsset* asset;
	std::vector<int> worldIndices;
};
class RenderQueue
{
public:
	RenderQueue();
	~RenderQueue();

	void init();
	void updateUniforms(Camera* camera);

	void forwardPass(std::vector<ModelInstance>* staticModels);
	// TEMP:
	std::vector<Gear::ParticleEmitter*> particleEmitters;

	void particlePass(std::vector<Gear::ParticleEmitter*>* particleEmitters);

private:
	ShaderProgram* allShaders[2];
	std::vector<ModelInstance> instances;
	glm::mat4* worldMatrices;
	glm::mat4* tempMatrices;
	int nrOfWorlds;
	int totalWorlds;

};