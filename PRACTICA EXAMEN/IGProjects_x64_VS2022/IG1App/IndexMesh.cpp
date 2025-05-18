#include "IndexMesh.h"

#include <numbers>

constexpr GLuint NONE = std::numeric_limits<GLuint>::max();

IndexMesh::IndexMesh(): mIBO(NONE)
{
}

IndexMesh::~IndexMesh()
{
	unload();
}

void IndexMesh::load()
{
	Mesh::load(); glBindVertexArray(mVAO);
	glGenBuffers(1, &mIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		vIndexes.size() * sizeof(GLuint),
		vIndexes.data(), GL_STATIC_DRAW);
	glBindVertexArray(0);
}

void IndexMesh::unload()
{
	if (mVAO != GL_NONE) {
		//Borramos los vertices
		glDeleteVertexArrays(1, &mVAO);
		glDeleteBuffers(1, &mIBO);
		mVAO = GL_NONE;
		mIBO = GL_NONE;

		if (mIBO != NONE) {
			glDeleteBuffers(1, &mIBO);
			mIBO = NONE;
		}
	}
}

IndexMesh* IndexMesh::generateByRevolution(const std::vector<glm::vec2>& profile,
	GLuint nSamples, GLfloat angleMax)
{
	//Crea la malla
	IndexMesh* mesh = new IndexMesh();
	//Pone la primitiva
	mesh->mPrimitive = GL_TRIANGLES;
	//Toma el tamano del perfil
	int tamPerfil = profile.size();
	//std::cout << tamPerfil;
	//Y reserva los vertices correspondientes a partir de las samples
	mesh->vVertices.reserve(nSamples * tamPerfil);
	// Genera los vertices de las muestras (como si fuera un poligono regular)
	GLdouble theta1 = angleMax / nSamples; //antes era 2 * std::numbers::pi (360º)
	//Crea los vertices
	for (int i = 0; i <= nSamples ; ++i) { // muestra i-esima
		GLdouble c = cos(i * theta1), s = sin(i * theta1);
		for (auto p : profile) // rota el perfil
			mesh->vVertices.emplace_back(p.x * c, p.y, -p.x * s);
	}

	//Despues une los vertices para formar caras
	for (int i = 0; i < nSamples; ++i) // caras i a i + 1 (todas las repeticiones del perfil)
		for (int j = 0; j < tamPerfil - 1; ++j) { // una cara (puntos dentro del perfil)
			if (profile[j].x != 0.0) // triangulo inferior
				for (auto [s, t] : { std::pair{i, j}, std::pair{i, j + 1}, std::pair{i + 1, j} })
				{
					mesh->vIndexes.push_back(s * tamPerfil + t);
				}

			
			if (profile[j + 1].x != 0.0) // triangulo superior
				for (auto [s, t] : { std::pair{i, j + 1}, std::pair{i + 1, j + 1}, std::pair{i + 1, j} })
				{
					mesh->vIndexes.push_back(s * tamPerfil + t);
				}
		}

	//Reserva vertices
	mesh->mNumVertices = mesh->vVertices.size();

	mesh->buildNormalVectors();
	//Devuelve la malla correspondiente
	return mesh;
}

