#include "geometry.h"

Geometry::Geometry(){}

Geometry::Geometry(
	const std::vector<float>& positions,
	const std::vector<float>& normals,
	const std::vector<float>& uvs,
	const std::vector<unsigned int>& indices) 
{
	mIndicesCount = indices.size();


	// 2 Create VBO
	glGenBuffers(1, &mPosVbo);
	glBindBuffer(GL_ARRAY_BUFFER, mPosVbo);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_STATIC_DRAW);


	glGenBuffers(1, &mUvVbo);
	glBindBuffer(GL_ARRAY_BUFFER, mUvVbo);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(float), uvs.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &mNormalVbo);
	glBindBuffer(GL_ARRAY_BUFFER, mNormalVbo);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);


	// 3 Create EBO
	glGenBuffers(1, &mEbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	// 4 Create vao
	glGenVertexArrays(1, &mVao);
	glBindVertexArray(mVao);

	// 5 Add vbo and ebo to vao
	glBindBuffer(GL_ARRAY_BUFFER, mPosVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);


	glBindBuffer(GL_ARRAY_BUFFER, mUvVbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, mNormalVbo);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);

	glBindVertexArray(0);


}

Geometry::Geometry(
	const std::vector<float>& positions,
	const std::vector<float>& normals,
	const std::vector<float>& uvs,
	const std::vector<float>& colors,
	const std::vector<unsigned int>& indices

) {
	mIndicesCount = indices.size();


	// 2 Create VBO
	glGenBuffers(1, &mPosVbo);
	glBindBuffer(GL_ARRAY_BUFFER, mPosVbo);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_STATIC_DRAW);


	glGenBuffers(1, &mUvVbo);
	glBindBuffer(GL_ARRAY_BUFFER, mUvVbo);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(float), uvs.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &mNormalVbo);
	glBindBuffer(GL_ARRAY_BUFFER, mNormalVbo);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &mColorVbo);
	glBindBuffer(GL_ARRAY_BUFFER, mColorVbo);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float), colors.data(), GL_STATIC_DRAW);


	// 3 Create EBO
	glGenBuffers(1, &mEbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	// 4 Create vao
	glGenVertexArrays(1, &mVao);
	glBindVertexArray(mVao);

	// 5 Add vbo and ebo to vao
	glBindBuffer(GL_ARRAY_BUFFER, mPosVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);


	glBindBuffer(GL_ARRAY_BUFFER, mUvVbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, mNormalVbo);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, mColorVbo);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);

	glBindVertexArray(0);


}

Geometry::Geometry(
	const std::vector<float>& positions,
	const std::vector<float>& normals,
	const std::vector<float>& uvs,
	const std::vector<unsigned int>& indices,
	const std::vector<float>& tangents
) {
	// 1 Calculate 
	mIndicesCount = indices.size();


	// 2 Create VBO
	glGenBuffers(1, &mPosVbo);
	glBindBuffer(GL_ARRAY_BUFFER, mPosVbo);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_STATIC_DRAW);


	glGenBuffers(1, &mUvVbo);
	glBindBuffer(GL_ARRAY_BUFFER, mUvVbo);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(float), uvs.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &mNormalVbo);
	glBindBuffer(GL_ARRAY_BUFFER, mNormalVbo);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &mTangentVbo);
	glBindBuffer(GL_ARRAY_BUFFER, mTangentVbo);
	glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(float), tangents.data(), GL_STATIC_DRAW);


	// 3 Create EBO
	glGenBuffers(1, &mEbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	// 4 Create vao
	glGenVertexArrays(1, &mVao);
	glBindVertexArray(mVao);

	// 5 Add vbo and ebo to vao
	glBindBuffer(GL_ARRAY_BUFFER, mPosVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);


	glBindBuffer(GL_ARRAY_BUFFER, mUvVbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, mNormalVbo);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, mTangentVbo);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);

	glBindVertexArray(0);

}

Geometry::~Geometry() {

	if (mVao != 0) {
		glDeleteVertexArrays(1, &mVao);
	}
	if (mPosVbo != 0) {
		glDeleteBuffers(1, &mPosVbo);
	}
	if (mUvVbo != 0) {
		glDeleteBuffers(1, &mUvVbo);
	}
	if (mEbo != 0) {
		glDeleteBuffers(1, &mEbo);
	}
	if (mNormalVbo != 0) {
		glDeleteBuffers(1, &mNormalVbo);
	}
	if (mColorVbo != 0) {
		glDeleteBuffers(1, &mColorVbo);
	}
	if (mTangentVbo != 0) {
		glDeleteBuffers(1, &mTangentVbo);
	}

}

