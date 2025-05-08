#include "Material.h"

#include <glm/gtc/type_ptr.hpp>

using namespace glm;

Material::Material(vec3 color, float shininess)
 : ambient(color)
 , diffuse(color)
 , specular(color)
 , expF(shininess)
{
}

void
Material::upload(Shader& lighting) const
{
	lighting.setUniform("material.ambient", ambient);
	lighting.setUniform("material.diffuse", diffuse);
	lighting.setUniform("material.specular", specular);
	lighting.setUniform("material.shininess", expF);

	// glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	// can be simulated with a Boolean uniform and
	// gl_FrontFacing in the fragment shader
}

void
Material::setCopper()
{
	ambient = {0.19125, 0.0735, 0.0225};
	diffuse = {0.7038, 0.27048, 0.0828};
	specular = {0.256777, 0.137622, 0.086014};
	expF = 12.8;
}

void Material::setDarkBlue()
{
	// Nota: el color ahora lo lleva el material.
	// Si Amb == Dif == Spec == color. El material es de ese color y ya.
	glm::vec4 darkBlue(0.0, 0.25, 0.42, 1.0);
	ambient = darkBlue;
	diffuse = darkBlue;
	specular = darkBlue;
	expF = 0;
}

void Material::setGreen()
{
	// Nota: el color ahora lo lleva el material.
	// Si Amb == Dif == Spec == color. El material es de ese color y ya.
	glm::vec4 green(0.0, 1.0, 0.0, 1.0);
	ambient = green;
	diffuse = green;
	specular = green;
	expF = 0;
}

void Material::setOrange()
{
	// Nota: el color ahora lo lleva el material.
	// Si Amb == Dif == Spec == color. El material es de ese color y ya.
	glm::vec4 orange(1.0, 0.55, 0.0, 1.0);
	ambient = orange;
	diffuse = orange;
	specular = orange;
	expF = 0;
}

void Material::setRed()
{
	// Nota: el color ahora lo lleva el material.
	// Si Amb == Dif == Spec == color. El material es de ese color y ya.
	glm::vec4 red(1.0, 0.0, 0.0, 1.0);
	ambient = red;
	diffuse = red;
	specular = red;
	expF = 0;
}

void Material::setGrey()
{
	// Nota: el color ahora lo lleva el material.
	// Si Amb == Dif == Spec == color. El material es de ese color y ya.
	glm::vec4 grey(0.3, 0.35, 0.4, 1.0);
	ambient = grey;
	diffuse = grey;
	specular = grey;
	expF = 0;
}

void Material::setYellow()
{
	// Nota: el color ahora lo lleva el material.
	// Si Amb == Dif == Spec == color. El material es de ese color y ya.
	glm::vec4 yellow(1.0, 0.91, 0.0, 1.0);
	ambient = yellow;
	diffuse = yellow;
	specular = yellow;
	expF = 0;
}

void Material::setGolden()
{
	ambient = { 0.24725, 0.1995, 0.0745 };
	diffuse = { 0.75164, 0.60648, 0.22648 };
	specular = { 0.628281, 0.555802, 0.366065 };
	expF = 51.2;
}
