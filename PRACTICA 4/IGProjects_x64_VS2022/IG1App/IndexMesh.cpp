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
	//Numero de vertices
	mesh->mNumVertices = 36;
	//Y reserva los vertices
	mesh->vVertices.reserve(mesh->mNumVertices);

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
	mesh->vVertices.emplace_back(l/2, l/2, -l/2); //0 - 0
	mesh->vVertices.emplace_back(l/2, -l/2, -l/2); //1 - 1
	mesh->vVertices.emplace_back(l/2, l/2, l/2); //2 - 2
	mesh->vVertices.push_back(mesh->vVertices[2]); //2 - 3
	mesh->vVertices.push_back(mesh->vVertices[1]); //1 - 4
	mesh->vVertices.emplace_back(l/2, -l/2, l/2); //3 - 5
	//Cara 2
	mesh->vVertices.push_back(mesh->vVertices[2]); //2 - 6
	mesh->vVertices.push_back(mesh->vVertices[5]); //3 - 7
	mesh->vVertices.emplace_back(-l/2, l/2, l/2); //4 - 8
	mesh->vVertices.push_back(mesh->vVertices[8]); //4 - 9
	mesh->vVertices.push_back(mesh->vVertices[5]); //3 - 10
	mesh->vVertices.emplace_back(-l/2, -l/2, l/2); //5 - 11
	//Cara 3
	mesh->vVertices.push_back(mesh->vVertices[8]); //4 - 12
	mesh->vVertices.push_back(mesh->vVertices[12]); //5 - 13
	mesh->vVertices.emplace_back(-l/2, l/2, -l/2); //6 - 14
	mesh->vVertices.push_back(mesh->vVertices[14]); //6 - 15
	mesh->vVertices.push_back(mesh->vVertices[12]); //5 - 16
	mesh->vVertices.emplace_back(-l/2, -l/2, -l/2); //7 - 17
	//Cara 4
	mesh->vVertices.push_back(mesh->vVertices[14]); //6 - 18
	mesh->vVertices.push_back(mesh->vVertices[17]); //7 - 19
	mesh->vVertices.push_back(mesh->vVertices[0]); //0 - 20
	mesh->vVertices.push_back(mesh->vVertices[0]); //0 - 21
	mesh->vVertices.push_back(mesh->vVertices[17]); //7 - 22
	mesh->vVertices.push_back(mesh->vVertices[1]); //1 - 23
	//Cara 5
	mesh->vVertices.push_back(mesh->vVertices[8]); //4 - 24
	mesh->vVertices.push_back(mesh->vVertices[14]); //6 - 25
	mesh->vVertices.push_back(mesh->vVertices[2]); //2 - 26
	mesh->vVertices.push_back(mesh->vVertices[2]); //2 - 27
	mesh->vVertices.push_back(mesh->vVertices[14]); //6 - 28
	mesh->vVertices.push_back(mesh->vVertices[0]); //0 - 29
	//Cara 6
	mesh->vVertices.push_back(mesh->vVertices[1]); //1 - 30
	mesh->vVertices.push_back(mesh->vVertices[17]); //7 - 31
	mesh->vVertices.push_back(mesh->vVertices[5]); //3 - 32
	mesh->vVertices.push_back(mesh->vVertices[5]); //3 - 33
	mesh->vVertices.push_back(mesh->vVertices[17]); //7 - 34
	mesh->vVertices.push_back(mesh->vVertices[12]); //5 - 35




	//// ---- CARA 1.
	//mesh->vVertices.emplace_back(-l, -l, -l); // 0.
	//mesh->vVertices.emplace_back(-l, -l, l); // 1.
	//mesh->vVertices.emplace_back(l, -l, -l); // 2.

	//mesh->vVertices.emplace_back(l, -l, l); // 3.
	//mesh->vVertices.push_back(mesh->vVertices[1]); // 4 (misma pos que el 1).
	//mesh->vVertices.push_back(mesh->vVertices[2]); // 5 (misma pos que el 2).

	//// ---- CARA 2.
	//mesh->vVertices.push_back(mesh->vVertices[2]); // 6 (misma pos que el 2).
	//mesh->vVertices.emplace_back(l, l, -l); // 7.
	//mesh->vVertices.push_back(mesh->vVertices[0]); // 8 (misma pos que el 0).

	//mesh->vVertices.emplace_back(-l, l, -l); // 9.
	//mesh->vVertices.push_back(mesh->vVertices[7]); // 10 (misma pos que el 7).
	//mesh->vVertices.push_back(mesh->vVertices[0]); // 11 (misma pos que el 0).

	//// ---- CARA 3.
	//mesh->vVertices.push_back(mesh->vVertices[0]); // 12 (misma pos que el 0).
	//mesh->vVertices.push_back(mesh->vVertices[9]); // 13 (misma pos que el 9).
	//mesh->vVertices.push_back(mesh->vVertices[1]); // 14 (misma pos que el 1).

	//mesh->vVertices.emplace_back(-l, l, l); // 15.
	//mesh->vVertices.push_back(mesh->vVertices[9]); // 16 (misma pos que el 9).
	//mesh->vVertices.push_back(mesh->vVertices[1]); // 17 (misma pos que el 1).

	//// ---- CARA 4 (PARALELA A CARA 1).
	//mesh->vVertices.push_back(mesh->vVertices[9]); // 18 (misma pos que el 9).
	//mesh->vVertices.push_back(mesh->vVertices[15]); // 19 (misma pos que el 15).
	//mesh->vVertices.push_back(mesh->vVertices[7]); // 20 (misma pos que el 7).

	//mesh->vVertices.emplace_back(l, l, l); // 21.
	//mesh->vVertices.push_back(mesh->vVertices[15]); // 22 (misma pos que el 15).
	//mesh->vVertices.push_back(mesh->vVertices[7]); // 23 (misma pos que el 7).

	//// ---- CARA 5 (PARALELA A CARA 2).
	//mesh->vVertices.push_back(mesh->vVertices[3]); // 24 (misma pos que el 3).
	//mesh->vVertices.push_back(mesh->vVertices[21]); // 25 (misma pos que el 21).
	//mesh->vVertices.push_back(mesh->vVertices[1]); // 26 (misma pos que el 1).

	//mesh->vVertices.push_back(mesh->vVertices[15]); // 27 (misma pos que el 15).
	//mesh->vVertices.push_back(mesh->vVertices[21]); // 28 (misma pos que el 21).
	//mesh->vVertices.push_back(mesh->vVertices[1]); // 29 (misma pos que el 1).

	//// ---- CARA 6 (PARALELA A CARA 3).
	//mesh->vVertices.push_back(mesh->vVertices[2]); // 30 (misma pos que el 2).
	//mesh->vVertices.push_back(mesh->vVertices[7]); // 31 (misma pos que el 7).
	//mesh->vVertices.push_back(mesh->vVertices[3]); // 32 (misma pos que el 3).

	//mesh->vVertices.push_back(mesh->vVertices[21]); // 33 (misma pos que el 21).
	//mesh->vVertices.push_back(mesh->vVertices[7]); // 34 (misma pos que el 7).
	//mesh->vVertices.push_back(mesh->vVertices[3]); // 35 (misma pos que el 3).
















	//// METODO NEWELL (REVISAR):
	//// TODO.
	////Y reserva los vertices de normales
	//mesh->vNormals.reserve(mesh->vVertices.size());

	//////Inicializa el vector de normales a 0
	////for (int i = 0; i < mesh->vNormals.size(); i++)
	////{
	////	mesh->vNormals.emplace_back(0, 0, 0);
	////}
	//
	//int index = 3 * 12; // 3 vertices * 12 triangulos = 36.
	//for (int k = 0; k < index; k += 3) {
	//	glm::vec3 normal = glm::normalize(glm::cross(
	//		mesh->vVertices[k + 1] - mesh->vVertices[k],
	//		mesh->vVertices[k + 2] - mesh->vVertices[k]));

	//	// suma la normal del triangulo a todos sus vertices.
	//	mesh->vNormals.emplace_back(normal.x, normal.y, normal.z);
	//	mesh->vNormals.emplace_back(normal.x, normal.y, normal.z);
	//	mesh->vNormals.emplace_back(normal.x, normal.y, normal.z);
	//	//mesh->vNormals[k] = { mesh->vNormals[k].x + normal.x, 1, 0 };
	//	//mesh->vNormals[k + 1] += normal;
	//	//mesh->vNormals[k + 2] += normal;
	//}

	//for (int i = 0; i < mesh->vNormals.size(); i++)
	//{
	//	mesh->vNormals[i] = glm::normalize(mesh->vNormals[i]);
	//}

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