Geometry* Geometry::createBox(float size, bool invNormal){

	Geometry* geometry = new Geometry();
	geometry->mIndicesCount = 36;

	float halfSize = size / 2.0f;

	float positions[] = {
		// Front face
		-halfSize, -halfSize, halfSize, halfSize, -halfSize, halfSize, halfSize, halfSize, halfSize, -halfSize, halfSize, halfSize,
		// Back face
		-halfSize, -halfSize, -halfSize, -halfSize, halfSize, -halfSize, halfSize, halfSize, -halfSize, halfSize, -halfSize, -halfSize,
		// Top face
		-halfSize, halfSize, halfSize, halfSize, halfSize, halfSize, halfSize, halfSize, -halfSize, -halfSize, halfSize, -halfSize,
		// Bottom face
		-halfSize, -halfSize, -halfSize, halfSize, -halfSize, -halfSize, halfSize, -halfSize, halfSize, -halfSize, -halfSize, halfSize,
		// Right face
		halfSize, -halfSize, halfSize, halfSize, -halfSize, -halfSize, halfSize, halfSize, -halfSize, halfSize, halfSize, halfSize,
		// Left face
		-halfSize, -halfSize, -halfSize, -halfSize, -halfSize, halfSize, -halfSize, halfSize, halfSize, -halfSize, halfSize, -halfSize
	};

	float uvs[] = {
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
	};

	float inv = invNormal ? -1.0 : 1.0;
	float normals[] = {

		// Front face
		0.0f, 0.0f, 1.0f * inv,
		0.0f, 0.0f, 1.0f * inv,
		0.0f, 0.0f, 1.0f * inv,
		0.0f, 0.0f, 1.0f * inv,

		// Back face
		0.0f, 0.0f, -1.0f * inv,
		0.0f, 0.0f, -1.0f * inv,
		0.0f, 0.0f, -1.0f * inv,
		0.0f, 0.0f, -1.0f * inv,

		// Top face
		0.0f, 1.0f * inv, 0.0f,
		0.0f, 1.0f * inv, 0.0f,
		0.0f, 1.0f * inv, 0.0f,
		0.0f, 1.0f * inv, 0.0f,

		// Bottom face
		0.0f, -1.0f * inv, 0.0f,
		0.0f, -1.0f * inv, 0.0f,
		0.0f, -1.0f * inv, 0.0f,
		0.0f, -1.0f * inv, 0.0f,

		// Right face
		1.0f * inv, 0.0f, 0.0f,
		1.0f * inv, 0.0f, 0.0f,
		1.0f * inv, 0.0f, 0.0f,
		1.0f * inv, 0.0f, 0.0f,

		// Left face
		-1.0f * inv, 0.0f, 0.0f,
		-1.0f * inv, 0.0f, 0.0f,
		-1.0f * inv, 0.0f, 0.0f,
		-1.0f * inv, 0.0f, 0.0f,
	};

	float tangents[] = {
		// Front face
		1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
		// Back face
		-1.0f, 0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
		// Top face
		1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
		// Bottom face
		1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
		// Right face
		0.0f, 0.0f, -1.0f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f, -1.0f,
		// Left face
		0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f
	};

	unsigned int indices[] = {
		0, 1, 2, 2, 3, 0,        // Front face
		4, 5, 6, 6, 7, 4,        // Back face
		8, 9, 10, 10, 11, 8,     // Top face
		12, 13, 14, 14, 15, 12,  // Bottom face
		16, 17, 18, 18, 19, 16,  // Right face
		20, 21, 22, 22, 23, 20   // Left face
	};

	// 2 Create VBO
	GLuint& posVbo = geometry->mPosVbo, uvVbo = geometry->mUvVbo, normalVbo = geometry->mNormalVbo;
	glGenBuffers(1, &posVbo);
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);


	glGenBuffers(1, &uvVbo);
	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);

	glGenBuffers(1, &normalVbo);
	glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
	
	glGenBuffers(1, &geometry->mTangentVbo);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->mTangentVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tangents), tangents, GL_STATIC_DRAW);

	// 3 Create EBO
	glGenBuffers(1, &geometry->mEbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 4 Create vao
	glGenVertexArrays(1, &geometry->mVao);
	glBindVertexArray(geometry->mVao);

	// 5 Add vbo and ebo to vao
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, geometry->mTangentVbo);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo);

	glBindVertexArray(0);

	return geometry;
}

