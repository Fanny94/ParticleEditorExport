#include "ParticleEmitter.h"
#include <math.h>

namespace Gear
{
	ParticleEmitter::ParticleEmitter()
	{


	}

	ParticleEmitter::ParticleEmitter(float gravity, int n, float life, float speed, float rate, int number, float focusSpread, float dirx, float dirY, float dirZ, float size, float shrink) : isActive(false), timer(0)
	{
		gravityFactor = gravity;
		maxParticles = n;
		allParticles = new Partikel[n];
		particlePos = new SendStruct[n];
		nrOfActiveParticles = 0;
		glGenBuffers(1, &particleVertexBuffer);
		this->lifeTime = life;
		partSpeed = speed;
		particleRate = 1 / rate;
		partPerRate = number;
		direction = { dirx, dirY, dirY };
		focus = focusSpread;
		particleSize = size;
		shrinkage = shrink;
		for (size_t i = 0; i <maxParticles; i++)
		{
			particlePos[i].size = particleSize;
		}

	}

	GEAR_API void Gear::ParticleEmitter::emitterInit(float gravity, int n, float life, float speed, float rate, int number, float focusSpread, float dirx, float dirY, float dirZ, float size, float shrink)
	{
		isActive = false; timer = 0;
		gravityFactor = gravity;
		maxParticles = n;
		allParticles = new Partikel[n];
		particlePos = new SendStruct[n];
		nrOfActiveParticles = 0;
		glGenBuffers(1, &particleVertexBuffer);
		this->lifeTime = life;
		partSpeed = speed;
		particleRate = 1 / rate;
		partPerRate = number;
		direction = { dirx, dirY, dirY };
		focus = focusSpread;
		particleSize = size;
		shrinkage = shrink;
		for (size_t i = 0; i <maxParticles; i++)
		{
			particlePos[i].size = particleSize;
		}
	}

	ParticleEmitter::~ParticleEmitter()
	{
		delete[] allParticles;
		delete[] particlePos;
	}
	void ParticleEmitter::spawn(float dt)
	{
		if (alive)
		{
			timer += dt;
			if (timer > particleRate)
			{
				glm::vec3 tempVec = this->position + direction * focus; //emit direction
				glm::vec3 temp2;
				int i = 0;
				while (nrOfActiveParticles < maxParticles && partPerRate > i++)
				{
					particlePos[nrOfActiveParticles].pos = this->position;
					allParticles[nrOfActiveParticles].lifeSpan = this->lifeTime;
					temp2 = glm::normalize(glm::vec3((rand() % 20 - 10), (rand() % 20 - 10), (rand() % 20 - 10))) + tempVec;
					allParticles[nrOfActiveParticles++].direction = glm::normalize(temp2 - this->position);
				}
				timer = 0;
			}
		}
	}

	bool ParticleEmitter::update(const float &dt)
	{
		if (isActive)
		{
			spawn(dt);

			float randomSpeed;
			for (int i = 0; i < nrOfActiveParticles; i++)
			{
				allParticles[i].lifeSpan -= dt;
				if (allParticles[i].lifeSpan > 0.0)
				{
					allParticles[i].direction.y += gravityFactor * dt;
					randomSpeed = rand() % (int)partSpeed;
					particlePos[i].pos += allParticles[i].direction * randomSpeed * dt;
					particlePos[i].size += shrinkage * dt;
				}
				else
				{
					particlePos[i] = particlePos[nrOfActiveParticles - 1];
					allParticles[i] = allParticles[--nrOfActiveParticles];
					particlePos[i].size = this->particleSize;
					if (nrOfActiveParticles <= 0)
						isActive = false;
				}
			}
		}
		return !isActive;
	}

	void ParticleEmitter::explode()
	{
		nrOfActiveParticles = 0;
		for (int i = 0; i < maxParticles; i++)
		{
			particlePos[i].pos = this->position;
			allParticles[i].lifeSpan = this->lifeTime;
			allParticles[i].direction = glm::normalize(glm::vec3(rand() % 10 - 5, rand() % 10 - 5, rand() % 10 - 5));
			allParticles[i].direction *= rand() % (int)partSpeed;
			nrOfActiveParticles = i;
		}
		isActive = true;
		alive = false;
	}

	void ParticleEmitter::resetEmitter()
	{
		for (int i = 0; i < this->maxParticles; i++)
		{
			particlePos[i].pos = this->position;
			allParticles[i].lifeSpan = 0;
			allParticles[i].direction = { 0, 0, 0 };
			particlePos[i].size = 1.0;
			
		}
		nrOfActiveParticles = 0;
	}

	GLuint ParticleEmitter::getPartVertexBuffer()
	{
		return particleVertexBuffer;
	}

	void ParticleEmitter::setEmitterPos(glm::vec3 pos)
	{
		this->position = pos;
	}

	void ParticleEmitter::setDirection(glm::vec3 dir)
	{
		this->direction = dir;
	}

	GEAR_API int ParticleEmitter::getNrOfActiveParticles()
	{
		return nrOfActiveParticles;
	}

	GEAR_API Partikel * ParticleEmitter::getThePartikels()
	{
		return allParticles;
	}

	GEAR_API void ParticleEmitter::activate()
	{
		isActive = true;
		alive = true;
	}

	GEAR_API void ParticleEmitter::deActivate()
	{
		alive = false;
	}

	GEAR_API SendStruct * ParticleEmitter::getPositions()
	{
		return particlePos;
	}

	//GEAR_API void ParticleEmitter::setParticlePosition(glm::vec3 * pos)
	//{
	//	this->particlePos = pos;
	//}

	GEAR_API void ParticleEmitter::setTextrue(Importer::TextureAsset * tAParticles, char* textureName)
	{
		textureAssetParticles = tAParticles;
		particleTextureName = textureName;
	}

	GEAR_API Importer::TextureAsset * ParticleEmitter::getTexture()
	{
		return textureAssetParticles;
	}

	GEAR_API char* ParticleEmitter::getTextureName()
	{
		return particleTextureName;
	}
}