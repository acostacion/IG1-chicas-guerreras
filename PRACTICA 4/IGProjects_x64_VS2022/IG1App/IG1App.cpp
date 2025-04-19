#include "IG1App.h"

#include <iostream>
#include "Image.h"

using namespace std;

// static single instance (singleton pattern)
IG1App IG1App::s_ig1app; // default constructor (constructor with no parameters)

// Print OpenGL errors and warnings
void GLAPIENTRY debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
	GLsizei length, const GLchar* message, const void* userParam)
{
	const char* prefix = (type == GL_DEBUG_TYPE_ERROR)
		? "\x1b[31m[ERROR]\x1b[0m "
		: "\x1b[33m[WARNING]\x1b[0m ";
	cout << prefix << message << endl;
}

void
IG1App::close()
{
	cout << "Closing GLFW...\n";
	glfwSetWindowShouldClose(mWindow, true); // stops main loop

	delete mViewPort;
	delete mCamera;
	for (Scene* scene : mScenes)
		delete scene;

}

void
IG1App::run() // enters the main event processing loop
{
	if (mWindow == 0) // if not intilialized
		init();

	// IG1App main loop
	while (!glfwWindowShouldClose(mWindow)) 
	{
		// Redisplay the window if needed
		if (mNeedsRedisplay) {
			display();

				mNeedsRedisplay = false;
			
		}

		// si se permite la actualizacion
		if (mUpdateEnabled) 
		{
			// tiempo transcurrido desde que abres la ventana en segundos
			mStartTime = glfwGetTime();

			// tiempo que se ha tardado en ejecutar lo anterior (si tarda aunque sea mu poquitillo mu poquitillo cosa de na se lo quita 2ms o menos o lo que sea para que sea preciso).
			mNextUpdate = glfwGetTime() - mStartTime;
		
			update(); // llama al metodo update de cada objeto de la escena
			
			// con el tiempo restante para llegar a mNextUpdate
			glfwWaitEventsTimeout(FRAME_DURATION - mNextUpdate);
		}
		else
		{
			// Stop and wait for new events
			glfwWaitEvents();
		}
	}

	destroy();
}

void
IG1App::init()
{
	// create an OpenGL Context
	iniWinOpenGL();

	// create the scene after creating the context
	// allocate memory and resources
	mViewPort = new Viewport(mWinW, mWinH);

	// Creamos dos camaras (para lo de la division de escenas del apartado 52).
	mCamera = new Camera(mViewPort);
	mCamera2 = new Camera(mViewPort);

	// Crea las escenas
	// Mete las escenas en el vector de escenas
	mScenes.push_back(new Scene0());
	mScenes.push_back(new Scene1());
	mScenes.push_back(new Scene2());
	mScenes.push_back(new Scene3());
	mScenes.push_back(new Scene4());
	mScenes.push_back(new Scene5());
	mScenes.push_back(new Scene6());
	mScenes.push_back(new Scene7());
	mScenes.push_back(new Scene8());

	mCamera->set2D();
	mCamera2->set2D();

	for (int i = 0; i < mScenes.size(); i++) 
	{
		mScenes[i]->init();
	}
	
	mScenes[mCurrentScene]->load(); // !! HACER LOAD, para cargar los objetos a la escena
}

void IG1App::update()
{
	//Si estan las 2 escenas activadas, depende de la posicion en x del raton
	if (m2Scenes && mMouseCoord.x > mWinW / 2)
	{
		mScenes[3]->update();
	}
	else if (m2Scenes && mMouseCoord.x <= mWinW / 2)
	{
		mScenes[1]->update();
	}
	//Si no, se actualiza la que este
	else if (!m2Scenes)
	{
		mScenes[mCurrentScene]->update();
	}

	mNeedsRedisplay = true;
}

