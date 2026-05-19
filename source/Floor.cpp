#include "Floor.h"


Floor::Floor() {
	 setup(50.0f,10);

}
void Floor::setup(float size, int tile) {


	float maxX = size / 2, maxY = size / 2;
	float minX = -size / 2, minY = -size / 2;

	int x, y,v[3], i;
	float xp, yp, xd, yd;

	v[2] = 0;
	xd = (maxX - minX) / ((float)tile);
	yd = (maxY - minY) / ((float)tile);

	for (x = 0, xp = minX; x < tile; x++, xp += xd) {
		for (y = 0, yp = minY, i = x;y < tile; y++,i++, yp += yd) {
			if (i % 2 == 1) {
				//홀수쪽 칸에는 색상 검정
				clists.push_back(glm::vec3(floorColor1[0], floorColor1[1], floorColor1[2]));
				clists.push_back(glm::vec3(floorColor1[0], floorColor1[1], floorColor1[2]));
				clists.push_back(glm::vec3(floorColor1[0], floorColor1[1], floorColor1[2]));
				clists.push_back(glm::vec3(floorColor1[0], floorColor1[1], floorColor1[2]));
				clists.push_back(glm::vec3(floorColor1[0], floorColor1[1], floorColor1[2]));
				clists.push_back(glm::vec3(floorColor1[0], floorColor1[1], floorColor1[2]));
			}
			else {
				//짝수쪽 칸
				clists.push_back(glm::vec3(floorColor2[0], floorColor2[1], floorColor2[2]));
				clists.push_back(glm::vec3(floorColor2[0], floorColor2[1], floorColor2[2]));
				clists.push_back(glm::vec3(floorColor2[0], floorColor2[1], floorColor2[2]));
				clists.push_back(glm::vec3(floorColor2[0], floorColor2[1], floorColor2[2]));
				clists.push_back(glm::vec3(floorColor2[0], floorColor2[1], floorColor2[2]));
				clists.push_back(glm::vec3(floorColor2[0], floorColor2[1], floorColor2[2]));
			}
			//첫번째 삼각형
			vlists.push_back(glm::vec3(xp, 0, yp));
			vlists.push_back(glm::vec3(xp, 0, yp + yd));
			vlists.push_back(glm::vec3(xp + xd, 0, yp + yd));
			//두번째 삼각형 
			vlists.push_back(glm::vec3(xp, 0, yp));
			vlists.push_back(glm::vec3(xp + xd, 0, yp + yd));
			vlists.push_back(glm::vec3(xp + xd, 0, yp));
			
		
		
		}
	}


	glCreateVertexArrays(1, &vao);
	glCreateBuffers(1, &vbo_v);
	glCreateBuffers(1, &vbo_c);

	glNamedBufferData(
		vbo_v,
		vlists.size() * sizeof(glm::vec3),
		vlists.data(),
		GL_STATIC_DRAW

	);
	glVertexArrayVertexBuffer(
		vao,
		0,
		vbo_v,
		0,
		sizeof(GLfloat) * 3
	);
	glVertexArrayAttribFormat(   //읽기 방법
		vao,
		0, //location
		3,  //구성 float
		GL_FLOAT,
		GL_FALSE,
		0 //offset
	);
	glVertexArrayAttribBinding(vao, 0, 0);
	glEnableVertexArrayAttrib(vao, 0);

	//color CPU의 데이터를 GPU로 보냄
	glNamedBufferData(
		vbo_c,
		clists.size() * sizeof(glm::vec3),
		clists.data(),
		GL_STATIC_DRAW
	);
	glVertexArrayVertexBuffer(
		vao,
		1,
		vbo_c,
		0,
		sizeof(GLfloat) * 3
	);
	glVertexArrayAttribFormat(   //읽기 방법
		vao,
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		0
	);
	glVertexArrayAttribBinding(vao, 1, 1);
	glEnableVertexArrayAttrib(vao, 1);


	



}

void Floor::draw() {
	glBindVertexArray(vao);

	glDrawArrays(  
		GL_TRIANGLES,0, vlists.size());
	glBindVertexArray(0);
}