#include "Octree.h"

BoundingBox::BoundingBox(const vec3& center, float size) : center(center), size(size) {

}

bool BoundingBox::contains(const vec3& point) const {
	return point.x >= center.x - size / 2 && point.x <= center.x + size / 2 &&
		point.y >= center.y - size / 2 && point.y <= center.y + size / 2 &&
		point.z >= center.z - size / 2 && point.z <= center.z + size / 2;
}

BoundingBox BoundingBox::childBox(int index) {
	vec3 newCenter = center;

	//printf("Old center: %f %f %f\n", center.x, center.y, center.z);

	// -------------------
	// 10		|		11
	// -------------------
	// 00		|		01
	// -------------------

	// If I want to return the 10 bounding box, I need to move the center of the new bounding box to the center of the 10 bounding box
	// So 01 & 01 = 1, so I need to move the center of the new bounding box to the right
	// 01 & 10 = 0, so I need to move the center of the new bounding box down

	// Move the center of the new bounding box to the correct position
	// Imagine the bounding box is divided into 8 smaller bounding boxes called octants
	// Each octant is a child bounding box and is represented by the index
	// The index is a 3-bit number where each bit represents the direction of the child bounding box
	// So the top-left farthest octant is 0b000 and the bottom-right nearest octant is 0b111
	// The center of the new bounding box is moved to the center of the child bounding box
	// The size of the new bounding box is halved
	// The new bounding box is returned
	if (index & 1) {
		newCenter.x += size / 4;
	}
	else {
		newCenter.x -= size / 4;
	}

	if (index & 2) {
		newCenter.y += size / 4;
	}
	else {
		newCenter.y -= size / 4;
	}

	if (index & 4) {
		newCenter.z += size / 4;
	}
	else {
		newCenter.z -= size / 4;
	}

	//printf("New center: %f %f %f\n\n", newCenter.x, newCenter.y, newCenter.z);

	return BoundingBox(newCenter, size / 2);
}

Node::Node(const BoundingBox& box) : box(box), voxel(nullptr) {
	for (int i = 0; i < 8; i++) {
		children[i] = nullptr;
	}
}

bool Node::isLeaf() const {
	for (int i = 0; i < 8; i++) {
		if (children[i] != nullptr) {
			return false;
		}
	}

	return true;
}

Octree::Octree(const BoundingBox& box) {
	root = new Node(box);
}

Octree::~Octree() {
	clear();
}

void Octree::insert(const vec3& position, std::unique_ptr<Voxel> voxel) {
	insert(root, position, std::move(voxel));
}

void Octree::insert(Node* node, const vec3& position, std::unique_ptr<Voxel> voxel) {
	// Check if the node contains this position
	if (!node->box.contains(position)) {
		return;
	}
	
	// If the node is a leaf node and currently empty, insert the voxel into the node
	if (node->isLeaf() && node->voxel == nullptr) {
		node->voxel = std::move(voxel);

		return;
	}

	// Check against min size
	//if (node->box.size <= 1.0f) {
	//	return;
	//}

	// If the node is a leaf node and not empty, subdivide the node
	if (node->isLeaf()) {
		subdivide(node);
	}

	// Find the child bounding box that contains the position
	for (int i = 0; i < 8; ++i) {
		if (node->children[i]->box.contains(position)) {
			insert(node->children[i], position, std::move(voxel));
			return;
		}
	}
}

void Octree::subdivide(Node* node) {
	// Subdivide the node into 8 smaller nodes
	for (int i = 0; i < 8; i++) {
		BoundingBox childBox = node->box.childBox(i);

		node->children[i] = new Node(childBox);
	}
}

Voxel* Octree::find(const vec3& position) {
	return find(root, position);
}

Voxel* Octree::find(Node* node, const vec3& position) {
	if (node == nullptr || !node->box.contains(position)) {
		return nullptr;
	}

	if (node->voxel != nullptr && node->voxel->pos == position) {
		return node->voxel.get();
	}

	// Find the child bounding box that contains the position
	for (int i = 0; i < 8; i++) {
		if (node->children[i] != nullptr) {
			if (node->children[i]->box.contains(position)) {
				return find(node->children[i], position);
			}
		}
	}

	return nullptr;
}

void Octree::clear() {
	clear(&root);
}

void Octree::clear(Node** node) {
	if (*node == nullptr) {
		return;
	}

	for (int i = 0; i < 8; i++) {
		if ((*node)->children[i] != nullptr) {
			clear(&((*node)->children[i]));
		}
	}

	(*node)->voxel.reset(); // Delete voxel

	delete *node;

	*node = nullptr;
}

void Octree::printMemoryUsage() {
	// Print the memory usage of the octree
	// Traverse the octree and count the number of nodes
	// Print the number of nodes and the memory usage
	size_t totalMemory = calculateMemoryUsage(root);

	printf("Total memory usage: %zu bytes\n", totalMemory);
}

size_t Octree::calculateMemoryUsage(Node* node) {
	if (node == nullptr) return 0;

	size_t memory = 0;

	if (node->voxel != nullptr) {
		memory += sizeof(Voxel);
	}

	for (int i = 0; i < 8; i++) {
		memory += calculateMemoryUsage(node->children[i]);
	}

	return memory;
}