void
IG1App::iniWinOpenGL()
{ // Initialization
	cout << "Starting GLFW...\n";
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // enable on macOS

	// Create window with its associated OpenGL context, return window's pointer
	mWindow = glfwCreateWindow(mWinW, mWinH, "IG1App", nullptr, nullptr);

	if (mWindow == nullptr) {
		glfwTerminate();
		throw std::logic_error("cannot create GLFW window");
	}

	glfwMakeContextCurrent(mWindow);

	// Initialize OpenGL extension library
	if (GLenum err = glewInit(); err != GLEW_OK) {
		glfwTerminate();
		throw std::logic_error("Error while loading extensions: "s +
			reinterpret_cast<const char*>(glewGetErrorString(err)));
	}

	// Callback registration
	glfwSetWindowSizeCallback(mWindow, s_resize);
	glfwSetCharCallback(mWindow, s_key);
	glfwSetKeyCallback(mWindow, s_specialkey);
	glfwSetWindowRefreshCallback(mWindow, s_display);

	
	glfwSetMouseButtonCallback(mWindow, s_mouse); // cuando se presiona o se suelta un botón del ratón.
	glfwSetCursorPosCallback(mWindow, s_motion); // cuando se mueve el ratón.
	glfwSetScrollCallback(mWindow, s_mouseWheel); // cuando se gira la rueda del ratón o se hace el gesto equivalente con el touchpad.

	// Error message callback (all messages)
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0u, 0, GL_TRUE);
	glDebugMessageCallback(debugCallback, nullptr);

	cout << glGetString(GL_VERSION) << '\n';
	cout << glGetString(GL_VENDOR) << '\n';
}

void
IG1App::destroy()
{ // release memory and resources
	for (Scene* scene : mScenes)
		delete scene;
	mScenes.clear();

	delete mCamera;
	mCamera = nullptr;

	delete mCamera2;
	mCamera2 = nullptr;

	delete mViewPort;
	mViewPort = nullptr;

	glfwTerminate();
}

void
IG1App::display() const
{ // double buffering

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clears the back buffer

	// El back color buffer queda con el color de fondo en todos los pixeles, 
	// y el Z-buffer con el valor 1 en todos los pixeles

	if (m2Vistas) { display2V(); } // 2 VISTAS.
	else if (m2Scenes) { display2S(mScenes[1], mScenes[3]); } // 2 ESCENAS
	else { mScenes[mCurrentScene]->render(*mCamera); } // uploads the viewport and camera to the GP

	glfwSwapBuffers(mWindow); // swaps the front and back buffer
}

void
IG1App::resize(int newWidth, int newHeight)
{
	mWinW = newWidth;
	mWinH = newHeight;

	// Resize Viewport to the new window size
	mViewPort->setSize(newWidth, newHeight);

	// Resize Scene Visible Area such that the scale is not modified
	mCamera->setSize(mViewPort->width(), mViewPort->height());
}

void
IG1App::key(unsigned int key)
{
	Camera* actualCam = mCamera;
	//Si esta el modo en 2 escenas, y el raton esta en la segunda mitad
	if (m2Scenes && mMouseCoord.x > mWinW / 2) { actualCam = mCamera2; }

	Scene* actualScene = mScenes[mCurrentScene];
	//ColorMaterialEntity* cME = new ColorMaterialEntity();

	bool need_redisplay = true;

	switch (key) {
	case '+':
		actualCam->setScale(+0.01); // zoom in  (increases the scale)
		break;
	case '-':
		actualCam->setScale(-0.01); // zoom out (decreases the scale)
		break;

		// 3D.
	case 'l':
		actualCam->set3D();
		break;

		// 2D.
	case 'o':
		actualCam->set2D();
		break;

		// CENITAL.
	case 'c':
		actualCam->setCenital();
		break;

		// DOS VISTAS
	case 'k':
		m2Vistas = !m2Vistas;
		m2Scenes = false; // si se activa m2Vistas, se quita m2Scenes.
		break;

		// DOS ESCENAS
	case 'm':
		m2Vistas = false; // si se activa m2Scenes, se quita m2Vistas.
		m2Scenes = !m2Scenes;

		if (m2Scenes)
		{
			mScenes[1]->load();
			mScenes[3]->load();
		}
		else
		{
			mScenes[mCurrentScene] = actualScene;
		}
		break;

		// UPDATE.
	case 'u':
		mUpdateEnabled = !mUpdateEnabled;
		break;

		// SCREENSHOT.
	case 'i': // es la i de instantanea jajaj
		captura();
		break;

		// METODOS DEL ADVANCEDTIE
	case 'f':
		if (mScenes[mCurrentScene] == mScenes[7])
		{
			mScenes[mCurrentScene]->rotate();
		}
		break;

	case 'g':
		if (mScenes[mCurrentScene] == mScenes[7])
		{
			mScenes[mCurrentScene]->orbit();
		}
		break;

		// MOVELR (left)
	case 'a':
		actualCam->moveLR(1); // !tst luego cambiar.
		break;

		// MOVELR (right)
	case 'd':
		actualCam->moveLR(-1);
		break;

		// MOVEUD (up)
	case 'w':
		actualCam->moveUD(-1);
		break;

		// MOVEUD (down)
	case 's':
		actualCam->moveUD(1);
		break;
		
		// MOVEFB (forward)
	case 'W':
		actualCam->moveFB(1);
		break;

		// MOVEFB (backward)
	case 'S':
		actualCam->moveFB(-1);
		break;

		// change projection
	case 'p':
		actualCam->changePrj();
		break;

		// orbita alrededor del eje y
	case 'q':
		actualCam->orbit(0.5);
		break;

	//case 'n':
	//	//ColorMaterialEntity::toggleShowNormals();
	//	break;

	default:
		if (key >= '0' && key <= '9' && !changeScene(key - '0'))
			cout << "[NOTE] There is no scene " << char(key) << ".\n";
		else
			need_redisplay = false;
		break;
	} // switch

	if (need_redisplay)
		mNeedsRedisplay = true;
	
}

