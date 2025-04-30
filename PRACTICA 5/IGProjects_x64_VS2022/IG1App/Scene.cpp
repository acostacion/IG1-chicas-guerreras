#include "Scene.h"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

void Scene::init()
{;
	setGL(); // OpenGL settings

	// allocate memory and load resources
	// Lights
	//Luz direccional (con id = 0 default)
	DirLight* dirLight = new DirLight();
	//valores de simple_light
	dirLight->setAmb(vec3(.25, .25, .25));
	dirLight->setDiff(vec3(.6, .6, .6));
	dirLight->setSpec(vec3(0, 0.2, 0));
	//se anade al array
	gLights.emplace_back(dirLight);


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

	for (Light* l : gLights)
		delete l;

	gLights.clear();
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

	//for(Light* obj : gLights)
		//obj->un
}

void Scene::destroyScene()
{
	destroy();
}

CompoundEntity* Scene::createAdvancedTie()
{
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
	WATIE->setModelMat(translate(glm::dmat4(1), glm::dvec3(0, 0, -wingsDistance / 2))); // se mueve la wingdistance para colocarla
	AdvancedTIE->addEntity(WATIE);

	// ----- ALA 2 -----
	WingAdvancedTIE* WATIE2 = new WingAdvancedTIE(200, 100, false);		// entidad
	WATIE2->setTexture(texNoche);												// establece la textura de esta entidad
	WATIE2->setModelMat(
		translate(glm::dmat4(1), glm::dvec3(0, 0, wingsDistance / 2)) // se mueve la wingdistance para colocarla
		* glm::rotate(dmat4(1), radians(180.0), dvec3(0.0, 1.0, 0.0))); 	// se rota 180 para girarla
	AdvancedTIE->addEntity(WATIE2);

	// ----- CILINDRO DE ALA1 A ALA2 -----
	Cone* cilinder = new Cone(wingsDistance, 20, 20, 20, 20);
	cilinder->setColor(glm::vec4(0.0f, 0.25f, 0.42f, 1.0f)); //anil
	cilinder->setModelMat(glm::rotate(dmat4(1), radians(90.0), dvec3(1.0, 0.0, 0.0)));
	AdvancedTIE->addEntity(cilinder);

	// ----- NUCLEO CENTRAL -----
	Sphere* nucleus = new Sphere(wingsDistance / 2 - 75, 500, 500);
	nucleus->setColor(glm::vec4(0.0f, 0.25f, 0.42f, 1.0f)); //anil
	AdvancedTIE->addEntity(nucleus);

	// TODO: da errores al meter una CompoundEntity dentro de otra. Pasa tambien con el nodo ficticio de Tatooine
	// ----- MORRO -----
	// Morro es una entidad compuesta por cilinder y disk
	CompoundEntity* nose = new CompoundEntity();

	// ----- CILINDER -----
	Cone* noseCilinder = new Cone(50, 20, 20, 20, 20);
	noseCilinder->setColor(glm::vec4(0.0f, 0.25f, 0.42f, 1.0f)); //anil
	noseCilinder->setModelMat(
		translate(glm::dmat4(1), glm::dvec3(150.0, 0.0, 0.0))
		* glm::rotate(dmat4(1), radians(90.0), dvec3(0.0, 0.0, 1.0)));
	nose->addEntity(noseCilinder);

	// ----- DISK -----
	Disk* noseDisk = new Disk(20, 0, 10, 40);
	noseDisk->setColor(glm::vec4(0.0f, 0.25f, 0.42f, 1.0f)); //anil
	noseDisk->setModelMat(
		translate(glm::dmat4(1), glm::dvec3(175, 0, 0))
		* glm::rotate(dmat4(1), radians(90.0), dvec3(0.0, 0.0, 1.0)));
	nose->addEntity(noseDisk);

	//Aniadimos la entidad morro a la general del TIE
	AdvancedTIE->addEntity(nose);

	return AdvancedTIE;
}

