#ifndef _H_Entities_H_
#define _H_Entities_H_

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Mesh.h"
#include "IndexMesh.h"
#include "Light.h"
#include "Material.h"
#include "Shader.h"
#include "Texture.h"

// ---------- ENTIDADES ----------

class Abs_Entity // abstract class
{
public:
	Abs_Entity()
	  : mModelMat(1.0)  // 4x4 identity matrix
	  , mShader(nullptr)
	  , mTexture(nullptr)
	  , mMaterial()
	{}

	virtual ~Abs_Entity();

	Abs_Entity(const Abs_Entity& e) = delete;            // no copy constructor
	Abs_Entity& operator=(const Abs_Entity& e) = delete; // no copy assignment

	virtual void render(const glm::dmat4& modelViewMat) const = 0; // abstract method
	virtual void update();

	// modeling matrix
	glm::dmat4 const& modelMat() const { return mModelMat; }
	void setModelMat(glm::dmat4 const& aMat) { mModelMat = aMat; }

	// load or unload entity data into the GPU
	virtual void load();
	virtual void unload();

protected:
	Mesh* mMesh = nullptr;		 // the mesh
	glm::dmat4 mModelMat;		 // modeling matrix
	Shader* mShader;			 // shader
	Material mMaterial;			 // material
	Texture* mTexture;			 // texture

	// transfers modelViewMat to the GPU
	virtual void upload(const glm::mat4& mModelViewMat) const;
};

class EntityWithColors : public Abs_Entity
{
public:
	explicit EntityWithColors();
	void render(const glm::dmat4& modelViewMat) const override;
};

class SingleColorEntity : public Abs_Entity // clase padre que hereda de abs entity (entidad con color single).
{
public:
	explicit SingleColorEntity(const glm::vec4& color = glm::vec4(1)); // explicit es para que no se pueda hacer una conversion implicita. Ej.: (int)algo.
	void render(const glm::dmat4& modelViewMat) const override;

	glm::vec4 getColor() const { return mColor; }
	void setColor(glm::vec4 const &c) { mColor = c; } // & para q no se copie y const porque no se modifica dentro.

protected:
	glm::vec4 mColor; // predefinido.
};

class EntityWithTexture : public Abs_Entity
{
public:
	explicit EntityWithTexture(GLboolean modulate = false, GLboolean alfaActive = false);
	virtual void render(const glm::dmat4& modelViewMat) const override;

	// getter y setter
	Texture* const& getTexture() const { return mTexture; }
	void setTexture(Texture* tex) { mTexture = tex; }

protected:
	bool mModulate;
	GLboolean mAlfaActive;
};

class EntityWithMaterial : public Abs_Entity
{
public:
	//explicit EntityWithMaterial();
	void render(const glm::dmat4& modelViewMat) const override;

	// getter y setter
	Material getMaterial() const { return mMaterial; }
	void setMaterial(Material& mat) { mMaterial = mat; }

	//static void toggleShowNormals();

protected:
	Material material;
	//inline static bool mShowNormals = false;

};

class ColorMaterialEntity : public EntityWithMaterial
{
public:
	explicit ColorMaterialEntity();
	//void render(const glm::dmat4& modelViewMat) const override;

	//getter y setter de color
	glm::vec4 getColor() const { return mColor; }
	void setColor(glm::vec4 const& c) { mColor = c; } // & para q no se copie y const porque no se modifica dentro.

protected:
	glm::vec4 mColor; 
};

class CompoundEntity : public Abs_Entity
{
public:
	virtual ~CompoundEntity();
	void addEntity(Abs_Entity* ae);

	void render(const glm::dmat4& modelViewMat) const override;
	void update() override;
	void load() override;
	void unload() override;

protected:
	std::vector<Abs_Entity*> gObjects;
};

// -----------------

class Cube: public SingleColorEntity
{
public:
	explicit Cube(GLdouble length);
};

class RGBCube : public EntityWithColors
{
public:
	explicit RGBCube(GLdouble length, int s);
	void render(const glm::dmat4& modelViewMat) const override;
	void update() override;

private:
	int scene = 0;
	double angle = 0.0;
	GLdouble l; // length
	int axisState = 0; // comienza en el EJE X.

	// ESTE ES ORDEN QUE TIENE QUE SEGUIR LA ANIMACIÓN.
	// 0 -> eje X.
	// 1 -> eje Z.
	// 2 -> eje Y.
	void rotateOnAxis(GLint n);
};

class RegularPolygon : public SingleColorEntity // clase hija que hereda SingleColorEntity con sus propias cosas de poligono regular.
{
public:
	explicit RegularPolygon(GLuint num, GLdouble r);
};

class RGBTriangle : public EntityWithColors
{
public:
	explicit RGBTriangle(int s);
	void render(const glm::dmat4& modelViewMat) const override;
	void update() override;
private:
	int scene = 0;
	double angle = 0.0;
};

class RGBRectangle : public EntityWithColors
{
public:
	explicit RGBRectangle(GLdouble w, GLdouble h);
	void render(const glm::dmat4& modelViewMat) const override;
};

class RGBAxes : public EntityWithColors
{
public:
	explicit RGBAxes(GLdouble l);
};

class Ground : public EntityWithTexture
{
public:
	explicit Ground(GLdouble w, GLdouble h, GLboolean modulate);
};

class BoxOutline : public EntityWithTexture
{
public:
	explicit BoxOutline(GLdouble length, GLboolean modulate);
	void render(const glm::dmat4& modelViewMat) const override;

