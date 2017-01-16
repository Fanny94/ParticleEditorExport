#include "Gear.h"

namespace Gear
{
	GearEngine::GearEngine()
	{
		glewInit();
		staticModels = &defaultModelList;
		queue.init();
	}

	GearEngine::~GearEngine()
	{
		glfwTerminate();
	}

	void GearEngine::queueModels(std::vector<ModelInstance>* models)
	{
		staticModels = models;
	}

	void GearEngine::queueParticles(std::vector<ParticleSystem*>* particles)
	{
		particleSystems = particles;
	}

	void GearEngine::drawParticle(Camera* camera)
	{
		queue.updateUniforms(camera);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		queue.particlePass(particleSystems);
	}

}