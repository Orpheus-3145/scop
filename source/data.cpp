#include <algorithm>
#include <unordered_map>
#include <cmath>
#include <cstddef>
#include <cstring>

#include "data.hpp"
#include "math/utilities.hpp"
#include "exception.hpp"


float const* VBO::getData( void ) const {
	return this->data.get();
}

uint32_t const* EBO::getData( void ) const {
	return this->data.get();
}


std::vector<std::string> const& ParsedData::getTmlFiles( void ) const noexcept {
	return this->_tmlFiles;
}

std::vector<VectF3> const& ParsedData::getVertices( void ) const noexcept {
	return this->_vertexes;
}

std::vector<VectF2> const& ParsedData::getTextures( void ) const noexcept {
	return this->_textures;
}

std::vector<VectF3> const& ParsedData::getVerticesNorm( void ) const noexcept {
	return this->_normals;
}

std::vector<VectF3> const& ParsedData::getParamSpaceVertices( void ) const noexcept {
	return this->_paramSpaceVertices;
}

std::list<Face> const& ParsedData::getFaces( void ) const noexcept {
	return this->_faces;
}

std::list<Line> const& ParsedData::getLines( void ) const noexcept {
	return this->_lines;
}

std::shared_ptr<VBO> const& ParsedData::getVBO( void ) const {
	if (!this->_VBOdata)
		throw ParsingException("VBO not initialized, call .fillBuffers()");
	return this->_VBOdata;
}

std::shared_ptr<EBO> const& ParsedData::getEBO( void ) const {
	if (!this->_EBOdata)
		throw ParsingException("EBO not initialized, call .fillBuffers()");
	return this->_EBOdata;
}

bool ParsedData::hasFaces( void ) const noexcept {
	return this->_faces.size() > 0;
}

void ParsedData::triangolate( void ) {
	if (this->_triangolationDone)
		return;

	for (auto currentFace=this->_faces.begin(); currentFace != this->_faces.end(); currentFace++) {
		if (currentFace->getIndexes().size() == 3)
			continue;

		Face newFace(currentFace->getFaceType());

		std::list<std::pair<VectUI3,VectF2>> vertexes = _create2Dvertexes(currentFace->getIndexes());
		std::list<VectF2> convexVertexes;
		std::list<VectF2> earVertexes;
		std::list<VectF2> reflexVertexes;
		// drop current polygon/face and insert N triangles
		currentFace = this->_faces.erase(currentFace);

		for (auto curr = vertexes.begin(); curr != vertexes.end(); ++curr) {
			if (this->_isConvex(curr, vertexes) == true) {
				convexVertexes.push_back((*curr).second);
				if (this->_isEar(curr, vertexes) == true)
					earVertexes.push_back((*curr).second);
			} else
			reflexVertexes.push_back((*curr).second);
		}
		// do ear clipping, creating a triangle for every iteration
		while (vertexes.size() > 3) {
			newFace.setIndexes(this->_spawnTriangle(vertexes, convexVertexes, earVertexes, reflexVertexes));
			currentFace = this->_faces.insert(currentFace, newFace);
		}
		std::vector<VectUI3> lastTriangle;
		for (auto const& [index, _] : vertexes)
			lastTriangle.push_back(index);
		newFace.setIndexes(lastTriangle);
		currentFace = this->_faces.insert(currentFace, newFace);
	}
	this->_triangolationDone = true;
}

void ParsedData::fixTrianglesOrientation( void ) {
	if (this->_triangolationDone == false)
		throw ParsingException("Faces must be triangolated, call .triangolate() first");
		
	VectF3 meshCenter{0.0f, 0.0f, 0.0f};
	for (Face const& face : this->_faces) {
		for ( VectUI3 const& index : face.getIndexes())
			meshCenter += this->_vertexes[index.i1];
	}
	meshCenter /= static_cast<float>(this->_faces.size() * 3);
	for (Face& face : this->_faces) {
		std::vector<VectUI3> vertexIndex = face.getIndexes();
		std::array<VectF3, 3> triangle{this->_vertexes[vertexIndex[0].i1], this->_vertexes[vertexIndex[1].i1], this->_vertexes[vertexIndex[2].i1]};
		VectF3 normal = getNormal(triangle);
		// center of the triangle
		VectF3 faceCenter = (triangle[0] + triangle[1] + triangle[2]) / 3.0f;
		VectF3 toOutside = faceCenter - meshCenter;
		if ((normal * toOutside) < F_ZERO)
			// swap vertexes position
			std::swap(vertexIndex[1], vertexIndex[2]);
		face.setIndexes(vertexIndex);
	}
}

