#include "ParticleEditor.h"

inline void TwEventMouseButtonGLFW3(GLFWwindow* window, int button, int action, int mods) { TwEventMouseButtonGLFW(button, action); }
inline void TwEventMousePosGLFW3(GLFWwindow* window, double xpos, double ypos) { TwMouseMotion(int(xpos), int(ypos)); }
int tempNumberParticles;
float tempLifeTime;
float tempSpeed;
float tempRatePerSecond;
int tempNrOfParticlesPerRate;
float tempGravity;
float tempFocusSpread;
float tempParticleSize;
float tempShrinkage;
glm::vec3 tempPosition;
bool active = false;
glm::vec3 tempDirection;
bool button1 = false;
bool button2 = false;
bool newEmitter = false;
bool buttonReset = false;
bool hardReset = false;
bool buttonSave = false;
Importer::TextureAsset* pTexture;
Importer::TextureAsset* particlesTexture;
char* pTexString;
char* StringToCopy;
std::string textureName = "redParticle.png";
std::string saveName = "filename";
Importer::Assets assets;
std::vector<ModelInstance> mI;

ParticleEditor::ParticleEditor(): selectedEmitter(0), nrOfEmitters(1)
{
	this->running = true;
	emitter.gravity = 0.0;
	emitter.numOfParticles = 50;
	emitter.lifeTime = 1.0;
	emitter.speed = 10.0;
	emitter.particleRate = 15.0;
	emitter.partPerRate = 5.0;
	emitter.focusSpread = 0.0;
	emitter.dirX = 0.0;
	emitter.dirY = 1.0;
	emitter.dirZ = 0.0;
	emitter.particleSize = 1.0;
	emitter.shrinkage = 0.0;
}

ParticleEditor::~ParticleEditor()
{
	for (int i = 0; i < nrOfEmitters; i++)
		delete particleEmitters.at(i);
	delete ps;
}

