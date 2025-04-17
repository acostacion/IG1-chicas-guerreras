#include "Scene.h"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

void Scene::init()
{
	setGL(); // OpenGL settings

	// allocate memory and load resources
	// Lights
	// Textures

	// Graphics objects (entities) of the scene
	gObjects.push_back(new RGBAxes(400.0)); // EJES XYZ.
}

void Scene::update()
{
	for (Abs_Entity* obj : gObjects)
		obj->update();

	for (Abs_Entity* obj : gObjectsTrans)
		obj->update();
}

Scene::~Scene()
{
	destroy();
	resetGL();
}

void
Scene::destroy()
{ // release memory and resources

	for (Abs_Entity* el : gObjects)
		delete el;

	gObjects.clear();

	for (Abs_Entity* el : gObjectsTrans)
		delete el;

	gObjectsTrans.clear();

	for (Texture* t : gTextures)
		delete t;

	gTextures.clear();
}

void
Scene::load()
{
	for (Abs_Entity* obj : gObjects)
		obj->load();

	for (Abs_Entity* obj : gObjectsTrans)
		obj->load();
}

void
Scene::unload()
{
	for (Abs_Entity* obj : gObjects)
		obj->unload();

	for (Abs_Entity* obj : gObjectsTrans)
		obj->unload();
}

void Scene::destroyScene()
{
	destroy();
}

void
Scene::setGL()
{
	// OpenGL basic setting
	glClearColor(0.6, 0.7, 0.8, 1.0); // background color (alpha = 1 -> opaque)
	glEnable(GL_DEPTH_TEST);							  // enable Depth test -> inicialzacion

	// BLENDING
	glEnable(GL_BLEND);
}
void
Scene::resetGL()
{
	glClearColor(.0, .0, .0, .0); // background color (alpha = 1 -> opaque)
	glDisable(GL_DEPTH_TEST);					  // disable Depth test

	// BLENDING
	glDisable(GL_BLEND);
}

// Para borrar las cosas al cambiar de una escena a otra (ponerla en blanco otra vez).
void Scene::reset()
{
	destroy(); // libera memoria.
	resetGL(); // resetea OpenGL.
	init();
}

void
Scene::render(Camera const& cam) const
{
	cam.upload();

	// opacos       -> primero objetos sin transparencia
	for (Abs_Entity* el : gObjects)
		el->render(cam.viewMat());

	// --- blending objetos translucidos
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // activa blend func antes de renderizar objetos translucidos
	glDepthMask(GL_FALSE);							   // 

	// translucidos -> despues objetos con transparencia
	for (Abs_Entity* el : gObjectsTrans) 
		el->render(cam.viewMat());

	glDepthMask(GL_TRUE);
	// ---
}

// --- ESCENAS HIJAS ---
// Scene0, Scene1, Scene2 ... 

// ---- SCENE 0 ----
void Scene0::init()
{
	Scene::init();

	// Triangulo cian.
	RegularPolygon *triang = new RegularPolygon(3, 200);
	triang->setColor({ 0,1,1,1 });
	gObjects.push_back(triang);

	// Circulo magenta.
	RegularPolygon *circ = new RegularPolygon(40, 200);
	circ->setColor({1,0,1,1});
	gObjects.push_back(circ);
}

// ---- SCENE 1 ----
void Scene1::init()
{
	// -- llama a init del padre
	Scene::init();

	// -- crea y mete los objetos en la escena
	RGBTriangle* triangulo = new RGBTriangle(1);

	// Triangulo RGB
	gObjects.push_back(triangulo);

	// Rectangulo RGB
	gObjects.push_back(new RGBRectangle(200, 100));
	// Circulo blanco.

	GLdouble r = 100.0;
	gObjects.push_back(new RegularPolygon(40, r));

	// -- traslaciones, escalas y rotaciones
	triangulo->setModelMat(translate(glm::dmat4(1), glm::dvec3(r, 0, 0)));

}

// ---- SCENE 2 ----
void Scene2::init()
{
	// -- llama a init del padre
	Scene::init();

	double length = 200;
	RGBCube* cube = new RGBCube(length, 2);

	gObjects.push_back(cube);
}