void ParsedData::fillTexturesAndNormals( void ) {
	if (this->_triangolationDone == false)
		throw ParsingException("Faces must be triangolated, call .triangolate() first");
	else if (this->_dataFilled)
		return;

	uint32_t textureIndex = this->_textures.size();
	uint32_t normalIndex = this->_normals.size();
	for (Face& face : this->_faces) {
		if (face.getFaceType() == VERTEX_TEXT_VNORM)
			continue;

		std::vector<VectUI3> vertexIndex = face.getIndexes();
		std::array<VectF3, 3> triangle{this->_vertexes[vertexIndex[0].i1], this->_vertexes[vertexIndex[1].i1], this->_vertexes[vertexIndex[2].i1]};

		VectF3 normal = getNormal(triangle);
		// texture
		VectF3 helper;
		if (fabs(normal.x) < 0.9f)
			helper = VectF3{1.0f, 0.0f, 0.0f};
		else
			helper = VectF3{0.0f, 1.0f, 0.0f};
		// building a basis orthonormal on the triangle
		VectF3 u = normalize(helper ^ normal);
		VectF3 v = normalize(normal ^ u);
		std::array<float,3> uCoors;
		std::array<float,3> vCoors;
		// projecting vertexes on the plane
		for (uint32_t i=0; i<vertexIndex.size(); i++) {
			uCoors[i] = triangle[i] * u;
			vCoors[i] = triangle[i] * v;
		}
		// ranges to normalise the projections in [0, 1]
		float uMin = *std::min_element(uCoors.begin(), uCoors.end());
		float uMax = *std::max_element(uCoors.begin(), uCoors.end());
		float vMin = *std::min_element(vCoors.begin(), vCoors.end());
		float vMax = *std::max_element(vCoors.begin(), vCoors.end());
		// normal
		// to avoid that small faces affect the result too much,
		// every normal is weighted with the area of its triangle
		float areaTriangle = 0.5f * getAbs(normal);
		for (uint32_t i=0; i<vertexIndex.size(); i++) {
			VectF2 uv{(uCoors[i] - uMin) / (uMax - uMin), (vCoors[i] - vMin) / (vMax - vMin)};
			this->_textures.push_back(uv);

			VectF3 smoothed = (this->_vertexes[vertexIndex[i].i1] + normal) * areaTriangle;
			this->_normals.push_back(normalize(smoothed));

			vertexIndex[i].i2 = textureIndex++;
			vertexIndex[i].i3 = normalIndex++;
		}
		// update face with texture and normals info
		face.setFaceType(VERTEX_TEXT_VNORM);
		face.setIndexes(vertexIndex);
	}
	this->_dataFilled = true;
}

