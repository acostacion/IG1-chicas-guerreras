#ifndef _H_Light_H
#define _H_Light_H

#include <string>

#include "Shader.h"
#include <glm/glm.hpp>

class Light { // Abstract class
public:
	virtual ~Light() = default;

	// Enciende o apaga la luz
	bool enabled() const { return bEnabled; }
	void setEnabled(bool enabled) { bEnabled = enabled; }

	// Carga sus atributos en la GPU
	virtual void upload(Shader& shader, const glm::mat4& modelViewMat) const;
	void unload(Shader& shader);

	void setAmb(const glm::vec3& ind);
	void setDiff(const glm::vec3& ind);
	void setSpec(const glm::vec3& ind);

	// Enciende y apaga las luces
	void toggleLight();

protected:
	Light(std::string name);
	Light(const std::string& name, int id);

	// Identificador del struct de la luz en el shader.
	std::string lightID; 

	bool bEnabled; // Si la luz está apagada o encendida

	// Atributos lumínicos y geométrico de la fuente de luz
	glm::vec3 ambient = {0.1, 0.1, 0.1};
	glm::vec3 diffuse = {0.5, 0.5, 0.5};
	glm::vec3 specular = {0.5, 0.5, 0.5};
};

class DirLight : public Light {
public:
	explicit DirLight(int id = 0);

	virtual void upload(Shader& shader, glm::mat4 const& modelViewMat) const override;
	void setDirection(const glm::vec3& dir);

protected:
	// la direccion de la luz en coordenadas de vista.
	glm::vec4 direction = {-1, -1, -1, 0};
};

inline void DirLight::setDirection(const glm::vec3& dir) {
	direction = glm::vec4(dir, 0.0); // 0 = dirección
}

class PosLight : public Light {
public:
	explicit PosLight(int id = 0);

	virtual void upload(Shader& shader, glm::mat4 const& modelViewMat) const override;

	void setPosition(const glm::vec3& dir);
	void setAttenuation(GLfloat kc, GLfloat kl, GLfloat kq);

protected:
	// posición de la luz en coordenadas de vista.
	glm::vec4 position = {0, 0, 0, 1};

	// coeficientes del factor de atenuación de la luz
	GLfloat constant = 1, linear = 0, quadratic = 0;
};

inline void PosLight::setPosition(const glm::vec3& pos) {
	position = glm::vec4(pos, 1.0); // 1 = posición
}

inline void PosLight::setAttenuation(GLfloat nkc, GLfloat nkl, GLfloat nkq) {
	constant = nkc;
	linear = nkl;
	quadratic = nkq;
}

class SpotLight : public PosLight {
public:
	SpotLight(const glm::vec3& pos = {0, 0, 0}, int id = 0);

	virtual void upload(Shader& shader, glm::mat4 const& modelViewMat) const override;
	void setDirection(const glm::vec3& dir) { direction = dir; }
	void setCutoff(float inner, float outer);

protected:
	// dirección a la que apunta el foco.
	glm::vec3 direction = {0, 0, -1};

	// cosenos de los ángulos interior y exterior del cono de luz.
	GLfloat cutoff = 60, outerCutoff = 120;
	//GLfloat cutoff = 0.91, outerCutoff = 0.82; TODO
	// GLfloat exp = 0; TODO
};

#endif //_H_Light_H_
