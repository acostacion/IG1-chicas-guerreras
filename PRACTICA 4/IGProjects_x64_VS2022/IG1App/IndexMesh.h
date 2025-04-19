#pragma once
#include "Mesh.h"
#include <numbers>

//Apartado 54
class IndexMesh : public Mesh
{

public:
	void load() override;
	void unload() override;

	//Apartado 55
	static IndexMesh* generateByRevolution(const std::vector<glm::vec2>& profile, GLuint nSamples,
		GLfloat angleMax = 2 * std::numbers::pi);

	// Apartado 59
	static IndexMesh* generateIndexedBox(GLdouble l);
protected:

	void draw() const override;
	std::vector<GLuint> vIndexes;


private:
	GLuint mIBO;  // vertex object buffer
};

