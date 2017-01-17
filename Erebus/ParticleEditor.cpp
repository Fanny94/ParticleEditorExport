#include "ParticleEditor.h"

inline void TwEventMouseButtonGLFW3(GLFWwindow* window, int button, int action, int mods) { TwEventMouseButtonGLFW(button, action); }
inline void TwEventMousePosGLFW3(GLFWwindow* window, double xpos, double ypos) { TwMouseMotion(int(xpos), int(ypos)); }
int tempNumberParticles;
float tempLifeTime;
float tempSpeed;
float tempEmitPerSecond;
int tempNrOfParticlesPerEmit;
float tempGravity;
float tempFocusSpread;
float tempParticleSize;
bool active = false;
glm::vec3 tempDirection;
bool button1 = false;
bool button2 = false;
bool newEmitter = false;
bool buttonReset = false;
bool hardReset = false;
bool buttonSave = false;
Importer::TextureAsset* pTexture;
Importer::TextureAsset* cubeTexture;
Importer::TextureAsset* particlesTexture;
char* pTexString;
std::string textureName = "fireball.png";
std::string saveName = "particle";
Importer::Assets assets;
std::vector<ModelInstance> mI;

ParticleEditor::ParticleEditor(): selectedEmitter(0), nrOfEmitters(1)
{
	this->running = true;
	p.numOfParticles = 50;
	p.lifeTime = 1;
	p.speed = 10;
	p.emitPerSecond = 15;
	p.nrOfParticlesPerEmit = 5;
	p.gravity = 0.0;
	p.focusSpread = 0;
}