void
IG1App::specialkey(int key, int scancode, int action, int mods)
{
	
	// Only interested in press events
	if (action == GLFW_RELEASE)
		return;

	bool need_redisplay = true;

	// Handle keyboard input
	// (key reference: https://www.glfw.org/docs/3.4/group__keys.html)
	switch (key) {
	case GLFW_KEY_ESCAPE:                     // Escape key
		glfwSetWindowShouldClose(mWindow, true); // stops main loop
		break;
	case GLFW_KEY_RIGHT:
		if (mods == GLFW_MOD_CONTROL)
			mCamera->rollReal(1); // rotates -1 on the Z axis
		else
			mCamera->yawReal(1); // rotates -1 on the Y axis
		break;
	case GLFW_KEY_LEFT:
		if (mods == GLFW_MOD_CONTROL)
			mCamera->rollReal(-1); // rotates 1 on the Z axis
		else
			mCamera->yawReal(-1); // rotate 1 on the Y axis
		break;
	case GLFW_KEY_UP:
		mCamera->pitchReal(	1); // rotates 1 on the X axis
		break;
	case GLFW_KEY_DOWN:
		mCamera->pitchReal(-1); // rotates -1 on the X axis
		break;
	default:
		need_redisplay = false;
		break;
	} // switch

	if (need_redisplay)
		mNeedsRedisplay = true;
}

void IG1App::mouse(int button, int action, int mods)
{
	//Mira hay teclas pulsadas
	//Si no, el boton default es uno que no tiene accion
	if (action == 0) { mMouseButt = 2; }
	//Y si hay tecla pulsada, pues la que sea
	else { mMouseButt = button;}

	// Guarda en mCoord la posicion (x, y) del raton
	glfwGetCursorPos(mWindow, &mMouseCoord.x, &mMouseCoord.y);

	// Guarda el modificador de Control
	if (mods == GLFW_MOD_CONTROL) { ctrlOn = true; }
	else ctrlOn = false;
}

void IG1App::motion(double x, double y)
{
	Camera* actualCam = mCamera;
	//Si esta el modo en 2 escenas, y el raton esta en la segunda mitad
	if (m2Scenes && mMouseCoord.x > mWinW / 2) { actualCam = mCamera2; }

	// Guarda en unavariable auxiliar mp la diferencia entre mCoord y(x, y)
	glm::dvec2 mp = { mMouseCoord.x - x, mMouseCoord.y - y };

	// Guarda en mCoord la posición (x, y) del ratón
	mMouseCoord.x = x; 
	mMouseCoord.y = y;

	// Si mBot es el botón izquierdo, la cámara orbita (mp.x * 0.05, mp.y)
	if (mMouseButt == 0) {
		actualCam->orbit(mp.x * 0.05, mp.y);
	}
	else if (mMouseButt == 1) {
		actualCam->moveUD(mp.y);
		actualCam->moveLR(mp.x);
	}
	mNeedsRedisplay = true;
}