void ParsedData::fillBuffers( void ) {
	if (this->_faces.size() == 0)
		return this->fillVBOnoFaces();

	// maps the unique vertex-texture-normal and their indexes
	std::unordered_map<SerializedVertex,uint32_t,VectorByteHash,VectorByteEqual> uniqueData;
	const uint32_t 			vertexSize = VBO_STRIDE / sizeof(float); // 11, see header
	uint32_t				uniqueIndex = 0, indexColor = 0;
	std::vector<float>		vbo;
	std::vector<uint32_t>	ebo;
	// allocate only once, maximal size is known, final size <= maximal size (some vertex are removed if duplicated)
	vbo.resize(this->_faces.size() * 3 * vertexSize);
	// allocate only once, size is known
	ebo.reserve(this->_faces.size() * 3);
	float* currentVertex = vbo.data();

	std::array<VectF3, 3> colors{
		VectF3{randomFloat(), randomFloat(), randomFloat()},
		VectF3{randomFloat(), randomFloat(), randomFloat()},
		VectF3{randomFloat(), randomFloat(), randomFloat()}
	};

	for (Face const& face : this->_faces) {
		for (VectUI3 const& vertexIndex : face.getIndexes()) {
			SerializedVertex serializedVertex = this->_serializeVertex(vertexIndex, face.getFaceType());
			if (uniqueData.count(serializedVertex) == 0) {
				// vertex is unique, insert it inside VBO
				uniqueData[serializedVertex] = uniqueIndex++;
				std::memcpy(currentVertex, serializedVertex.data(), serializedVertex.size());
				currentVertex += serializedVertex.size() / sizeof(float);
				std::memcpy(currentVertex, &colors[indexColor++ % 3], sizeof(VectF3));
				currentVertex += sizeof(VectF3) / sizeof(float);
			}
			ebo.push_back(uniqueData[serializedVertex]);
		}
	}
	this->_VBOdata = std::make_shared<VBO>();
	this->_VBOdata->size = uniqueIndex;
	this->_VBOdata->stride = VBO_STRIDE;
	this->_VBOdata->data = std::make_unique<float[]>(uniqueIndex * vertexSize);
	std::move(vbo.data(), vbo.data() + uniqueIndex * vertexSize, this->_VBOdata->data.get());

	this->_EBOdata = std::make_shared<EBO>();
	this->_EBOdata->size = ebo.size();
	this->_EBOdata->stride = EBO_STRIDE;
	this->_EBOdata->data = std::make_unique<uint32_t[]>(ebo.size());
	std::move(ebo.data(), ebo.data() + ebo.size(), this->_EBOdata->data.get());
}

void ParsedData::fillVBOnoFaces( void ) {
	if (this->_vertexes.size() == 0)
		throw ParsingException("No vertexes found in file");

	std::shared_ptr<VBO> vbo = std::make_shared<VBO>();
	vbo->size = std::max({this->_vertexes.size(), this->_textures.size(), this->_normals.size()});
	vbo->stride = VBO_STRIDE;
	vbo->data = std::make_unique<float[]>(vbo->size * vbo->stride / sizeof(float));

	uint32_t indexColor = 0;
	std::array<VectF3, 3> colors{
		VectF3{randomFloat(), randomFloat(), randomFloat()},
		VectF3{randomFloat(), randomFloat(), randomFloat()},
		VectF3{randomFloat(), randomFloat(), randomFloat()}
	};

	float* vboPtr = vbo->data.get();
	for (uint32_t i=0; i<vbo->size; i++) {
		VectUI3 index{i, i, i};
		FaceType type = VERTEX;
		if (i < this->_textures.size() and i < this->_normals.size())
			type = VERTEX_TEXT_VNORM;
		else if (i < this->_textures.size())
			type = VERTEX_TEXT;
		else if (i < this->_normals.size())
			type = VERTEX_VNORM;

		SerializedVertex serializedVertex = this->_serializeVertex(index, type);
		std::memcpy(vboPtr, serializedVertex.data(), serializedVertex.size());
		vboPtr += vbo->stride;
		std::memcpy(vboPtr, &colors[indexColor++ % 3], sizeof(VectF3));
		vboPtr += sizeof(VectF3) / sizeof(float);
	}
	this->_VBOdata = std::move(vbo);
}

