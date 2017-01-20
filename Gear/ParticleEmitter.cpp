#include "ParticleEmitter.h"
#include <math.h>

namespace Gear
{
	ParticleEmitter::ParticleEmitter() : particleSize(1.0)
	{


	}

	ParticleEmitter::ParticleEmitter(int n, float life, float speed, float rate, int number) : isActive(false), timer(0), particleSize(1.0)
	{
		gravityFactor = 0.0;
		maxParticles = n;
		allParticles = new Partikel[n];
		particlePos = new glm::vec3[n];
		nrOfActiveParticles = 0;
		glGenBuffers(1, &particleVertexBuffer);
		this->lifeTime = life;
		partSpeed = speed;
		particleRate = 1 / rate;
		partPerRate = number;
		direction = { 1, 0, 0 };
		direction = { 0, 0, 0 };
		focus = 0;
	}

	GEAR_API void Gear::ParticleEmitter::emitterInit(int n, float life, float speed, float rate, int number)
	{
		isActive = false; timer = 0;
		gravityFactor = 0.0;
		maxParticles = n;
		allParticles = new Partikel[n];
		particlePos = new glm::vec3[n];
		nrOfActiveParticles = 0;
		glGenBuffers(1, &particleVertexBuffer);
		this->lifeTime = life;
		partSpeed = speed;
		particleRate = 1 / rate;
		partPerRate = number;
		direction = { 1, 0, 0 };
		direction = { 0, 0, 0 };
		focus = 0;

	}

	ParticleEmitter::~ParticleEmitter()
	{
		delete[] allParticles;
		delete[] particlePos;
	}
	GEAR_API void ParticleEmitter::update(const float &dt)
	{
		if (isActive)
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

						particlePos[nrOfActiveParticles] = this->position;
						allParticles[nrOfActiveParticles].lifeSpan = this->lifeTime;
						temp2 = glm::normalize(glm::vec3((rand() % 20 - 10), (rand() % 20 - 10), (rand() % 20 - 10))) + tempVec;
						allParticles[nrOfActiveParticles++].direction = glm::normalize(temp2 - this->position);
					}
					timer = 0;
				}
			}
			float randomSpeed;
			for (int i = 0; i < nrOfActiveParticles; i++)
			{
				allParticles[i].lifeSpan -= dt;
				if (allParticles[i].lifeSpan > 0.0)
				{
					allParticles[i].direction.y += gravityFactor * dt;
					randomSpeed = rand() % (int)partSpeed;
					particlePos[i] += allParticles[i].direction * randomSpeed * dt;
				}
				else
				{
					particlePos[i] = particlePos[nrOfActiveParticles - 1];
					allParticles[i] = allParticles[--nrOfActiveParticles];
					if (nrOfActiveParticles <= 0)
						isActive = false;
				}
			}
		}
	}

	void ParticleEmitter::explode()
	{
		nrOfActiveParticles = 0;
		for (int i = 0; i < maxParticles; i++)
		{
			particlePos[i] = this->position;
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
			particlePos[i] = this->position;
			allParticles[i].lifeSpan = 0;
			allParticles[i].direction = { 0, 0, 0 };
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

	GEAR_API glm::vec3 * ParticleEmitter::getPositions()
	{
		return particlePos;
	}

	GEAR_API void ParticleEmitter::setParticlePosition(glm::vec3 * pos)
	{
		this->particlePos = pos;
	}

	GEAR_API void ParticleEmitter::setColor(float r, float g, float b)
	{
		color.r = r; color.g = g; color.b = b;
	}

	GEAR_API Color ParticleEmitter::getColor() const
	{
		return color;
	}

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