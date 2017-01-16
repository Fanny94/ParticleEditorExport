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
bool active = false;
glm::vec3 tempDirection;
bool button1 = false;
bool button2 = false;
bool buttonReset = false;
bool buttonSave = false;
Importer::TextureAsset* pTexture;
Importer::TextureAsset* particlesTexture1;
Importer::TextureAsset* particlesTexture2;
char* pTexString;
std::string textureName = "fireball";
std::string saveName = "particle";

ParticleEditor::ParticleEditor()
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
	delete ps.at(0);
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
	
	setBar();

	glfwSetMouseButtonCallback(window.getGlfwWindow(), (GLFWmousebuttonfun)TwEventMouseButtonGLFW3);
	glfwSetCursorPosCallback(window.getGlfwWindow(), (GLFWcursorposfun)TwEventMousePosGLFW3);
	
	particlesTexture1 = assets.load<TextureAsset>("Textures/fireball.png");
	particlesTexture2 = assets.load<TextureAsset>("Textures/red.png");

	PerformanceCounter counter;
	double deltaTime;
	bool lockMouse = false;
	window.changeCursorStatus(lockMouse);

	ps.push_back(new Gear::ParticleSystem(p.numOfParticles, p.lifeTime, p.speed, p.emitPerSecond, p.nrOfParticlesPerEmit));
	
	ps.at(0)->isActive = false;
	pTexture = particlesTexture1;
	pTexString = "fireball.png";
	ps.at(0)->setTextrue(pTexture);
	ps.at(0)->setEmmiterPos(glm::vec3(0, 0, -2));

	ps.at(0)->direction = { 0, 5, 0 };
	ps.at(0)->focus = 2;
	while (running == true && window.isWindowOpen())
	{
	
		deltaTime = counter.getDeltaTime();
		inputs.update();

		camera.updateLevelEditorCamera(deltaTime);
		updateSystem();

		ps.at(0)->update(deltaTime);
		engine.queueParticles(&ps);

		engine.drawParticle(&camera);
		
		if (inputs.keyPressed(GLFW_KEY_ESCAPE))
			running = false;
		update();
		glfwPollEvents();
		TwDraw();
		window.update();
	}
	TwTerminate();
}

void TW_CALL ParticleEditor::newTexture1(void*)
{
	button1 = true;
}

void TW_CALL ParticleEditor::newTexture2(void*)
{
	button2 = true;
}

void TW_CALL ParticleEditor::start(void*)
{
	active = !active;
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
	tempDirection = { 0, 5, 0 };

	editorBar = TwNewBar("ParticleEditorBar");

	TwDefine("ParticleEditorBar label='Particle Editor' position='0 0' size='300 720'resizable=false buttonalign=right color='192 255 192' movable=false");

	TwAddVarRW(editorBar, "Number Particles", TW_TYPE_INT32, &tempNumberParticles, "label='Number Particles' min=0");
	TwAddVarRW(editorBar, "Life Time", TW_TYPE_FLOAT, &tempLifeTime, "label='Life Time' min=1 step=0.1");
	TwAddVarRW(editorBar, "Speed", TW_TYPE_FLOAT, &tempSpeed, "label='Speed' min=1 step=0.1");
	TwAddVarRW(editorBar, "Emits per Second", TW_TYPE_FLOAT, &tempEmitPerSecond, "label='Emits / Sec' min=1");
	TwAddVarRW(editorBar, "Number of Particles per Emit", TW_TYPE_INT32, &tempNrOfParticlesPerEmit, "label='Particles / Emit' min=0");
	TwAddVarRW(editorBar, "Focus Spread", TW_TYPE_FLOAT, &tempFocusSpread, "label='Focus Spread' step=0.1");
	TwAddVarRW(editorBar, "Gravity", TW_TYPE_FLOAT, &tempGravity, "label='Gravity' step=0.1");
	TwAddVarRW(editorBar, "Direction", TW_TYPE_DIR3F, &tempDirection, "label='Direction' opened=true");
	TwAddSeparator(editorBar, "Sep1", NULL);
	TwAddButton(editorBar, "Activate", start, NULL, "label='Activate'");
	TwAddButton(editorBar, "Fireball Texture", newTexture1, NULL, "label='Fireball Texture'");
	TwAddButton(editorBar, "Red Texture", newTexture2, NULL, "label='Red Texture'");
	TwAddVarCB(editorBar, "Texture Name", TW_TYPE_STDSTRING, SetTextureStringCB, GetTextureStringCB, NULL, "label='Texture Name'");
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
	saveName = saveName + ".dp";
	const char* newString = saveName.c_str();
	file = fopen(newString /*+ "particle.dp"*/, "wb");
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
		ps.at(0)->textureAssetParticles = particlesTexture1;
		button1 = false;
		pTexString = "fireball.png";
	}
	if (button2 == true)
	{
		ps.at(0)->textureAssetParticles = particlesTexture2;
		button2 = false;
		pTexString = "red.png";
	}
	if (buttonReset == true)
	{
		tempNumberParticles = 50;
		tempLifeTime = 1;
		tempSpeed = 10;
		tempEmitPerSecond = 15;
		tempNrOfParticlesPerEmit = 5;
		tempFocusSpread = 0;
		tempGravity = 0.0;
		ps.at(0)->resetEmitter();
		pTexture = particlesTexture1;
		buttonReset = false;
	}
	if (buttonSave == true)
	{
		writeToFile();
		buttonSave = false;
	}
}

void ParticleEditor::updateSystem()
{
	ps.at(0)->particleRate = 1 / tempEmitPerSecond;
	ps.at(0)->focus = tempFocusSpread;
	ps.at(0)->gravityFactor = tempGravity;
	ps.at(0)->lifeTime = tempLifeTime;
	ps.at(0)->partPerRate = tempNrOfParticlesPerEmit;
	ps.at(0)->maxParticles = tempNumberParticles;
	ps.at(0)->partSpeed = tempSpeed;
	ps.at(0)->direction = tempDirection;
	ps.at(0)->isActive = active;
}