IndexMesh* IndexMesh::generateIndexedBox(GLdouble l)
{
	//Crea la malla
	IndexMesh* mesh = new IndexMesh();
	//Pone la primitiva
	mesh->mPrimitive = GL_TRIANGLES;
	//Numero de vertices
	mesh->mNumVertices = 8;
	//Y reserva los vertices
	mesh->vVertices.reserve(mesh->mNumVertices);

	/* Explicacion:
	   Creamos los 8 vertices (y los 36 indices asociados)
		0 (1, 1, -1) 
		1 (1, -1, -1)
		2 (1, 1, 1) 
		3 (1, -1, 1)
		4 (-1, 1, 1)
		5 (-1, -1, 1)
		6 (-1, 1, -1) 
		7 (-1, -1, -1) 

		0, 1, 2, 2, 1, 3,
		2, 3, 4, 4, 3, 5,
		4, 5, 6, 6, 5, 7,
		6, 7, 0, 0, 7, 1,
		4, 6, 2, 2, 6, 0,
		1, 7, 3, 3, 7, 5
	*/

	// Crea 8 vertices (indices de 0 a 7).
	mesh->vVertices.emplace_back(l / 2, l / 2, -l / 2); // 0
	mesh->vVertices.emplace_back(l / 2, -l / 2, -l / 2); // 1
	mesh->vVertices.emplace_back(l / 2, l / 2, l / 2); // 2
	mesh->vVertices.emplace_back(l / 2, -l / 2, l / 2); // 3
	mesh->vVertices.emplace_back(-l / 2, l / 2, l / 2); // 4
	mesh->vVertices.emplace_back(-l / 2, -l / 2, l / 2); // 5
	mesh->vVertices.emplace_back(-l / 2, l / 2, -l / 2); // 6
	mesh->vVertices.emplace_back(-l / 2, -l / 2, -l / 2); // 7

	// Orden de los indices.
	std::vector<int> index = {
		0, 1, 2, 2, 1, 3,
		2, 3, 4, 4, 3, 5,
		4, 5, 6, 6, 5, 7,
		6, 7, 0, 0, 7, 1,
		4, 6, 2, 2, 6, 0,
		1, 7, 3, 3, 7, 5
	};

	//No hace falta reservar espacio en los indices
	// Lo va rellenando con el contenido de los indices.
	for (int i = 0; i < index.size(); i++)
	{
		mesh->vIndexes.push_back(index[i]); 
	}

	/* ---- - Calculo de las NORMALES a mano(sigue el orden de los vertices de arriba, con la explicacion del enunciado).
	// Nota: ver el dibujo de chuletas para entender.

	//// --- Cara ROJA.
	//mesh->vNormals.emplace_back(glm::normalize(glm::vec3(2, 2, -2))); // a
	//mesh->vNormals.emplace_back(glm::normalize(glm::vec3(1, -2, -1))); // b
	//mesh->vNormals.emplace_back(glm::normalize(glm::vec3(1, 1, 2))); // c 
	//mesh->vNormals.emplace_back(glm::normalize(glm::vec3(2, -1, 1))); // d

	//// --- Cara MORADA.
	//mesh->vNormals.emplace_back(glm::normalize(glm::vec3(-1, 2, 1))); // e
	//mesh->vNormals.emplace_back(glm::normalize(glm::vec3(-2, -2, 2))); // f
	//mesh->vNormals.emplace_back(glm::normalize(glm::vec3(-2, 1, -1))); // g
	//mesh->vNormals.emplace_back(glm::normalize(glm::vec3(-1, -1, -2))); // h 
	*/

	//Normales usando los valores de los vertices
	mesh->vNormals.emplace_back(glm::normalize(glm::vec3(1, 1, -1))); // a
	mesh->vNormals.emplace_back(glm::normalize(glm::vec3(1, -1, -1))); // b
	mesh->vNormals.emplace_back(glm::normalize(glm::vec3(1, 1, 1))); // c 
	mesh->vNormals.emplace_back(glm::normalize(glm::vec3(1, -1, 1))); // d 
	mesh->vNormals.emplace_back(glm::normalize(glm::vec3(-1, 1, 1))); // e
	mesh->vNormals.emplace_back(glm::normalize(glm::vec3(-1, -1, 1))); // f
	mesh->vNormals.emplace_back(glm::normalize(glm::vec3(-1, 1, -1))); // g 
	mesh->vNormals.emplace_back(glm::normalize(glm::vec3(-1, -1, -1))); // h 


	//Devuelve la malla correspondiente
	return mesh;
}

void IndexMesh::draw() const
{
	glDrawElements(
		mPrimitive, // primitiva ( GL_TRIANGLES , etc.)
		vIndexes.size(), // número de índices
		GL_UNSIGNED_INT, // tipo de los índices
		nullptr // offset en el VBO de índices
	);
}

void IndexMesh::buildNormalVectors()
{
	// METODO NEWELL (REVISAR):
	//Rellena inicialmente con (0.0, 0.0, 0.0)
	for (int i = 0; i < vIndexes.size(); i++)
		vNormals.emplace_back(0.0, 0.0, 0.0);

	//Define las normales con el producto vectorial
	for (int i = 0; i < vIndexes.size(); i += 3) //(36 indices/12 triangulos) = 3(vertices por triangulo))
	{
		// Calculo normal
		glm::vec3 normal = glm::normalize(glm::cross(
			vVertices[vIndexes[i + 1]] - vVertices[vIndexes[i]],
			vVertices[vIndexes[i + 2]] - vVertices[vIndexes[i]])
		);

		//Los rellena
		vNormals[vIndexes[i]] +=  normal;
		vNormals[vIndexes[i + 1]] += normal;
		vNormals[vIndexes[i + 2]] += normal;
	}

	//Normalizamos los vectores de vNormals
	for (int i = 0; i < vNormals.size(); i++)
		vNormals[i] = glm::normalize(vNormals[i]);
}