CompoundEntity* Scene::createFarmer()
{
	// ----- FARMER -----
	CompoundEntity* farmer = new CompoundEntity();
	gObjects.push_back(farmer);

	// ----- Cabeza -----
	Sphere* cabeza = new Sphere(300, 500, 500);
	cabeza->setColor(glm::vec4(1.0f, 0.55f, 0.0f, 1.0f)); //orange (de la tabla)
	farmer->addEntity(cabeza);

	// ----- Boca -----
	PartialDisk* boca = new PartialDisk(230, 30, 5, 50, glm::radians(180.0));
	//boca->setColor(glm::vec4(0.0f, 255.0, 0.0, 255.0)); //green sin shader
	boca->setColor(glm::vec4(0.0f, 1.0, 0.0, 1.0)); //green
	boca->setModelMat(
		glm::translate(glm::dmat4(1), dvec3(0.0, 0.0, 190.0))
		* glm::rotate(glm::dmat4(1), radians(-90.0), dvec3(1.0, 0.0, 0.0))
	);
	farmer->addEntity(boca);

	// ----- Sombrero -----
	Disk* sombrero = new Disk(400, 0, 5, 50);
	//sombrero->setColor(glm::vec4(255.0, 0.0, 0.0, 255.0)); //red sin shader
	sombrero->setColor(glm::vec4(1.0, 0.0, 0.0, 1.0)); //red
	sombrero->setModelMat(
		glm::translate(glm::dmat4(1), dvec3(0.0, 200.0, 0.0))
	);
	farmer->addEntity(sombrero);

	// ----- Ojo derecho -----
	Cone* ojoDer = new Cone(70, 5, 40, 5, 50);
	ojoDer->setColor(glm::vec4(0.3, 0.35, 0.4,1.0)); //gris marengo, (76, 88, 102)
	ojoDer->setModelMat(
		glm::translate(glm::dmat4(1), dvec3(-70.0, 100.0, 300.0))
		* glm::rotate(glm::dmat4(1), radians(-90.0), dvec3(1.0, 0.0, 0.0))
	);
	farmer->addEntity(ojoDer);

	// ----- Ojo izquierdo -----
	Cone* ojoIzq = new Cone(70, 5, 30, 5, 50);
	ojoIzq->setColor(glm::vec4(0.0, 0.0, 0.5, 1.0)); //azul marino, (0, 0, 128)
	ojoIzq->setModelMat(
		glm::translate(glm::dmat4(1), dvec3(70.0, 100.0, 300.0))
		* glm::rotate(glm::dmat4(1), radians(-90.0), dvec3(1.0, 0.0, 0.0))
	);
	farmer->addEntity(ojoIzq);

	return farmer;
}

void Scene::initialAdvandedTieConfig()
{
	//Colocamos el tie en la posicion del nodo con la escala
	_advancedTie->setModelMat(
		scale(_advancedTieInTatooine->modelMat(), glm::dvec3(0.15, 0.15, 0.15)) // su escala, porque si no al rotar se hace grande.
		* translate(glm::dmat4(1), glm::dvec3(0.0, 1200.0, 0.0)) // se coloca en el polo norte, porque si no avanza dentro del planeta.
		// nota: el translate lo hacemos desde el origen (0,0,0) para que se desplace esos 1200 desde el origen en si, no desde la pos del origen del nodo!!
	);
}

void Scene::rotate()
{
	// colocacion del advancedtie en la escena.
	initialAdvandedTieConfig();

	//Rotamos el nodo ficticio
	_advancedTieInTatooine->setModelMat(
		glm::rotate(_advancedTieInTatooine->modelMat(), radians(_advancedTie->_advancedTieAngle), dvec3(0.0, 1.0, 0.0))
	);
	//No hace falta actualizar el angulo porque en nodo ficticio no es igual que antes.
}

void Scene::orbit()
{
	// colocacion del advancedtie en la escena.
	initialAdvandedTieConfig();

	// movemos hacia adelante el nodo ficticio con la rotacion alrededor del planeta.
	_advancedTieInTatooine->setModelMat(
		glm::translate(_advancedTieInTatooine->modelMat(), dvec3(_advancedTie->_advancedTieMovement, 0.0, 0.0)) // avanza tantos pasos
		* glm::rotate(glm::dmat4(1), radians(4.0), dvec3(0.0, 0.0, -1.0)) // rota alrededor del planeta en la direccion del morro
	);
}