void IG1App::mouseWheel(double dx, double dy)
{
	Camera* actualCam = mCamera;
	//Si esta el modo en 2 escenas, y el raton esta en la segunda mitad
	if (m2Scenes && mMouseCoord.x > mWinW / 2) { actualCam = mCamera2; }

	// si se pulsa ctrl
	if (ctrlOn) {
		// escala la escena, de nuevo según el valor de d
		actualCam->setScale(dy);
	}
	// si no está pulsada ninguna tecla modificadora.
	else {
		// desplaza la cámara en su dirección de vista.
		actualCam->moveFB(dy);
	}
	
	mNeedsRedisplay = true;
}

void IG1App::captura()
{
	Texture tex;
	tex.loadColorBuffer(800.0, 600.0);
	tex.saveScreenshot("../capturas/cap.bmp");
}

void IG1App::display2V() const
{
	// para renderizar las vistas utilizamos una camara auxiliar:
	Camera auxCam = *mCamera; // copiando mCamera
	// el puerto de vista queda compartido (se copia el puntero)
	//lo copiamos en una var.aux
	Viewport auxVP = *mViewPort;
	// el tamaño de los 2 puertos de vista es el mismo,
	// lo configuramos
	mViewPort->setSize(mWinW / 2, mWinH);
	// igual que en resize, para que no cambie la escala,
	// tenemos que cambiar el tamaño de la ventana de vista de la cámara
	auxCam.setSize(mViewPort->width(), mViewPort->height());

	// vista 3D ->
	// configurar la posición
	mViewPort->setPos(0, 0);
	// cambiar la posición y orientacion de la cámara
	auxCam.set3D();
	// renderizamos con la cámara y el puerto de vista configurados
	mScenes[mCurrentScene]->render(auxCam);

	// vista Cenital ->
	// configurar la posición
	mViewPort->setPos(mWinW / 2, 0);
	// cambiar la posición y orientacion de la cámara
	auxCam.setCenital();
	// renderizamos con la cámara y el puerto de vista configurados
	mScenes[mCurrentScene]->render(auxCam);

	*mViewPort = auxVP; // * restaurar el puerto de vista 
}

void IG1App::display2S(Scene* s1, Scene* s2) const
{
	// para renderizar las vistas utilizamos dos camaras
	Camera auxCam = *mCamera;
	Camera auxCam2 = *mCamera2;

	// el puerto de vista queda compartido (se copia el puntero)
	Viewport auxVP = *mViewPort;

	// el tamaño de los 2 puertos de vista es el mismo, lo configuramos
	mViewPort->setSize(mWinW / 2, mWinH);

	// Escena 4 ->
	// igual que en resize, para que no cambie la escala, tenemos que cambiar el tamaño de la ventana de vista de la cámara
	auxCam.setSize(mViewPort->width(), mViewPort->height());
	// configurar la posición
	mViewPort->setPos(0, 0);
	// renderizamos con la cámara y el puerto de vista configurados
	s1->render(auxCam);

	// Escena 2 ->
	// igual que en resize, para que no cambie la escala, tenemos que cambiar el tamaño de la ventana de vista de la cámara
	auxCam2.setSize(mViewPort->width(), mViewPort->height());
	// configurar la posición
	mViewPort->setPos(mWinW / 2, 0);
	// renderizamos con la cámara y el puerto de vista configurados
	s2->render(auxCam2);

	*mViewPort = auxVP; // * restaurar el puerto de vista 
}

bool
IG1App::changeScene(size_t sceneNr)
{
	// Check whether the scene exists
	if (sceneNr >= mScenes.size())
		return false;

	// Change only if a different scene
	if (sceneNr != mCurrentScene) {
		mScenes[mCurrentScene]->unload(); // descarga cosas internas
		mCurrentScene = sceneNr;
		mScenes[mCurrentScene]->load(); // carga cosas internas
		mNeedsRedisplay = true;
	}

	return true;
}