void ParticleEditor::start()
{
	Importer::Assets assets;

	TwInit(TW_OPENGL_CORE, NULL);

	Gear::GearEngine engine;
	GLFWwindow* w = window.getGlfwWindow();
	Inputs inputs(w);

	glEnable(GL_DEPTH_TEST);

	Camera camera(45.f, 1280.f / 720.f, 0.1f, 2000.f, &inputs);

	TwWindowSize(1280, 720);
	ps = new Gear::ParticleSystem();

	pEmitter = new Gear::ParticleEmitter(emitter.gravity, emitter.numOfParticles, emitter.lifeTime, emitter.speed, emitter.particleRate, emitter.partPerRate, emitter.focusSpread, emitter.dirX, emitter.dirY, emitter.dirZ, emitter.particleSize, emitter.shrinkage);
	particleEmitters.push_back(pEmitter);
	ps->addEmitter(pEmitter);

	setBar();

	glfwSetMouseButtonCallback(window.getGlfwWindow(), (GLFWmousebuttonfun)TwEventMouseButtonGLFW3);
	glfwSetCursorPosCallback(window.getGlfwWindow(), (GLFWcursorposfun)TwEventMousePosGLFW3);
	
	particlesTexture = assets.load<TextureAsset>("Textures/redParticle.png");
	
	PerformanceCounter counter;
	double deltaTime;

	//Importer::ModelAsset* mA = assets.load<ModelAsset>("Models/testCube.model");

	window.changeCursorStatus(false);
	//mI.resize(1);
	//mI.at(0).asset = mA;

	particleEmitters.at(selectedEmitter)->isActive = false;

	pTexture = particlesTexture;
	pTexString = "redParticle.png";
	particleEmitters.at(selectedEmitter)->setTextrue(pTexture);
	particleEmitters.at(selectedEmitter)->texName = pTexString;

	//engine.queueModels(&mI);
	engine.queueParticles(&particleEmitters);

	while (running == true && window.isWindowOpen())
	{
		deltaTime = counter.getDeltaTime();
		inputs.update();
		/*Används camera.update() ???*/
		//camera.updateLevelEditorCamera(deltaTime);
		if(nrOfEmitters > 0)
			updateSystem();

		ps->update(deltaTime, ps->systemPos);
		engine.draw(&camera);
		
		if (inputs.keyPressed(GLFW_KEY_ESCAPE))
			running = false;
		if (inputs.keyPressed(GLFW_KEY_UP))
			particleEmitters.at(selectedEmitter)->localPos += glm::vec3(0, 2.0 * deltaTime, 0);
		if (inputs.keyPressed(GLFW_KEY_DOWN))
			particleEmitters.at(selectedEmitter)->localPos += glm::vec3(0, -2.0 * deltaTime, 0);
		if (inputs.keyPressed(GLFW_KEY_LEFT))
			particleEmitters.at(selectedEmitter)->localPos += glm::vec3(-2.0 * deltaTime, 0, 0);
		if (inputs.keyPressed(GLFW_KEY_RIGHT))
			particleEmitters.at(selectedEmitter)->localPos += glm::vec3(2.0 * deltaTime, 0, 0);
		if (inputs.keyPressed(GLFW_KEY_KP_ADD))
			particleEmitters.at(selectedEmitter)->localPos += glm::vec3(0, 0, 2.0 * deltaTime);
		if (inputs.keyPressed(GLFW_KEY_KP_SUBTRACT))
			particleEmitters.at(selectedEmitter)->localPos += glm::vec3(0, 0, -2.0 * deltaTime);
		if (inputs.keyPressedThisFrame(GLFW_KEY_TAB))
		{
			selectedEmitter = (selectedEmitter + 1) % nrOfEmitters;
			copyOverVariables();
		}

		if (inputs.keyPressed(GLFW_KEY_W))
			ps->systemPos += glm::vec3(0, 2.0 * deltaTime, 0);
		if (inputs.keyPressed(GLFW_KEY_S))
				ps->systemPos += glm::vec3(0, -2.0 * deltaTime, 0);
		if (inputs.keyPressed(GLFW_KEY_A))
			ps->systemPos += glm::vec3(-2.0 * deltaTime, 0, 0);
		if (inputs.keyPressed(GLFW_KEY_W))
			ps->systemPos += glm::vec3(2.0 * deltaTime, 0, 0);


		if (inputs.keyPressedThisFrame(GLFW_KEY_ENTER))
		{
			if (hardReset)
			{
				hardReset = false;
				particleEmitters.at(selectedEmitter)->emitterInit(tempGravity, tempNumberParticles, tempLifeTime, tempSpeed, tempRatePerSecond, tempNrOfParticlesPerRate, tempFocusSpread, tempDirection.x, tempDirection.y, tempDirection.z, tempParticleSize, tempShrinkage);

			}
		}
		if (inputs.keyPressedThisFrame(GLFW_KEY_DELETE))
		{
			delete particleEmitters.at(selectedEmitter);
			particleEmitters.erase(particleEmitters.begin() + selectedEmitter);
			nrOfEmitters--;
			if(nrOfEmitters > 0)
				selectedEmitter = (selectedEmitter + 1) % nrOfEmitters;
		}

		update();
		glfwPollEvents();
		TwDraw();
		window.update();
	}
	TwTerminate();
}

void TW_CALL ParticleEditor::newTexture(void*)
{
	button1 = true;
}

void TW_CALL ParticleEditor::start(void*)
{
	active = !active;
}

void TW_CALL ParticleEditor::addEmitter(void*)
{
	newEmitter = true;
}

void TW_CALL ParticleEditor::reset(void *)
{
	buttonReset = true;
}

void TW_CALL ParticleEditor::save(void *)
{
	buttonSave = true;
}

void TW_CALL SetTextureStringCB(const void *value, void *clientData)
{
	const std::string *srcPtr = static_cast<const std::string *>(value);
	textureName = *srcPtr;
}

void TW_CALL GetTextureStringCB(void *value, void *)
{
	std::string *destPtr = static_cast<std::string *>(value);
	TwCopyStdStringToLibrary(*destPtr, textureName);
}

void TW_CALL SetSaveNameStringCB(const void *value, void *clientData)
{
	const std::string *srcPtr = static_cast<const std::string *>(value);
	saveName = *srcPtr;
}

void TW_CALL GetSaveNameStringCB(void *value, void *)
{
	std::string *destPtr = static_cast<std::string *>(value);
	TwCopyStdStringToLibrary(*destPtr, saveName);
}

