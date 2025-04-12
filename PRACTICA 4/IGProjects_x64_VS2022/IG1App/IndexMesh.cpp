#include "IndexMesh.h"
#include <numbers>



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

		//if (mCBO != NONE) {
		//	glDeleteBuffers(1, &mCBO);
		//	mCBO = NONE;
		//}

		//// eliminar las coordenadas de textura de la GPU
		//if (mTCO != NONE) {
		//	glDeleteBuffers(1, &mTCO);
		//	mTCO = NONE;
		//}
	}
}

IndexMesh* IndexMesh::generateByRevolution(const std::vector<glm::vec2>& profile,
	GLuint nSamples, GLfloat angleMax)
{
	//Crea la malla
	IndexMesh* mesh = new IndexMesh;
	//Pone la primitiva
	mesh->mPrimitive = GL_TRIANGLES;
	//Toma el tamano del perfil
	int tamPerfil = profile.size();
	//Y reserva los vertices correspondientes a partir de las samples
	mesh->vVertices.reserve(nSamples * tamPerfil);
	// Genera los vertices de las muestras
	GLdouble theta1 = angleMax / nSamples; //antes era 2 * std::numbers::pi (360º)
	//Crea los vertices
	for (int i = 0; i <= nSamples; ++i) { // muestra i-esima
		GLdouble c = cos(i * theta1), s = sin(i * theta1);
		for (auto p : profile) // rota el perfil
			mesh->vVertices.emplace_back(p.x * c, p.y, -p.x * s);
	}
	//Despues une los vertices para formar caras

	for (int i = 0; i < nSamples; ++i) // caras i a i + 1
		for (int j = 0; j < tamPerfil - 1; ++j) { // una cara
			if (profile[j].x != 0.0) // triangulo inferior
				for (auto [s, t] : { std::pair{i, j}, std::pair{i, j + 1}, std::pair{i + 1, j} })
					mesh->vIndexes.push_back(s * tamPerfil + t);
			if (profile[j + 1].x != 0.0) // triangulo superior
				for (auto [s, t] : { std::pair{i, j + 1}, std::pair{i + 1, j + 1}, std::pair{i + 1, j} })
					mesh->vIndexes.push_back(s * tamPerfil + t);
		}

	//Reserva vertices
	mesh->mNumVertices = mesh->vVertices.size();
	//Devuelve la malla correspondiente
	return mesh;
}

IndexMesh* IndexMesh::generateIndexedBox(GLdouble l)
{
	//Crea la malla
	IndexMesh* mesh = new IndexMesh;
	//Pone la primitiva
	mesh->mPrimitive = GL_TRIANGLES;
	//Y reserva los vertices
	mesh->vVertices.reserve(36);
	//Numero de vertices
	mesh->mNumVertices = mesh->vVertices.size();

	//Creamos los 8 vertices (y los 36 asociados)
	/*  0 (1, 1, -1)
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
	//Cara 1
	mesh->vVertices.emplace_back(l/2, l/2, -l/2); //0
	mesh->vVertices.emplace_back(l/2, -l/2, -l/2); //1
	mesh->vVertices.emplace_back(l/2, l/2, l/2); //2
	mesh->vVertices.push_back(mesh->vVertices[2]); //2
	mesh->vVertices.push_back(mesh->vVertices[1]); //1
	mesh->vVertices.emplace_back(l/2, -l/2, l/2); //3
	//Cara 2
	mesh->vVertices.push_back(mesh->vVertices[2]); //2
	mesh->vVertices.push_back(mesh->vVertices[5]); //3
	mesh->vVertices.emplace_back(-l/2, l/2, l/2); //4
	mesh->vVertices.push_back(mesh->vVertices[8]); //4
	mesh->vVertices.push_back(mesh->vVertices[5]); //3
	mesh->vVertices.emplace_back(-l/2, -l/2, l/2); //5
	//Cara 3
	mesh->vVertices.push_back(mesh->vVertices[8]); //4
	mesh->vVertices.push_back(mesh->vVertices[12]); //5
	mesh->vVertices.emplace_back(-l/2, l/2, -l/2); //6
	mesh->vVertices.push_back(mesh->vVertices[15]); //6
	mesh->vVertices.push_back(mesh->vVertices[12]); //5
	mesh->vVertices.emplace_back(-l/2, -l/2, -l/2); //7
	//Cara 4
	mesh->vVertices.push_back(mesh->vVertices[15]); //6
	mesh->vVertices.push_back(mesh->vVertices[18]); //7
	mesh->vVertices.push_back(mesh->vVertices[0]); //0
	mesh->vVertices.push_back(mesh->vVertices[0]); //0
	mesh->vVertices.push_back(mesh->vVertices[18]); //7
	mesh->vVertices.push_back(mesh->vVertices[1]); //1
	//Cara 5
	mesh->vVertices.push_back(mesh->vVertices[8]); //4
	mesh->vVertices.push_back(mesh->vVertices[15]); //6
	mesh->vVertices.push_back(mesh->vVertices[2]); //2
	mesh->vVertices.push_back(mesh->vVertices[2]); //2
	mesh->vVertices.push_back(mesh->vVertices[15]); //6
	mesh->vVertices.push_back(mesh->vVertices[0]); //0
	//Cara 6
	mesh->vVertices.push_back(mesh->vVertices[1]); //1
	mesh->vVertices.push_back(mesh->vVertices[18]); //7
	mesh->vVertices.push_back(mesh->vVertices[5]); //3
	mesh->vVertices.push_back(mesh->vVertices[5]); //3
	mesh->vVertices.push_back(mesh->vVertices[18]); //7
	mesh->vVertices.push_back(mesh->vVertices[12]); //5

	// METODO NEWELL (REVISAR):
	// TODO.
	//Y reserva los vertices de normales
	mesh->vNormals.reserve(mesh->vVertices.size());

	//Inicializa el vector de normales a 0
	for (int i = 0; i < mesh->vNormals.size(); i++)
	{
		mesh->vNormals.emplace_back(0, 0, 0);
	}
	
	int index = 3 * 12; // 3 vertices * 12 triangulos = 36.
	for (int k = 0; k < index; k += 3) {
		glm::vec3 normal = glm::normalize(glm::cross(
			mesh->vVertices[k + 1] - mesh->vVertices[k],
			mesh->vVertices[k + 2] - mesh->vVertices[k]));

		// suma la normal del triangulo a todos sus vertices.
		mesh->vNormals[k] += normal;
		mesh->vNormals[k + 1] += normal;
		mesh->vNormals[k + 2] += normal;
	}

	for (int i = 0; i < mesh->vNormals.size(); i++)
	{
		mesh->vNormals[i] = glm::normalize(mesh->vNormals[i]);
	}

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
