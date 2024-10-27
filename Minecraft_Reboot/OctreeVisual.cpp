#include "OctreeVisual.h"

OctreeVisual::OctreeVisual(Octree** octree, const vec3 position) : 
	faceCount(0),
	verticesCount(0),
	indicesCount(0),
	vboGenerated(false),
	isOctreeSet(false),
	VAO(0)
{
	setOctree(octree);

	transform.position = position;

	model = translate(mat4(1.0f), transform.position);
}

OctreeVisual::~OctreeVisual()
{
	delete octree;

	octree = nullptr;
}

void OctreeVisual::setOctree(Octree** octree)
{
	if (octree  == nullptr || *octree == nullptr) {
		return;
	}

	this->octree = *octree;

	isOctreeSet = true;
}

void OctreeVisual::addQuad(const vec3& a, const vec3& b, const vec3& c, const vec3& d)
{
	vertices.push_back(a.x);
	vertices.push_back(a.y);
	vertices.push_back(a.z);

	vertices.push_back(b.x);
	vertices.push_back(b.y);
	vertices.push_back(b.z);

	vertices.push_back(c.x);
	vertices.push_back(c.y);
	vertices.push_back(c.z);

	vertices.push_back(d.x);
	vertices.push_back(d.y);
	vertices.push_back(d.z);

	// Add quad indices
	indices.push_back(0 + 4 * faceCount);
	indices.push_back(1 + 4 * faceCount);
	indices.push_back(2 + 4 * faceCount);

	indices.push_back(2 + 4 * faceCount);
	indices.push_back(3 + 4 * faceCount);
	indices.push_back(0 + 4 * faceCount);

	faceCount++;
}

void OctreeVisual::onOctreeTraverse(Node* node)
{
	// Add bounding box vertices to the vertices vector
	BoundingBox box = node->box;

	vec3 min = box.center - 0.5f * box.dimensions;

	vec3 max = box.center + 0.5f * box.dimensions;

	// Front face
	addQuad(
		vec3(min.x, min.y, min.z), 
		vec3(max.x, min.y, min.z), 
		vec3(max.x, max.y, min.z), 
		vec3(min.x, max.y, min.z)
	);

	// Back face
	addQuad(
		vec3(min.x, min.y, max.z),
		vec3(max.x, min.y, max.z),
		vec3(max.x, max.y, max.z),
		vec3(min.x, max.y, max.z)
	);

	// Left face
	addQuad(
		vec3(min.x, min.y, min.z),
		vec3(min.x, min.y, max.z),
		vec3(min.x, max.y, max.z),
		vec3(min.x, max.y, min.z)
	);

	// Right face
	addQuad(
		vec3(max.x, min.y, min.z),
		vec3(max.x, min.y, max.z),
		vec3(max.x, max.y, max.z),
		vec3(max.x, max.y, min.z)
	);

	// Top face
	addQuad(
		vec3(min.x, max.y, min.z),
		vec3(max.x, max.y, min.z),
		vec3(max.x, max.y, max.z),
		vec3(min.x, max.y, max.z)
	);

	// Bottom face
	addQuad(
		vec3(min.x, min.y, min.z),
		vec3(max.x, min.y, min.z),
		vec3(max.x, min.y, max.z),
		vec3(min.x, min.y, max.z)
	);
}

void OctreeVisual::generateVBO() {
	if (!vboGenerated) {
		// Generate VAO and VBO and EBO
		GLuint VBO, EBO;

		glGenVertexArrays(1, &VAO);

		glGenBuffers(1, &VBO);

		glGenBuffers(1, &EBO);

		// Bind VAO
		glBindVertexArray(VAO);

		// Bind VBO
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		// Populate VBO
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

		// Bind EBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

		// Populate EBO
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		// For position attribute
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		// Unbind VAO
		glBindVertexArray(0);

		vboGenerated = true;

		verticesCount = vertices.size();

		indicesCount = indices.size();

		vertices.clear();

		indices.clear();
	}
}

void OctreeVisual::init()
{
	assert(isOctreeSet);

	// Populate the vertices and indices vectors
	octree->traverse([this](Node* node) {
		onOctreeTraverse(node);
	});
}

void OctreeVisual::update(float deltaTime)
{
}

void OctreeVisual::render(float deltaTime)
{
	if (!vboGenerated) {
		generateVBO();
	}
		
	// Use the Octree Shader Program
	auto shaderProgram = shaderManager->getShaderProgram(OCTREE_SHADER_PROGRAM);

	// Set the model matrix
	shaderProgram->SetMatrix4("model", model);

	// Projection Matrix and View matrix are set in the Camera class

	shaderProgram->Use();

	// Draw
	glBindVertexArray(VAO);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Disable CULL_FACE
	glDisable(GL_CULL_FACE);

	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDepthMask(GL_FALSE);

	glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);

	//glDrawArrays(GL_POINTS, 0, vertices.size());

	glBindVertexArray(0);

	// Unbind shader program
	glUseProgram(0);

	// Restore states
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

#ifdef ENABLE_CULL_FACE
	glEnable(GL_CULL_FACE);
#endif // ENABLE_CULL_FACE

	glDisable(GL_BLEND);

	glDepthMask(GL_TRUE);
}

void OctreeVisual::destroy()
{
	glDeleteVertexArrays(1, &VAO);

	vertices.clear();

	indices.clear();

	octree = nullptr;
}