Geometry* Geometry::createSphere(float radius){

	Geometry* geometry = new Geometry();

	// 1 Variables
	std::vector<GLfloat> positions{};
	std::vector<GLfloat> uvs{};
	std::vector<GLfloat> normals{};
	std::vector<GLfloat> tangents{};
	std::vector<GLuint> indices{};

	int numLatLines = 60;
	int numLongLines = 60;

	// 2 Create positions and uvs
	for (int i = 0; i <= numLatLines; i++) {
		for (int j = 0; j <= numLongLines; j++) {

			float phi = i * glm::pi<float>() / numLatLines;
			float theta = j * 2 * glm::pi<float>() / numLongLines;

			float y = radius * cos(phi);
			float x = radius * sin(phi) * cos(theta);
			float z = radius * sin(phi) * sin(theta);

			positions.push_back(x);
			positions.push_back(y);
			positions.push_back(z);


			float u = 1.0 - (float)j / (float)numLongLines;
			float v = 1.0 - (float)i / (float)numLatLines;

			uvs.push_back(u);
			uvs.push_back(v);

			normals.push_back(x);
			normals.push_back(y);
			normals.push_back(z);
		}

	}

	// 3 Create indices
	for (int i = 0; i < numLatLines; i++) {
		for (int j = 0; j < numLongLines; j++) {

			int p1 = i * (numLongLines + 1) + j;
			int p2 = p1 + numLongLines + 1;
			int p3 = p1 + 1;
			int p4 = p2 + 1;

			indices.push_back(p1);
			indices.push_back(p2);
			indices.push_back(p3);

			indices.push_back(p3);
			indices.push_back(p2);
			indices.push_back(p4);

		}
	}

	// 4 Calculate the tangents
	tangents.resize(positions.size());
	for (int i = 0; i < indices.size(); i += 3) {

		int idx0 = indices[0];
		int idx1 = indices[i + 1];
		int idx2 = indices[i + 2];

		auto p0 = glm::vec3(positions[idx0 * 3], positions[idx0 * 3 + 1], positions[idx0 * 3 + 2]);
		auto p1 = glm::vec3(positions[idx1 * 3], positions[idx1 * 3 + 1], positions[idx1 * 3 + 2]);
		auto p2 = glm::vec3(positions[idx2 * 3], positions[idx2 * 3 + 1], positions[idx2 * 3 + 2]);

		if (p0 == p1 || p1 == p2 || p2 == p0) {

			continue;
		}


		auto uv0 = glm::vec2(uvs[idx0 * 2], uvs[idx0 * 2 + 1]);
		auto uv1 = glm::vec2(uvs[idx1 * 2], uvs[idx1 * 2 + 1]);
		auto uv2 = glm::vec2(uvs[idx2 * 2], uvs[idx2 * 2 + 1]);

		// delta e1, e2, e3, uv
		glm::vec3 e0 = p1 - p0;
		glm::vec3 e1 = p2 - p1;

		glm::vec2 dUV0 = uv1 - uv0;
		glm::vec2 dUV1 = uv2 - uv1;

		// |A|
		float f = 1.0f / (dUV0.x * dUV0.y - dUV1.x * dUV0.y);

		// Tx, Ty, Tz
		glm::vec3 tangent;
		tangent.x = f * (dUV1.y * e0.x - dUV0.y * e1.x);
		tangent.y = f * (dUV1.y * e0.y - dUV0.y * e1.y);
		tangent.z = f * (dUV1.y * e0.z - dUV0.y * e1.z);

		auto normal0 = glm::normalize(glm::vec3(normals[idx0 * 3], normals[idx0 * 3 + 1], normals[idx0 * 3 + 2]));
		auto normal1 = glm::normalize(glm::vec3(normals[idx1 * 3], normals[idx1 * 3 + 1], normals[idx1 * 3 + 2]));
		auto normal2 = glm::normalize(glm::vec3(normals[idx2 * 3], normals[idx2 * 3 + 1], normals[idx2 * 3 + 2]));

		auto tangent0 = tangent - glm::dot(tangent, normal0) * normal0;
		auto tangent1 = tangent - glm::dot(tangent, normal1) * normal1;
		auto tangent2 = tangent - glm::dot(tangent, normal2) * normal2;

		tangents[idx0 * 3] += tangent0.x;
		tangents[idx0 * 3 + 1] += tangent0.y;
		tangents[idx0 * 3 + 2] += tangent0.z;

		tangents[idx1 * 3] += tangent1.x;
		tangents[idx1 * 3 + 1] += tangent1.y;
		tangents[idx1 * 3 + 2] += tangent1.z;

		tangents[idx2 * 3] += tangent2.x;
		tangents[idx2 * 3 + 1] += tangent2.y;
		tangents[idx2 * 3 + 2] += tangent2.z;

	}

	for (int i = 0; i < tangents.size(); i += 3) {

		glm::vec3 tangent = glm::vec3(tangents[i], tangents[i + 1], tangents[i + 2]);
		tangent = glm::normalize(tangent);
		tangents[i] = tangent.x;
		tangents[i + 1] = tangent.y;
		tangents[i + 2] = tangent.z;
	}

	// 5 Create vao
	// 5.1 Create VBO
	GLuint& posVbo = geometry->mPosVbo, uvVbo = geometry->mUvVbo, normalVbo = geometry->mNormalVbo;
	glGenBuffers(1, &posVbo);
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_STATIC_DRAW);


	glGenBuffers(1, &uvVbo);
	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(float), uvs.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &normalVbo);
	glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &geometry->mTangentVbo);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->mTangentVbo);
	glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(float), tangents.data(), GL_STATIC_DRAW);


	// 3 Create EBO
	glGenBuffers(1, &geometry->mEbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	// 4 Create vao
	glGenVertexArrays(1, &geometry->mVao);
	glBindVertexArray(geometry->mVao);

	// 5 Add vbo and ebo to vao
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);


	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);


	glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, geometry->mTangentVbo);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo);

	glBindVertexArray(0);

	geometry->mIndicesCount = indices.size();

	return geometry;

}