ParticleEditor::~ParticleEditor()
{
	for (int i = 0; i < nrOfEmitters; i++)
		delete ps.at(i);
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
	

	ps.push_back(new Gear::ParticleSystem(p.numOfParticles, p.lifeTime, p.speed, p.emitPerSecond, p.nrOfParticlesPerEmit));
	setBar();

	glfwSetMouseButtonCallback(window.getGlfwWindow(), (GLFWmousebuttonfun)TwEventMouseButtonGLFW3);
	glfwSetCursorPosCallback(window.getGlfwWindow(), (GLFWcursorposfun)TwEventMousePosGLFW3);
	
	particlesTexture = assets.load<TextureAsset>("Textures/fireball.png");

	PerformanceCounter counter;
	double deltaTime;

	Importer::ModelAsset* mA = assets.load<ModelAsset>("Models/cube.model");

	window.changeCursorStatus(false);
	mI.resize(1);
	mI.at(0).asset = mA;

	ps.at(selectedEmitter)->isActive = false;

	pTexture = particlesTexture;
	pTexString = "fireball.png";
	ps.at(selectedEmitter)->setTextrue(pTexture);
	engine.queueParticles(&ps);

	engine.queueModels(&mI);
	engine.queueParticles(&ps);

	while (running == true && window.isWindowOpen())
	{
		deltaTime = counter.getDeltaTime();
		inputs.update();
		/*Används camera.update() ???*/
		//camera.updateLevelEditorCamera(deltaTime);
		if(nrOfEmitters > 0)
			updateSystem();

		for (int i = 0; i < nrOfEmitters; i++)
			ps.at(i)->update(deltaTime);


		engine.draw(&camera);
		
		if (inputs.keyPressed(GLFW_KEY_ESCAPE))
			running = false;
		if (inputs.keyPressed(GLFW_KEY_UP))
			ps.at(selectedEmitter)->position += glm::vec3(0, 2.0 * deltaTime, 0);
		if (inputs.keyPressed(GLFW_KEY_DOWN))
			ps.at(selectedEmitter)->position += glm::vec3(0, -2.0 * deltaTime, 0);
		if (inputs.keyPressed(GLFW_KEY_LEFT))
			ps.at(selectedEmitter)->position += glm::vec3(-2.0 * deltaTime, 0, 0);
		if (inputs.keyPressed(GLFW_KEY_RIGHT))
			ps.at(selectedEmitter)->position += glm::vec3(2.0 * deltaTime, 0, 0);
		if (inputs.keyPressed(GLFW_KEY_KP_ADD))
			ps.at(selectedEmitter)->position += glm::vec3(0, 0, 2.0 * deltaTime);
		if (inputs.keyPressed(GLFW_KEY_KP_SUBTRACT))
			ps.at(selectedEmitter)->position += glm::vec3(0, 0, -2.0 * deltaTime);
		if (inputs.keyPressedThisFrame(GLFW_KEY_TAB))
		{
			selectedEmitter = (selectedEmitter + 1) % nrOfEmitters;
			copyOverVariables();
		}
		if (inputs.keyPressedThisFrame(GLFW_KEY_ENTER))
		{
			if (hardReset)
			{
				hardReset = false;
				ps.at(selectedEmitter)->systemInit(p.numOfParticles, p.lifeTime, p.speed, p.emitPerSecond, p.nrOfParticlesPerEmit);
			}
		}
		if (inputs.keyPressedThisFrame(GLFW_KEY_DELETE))
		{
			delete ps.at(selectedEmitter);
			ps.erase(ps.begin() + selectedEmitter);
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
	tempEmitPerSecond = 15;
	tempNrOfParticlesPerEmit = 5;
	tempFocusSpread = 0;
	tempGravity = 0.0;
	tempDirection = { 0, 1, 0 };
	tempParticleSize = 1.0;

	editorBar = TwNewBar("ParticleEditorBar");

	TwDefine("ParticleEditorBar label='Particle Editor' position='0 0' size='300 720' valueswidth=145 buttonalign=right color='192 255 192' movable=false resizable=false iconifiable=false");

	TwAddVarRO(editorBar, "Number of Emitters", TW_TYPE_INT32, &nrOfEmitters, "label='Number of Emitters' opened=true");
	TwAddVarRO(editorBar, "Selected Emitter", TW_TYPE_INT32, &selectedEmitter, "label='Selected Emitter' opened=true");
	TwAddVarRW(editorBar, "Number Particles", TW_TYPE_INT32, &tempNumberParticles, "label='Number Particles' min=0");
	TwAddVarRW(editorBar, "Life Time", TW_TYPE_FLOAT, &tempLifeTime, "label='Life Time' min=1 step=0.1");
	TwAddVarRW(editorBar, "Speed", TW_TYPE_FLOAT, &tempSpeed, "label='Speed' min=1 step=0.1");
	TwAddVarRW(editorBar, "Emits per Second", TW_TYPE_FLOAT, &tempEmitPerSecond, "label='Emits / Sec' min=1");
	TwAddVarRW(editorBar, "Number of Particles per Emit", TW_TYPE_INT32, &tempNrOfParticlesPerEmit, "label='Particles / Emit' min=0");
	TwAddVarRW(editorBar, "Focus Spread", TW_TYPE_FLOAT, &tempFocusSpread, "label='Focus Spread' step=0.1");
	TwAddVarRW(editorBar, "Gravity", TW_TYPE_FLOAT, &tempGravity, "label='Gravity' step=0.1");
	TwAddVarRW(editorBar, "Particle Size", TW_TYPE_FLOAT, &tempParticleSize, "label='Particle Size' step=0.1");
	TwAddVarRW(editorBar, "Direction", TW_TYPE_DIR3F, &tempDirection, "label='Direction' opened=true");
	TwAddVarRO(editorBar, "Position", TW_TYPE_DIR3F, &(ps.at(selectedEmitter)->position), "label='Position' opened=true");
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
	std::string texture;
	char* ptr = pTexString;
	memcpy(&p.textureName, ptr, sizeof(const char[32]));
	FILE* file = NULL;
	file = fopen("particle.dp", "wb");
	if (file)
	{
		fwrite(&p, sizeof(particle), 1, file);
		fclose(file);
	}
}

void ParticleEditor::update()
{
	if (button1 == true)
	{
		particlesTexture = assets.load<TextureAsset>("Textures/" + textureName);
		ps.at(selectedEmitter)->textureAssetParticles = particlesTexture;
		pTexture = particlesTexture;
		pTexString = (char*)textureName.c_str();
		button1 = false;

	}
	if (button2 == true)
	{
		ps.at(selectedEmitter)->textureAssetParticles = particlesTexture;
		button2 = false;
		pTexString = "red.png";
	}
	if (buttonReset == true)
	{
		ps.at(selectedEmitter)->resetEmitter();
		pTexture = particlesTexture;
		buttonReset = false;
	}
	if (newEmitter)
	{
		ps.push_back(new Gear::ParticleSystem());
		nrOfEmitters++;
		selectedEmitter = (selectedEmitter + 1) % nrOfEmitters;
		ps.at(selectedEmitter)->setTextrue(pTexture);
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
	ps.at(selectedEmitter)->particleRate = 1 / tempEmitPerSecond;
	ps.at(selectedEmitter)->focus = tempFocusSpread;
	ps.at(selectedEmitter)->gravityFactor = tempGravity;
	ps.at(selectedEmitter)->lifeTime = tempLifeTime;
	ps.at(selectedEmitter)->partPerRate = tempNrOfParticlesPerEmit;
	ps.at(selectedEmitter)->partSpeed = tempSpeed;
	ps.at(selectedEmitter)->direction = tempDirection;
	ps.at(selectedEmitter)->isActive = active;
	ps.at(selectedEmitter)->particleSize = tempParticleSize;
}

void ParticleEditor::copyOverVariables()
{
	tempNumberParticles = ps.at(selectedEmitter)->maxParticles;
	tempLifeTime = ps.at(selectedEmitter)->lifeTime;
	tempSpeed = ps.at(selectedEmitter)->partSpeed;
	tempEmitPerSecond = 1 / ps.at(selectedEmitter)->particleRate;
	tempNrOfParticlesPerEmit = ps.at(selectedEmitter)->partPerRate;
	tempGravity = ps.at(selectedEmitter)->gravityFactor;
	tempFocusSpread = ps.at(selectedEmitter)->focus;
	tempDirection = ps.at(selectedEmitter)->direction;
	tempParticleSize = ps.at(selectedEmitter)->particleSize;
}