void ParticleEditor::setBar()
{
	tempNumberParticles = 50;
	tempLifeTime = 1;
	tempSpeed = 10;
	tempRatePerSecond = 15;
	tempNrOfParticlesPerRate = 5;
	tempFocusSpread = 0;
	tempGravity = 0.0;
	tempDirection = { 0, 1, 0 };
	tempParticleSize = 1.0;
	tempShrinkage = 0.0;

	editorBar = TwNewBar("ParticleEditorBar");

	TwDefine("ParticleEditorBar label='Particle Editor' position='0 0' size='300 720' valueswidth=145 buttonalign=right color='192 255 192' movable=false resizable=false iconifiable=false");

	TwAddVarRO(editorBar, "Number of Emitters", TW_TYPE_INT32, &nrOfEmitters, "label='Number of Emitters' opened=true");
	TwAddVarRO(editorBar, "Selected Emitter", TW_TYPE_INT32, &selectedEmitter, "label='Selected Emitter' opened=true");
	TwAddVarRW(editorBar, "Number Particles", TW_TYPE_INT32, &tempNumberParticles, "label='Number Particles' min=0");
	TwAddVarRW(editorBar, "Life Time", TW_TYPE_FLOAT, &tempLifeTime, "label='Life Time' min=1 step=0.1");
	TwAddVarRW(editorBar, "Speed", TW_TYPE_FLOAT, &tempSpeed, "label='Speed' min=1 step=0.1");
	TwAddVarRW(editorBar, "Rate per Second", TW_TYPE_FLOAT, &tempRatePerSecond, "label='Emits / Sec' min=1");
	TwAddVarRW(editorBar, "Number of Particles per Rate", TW_TYPE_INT32, &tempNrOfParticlesPerRate, "label='Particles / rate' min=0");
	TwAddVarRW(editorBar, "Focus Spread", TW_TYPE_FLOAT, &tempFocusSpread, "label='Focus Spread' step=0.1");
	TwAddVarRW(editorBar, "Gravity", TW_TYPE_FLOAT, &tempGravity, "label='Gravity' step=0.1");
	TwAddVarRW(editorBar, "Particle Size", TW_TYPE_FLOAT, &tempParticleSize, "label='Particle Size' step=0.1");
	TwAddVarRW(editorBar, "Shrink Size", TW_TYPE_FLOAT, &tempShrinkage, "label='Shrink Size' step = 0.1");
	TwAddVarRW(editorBar, "Direction", TW_TYPE_DIR3F, &tempDirection, "label='Direction' opened=true");
	TwAddVarRO(editorBar, "Position", TW_TYPE_DIR3F, &tempPosition, "label='Position' opened=true");
	TwAddSeparator(editorBar, "Sep1", NULL);
	TwAddButton(editorBar, "Activate", start, NULL, "label='Activate'");
	TwAddButton(editorBar, "Add Emitter", addEmitter, NULL, "label='Add Emitter'");
	TwAddVarCB(editorBar, "Texture Name", TW_TYPE_STDSTRING, SetTextureStringCB, GetTextureStringCB, NULL, "label='Texture Name'");
	TwAddButton(editorBar, "Load Texture", newTexture, NULL, "label='Load Texture'");
	TwAddSeparator(editorBar, "Sep2", NULL);
	TwAddButton(editorBar, "Reset", reset, NULL, "label='Reset'");
	TwAddVarCB(editorBar, "Name of Saved File", TW_TYPE_STDSTRING, SetSaveNameStringCB, GetSaveNameStringCB, NULL, "label='Name of Saved File'");
	TwAddButton(editorBar, "Save", save, NULL, "label='Save'");
}