// ---- SCENE 3 ----
void Scene3::init()
{
	// -- llama a init del padre
	Scene::init();

	// ----- SUELO -----
	// --- texturas
	// creamos y cargamos (con load()) las texturas de los objetos de la escena
	Texture* texB = new Texture();								// crea nueva textura
	const std::string bal = "../assets/images/baldosaC.png";	// ruta de la textura
	texB->load(bal, 255);									// carga la textura con su alfa
	gTextures.push_back(texB);									// lo metemos en el vector de texturas de la escena para poder eliminarla luego
	
	// --- entidad
	Ground* ground = new Ground(200.0, 200.0, false);
	ground->setTexture(texB);	// establece la textura de esta entidad
	gObjects.push_back(ground); // mete la entidad en la escena

	// ----- CAJA SIN TAPAS -----
	// --- texturas
	// creamos y cargamos (con load()) las texturas de los objetos de la escena
	// ----> textura por fuera <----
	Texture* texC = new Texture();								// crea nueva textura
	const std::string con = "../assets/images/container.jpg";	// ruta de la textura
	texC->load(con, 255);									// carga la textura con su alfa
	gTextures.push_back(texC);									// lo metemos en el vector de texturas de la escena para poder eliminarla luego
	// ----> textura por dentro <---
	Texture* texP = new Texture();								// crea nueva textura
	const std::string pap = "../assets/images/papelE.png";		// ruta de la textura
	texP->load(pap, 255);									// carga la textura con su alfa
	gTextures.push_back(texP);									// lo metemos en el vector de texturas de la escena para poder eliminarla luego
	// --- entidad
	BoxOutline* bo = new BoxOutline(30, false);
	bo->setTexture(texC);	// establece la textura de esta entidad
	bo->setTextureInterior(texP); // textura para el interior
	// --- traslaciones, escalas y rotaciones
	bo->setModelMat(translate(glm::dmat4(1), glm::dvec3(80, 30 / 2, 80)));
	gObjects.push_back(bo); // mete la entidad en la escena

	// ----- ESTRELLA -----
	// --- texturas
	// creamos y cargamos (con load()) las texturas de los objetos de la escena
	Texture* texD = new Texture();								// crea nueva textura
	const std::string bp = "../assets/images/baldosaP.png";		// ruta de la textura
	texD->load(bp, 255);									// carga la textura con su alfa
	gTextures.push_back(texD);									// lo metemos en el vector de texturas de la escena para poder eliminarla luego
	// --- entidad
	Star3D* estrella = new Star3D(15.0, 8.0, 15.0, 3, false);
	estrella->setTexture(texD);	// establece la textura de esta entidad
	gObjects.push_back(estrella); // mete la entidad en la escena

	// ----- CRISTAL -----
	Texture* texG = new Texture();										// crea nueva textura
	const std::string win = "../assets/images/windowV.jpg";				// ruta de la textura
	texG ->load(win, 100);											// carga la textura con su alfa 255 opaco
	gTextures.push_back(texG);											// lo metemos en el vector de texturas 
	GlassParapet* gla = new GlassParapet(200, false);		// entidad
	gla->setTexture(texG);												// establece la textura de esta entidad
	gla->setModelMat(translate(glm::dmat4(1), glm::dvec3(0, 100, 0)));
	gObjectsTrans.push_back(gla);										// mete la entidad en la escena

	// ----- FOTO -----
	Texture* texF = new Texture();								// crea nueva textura
	texF->loadColorBuffer(800.0, 600.0);						
	gTextures.push_back(texF);									// lo metemos en el vector de texturas
	Photo* foto = new Photo(50.0, 50.0, false);		// entidad
	foto->setTexture(texF);									    // establece la textura de esta entidad
	gObjects.push_back(foto);									// mete la entidad en la escena
	
	// ----- CAJA CON TAPAS -----
	// --- texturas
	// creamos y cargamos (con load()) las texturas de los objetos de la escena
	// ----> textura por fuera <----
	Texture* texCT = new Texture();									// crea nueva textura
		const std::string conT = "../assets/images/container.jpg";	// ruta de la textura
		texCT->load(conT, 255);								// carga la textura con su alfa
	gTextures.push_back(texCT);										// al vector de texturas de la escena
	// ----> textura por dentro <---
	Texture* texPT = new Texture();									// crea nueva textura
		const std::string papT = "../assets/images/papelE.png";		// ruta de la textura
		texPT->load(papT, 255);								// carga la textura con su alfa
	gTextures.push_back(texPT);										// sl vector de texturas de la escena

	// --- entidad
	Box* boT = new Box(30.0, false, 3);
	boT->setTexture(texCT);	// establece la textura de esta entidad
	boT->setTextureInterior(texPT); // textura para el interior
	boT->setModelMat(translate(glm::dmat4(1), glm::dvec3(-80, 30 / 2, -80)));
	boT->setModelMatAbj(translate(glm::dmat4(1), glm::dvec3(-80, 30 / 2, -80)));
	gObjects.push_back(boT); // mete la entidad en la escena

	// ----- HIERBA -----
	Texture* texH = new Texture();								// crea nueva textura
	const std::string h = "../assets/images/grass_alpha.png";	// ruta de la textura
	texH->load(h, 255);									// carga la textura con su alfa 255 opaco
	gTextures.push_back(texH);									// lo metemos en el vector de texturas 
	Grass* hier = new Grass(40.0, false);				// entidad
	hier->setTexture(texH);										// establece la textura de esta entidad
	gObjects.push_back(hier);									// mete la entidad en la escena
}

