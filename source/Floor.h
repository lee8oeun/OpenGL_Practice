#include <iostream>
#include <GL/gl3w.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"

#include <vector>

class Floor{
public:
	Floor();
	std::vector<glm::vec3> vlists;
	std::vector<glm::vec3> clists;
	glm::vec3 floorColor1 = glm::vec3(0.0f,0.1f,0.0f);
	glm::vec3 floorColor2 = glm::vec3(0.0f,0.1f,0.0f);
	void setup(float size, int tile);
	void draw();

	GLuint vao;
	GLuint vbo_v, vbo_c, ibo;
};