	void setTextureInterior(Texture* tex) { mTextureInterior = tex; }

protected:
	Texture* mTextureInterior = nullptr; // texture
};

class Box : public EntityWithTexture
{
public:
	explicit Box(GLdouble length, GLboolean modulate, int s);
	~Box() override;
	void render(const glm::dmat4& modelViewMat) const override;
	void update() override;
	void setTextureInterior(Texture* tex) { mTextureInterior = tex; }
	void load() override;
	void unload() override;
	void setModelMatAbj(glm::dmat4 const& bMat) { mModelMatAbj = bMat; }
	void setModelMatArr(glm::dmat4 const& cMat) { mModelMatArr = cMat; }

protected:
	Texture* mTextureInterior = nullptr; // texture

	GLdouble _length; // length de la box.

	// --- tapa abajo
	Mesh* mMeshTapaAbj = nullptr;
	glm::dmat4 mModelMatAbj = glm::dmat4(1.0);

	// --- tapa arriba
	Mesh* mMeshTapaArr = nullptr;
	glm::dmat4 mModelMatArr = glm::dmat4(1.0);

private:
	// --- para el update.
	int scene = 0;
	double angle = 180.0;  // empiza en 180 grados (empieza abierta)
	GLint openState = 1.0; // 0 -> abres 1 -> cierras
};

class Star3D : public EntityWithTexture
{
public:
	explicit Star3D(GLdouble re, GLuint np, GLdouble h, int s, GLboolean modulate);
	void render(const glm::dmat4& modelViewMat) const override;
	void update() override;

private:
	int scene = 0;
	double angle = 0.0;
};

class GlassParapet : public EntityWithTexture 
{
public:
	explicit GlassParapet(GLdouble length, GLboolean modulate);
	void render(const glm::dmat4& modelViewMat) const override;
};

class Grass : public EntityWithTexture 
{
public:
	explicit Grass(GLdouble l, GLboolean modulate);
	void render(const glm::dmat4& modelViewMat) const override;

private:
	glm::dmat4 createGrassPos(GLdouble rot);

protected:
	GLdouble length;

};

class Photo : public EntityWithTexture 
{
public:
	explicit Photo(GLdouble w, GLdouble h, GLboolean modulate);
	void render(const glm::dmat4& modelViewMat) const override;
	void update() override;
};

//Apartado 56
class Torus : public SingleColorEntity
{
public:
	/*
	R - radio desde origen de coordenadas
	r - radio del perfil
	nPoints - vertices de la circunferencia del perfil
	nSamples - numero de repeticiones del perfil en la figura final
	*/
	explicit Torus(GLdouble R, GLdouble r, GLuint nPoints = 40, GLuint nSamples = 40);
	void render(const glm::dmat4& modelViewMat) const override;
};

//Apartado 60
class IndexedBox : public ColorMaterialEntity
{
public:
	explicit IndexedBox(GLdouble l);
	void render(const glm::dmat4& modelViewMat) const override;
};

//Apartado 64
class Sphere : public ColorMaterialEntity 
{
public:
	explicit Sphere(GLdouble radius, GLuint nParallels, GLuint nMeridians);
	void render(const glm::dmat4& modelViewMat) const override;
};

class Disk : public ColorMaterialEntity 
{
public:
	explicit Disk(GLdouble R, GLdouble r, GLuint nRings, GLuint nSamples);
	void render(const glm::dmat4& modelViewMat) const override;
};

//Apartado 69
class PartialDisk : public ColorMaterialEntity 
{
public:
	explicit PartialDisk(GLdouble R, GLdouble r, GLuint nRings, GLuint nSamples, GLfloat maxAngle);
	void render(const glm::dmat4& modelViewMat) const override;
};

class Cone : public ColorMaterialEntity 
{
	/*
		· cono(r = 0 o R = 0)
		· cilindro(R = r)
		· tronco de cono de altura h, radio inferior r, radio superior R, rRings vértices en el perfil y nSamples muestras de revolución.
	*/
public:
	explicit Cone(GLdouble h, GLdouble r, GLdouble R, GLuint nRings, GLuint nSamples);
	void render(const glm::dmat4& modelViewMat) const override;
};

class WingAdvancedTIE : public EntityWithTexture
{
public:
	explicit WingAdvancedTIE(GLdouble w, GLdouble h, GLboolean modulate, Texture* tex);
	void render(const glm::dmat4& modelViewMat) const override;
};

class NoseAdvancedTIE : public CompoundEntity
{
public:
	explicit NoseAdvancedTIE();
};

class AdvancedTIE: public CompoundEntity
{
public:
	explicit AdvancedTIE(Texture* wingsTex, GLboolean alfaActive = false);

	// metodos de tiefighter starwars
	void rotate();
	void orbit();

	//SpotLight* getTieLight() { return tieLight; }
private:
	bool mModulate;
	GLboolean mAlfaActive;

	//Para el movimiento cuando hay planeta
	double _advancedTieAngle = 1.0;
	float _advancedTieMovement = 1.0;

	//Luz Focal
	//SpotLight* tieLight;
};

class Farmer : public CompoundEntity
{
public:
	explicit Farmer();

	// metodos de farmer
	void rotate();
	void orbitAH();
	void orbitH();
private:

	//Para el movimiento cuando hay planeta
	double _farmerAngle = 180.0;
	float _farmerMovement = 1.0;
	bool _isAntiHorario = false;
};

#endif //_H_Entities_H_