void
Scene::setGL()
{
	setBackgroundColor();	//elige el color que se le pase en cada escena, es color base es azul
	glEnable(GL_DEPTH_TEST);							  // enable Depth test -> inicialzacion

	// BLENDING
	glEnable(GL_BLEND);
}
void Scene::setBackgroundColor()
{
	// OpenGL basic setting
	glClearColor(0.6, 0.7, 0.8, 1.0); // background color (alpha = 1 -> opaque)
}
void
Scene::resetGL()
{
	glClearColor(0.0, 0.0, 0.0, 0.0); // background color (alpha = 1 -> opaque)
	glDisable(GL_DEPTH_TEST);					  // disable Depth test

	// BLENDING
	glDisable(GL_BLEND);
}

void Scene::uploadLights(Camera const& cam) const
{
	for (Light* l : gLights) {
		// se pasa el shader "light" y el modelviewmat de la camera.
		Shader s = Shader("light");
		l->upload(s, cam.viewMat());
	}
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

	uploadLights(cam);

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
	torus->setModelMat (glm::rotate(glm::dmat4(1), radians(-180.0), dvec3(0.0, 1.0, 0.0)));
	gObjects.push_back(torus);
}

// ---- SCENE 5 ----
void Scene5::init()
{
	// -- llama a init del padre
	Scene::init();

	// ----- INDEXEDBOX -----
	IndexedBox* ibox = new IndexedBox(200);
	gObjects.push_back(ibox);

}

// ---- SCENE 6 ----
void Scene6::init()
{
	// -- en lugar de llamar al init del padre hacemos:
	setGL();
	gObjects.push_back(new RGBAxes(400.0)); // EJES XYZ.

	createAdvancedTie();
}

void Scene6::setBackgroundColor()
{
	glClearColor(0.0, 0.0, 0.0, 0.0); // background color (alpha = 1 -> opaque)
}

// ---- SCENE 7 ----
void Scene7::init()
{
	// -- en lugar de llamar al init del padre hacemos:
	setGL(); 
	gObjects.push_back(new RGBAxes(400.0)); // EJES XYZ.

	// ----- TATOOINE -----
	Sphere* tatooine = new Sphere(150, 500, 500);
	tatooine->setColor(glm::vec4(1.0f, 0.91f, 0.0f, 1.0f)); //amarillo
	gObjects.push_back(tatooine);

	// ----- ADVANCED TIE -----
	_advancedTie = createAdvancedTie();

	//AdvancedTIE esta en el polo norte del planeta y con menos tamano
	_advancedTie->setModelMat(scale(glm::dmat4(1), glm::dvec3(0.15, 0.15, 0.15))
	*	translate(glm::dmat4(1), glm::dvec3(0.0, 1200.0, 0.0)));

	// nodo ficticio.
	_advancedTieInTatooine = new CompoundEntity();

	_advancedTieInTatooine->addEntity(_advancedTie);

}

void Scene7::setBackgroundColor()
{
	glClearColor(0.0, 0.0, 0.0, 0.0); // background color (alpha = 1 -> opaque)
}

// ---- SCENE 8 ----
void Scene8::init()
{
	// -- llama a init del padre
	Scene::init();

	// Creamos la entidad compuesta farmer
	_farmer = createFarmer();
	_farmer->setModelMat(
		scale(glm::dmat4(1), glm::dvec3(0.5, 0.5, 0.5))
		* glm::rotate(glm::dmat4(1), radians(45.0), dvec3(0.0, 1.0, 1.0))
	);
}

void Scene9::init()
{
	// -- llama a init del padre
	Scene::init();

	// ----- TATOOINE -----
	Sphere* tatooine = new Sphere(100, 500, 500);
	tatooine->setColor(glm::vec4(1.0f, 0.91f, 0.0f, 1.0f)); //amarillo
	tatooine->setModelMat(translate(glm::dmat4(1), glm::dvec3(250, 0, 0)));
	gObjects.push_back(tatooine);

	// ----- GOLDEN TATOOINE -----
	Sphere* tatooineG = new Sphere(100, 500, 500);
	tatooineG->setColor(glm::vec4(1.0f, 0.91f, 0.0f, 1.0f)); //amarillo
	tatooineG->setModelMat(translate(glm::dmat4(1), glm::dvec3(0, 0, 250)));
	gObjects.push_back(tatooineG);


}