std::vector<VectUI3> ParsedData::_spawnTriangle(std::list<std::pair<VectUI3,VectF2>>& vertexes, std::list<VectF2>& convexVertexes, std::list<VectF2>& earVertexes, std::list<VectF2>& reflexVertexes ) const noexcept {
	std::list<std::pair<VectUI3,VectF2>>::const_iterator curr = vertexes.begin();
	// find the vertex which is an ear
	while (std::find(earVertexes.begin(), earVertexes.end(), (*curr).second) == earVertexes.end())
		curr = std::next(curr);

	earVertexes.erase(earVertexes.begin());

	std::list<std::pair<VectUI3,VectF2>>::const_iterator prev = std::prev(curr);
	if (curr == vertexes.begin())
		prev = std::prev(vertexes.end());
	std::list<std::pair<VectUI3,VectF2>>::const_iterator post = std::next(curr);
	if (post == vertexes.end())
		post = vertexes.begin();

	// if v(i +- 1) is convex check if becomes an ear
	if (std::find(convexVertexes.begin(), convexVertexes.end(), (*prev).second) != convexVertexes.end()) {
		if (this->_isEar(prev, vertexes) == true)
			earVertexes.push_back((*prev).second);
	}
	if (std::find(convexVertexes.begin(), convexVertexes.end(), (*post).second) != convexVertexes.end()) {
		if (this->_isEar(post, vertexes) == true)
			earVertexes.push_back((*post).second);
	}
	// if v(i +- 1) is an ear check if it remains an ear
	if (std::find(earVertexes.begin(), earVertexes.end(), (*prev).second) != earVertexes.end()) {
		if (this->_isEar(prev, vertexes) == false)
			earVertexes.remove((*prev).second);
	}
	if (std::find(earVertexes.begin(), earVertexes.end(), (*post).second) != earVertexes.end()) {
		if (this->_isEar(prev, vertexes) == false)
			earVertexes.remove((*post).second);
	}
	// if v(i +- 1) is reflex check if it becomes convex or even an ear
	if (std::find(reflexVertexes.begin(), reflexVertexes.end(), (*prev).second) != reflexVertexes.end()) {
		if (this->_isConvex(prev, vertexes) == true) {
			reflexVertexes.remove((*prev).second);
			convexVertexes.push_back((*prev).second);
			if (this->_isEar(prev, vertexes) == true)
				earVertexes.push_back((*post).second);
		}
	}
	if (std::find(reflexVertexes.begin(), reflexVertexes.end(), (*post).second) != reflexVertexes.end()) {
		if (this->_isConvex(post, vertexes) == true) {
			reflexVertexes.remove((*post).second);
			convexVertexes.push_back((*post).second);
			if (this->_isEar(post, vertexes) == true)
				earVertexes.push_back((*post).second);
		}
	}

	vertexes.erase(curr);
	return std::vector<VectUI3>{(*prev).first, (*curr).first, (*post).first};
}

std::list<std::pair<VectUI3,VectF2>> ParsedData::_create2Dvertexes( std::vector<VectUI3> const& indexList ) const noexcept {
	// Newell algorithm to find the normal of a plane
	float nx = 0.0f, ny = 0.0f, nz = 0.0f;
	for(uint32_t i=0; i < indexList.size(); i++) {
		VectF3 current = this->_vertexes[indexList[i].i1];
		VectF3 next = this->_vertexes[indexList[(i + 1) % indexList.size()].i1];
		nx += (current.y - next.y) * (current.z + next.z);
		ny += (current.z - next.z) * (current.x + next.x);
		nz += (current.x - next.x) * (current.y + next.y);
	}

	VectF3 normal = normalize(VectF3{nx, ny, nz});
	VectF3 u, v;
	if (fabs(normal.x) > fabs(normal.y))
		u = VectF3{normal.z * -1, 0.0f, normal.x};
	else
		u = VectF3{0.0f, normal.z * -1, normal.y};
	u = normalize(u);
	v = normal ^ u;

	VectF3 p0 = this->_vertexes[indexList[0].i1];
	std::list<std::pair<VectUI3,VectF2>> vertexes;
	for (auto const& faceIndex : indexList) {
		VectF3 piOriginp0 = this->_vertexes[faceIndex.i1] - p0;
		VectF2 projectedPi = VectF2{piOriginp0 * u, piOriginp0 * v};
		vertexes.push_back(std::pair<VectUI3,VectF2>(faceIndex, projectedPi));
	}
	return vertexes;
}

