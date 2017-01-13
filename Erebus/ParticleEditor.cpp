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
bool button1 = false;
bool button2 = false;
bool buttonReset = false;
Importer::TextureAsset* pTexture;
Importer::TextureAsset* particlesTexture1;
Importer::TextureAsset* particlesTexture2;
char* pTexString;

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

	engine.addDebugger(Debugger::getInstance());

	ps.push_back(new Gear::ParticleSystem(p.numOfParticles, p.lifeTime, p.speed, p.emitPerSecond, p.nrOfParticlesPerEmit));

	glm::vec3 tempVec = { 0, 0, -2 };

	ps.at(0)->setEmmiterPos(tempVec);
	ps.at(0)->isActive = true;
	pTexture = particlesTexture1;
	pTexString = "fireball.png";

	while (running == true && window.isWindowOpen())
	{
		ps.at(0)->setTextrue(pTexture);

		deltaTime = counter.getDeltaTime();
		inputs.update();

		camera.updateLevelEditorCamera(deltaTime);

		ps.at(0)->updateParticleEditor(deltaTime, tempNumberParticles, tempLifeTime, tempSpeed, tempEmitPerSecond, tempNrOfParticlesPerEmit, tempFocusSpread, tempGravity);

		engine.queueParticles(&ps);

		engine.drawParticle(&camera);
		
		if (inputs.keyPressed(GLFW_KEY_ESCAPE))
			running = false;

		update();

		glfwPollEvents();
		TwDraw();
		window.update();
	}
	
	writeToFile();
	TwTerminate();
	glfwTerminate();
}

void TW_CALL ParticleEditor::newTexture1(void*)
{
	button1 = true;
}

void TW_CALL ParticleEditor::newTexture2(void*)
{
	button2 = true;
}

void TW_CALL ParticleEditor::reset(void *)
{
	buttonReset = true;
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

	editorBar = TwNewBar("ParticleEditorBar");

	TwDefine("ParticleEditorBar label='Particle Editor' position='0 0' size='300 720'resizable=false buttonalign=right color='192 255 192' movable=false");

	TwAddVarRW(editorBar, "Number Particles", TW_TYPE_INT32, &tempNumberParticles, "label='Number Particles' min=0");
	TwAddVarRW(editorBar, "Life Time", TW_TYPE_FLOAT, &tempLifeTime, "label='Life Time' min=1 step=0.1");
	TwAddVarRW(editorBar, "Speed", TW_TYPE_FLOAT, &tempSpeed, "label='Speed' min=1 step=0.1");
	TwAddVarRW(editorBar, "Emits per Second", TW_TYPE_FLOAT, &tempEmitPerSecond, "label='Emits / Sec' min=1");
	TwAddVarRW(editorBar, "Number of Particles per Emit", TW_TYPE_INT32, &tempNrOfParticlesPerEmit, "label='Particles / Emit' min=0");
	TwAddVarRW(editorBar, "Focus Spread", TW_TYPE_FLOAT, &tempFocusSpread, "label='Focus Spread' step=0.1");
	TwAddVarRW(editorBar, "Gravity", TW_TYPE_FLOAT, &tempGravity, "label='Gravity' step=0.1");
	TwAddSeparator(editorBar, "Sep1", NULL);
	TwAddButton(editorBar, "Fireball Texture", newTexture1, NULL, "label='Fireball Texture'");
	TwAddButton(editorBar, "Red Texture", newTexture2, NULL, "label='Red Texture'");
	TwAddSeparator(editorBar, "Sep2", NULL);
	TwAddButton(editorBar, "Reset", reset, NULL, "label='Reset'");
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
	int x = 0;
}

void ParticleEditor::update()
{
	if (button1 == true)
	{
		pTexture = particlesTexture1;
		button1 = false;
		pTexString = "fireball.png";
	}
	if (button2 == true)
	{
		pTexture = particlesTexture2;
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
		pTexture = particlesTexture1;
		buttonReset = false;
	}
}