Geometry* Geometry::createPlane(float width, float height) {

	Geometry* geometry = new Geometry();
	geometry->mIndicesCount = 6;

	float halfW = width / 2.0f;
	float halfH = height / 2.0f;

	float positions[] = {
	-halfW, -halfH, 0.0f,
	halfW, -halfH, 0.0f,
	halfW, halfH, 0.0f,
	-halfW, halfH, 0.0f,
	};

	float uvs[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};

	float normals[] = {
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
	};

	float tangents[] = {
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	// 2 Create vbo
	GLuint& posVbo = geometry->mPosVbo, uvVbo = geometry->mUvVbo, normalVbo = geometry->mNormalVbo;
	glGenBuffers(1, &posVbo);
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

	glGenBuffers(1, &uvVbo);
	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);

	glGenBuffers(1, &normalVbo);
	glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);

	glGenBuffers(1, &geometry->mTangentVbo);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->mTangentVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tangents), tangents, GL_STATIC_DRAW);

	// 3 Create ebo
	glGenBuffers(1, &geometry->mEbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 4 Create vao
	glGenVertexArrays(1, &geometry->mVao);
	glBindVertexArray(geometry->mVao);

	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, geometry->mTangentVbo);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo);

	glBindVertexArray(0);

	return geometry;

}

Geometry* Geometry::createScreenPlane() {

	Geometry* geometry = new Geometry();
	geometry->mIndicesCount = 6;

	float positions[] = {
		-1.0f,  1.0f,
		-1.0f, -1.0f,
		 1.0f, -1.0f,
		 1.0f,  1.0f
	};

	float uvs[] = {
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f

	};

	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3
	};


	// 2 Create vbo
	GLuint& posVbo = geometry->mPosVbo, uvVbo = geometry->mUvVbo;
	glGenBuffers(1, &posVbo);
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

	glGenBuffers(1, &uvVbo);
	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);


	// 3 Create ebo
	glGenBuffers(1, &geometry->mEbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 4 Create vao
	glGenVertexArrays(1, &geometry->mVao);
	glBindVertexArray(geometry->mVao);

	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo);

	glBindVertexArray(0);

	return geometry;
}