bool ParsedData::_isConvex(std::list<std::pair<VectUI3,VectF2>>::const_iterator const& curr, std::list<std::pair<VectUI3,VectF2>> const& vertexes) const noexcept {
	std::list<std::pair<VectUI3,VectF2>>::const_iterator prev = std::prev(curr);
	if (curr == vertexes.begin())
		prev = std::prev(vertexes.cend());
	std::list<std::pair<VectUI3,VectF2>>::const_iterator post = std::next(curr);
	if (post == vertexes.end())
		post = vertexes.begin();

	return width((*prev).second, (*curr).second, (*post).second) < M_PI;
}

bool ParsedData::_isEar(std::list<std::pair<VectUI3,VectF2>>::const_iterator const& curr, std::list<std::pair<VectUI3,VectF2>> const& vertexes) const noexcept {
	std::list<std::pair<VectUI3,VectF2>>::const_iterator prev = std::prev(curr);
	if (curr == vertexes.begin())
		prev = std::prev(vertexes.cend());
	std::list<std::pair<VectUI3,VectF2>>::const_iterator post = std::next(curr);
	if (post == vertexes.end())
		post = vertexes.begin();

	for (auto check = std::next(post); check != std::prev(prev); ++check) {
		if (check == vertexes.end())
			check = vertexes.begin();
		if (triangleContainmentTest((*prev).second, (*curr).second, (*post).second, (*check).second) == false)
			return false;
	}
	return true;

}

SerializedVertex ParsedData::_serializeVertex( VectUI3 const& index, FaceType faceType ) const {
	SerializedVertex serializedVertex;
	std::byte* rawVertexData = serializedVertex.data();

	if (index.i1 >= this->_vertexes.size())
		throw ParsingException("Vertex index out of bounds, couldn't create VBO");
	VectF3 vertex = this->_vertexes[index.i1];

	VectF2 texture{vertex.x * 0.5f + 0.5f, vertex.y * 0.5f + 0.5f};
	if (faceType == VERTEX_TEXT or faceType == VERTEX_TEXT_VNORM) {
		if (index.i2 >= this->_textures.size())
			throw ParsingException("Texture index out of bounds, couldn't create VBO");
		texture = this->_textures[index.i2];
	}

	VectF3 norm{0.5f, 0.5f, 0.5f};
	if (faceType == VERTEX_VNORM or faceType == VERTEX_TEXT_VNORM) {
		if (index.i3 >= this->_normals.size())
			throw ParsingException("Normal index out of bounds, couldn't create VBO");
		norm = this->_normals[index.i3];
	}

	std::memcpy(rawVertexData, &vertex, sizeof(VectF3));
	rawVertexData += sizeof(VectF3);
	std::memcpy(rawVertexData, &texture, sizeof(VectF2));
	rawVertexData += sizeof(VectF2);
	std::memcpy(rawVertexData, &norm, sizeof(VectF3));
	return serializedVertex;
}


std::ostream& operator<<( std::ostream& os, VBO const& data) {
	for (uint32_t i=0; i < data.size; i++) {
		for (uint32_t j=0; j < data.stride / sizeof(float); j++)
			os << data.getData()[i * data.stride / sizeof(float) + j] << " ";
		os << std::endl;
	}
	return os;
}

std::ostream& operator<<(std::ostream& os, EBO const& data) {
	uint32_t const* indexes = data.getData();

	for (uint32_t i=0; i<data.size; i++)
		os << *(indexes + i) << std::endl;
	return os;
}

std::ostream& operator<<(std::ostream& os, ParsedData const& obj) {
	for (std::string const& fileName : obj.getTmlFiles())
		os << "file: " << fileName << std::endl;
	for (VectF3 const& vertex : obj.getVertices())
		os << "v: " << vertex << std::endl;
	for (VectF2 const& texture : obj.getTextures())
		os << "vt: " << texture << std::endl;
	for (VectF3 const& vertexNorm : obj.getVerticesNorm())
		os << "vn: " << vertexNorm << std::endl;
	for (VectF3 const& vertexParam : obj.getParamSpaceVertices())
		os << "vp: " << vertexParam << std::endl;
	for (Face const& face : obj.getFaces())
		os << "f: " << face << std::endl;
	for (Line const& line : obj.getLines())
		os << "l: " << line << std::endl;
	return os;
}