void ParticleEditor::writeToFile()
{

	FILE* file = NULL;
	saveName = saveName + ".particle";
	file = fopen((char*)("ParticleFiles/" + saveName).c_str(), "wb");

	if (file)
	{

		fwrite(&nrOfEmitters, sizeof(int), 1, file);
		for (int i = 0; i < particleEmitters.size(); i++)
		{
			emitter.numOfParticles = particleEmitters.at(i)->maxParticles;

			emitter.posX = particleEmitters.at(i)->position.x;
			emitter.posY = particleEmitters.at(i)->position.y;
			emitter.posZ = particleEmitters.at(i)->position.z;

			emitter.lifeTime = particleEmitters.at(i)->lifeTime;
			emitter.speed = particleEmitters.at(i)->partSpeed;
			emitter.particleRate = particleEmitters.at(i)->particleRate;
			emitter.partPerRate = particleEmitters.at(i)->partPerRate;
			emitter.gravity = particleEmitters.at(i)->gravityFactor;
			emitter.focusSpread = particleEmitters.at(i)->focus;
			emitter.particleSize = particleEmitters.at(i)->particleSize;

			emitter.dirX = particleEmitters.at(i)->direction.x;
			emitter.dirY = particleEmitters.at(i)->direction.y;
			emitter.dirZ = particleEmitters.at(i)->direction.z;

			emitter.shrinkage = particleEmitters.at(i)->shrinkage;

			StringToCopy = particleEmitters.at(i)->texName;
			
			memcpy(&emitter.textureName, StringToCopy, sizeof(const char[32]));
			fwrite(&emitter, sizeof(emitter), 1, file);
		}

		fclose(file);
	}

}

void ParticleEditor::update()
{
	if (button1 == true)
	{
		particlesTexture = assets.load<TextureAsset>("Textures/" + textureName);
		particleEmitters.at(selectedEmitter)->textureAssetParticles = particlesTexture;
		pTexture = particlesTexture;
		pTexString = (char*)textureName.c_str();
		particleEmitters.at(selectedEmitter)->texName = pTexString;
		button1 = false;

	}
	//if (button2 == true)
	//{
	//	particleEmitters.at(selectedEmitter)->textureAssetParticles = particlesTexture;
	//	button2 = false;
	//	pTexString = "brightParticle.png";
	//}
	if (buttonReset == true)
	{
		particleEmitters.at(selectedEmitter)->resetEmitter();
		pTexture = particlesTexture;
		tempShrinkage = 0.0;
		tempParticleSize = 1.0;
		buttonReset = false;
	}
	if (newEmitter)
	{
		pEmitter = new Gear::ParticleEmitter();
		particleEmitters.push_back(pEmitter);
		ps->addEmitter(pEmitter);

		nrOfEmitters++;
		selectedEmitter = (selectedEmitter + 1) % nrOfEmitters;
		particleEmitters.at(selectedEmitter)->setTextrue(particlesTexture);
		particleEmitters.at(selectedEmitter)->texName = pTexString;
		hardReset = true;
		newEmitter = false;
	}
	if (buttonSave == true)
	{
		writeToFile();
		buttonSave = false;
	}
}

void ParticleEditor::updateSystem()
{
	particleEmitters.at(selectedEmitter)->particleRate = 1 / tempRatePerSecond;
	particleEmitters.at(selectedEmitter)->focus = tempFocusSpread;
	particleEmitters.at(selectedEmitter)->gravityFactor = tempGravity;
	particleEmitters.at(selectedEmitter)->lifeTime = tempLifeTime;
	particleEmitters.at(selectedEmitter)->partPerRate = tempNrOfParticlesPerRate;
	particleEmitters.at(selectedEmitter)->partSpeed = tempSpeed;
	particleEmitters.at(selectedEmitter)->direction = tempDirection;
	particleEmitters.at(selectedEmitter)->isActive = active;
	particleEmitters.at(selectedEmitter)->particleSize = tempParticleSize;
	particleEmitters.at(selectedEmitter)->shrinkage = tempShrinkage;
}

void ParticleEditor::copyOverVariables()
{
	tempNumberParticles = particleEmitters.at(selectedEmitter)->maxParticles;
	tempLifeTime = particleEmitters.at(selectedEmitter)->lifeTime;
	tempSpeed = particleEmitters.at(selectedEmitter)->partSpeed;
	tempRatePerSecond = 1 / particleEmitters.at(selectedEmitter)->particleRate;
	tempNrOfParticlesPerRate = particleEmitters.at(selectedEmitter)->partPerRate;
	tempGravity = particleEmitters.at(selectedEmitter)->gravityFactor;
	tempFocusSpread = particleEmitters.at(selectedEmitter)->focus;
	tempDirection = particleEmitters.at(selectedEmitter)->direction;
	tempParticleSize = particleEmitters.at(selectedEmitter)->particleSize;
	tempShrinkage = particleEmitters.at(selectedEmitter)->shrinkage;
	tempPosition = particleEmitters.at(selectedEmitter)->position;
}
