
#define GLM_ENABLE_EXPERIMENTAL
#include "../MyGL/MyGlWindow.h"
#include <vector>

#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>


const double PI = 3.141592;

static float DEFAULT_VIEW_POINT[3] = { 40, 40, 40 };
static float DEFAULT_VIEW_CENTER[3] = { 0, 0, 0 };
static float DEFAULT_UP_VECTOR[3] = { 0, 1, 0 };

//fix it to use smart pointers
//std::unique_ptr<ShaderProgram> shaderProgram = nullptr;
std::unique_ptr<Program> program = nullptr;

MyGlWindow::MyGlWindow(int w, int h)
//==========================================================================
{

	m_width = w;
	m_height = h;
	m_bs = nullptr;


	glm::vec3 viewPoint(DEFAULT_VIEW_POINT[0], DEFAULT_VIEW_POINT[1], DEFAULT_VIEW_POINT[2]);
	glm::vec3 viewCenter(DEFAULT_VIEW_CENTER[0], DEFAULT_VIEW_CENTER[1], DEFAULT_VIEW_CENTER[2]);
	glm::vec3 upVector(DEFAULT_UP_VECTOR[0], DEFAULT_UP_VECTOR[1], DEFAULT_UP_VECTOR[2]);

	float aspect = (w / (float) h);
	//fix it to use smart pointers
	m_viewer = std::make_unique<Viewer>(viewPoint, viewCenter, upVector, 60.0, aspect);

	initialize();
}

glm::mat4 lookAt(glm::vec3 pos, glm::vec3 look, glm::vec3 up)
{
	glm::vec3 zaxis = glm::normalize(pos - look);
	glm::vec3 xaxis = glm::normalize(glm::cross(up,zaxis));
	glm::vec3 yaxis = glm::normalize(glm::cross(zaxis,xaxis));

	glm::mat4 R;
	
	R[0] = glm::vec4(xaxis.x, yaxis.x, zaxis.x, 0.0f);
	R[1] = glm::vec4(xaxis.y, yaxis.y, zaxis.y, 0.0f);
	R[2] = glm::vec4(xaxis.z, yaxis.z, zaxis.z, 0.0f);
	R[3] = glm::vec4(0.0f,0.0f,0.0f,1.0f);
	
	glm::mat4 T;

	T[0] = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
	T[1] = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
	T[2] = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
	T[3] = glm::vec4(-pos.x, -pos.y, -pos.z, 1.0f);

	
	return R*T;
}


glm::mat4 perspective(float fov, float aspect, float n, float f)//(매개변수 :)시야각, 종횡비, 카메라가 볼 수 있는 최소거리, 최대거리 
{
	glm::mat4 P(0.0f);

	const float tanHalfFOV = tan(glm::radians(fov) / 2.0f);

	float A = (-f - n) / (f - n);
	float B = (2 * (n * f)) / ( n - f);

	P[0] = glm::vec4(1.0f / (aspect * tanHalfFOV), 0, 0, 0);
	P[1] = glm::vec4(0, 1.0 / tanHalfFOV, 0, 0);
	P[2] = glm::vec4(0, 0, A, -1.0f);
	P[3] = glm::vec4(0, 0, B, 0.0f);

	return P;
}

void MyGlWindow::draw(void)
{
	m_shaderNew->BindProgram();

	glm::mat4 model(1.0f);
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

	glm::vec3 camPos = m_viewer->getViewPoint();
	glm::vec3 camLook = m_viewer->getViewCenter();
	glm::vec3 upVector = m_viewer->getUpVector();

	glm::mat4 viewMat = lookAt(camPos, camLook, upVector);

	glm::mat4 mview = viewMat * model;
	m_shaderNew->SetMatrix("mview", mview);

	glm::mat4 imvp = glm::inverse(mview);
	glm::mat3 nmat = glm::mat3(glm::transpose(imvp));
	m_shaderNew->SetMatrix("nmat", nmat);

	glm::mat4 projection(1.0f);
	projection = perspective(45.0f, 1.0f * m_width / m_height, 0.1f, 500.0f);

	glm::mat4 mvp = projection * viewMat * model;
	m_shaderNew->SetMatrix("mvp", mvp);

	// 여기서 선언
	// view space 기준: 카메라 정면 위쪽에 강한 빛 배치
// view space 기준으로 카메라 앞쪽에 빛 배치
	glm::vec4 lightPos(0.0f, 10.0f, 30.0f, 1.0f);

	glm::vec3 Ia(0.75f,0.75f,0.75f);
	glm::vec3 Id(1.65f, 1.65f, 1.65f);
	glm::vec3 Is(2.0f,2.0f,2.0f);

	// 교수님 자료 참고: specular가 너무 강하면 1.0 사용
	GLfloat shiness = 10.0f;

	glm::vec3 Ka(0.3f,0.3f,0.3f);
	glm::vec3 Kd(1.0f, 1.0f, 1.0f);
	glm::vec3 Ks(0.15f, 0.15f, 0.15f);

	m_shaderNew->SetVector("LightPos", lightPos);

	m_shaderNew->SetVector("Light.Ia", Ia);
	m_shaderNew->SetVector("Light.Id", Id);
	m_shaderNew->SetVector("Light.Is", Is);

	m_shaderNew->SetVector("Material.Ka", Ka);
	m_shaderNew->SetVector("Material.Kd", Kd);
	m_shaderNew->SetVector("Material.Ks", Ks);
	m_shaderNew->SetFloat("Material.Shiness", shiness);

	m_shaderNew->SetTexture("ColorTex", 0);
	m_shaderNew->SetTexture("NormalMapTex", 1);

	m_bs->draw();

	m_shaderNew->UnbindProgram();
}

MyGlWindow::~MyGlWindow()
{

}


void MyGlWindow::initialize()
{	//초기화하는 것들은 다 여기 몰아놈, 큐브도 여기서 만들고 쉐이더도 여기서 만들고 ...
	
	
	m_bs = std::make_unique<MeshBs>();
	
	try {
		m_shaderNew = std::unique_ptr<Program>(Program::GenerateFromFileVsFs("shaders/simple.vert", "shaders/simple.frag"));
	}
	catch (const std::runtime_error & e) {
		std::cerr << "Shader Error : " << e.what() << std::endl;
		std::cerr << "Failed to load shader" << std::endl;
		exit(1);
	}
	
	
}

