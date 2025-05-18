#ifndef _H_Scene_H_
#define _H_Scene_H_

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Camera.h"
#include "Light.h"
#include "Entity.h"

#include <vector>


class Scene
{
public:
	Scene() = default;
	virtual ~Scene(); // los hijos al no tener destructora definida van a llamar directamente a la del padre

	Scene(const Scene& s) = delete;            // no copy constructor
	Scene& operator=(const Scene& s) = delete; // no copy assignment

	virtual void init();
	virtual void update();

	void render(Camera const& cam) const;

	// load or unload scene data into the GPU
	void load();
	void unload();
	void destroyScene();

	void reset();

	//Maneja input de escenas concretas
	virtual void handleKey(unsigned int key);

	//CompoundEntity* _advancedTie;

protected:
	void uploadLights(Camera const& cam) const;

	void destroy();
	void setGL();
	virtual void setBackgroundColor();
	void resetGL();

	std::vector<Abs_Entity*> gObjects;		// Entities (graphic objects) of the scene
	std::vector<Abs_Entity*> gObjectsTrans; // Entidades con transparencias
	std::vector<Texture*> gTextures;		// texturas de la escena
	std::vector<Light*> gLights;
};

// --- ESCENAS HIJAS ---
// Scene0, Scene1, Scene2 ...

class Scene0 : public Scene{
public:
	void init() override;
};

class Scene1 : public Scene{
public:
	void init() override;
};

class Scene2 : public Scene{
public:
	void init() override;
};

class Scene3 : public Scene{
public:
	void init() override;
};

class Scene4 : public Scene{
public:
	void init() override;
};

class Scene5 : public Scene{
public:
	void init() override;
};

class Scene6 : public Scene{
public:
	void init() override;
	void setBackgroundColor() override;
};

class Scene7 : public Scene{
public:
	void init() override;
	void setBackgroundColor() override;

	void handleKey(unsigned int key) override;

private:
	void rotateTie();
	void orbitTie();
};

class Scene8 : public Scene{
public:
	void init() override;
};

class Scene9 : public Scene{
public:
	void init() override;
};

#endif //_H_Scene_H_