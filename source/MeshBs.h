#ifndef _BS
#define _BS

#include <GL/gl3w.h>
#include <glm/glm.hpp>

#include <glm//gtc/constants.hpp>
#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include <vector>
class MeshBs {
public:
	MeshBs();
	void setup();
	void draw();
private:
	void CacultateTagent(std::vector<glm::vec3>& vertices,
		std::vector<glm::vec2>& uvs,
		std::vector<glm::vec3>& normals,
		std::vector<glm::vec3>& tangents,
		std::vector<glm::vec3>& bitangents);
	GLuint vao, vbo, tangentVBO, bitangentVBO, ibo;
	GLuint colorTexID;      // ✅ 추가
	GLuint normalMapTexID;  // ✅ 추가
};

#endif // !_BS
