#include "ParticleEditor.h"

inline void TwEventMouseButtonGLFW3(GLFWwindow* window, int button, int action, int mods) { TwEventMouseButtonGLFW(button, action); }
inline void TwEventMousePosGLFW3(GLFWwindow* window, double xpos, double ypos) { TwMouseMotion(int(xpos), int(ypos)); }
glm::vec3 tempColor;
glm::vec3 tempPos;
bool button = false;

ParticleEditor::ParticleEditor()
{
	this->running = true;
	//nrPs.numPS = 1;
	p.numOfParticles = 50;
	p.lifeTime = 1;
	p.speed = 10;
	p.emitPerSecond = 15;
	p.nrOfParticlesPerEmit = 5;

	char* ptr = "fireball";
	memcpy(&p.textureName, ptr, sizeof(const char[32]));	
	
	lifeTime = 3;
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
	
	Importer::TextureAsset* particlesTexture = assets.load<TextureAsset>("Textures/fireball.png");

	writeToFile();

	PerformanceCounter counter;
	double deltaTime;
	bool lockMouse = false;
	window.changeCursorStatus(lockMouse);

	engine.addDebugger(Debugger::getInstance());

	ps.push_back(new Gear::ParticleSystem(p.numOfParticles, p.lifeTime, p.speed, p.emitPerSecond, p.nrOfParticlesPerEmit));

	glm::vec3 tempVec = { 0, 0, -2 };

	ps.at(0)->setEmmiterPos(tempVec);
	ps.at(0)->isActive = true;
	ps.at(0)->setTextrue(particlesTexture);

	while (running == true && window.isWindowOpen())
	{
		deltaTime = counter.getDeltaTime();
		inputs.update();

		update(deltaTime);

		camera.updateLevelEditorCamera(deltaTime);

		ps.at(0)->updateParticleEditor(deltaTime);

		engine.queueParticles(&ps);

		engine.drawParticle(&camera);
		
		if (inputs.keyPressed(GLFW_KEY_ESCAPE))
			running = false;

		glfwPollEvents();
		TwDraw();
		window.update();
	}
	
	TwTerminate();
	glfwTerminate();
}

void ParticleEditor::update(float dt)
{
	if (lifeTime = lifeTime)
	{
		setAlive();
	}

	lifeTime = lifeTime - dt;
	if (lifeTime <= 0)
	{
		setAlive();
	}
}

void ParticleEditor::setAlive()
{
	ps.at(0)->activate();
}

void ParticleEditor::die(const float & dt)
{
	ps.at(0)->deActivate();
	//ps.at(0)->updateWhenDead(dt);

}

void TW_CALL ParticleEditor::addParticle(void*)
{
	button = true;
}

void ParticleEditor::setBar()
{
	tempPos = { 1.0, 0.0, 0.0 };
	tempColor = { 0.0, 0.5, 0.5 };

	editorBarName = "ParticleEditorBar";

	editorBar = TwNewBar(editorBarName.c_str());

	TwDefine("ParticleEditorBar label='Particle Editor' position='0 0' size='300 720'resizable=false buttonalign=right color='192 255 192' movable=false");

	TwAddVarRW(editorBar, "Position", TW_TYPE_DIR3F, &tempPos, " label='Position' opened=true");
	TwAddVarRW(editorBar, "Color", TW_TYPE_COLOR3F, &tempColor, " label='Color' opened=true");
	//TwAddVarRW(editorBar, "Duration", TW_TYPE_FLOAT, &tempDuration, " label='Duration' min=0 max=10 step=0.01");
	//TwAddVarRW(editorBar, "Speed", TW_TYPE_FLOAT, &tempSpeed, " label='Speed' min=1 max=10 step=0.1");
	//TwAddVarRW(editorBar, "Angle", TW_TYPE_FLOAT, &tempAngle, " label='Angle' min=0 max=180 step=1");
	TwAddSeparator(editorBar, "Sep", NULL);
	TwAddButton(editorBar, "AddParticleButton", addParticle, NULL, "label='Add Particle'");
}

void ParticleEditor::writeToFile()
{

	std::string texture;

	FILE* file = NULL;
	file = fopen("particle.dp", "wb");

	if (file)
	{

		fwrite(&p, sizeof(particle), 1, file);

		fclose(file);
	}
	int x = 0;

}