// ---- SCENE 4 ----
void Scene4::init() {
	// -- llama a init del padre
	Scene::init();

	// ----- TORUS -----
	Torus* torus = new Torus(200, 50, 40, 40);
	gObjects.push_back(torus);
}

void Scene5::init()
{
	// -- llama a init del padre
	Scene::init();

	//// ----- INDEXEDBOX -----
	//IndexedBox* ibox = new IndexedBox(200);
	//gObjects.push_back(ibox);
	////Cube* c = new Cube(200);
	////gObjects.push_back(c);

}

void Scene6::init()
{
	// -- llama a init del padre
	Scene::init();

	//// ----- SPHERE -----
	//Sphere* sphere = new Sphere(200, 20, 20);
	//gObjects.push_back(sphere);

	// ----- DISK -----
	/*Disk* disk = new Disk(200, 0, 10, 40);
	gObjects.push_back(disk);*/

	// ----- CONE -----
	/*Cone* cone = new Cone(200, 50, 200, 2, 50);
	gObjects.push_back(cone);*/


	// ----- TIE FIGHTER -----
	CompoundEntity* AdvancedTIE = new CompoundEntity();
	gObjects.push_back(AdvancedTIE);

	// ----- cosas para ambas alas:
	Texture* texNoche = new Texture();										// crea nueva textura
	const std::string win = "../assets/images/noche.jpg";				// ruta de la textura
	texNoche->load(win, 200);											// carga la textura con su alfa 255 opaco
	gTextures.push_back(texNoche);

	// distancia entre ala y ala (ademas de la h del cilindro que las atraviesa)
	int wingsDistance = 400;
	
	// ----- ALA 1 -----
	WingAdvancedTIE* WATIE = new WingAdvancedTIE(200, 100, false);		// entidad
	WATIE->setTexture(texNoche);												// establece la textura de esta entidad
	WATIE->setModelMat(translate(glm::dmat4(1), glm::dvec3(0, 0, -wingsDistance/2))); // se mueve la wingdistance para colocarla
	AdvancedTIE->addEntity(WATIE);

	// ----- ALA 2 -----
	WingAdvancedTIE* WATIE2 = new WingAdvancedTIE(200, 100, false);		// entidad
	WATIE2->setTexture(texNoche);												// establece la textura de esta entidad
	WATIE2->setModelMat(
		rotate(dmat4(1), radians(180.0), dvec3(0.0, 1.0, 0.0)) // se rota 180 para girarla
		* translate(glm::dmat4(1), glm::dvec3(0, 0, -wingsDistance / 2)) // se mueve la wingdistance para colocarla
	);
	AdvancedTIE->addEntity(WATIE2);

	// ----- CILINDRO DE ALA1 A ALA2 -----
	Cone* cilinder = new Cone(wingsDistance, 10, 10, 20, 20);
	cilinder->setModelMat(rotate(dmat4(1), radians(90.0), dvec3(1.0, 0.0, 0.0)));
	AdvancedTIE->addEntity(cilinder);
}
