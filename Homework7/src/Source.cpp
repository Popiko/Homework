#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// glm
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


// GLEW
#define GLEW_STATIC
#include <GL/glew.h>


// GLFW
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <string.h>

#include <stdlib.h>
#include <algorithm>
#include <math.h>

// SOIL
#include <SOIL.h>

#define WIDTH 1600
#define HEIGHT 1200

using namespace std;

// KEYPOINT
#include "Shader.h"
#include "Camera.h"

// ==================== homework 3 function ============================
// rasterization
typedef struct tri_point
{
	int x;
	int y;
	tri_point(int x_in, int y_in) {
		x = x_in;
		y = y_in;
	}
}tri_point;
bool compareByY(const tri_point& a, const tri_point& b);
vector<float> rasterization(GLint* p1, GLint* p2, GLint* p3);
void rasterization_downtri(vector<float>& points, tri_point p1, tri_point p2, tri_point p3);
void rasterization_uptri(vector<float>& points, tri_point p1, tri_point p2, tri_point p3);
vector<float> edgeEquationRasterization(GLint* p1, GLint* p2, GLint* p3);
int edgeEquation(int x, int y, GLint* p1, GLint* p2);

// draw lines & circle
vector<float> calculateLinePoints(GLint* p1, GLint* p2);
vector<float> calculateCirclePoints(GLint* p, GLint r);
void calculate8points(vector<float>& points, int x, int y, int xc, int yc);


// ==================== homework 4 function =================================
vector<float> calculateSphereParam(float radius, int sectorCount, int stackCount);
vector<int> calculateIndex(int stackCount, int sectorCount);

void processInput(GLFWwindow* window);
void processInputCameraClass(GLFWwindow* window, Camera& myCamera);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void key_callback(GLFWwindow* window, int key, int  scancode, int action, int mode);
void mouse_callback_camera_class(GLFWwindow* window, double xpos, double ypos);

// homework7 functions
GLuint loadTexture(string path);
void renderScene(const Shader& shader);
void renderCube();
void renderQuad();

// == homework5 camera parameters ==========
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f); // 为什么这个和pos加起来总是会注视目标方向啊
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float deltaTime = 0.0f; // 当前帧与上一帧
float lastFrame = 0.0f; // 上一帧的时间

bool firstMouse = true; //

float lastX = 800, lastY = 600;
float yaw = -90.0f;
float pitch = 0.0f;


// homework 7 global variables
GLuint woodTexture;
GLuint planeVAO;
const int NUM_CASCADES = 3;
const int NUM_FRUSTUM_CORNERS = 8;
typedef struct OrthoProjInfo {
	float r;
	float l;
	float b;
	float t;
	float f;
	float n;
}OrthoProjInfo;

float m_cascadeEnd[NUM_CASCADES + 1];


OrthoProjInfo orthoArray[NUM_CASCADES];
void CalcOrthoProjs(Camera myCamera, glm::mat4 lightViewMatrix, float Height, float Width, float FOV);


int main() {
	glfwInit();

	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGL的主版本号为3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // OpenGL的次版本号为3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 设置为核心模式
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // 不允许用户调整窗口大小

	GLFWwindow* window = glfwCreateWindow(1600, 1200, "HW2-ColorChooseTriangle", nullptr, nullptr);

	if (window == nullptr) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//glfwSetCursorPosCallback(window, mouse_callback);


	// 注册回调
	//glfwSetKeyCallback(window, key_callback);

	// GLEW是用来管理OpenGL的函数指针的，所以在调用任何OpenGL的函数之前要初始化GLEW

	// 将glewExperimental的变量的值设置为GL_TRUE，这样能让GLEW在管理OpenGL的函数指针时更多地使用现代化技术 ：）
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	// =================== 一些暂时用不到的数据 =========================
	/*GLfloat vertices[] = {  // 第三个数字是深度
		// location
		-0.5f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f

	};*/

	/*float vertices[] = {
		-0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 1.0f
	};*/

	float vertices[] = {
		-2.0f, -2.0f, -2.0f,  1.0f, 0.0f, 0.0f,       // 正方体背面 红色 
		 2.0f, -2.0f, -2.0f,  1.0f, 0.0f, 0.0f,
		 2.0f,  2.0f, -2.0f,  1.0f, 0.0f, 0.0f,
		 2.0f,  2.0f, -2.0f,  1.0f, 0.0f, 0.0f,
		-2.0f,  2.0f, -2.0f,  1.0f, 0.0f, 0.0f,
		-2.0f, -2.0f, -2.0f,  1.0f, 0.0f, 0.0f,

		-2.0f, -2.0f,  2.0f,  0.0f, 1.0f, 0.0f,       // 正方体前面 绿色
		 2.0f, -2.0f,  2.0f,  0.0f, 1.0f, 0.0f,
		 2.0f,  2.0f,  2.0f,  0.0f, 1.0f, 0.0f,
		 2.0f,  2.0f,  2.0f,  0.0f, 1.0f, 0.0f,
		-2.0f,  2.0f,  2.0f,  0.0f, 1.0f, 0.0f,
		-2.0f, -2.0f,  2.0f,  0.0f, 1.0f, 0.0f,

		-2.0f,  2.0f,  2.0f,  0.0f, 0.0f, 1.0f,       // 正方体左面 蓝色
		-2.0f,  2.0f, -2.0f,  0.0f, 0.0f, 1.0f,
		-2.0f, -2.0f, -2.0f,  0.0f, 0.0f, 1.0f,
		-2.0f, -2.0f, -2.0f,  0.0f, 0.0f, 1.0f,
		-2.0f, -2.0f,  2.0f,  0.0f, 0.0f, 1.0f,
		-2.0f,  2.0f,  2.0f,  0.0f, 0.0f, 1.0f,

		 2.0f,  2.0f,  2.0f,  1.0f, 1.0f, 0.0f,       // 正方体右面 黄色
		 2.0f,  2.0f, -2.0f,  1.0f, 1.0f, 0.0f,
		 2.0f, -2.0f, -2.0f,  1.0f, 1.0f, 0.0f,
		 2.0f, -2.0f, -2.0f,  1.0f, 1.0f, 0.0f,
		 2.0f, -2.0f,  2.0f,  1.0f, 1.0f, 0.0f,
		 2.0f,  2.0f,  2.0f,  1.0f, 1.0f, 0.0f,

		-2.0f, -2.0f, -2.0f,  1.0f, 0.0f, 1.0f,       // 正方体底面 品红（粉红）色
		 2.0f, -2.0f, -2.0f,  1.0f, 0.0f, 1.0f,
		 2.0f, -2.0f,  2.0f,  1.0f, 0.0f, 1.0f,
		 2.0f, -2.0f,  2.0f,  1.0f, 0.0f, 1.0f,
		-2.0f, -2.0f,  2.0f,  1.0f, 0.0f, 1.0f,
		-2.0f, -2.0f, -2.0f,  1.0f, 0.0f, 1.0f,

		-2.0f,  2.0f, -2.0f,  0.0f, 1.0f, 1.0f,       // 正方体上面 青色
		 2.0f,  2.0f, -2.0f,  0.0f, 1.0f, 1.0f,
		 2.0f,  2.0f,  2.0f,  0.0f, 1.0f, 1.0f,
		 2.0f,  2.0f,  2.0f,  0.0f, 1.0f, 1.0f,
		-2.0f,  2.0f,  2.0f,  0.0f, 1.0f, 1.0f,
		-2.0f,  2.0f, -2.0f,  0.0f, 1.0f, 1.0f
	};
	float vertices_hw5[] = {
	-3.5f, -1.5f, -3.5f, 1.0f, 0.0f, 0.0f,
	0.5f, -1.5f, -3.5f, 1.0f, 0.0f, 0.0f,
	0.5f, 2.5f, -3.5f, 1.0f, 0.0f, 0.0f,
	0.5f, 2.5f, -3.5f, 1.0f, 0.0f, 0.0f,
	-3.5f, 2.5f, -3.5f, 1.0f, 0.0f, 0.0f,
	-3.5f, -1.5f, -3.5f, 1.0f, 0.0f, 0.0f,

	-3.5f, -1.5f, 0.5f, 0.0f, 1.0f, 0.0f,
	0.5f, -1.5f, 0.5f, 0.0f, 1.0f, 0.0f,
	0.5f, 2.5f, 0.5f, 0.0f, 1.0f, 0.0f,
	0.5f, 2.5f, 0.5f, 0.0f, 1.0f, 0.0f,
	-3.5f, 2.5f, 0.5f, 0.0f, 1.0f, 0.0f,
	-3.5f, -1.5f, 0.5f, 0.0f, 1.0f, 0.0f,

	-3.5f, 2.5f, 0.5f, 0.0f, 0.0f, 1.0f,
	-3.5f, 2.5f, -3.5f, 0.0f, 0.0f, 1.0f,
	-3.5f, -1.5f, -3.5f, 0.0f, 0.0f, 1.0f,
	-3.5f, -1.5f, -3.5f, 0.0f, 0.0f, 1.0f,
	-3.5f, -1.5f, 0.5f, 0.0f, 0.0f, 1.0f,
	-3.5f, 2.5f, 0.5f, 0.0f, 0.0f, 1.0f,

	0.5f, 2.5f, 0.5f, 1.0f, 1.0f, 0.0f,
	0.5f, 2.5f, -3.5f, 1.0f, 1.0f, 0.0f,
	0.5f, -1.5f, -3.5f, 1.0f, 1.0f, 0.0f,
	0.5f, -1.5f, -3.5f, 1.0f, 1.0f, 0.0f,
	0.5f, -1.5f, 0.5f, 1.0f, 1.0f, 0.0f,
	0.5f, 2.5f, 0.5f, 1.0f, 1.0f, 0.0f,

	-3.5f, -1.5f, -3.5f, 1.0f, 0.0f, 1.0f,
	0.5f, -1.5f, -3.5f, 1.0f, 0.0f, 1.0f,
	0.5f, -1.5f, 0.5f, 1.0f, 0.0f, 1.0f,
	0.5f, -1.5f, 0.5f, 1.0f, 0.0f, 1.0f,
	-3.5f, -1.5f, 0.5f, 1.0f, 0.0f, 1.0f,
	-3.5f, -1.5f, -3.5f, 1.0f, 0.0f, 1.0f,

	-3.5f, 2.5f, -3.5f, 0.0f, 1.0f, 1.0f,
	0.5f, 2.5f, -3.5f, 0.0f, 1.0f, 1.0f,
	0.5f, 2.5f, 0.5f, 0.0f, 1.0f, 1.0f,
	0.5f, 2.5f, 0.5f, 0.0f, 1.0f, 1.0f,
	-3.5f, 2.5f, 0.5f, 0.0f, 1.0f, 1.0f,
	-3.5f, 2.5f, -3.5f, 0.0f, 1.0f, 1.0f

	};
	GLfloat vertices2[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f
	};

	GLfloat test1[] = {
		0.0f, 0.0f, 0.0f,           1.0f, 0.0f, 0.0f
	};

	//========================================================================


	// ===========  homework 4 texture =======================================
	GLuint texture1;

	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	// set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	int texWidth, texHeight;
	unsigned char* image = SOIL_load_image("moon1024.bmp", &texWidth, &texHeight, 0, SOIL_LOAD_RGB);

	// attrib1 -> target
	// attrib2 -> 多级渐远纹理的级别
	// attrib3 -> 把纹理储存为何种格式
	// attrib4 -> width
	// attrib5 -> height
	// attrib6 -> 0
	// attrib7 -> 源图格式
	// attrib8 -> 源图数据类型
	// attrib9 -> 图像数据
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	//===========================================================================

	GLuint VBO;
	glGenBuffers(1, &VBO);



	// KEYPOINT
	//Shader newShader("shader.vs", "shader.frag");
	Shader ourShader("shader.vs", "shader.frag");
	Shader lightShader("lightShader.vs", "lightShader.frag");

	Camera myCamera(45.0f, (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f, cameraPos, cameraFront, cameraUp, ourShader);

	glfwSetWindowUserPointer(window, &myCamera); ///!!!

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);


	bool show_demo_window = true;
	ImVec4 triangle_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	int choose = 1;
	GLfloat points2[] = {
		0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f };



	// 创建一个VAO
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);



	// 不可以使用sizeof(data)
	glBufferData(GL_ARRAY_BUFFER, sizeof(points2), points2, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);


	int r = 50; // -> radius


	GLint p1[] = { 0, 400 };
	GLint p2[] = { 700, 800 };
	GLint p3[] = { 400, 200 };
	rasterization(p1, p2, p3);


	//rasterization(p1, p2, p3);
	GLfloat ppp1[] = { 0.5f, 0.0f };

	bool ras_flag = false; // -> rasterization flag 
	bool ras_walk = false; // -> rasterization method flag

	int point1_x = 0;
	int point2_x = 0;
	int point3_x = 0;

	int point1_y = 0;
	int point2_y = 0;
	int point3_y = 0;



	// =================================== homework 4 transform (practice)===========================
	// 把一个向量(1,0,0)位移(1,1,0)个单位
	// 先用glm内建的向量类定义一个叫做vec的向量
	glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);

	// 矩阵初始化
	// 接下来定义一个mat4类型的trans
	glm::mat4 trans = glm::mat4(1.0f);

	// 创建一个变换矩阵
	// 把单位矩阵和一个位移向量传递给glm::translate函数
	trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
	vec = trans * vec;
	std::cout << vec.x << vec.y << vec.z << std::endl;


	glm::mat4 trans2 = glm::mat4(1.0f);
	// 每个轴都缩放0.5倍，然后沿z轴旋转90度
	// glm::radians 角度 -》 弧度
	trans2 = glm::rotate(trans2, glm::radians(45.0f), glm::vec3(0.0, 0.0, 1.0));
	trans2 = glm::scale(trans2, glm::vec3(0.5, 0.5, 0.5));


	// 空间变换 
	// 正射矩阵
	// attrib 1 & 2 -> 平截头体的左右坐标
	// attrib 3 & 4 -> 平截头体的顶部和底部
	// attrib 5 & 6 -> 近平面和远平面的距离
	glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);

	// 透视矩阵
	// attrib1 -> fov(field of view) 视野
	// attrib2 -> 宽高比 视口的宽除以高所得 视口是指前面的那个？？？
	// attrib 3 & 4 -> 远近平面
	glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);

	// ===============================================================================



	// ========= homework 4 some parameters =========================================
	static bool my_tool_active = true;
	static bool show_app_main_menu_bar = false;
	static bool homework4 = false;
	static bool moon = false;
	bool depth_test_flag = false;
	static int selected_transformation = -1;
	glm::mat4 modelstore = glm::mat4(1.0f);
	float recordZ = 0.0f;
	float recordtime = 0.0f;
	float basetime = 0.0f;
	// ===============================================================================

	// ========= homework 5 parameters =============================================
	static bool homework5_projection = false;
	static bool homework5_view_changing = false;
	static bool homework5_move_freely = false;
	static bool homework5_camera_class = false;
	static int selected_projection = -1;
	float ortho_left = -5.0f;
	float ortho_right = 5.0f;
	float ortho_top = 5.0f;
	float ortho_bottom = -5.0f;
	float ortho_near = 0.1f;
	float ortho_far = 100.0f;

	float persp_radians = 45.0f;
	float persp_width = WIDTH;
	float persp_height = HEIGHT;
	float persp_near = 0.1f;
	float persp_far = 100.0f;

	// ========= homework 4 generate points of a sphere ==============================
	auto testv = calculateSphereParam(1.0f, 100, 100);
	float* sphereVerticesData2 = testv.data();
	size_t sphereVerticesData_len2 = testv.size() * sizeof(float);

	auto test_index = calculateIndex(100, 100);
	int* sphereIndicesData2 = test_index.data();
	size_t sphereIndicesData_len2 = test_index.size() * sizeof(int);

	GLuint iboId;
	glGenBuffers(1, &iboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndicesData_len2, sphereIndicesData2, GL_STATIC_DRAW);
	// ===================================================================================================


	// =================== homework 6 parameters =====================================
	int choose_index = 0;
	GLfloat vertices_hw6[] = {
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f
	};
	float lightSource[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	float vertices_normal[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};
	// 全局光源
	//glm::vec3 lightPos(1.2f, 1.0f, 10.0f);
	// 使用着色器

	// 光源色和物体颜色的反射运算
	glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
	glm::vec3 toyColor(1.0f, 0.5f, 0.31f);
	glm::vec3 result = lightColor * toyColor; // = (1.0f, 0.5f, 0.31f)

	// 因子
	float ambientStrength = 0.1f;
	float diffuseStrength = 0.5f;
	float specularStrength = 0.5f;
	int nshiness = 32;

	int selected_shading = -1;




	GLuint lightVAO;
	GLuint lightVBO;
	glGenBuffers(1, &lightVBO);
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lightSource), lightSource, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	GLuint gouraudVAO;
	GLuint gouraudVBO;
	glGenBuffers(1, &gouraudVBO);
	glGenVertexArrays(1, &gouraudVAO);



	//////////////// hw7 ////////////////////////
	int selected_shadow = -1;
	float bias = 0.0f;
	bool front_culling_flag = false;
	int pcf_flag = 0;
	int deal_with_over_sample_flag = 0;
	int csm_flag = 0;
	bool debug = false;
	m_cascadeEnd[0] = 1.0f;
	m_cascadeEnd[1] = 25.0f,
	m_cascadeEnd[2] = 70.0f,
	m_cascadeEnd[3] = 100.5f;

	/*m_cascadeEnd[0] = 1.0f;
	m_cascadeEnd[1] = 25.0f,
	m_cascadeEnd[2] = 90.0f,
	m_cascadeEnd[3] = 200.5f;*/
	


	Shader shadowShader("shadow_mapping.vs", "shadow_mapping.frag");
	Shader simpleDepthShader("simpleDepthShader.vs", "simpleDepthShader.frag");
	Shader debugDepthQuad("debug_quad.vs", "debug_quad.frag");

	// 设置纹理
	shadowShader.Use();
	GLuint diffuseTextureLoc = glGetUniformLocation(shadowShader.Program, "diffuseTexture");
	GLuint shadowMapLocation = glGetUniformLocation(shadowShader.Program, "shadowMap");
	GLuint shadowMap_CSM_0_Loc = glGetUniformLocation(shadowShader.Program, "shadowMap_CSM[0]");
	GLuint shadowMap_CSM_1_Loc = glGetUniformLocation(shadowShader.Program, "shadowMap_CSM[1]");
	GLuint shadowMap_CSM_2_Loc = glGetUniformLocation(shadowShader.Program, "shadowMap_CSM[2]");
	glUniform1i(diffuseTextureLoc, 0); // 纹理单元0
	glUniform1i(shadowMapLocation, 1); // 纹理单元1
	glUniform1i(shadowMap_CSM_0_Loc, 2); // 纹理单元2
	glUniform1i(shadowMap_CSM_1_Loc, 3); // 纹理单元3
	glUniform1i(shadowMap_CSM_2_Loc, 4); // 纹理单元4


	// 平面顶点坐标
	float planeVertices[] = {
		// positions            // normals         // texcoords
		 25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
		-25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
		-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

		 25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
		-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
		 25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
	};
	// plane VAO
	unsigned int planeVBO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glBindVertexArray(0);

	glm::vec3 lightPos(-2.0f, 4.0f, -1.0f);

	
	// 纹理
	string str = "wall.jpg";
	GLuint woodTexture = loadTexture(str);

	// configure depth map FBO
	// -----------------------
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	


	/// csm
	unsigned int depthMapFBO_CSM;
	glGenFramebuffers(1, &depthMapFBO_CSM);

	unsigned int depthMap_CSM[3];
	glGenTextures(3, depthMap_CSM);
	for (int i = 0; i < 3; i++) {
		glBindTexture(GL_TEXTURE_2D, depthMap_CSM[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO_CSM);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap_CSM[0], 0);  // 为什么这里只用[0]

	/////////////////////////////////////////////////////////////////
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	/*
	// shader configuration
	// --------------------
	debugDepthQuad.Use();
	GLuint depthMapLoc = glGetUniformLocation(debugDepthQuad.Program, "depthMap");
	glUniform1i(depthMapLoc, 0);
	//debugDepthQuad.setInt("depthMap", 0);

	// lighting info
	// -------------
	glm::vec3 lightPos(-2.0f, 4.0f, -1.0f);
	*/


	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);







	while (!glfwWindowShouldClose(window)) {
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();


		// ========= ImGui =======================================================================================
		{
			// ======================== 工具栏 ==========================================
			ImGui::Begin("My First Tool", &my_tool_active, ImGuiWindowFlags_MenuBar);

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("hw4", "Ctrl+O")) {
						homework4 = !homework4;
						moon = false;
						homework5_projection = false;
						homework5_view_changing = false;
						homework5_move_freely = false;
						homework5_camera_class = false;
					}
					if (ImGui::MenuItem("moon", "Ctrl+F")) {
						homework4 = false;
						moon = !moon;
						homework5_projection = false;
						homework5_view_changing = false;
						homework5_move_freely = false;
						homework5_camera_class = false;

					}
					if (ImGui::MenuItem("homework5-projection")) {
						homework5_projection = true;
						homework4 = false;
						moon = false;
						homework5_view_changing = false;
						homework5_move_freely = false;
						homework5_camera_class = false;
					}
					if (ImGui::MenuItem("homework5-view-changing")) {
						homework4 = false;
						homework5_projection = false;
						moon = false;
						homework5_view_changing = true;
						homework5_move_freely = false;
						homework5_camera_class = false;
					}
					if (ImGui::MenuItem("homework5-move-freely")) {
						homework4 = false;
						homework5_projection = false;
						moon = false;
						homework5_view_changing = false;
						homework5_camera_class = false;
						homework5_move_freely = true;
					}
					if (ImGui::MenuItem("homework5-camera-class")) {
						homework4 = false;
						homework5_projection = false;
						moon = false;
						homework5_view_changing = false;
						homework5_move_freely = false;
						homework5_camera_class = true;
					}
					if (ImGui::MenuItem("homework6-playgorund")) {
						choose_index = 6;
						homework4 = false;
						homework5_projection = false;
						moon = false;
						homework5_view_changing = false;
						homework5_move_freely = false;
						homework5_camera_class = false;
					}
					if (ImGui::MenuItem("homework7-playgorund")) {
						choose_index = 7;
						homework4 = false;
						homework5_projection = false;
						moon = false;
						homework5_view_changing = false;
						homework5_move_freely = false;
						homework5_camera_class = false;
					}
					if (ImGui::MenuItem("homework7-csm")) {
						choose_index = 8;
						homework4 = false;
						homework5_projection = false;
						moon = false;
						homework5_view_changing = false;
						homework5_move_freely = false;
						homework5_camera_class = false;
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}
			// ==============================================================================


			// ========= homework 3 ImGui =============================
			// 调整三角形 
			ImGui::SliderInt("X1", &point1_x, 0, 1600);
			ImGui::SliderInt("Y1", &point1_y, 0, 1200);
			ImGui::SliderInt("X2", &point2_x, 0, 1600);
			ImGui::SliderInt("Y2", &point2_y, 0, 1200);
			ImGui::SliderInt("X3", &point3_x, 0, 1600);
			ImGui::SliderInt("Y3", &point3_y, 0, 1200);

			// 光栅化三角形
			ImGui::Checkbox("Rasterization", &ras_flag);

			// 颜色选择
			ImGui::ColorEdit3("triangle color", (float*)& triangle_color);

			// 调整半径
			ImGui::SliderInt("R", &r, 0, 1000);

			// 调整图元
			if (ImGui::Button("Switch primitive")) {
				choose = -choose;
			}

			// 调整光栅化方法
			if (ImGui::Button("Switch rasterization method")) {
				ras_walk = !ras_walk;

			}
			// =======================================================


			// ========= homework 4 ImGui ========================================================================
			// moon flag -> 画跳跃月球还是画正方体
			GLint moonFlagLocation = glGetUniformLocation(ourShader.Program, "moon");
			glUniform1i(moonFlagLocation, moon);

			// 开启深度测试
			if (ImGui::Button("Depth test")) {
				depth_test_flag = !depth_test_flag;
				if (depth_test_flag) {
					glEnable(GL_DEPTH_TEST);
				}
				else {
					glDisable(GL_DEPTH_TEST);
				}
			}

			// 选择transformation工具栏
			ImGui::TextWrapped("Please select a transformation");
			const char* names[] = { "Translation", "Rotation", "Scaling", "Still" };
			if (ImGui::Button("Select.."))
				ImGui::OpenPopup("my_select_popup");
			ImGui::SameLine();
			ImGui::TextUnformatted(selected_transformation == -1 ? "<None>" : names[selected_transformation]);
			if (ImGui::BeginPopup("my_select_popup"))
			{
				ImGui::Text("Transformation");
				ImGui::Separator();
				for (int i = 0; i < IM_ARRAYSIZE(names); i++)
					if (ImGui::Selectable(names[i])) {
						selected_transformation = i;
					}
				ImGui::EndPopup();
			}

			// 如果设置为显示homework5_projection 工具栏才会显示
			if (homework5_projection) {

				// 选择投影方式
				ImGui::TextWrapped("Please select a projection");
				const char* names2[] = { "Orthographic", "Perspective" };
				if (ImGui::BeginPopup("my_select_popup2"))
				{
					ImGui::Text("Projection");
					ImGui::Separator();
					for (int i = 0; i < IM_ARRAYSIZE(names2); i++)
						if (ImGui::Selectable(names2[i])) {
							selected_projection = i;
						}
					ImGui::EndPopup();
				}
				if (ImGui::Button("Select projection.."))
					ImGui::OpenPopup("my_select_popup2");
				ImGui::SameLine();
				ImGui::TextUnformatted(selected_projection == -1 ? "<None>" : names2[selected_projection]);

				// 参数设置
				glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
				if (selected_projection == 0) {

					ImGui::SliderFloat("Left", &ortho_left, -10.0f, ortho_right);
					ImGui::SliderFloat("Right", &ortho_right, ortho_left, 1600.0f);
					ImGui::SliderFloat("Top", &ortho_top, ortho_bottom, 1200.0f);
					ImGui::SliderFloat("Bottom", &ortho_bottom, -10.0f, ortho_top);
					ImGui::SliderFloat("Near", &ortho_near, -100.0f, ortho_far);
					ImGui::SliderFloat("Far", &ortho_far, ortho_near, 1000.0f);

				}
				else if (selected_projection == 1) {

					ImGui::SliderFloat("Radians", &persp_radians, 0.0f, 360.0f);
					ImGui::SliderFloat("Width", &persp_width, 0.0f, WIDTH);
					ImGui::SliderFloat("Height", &persp_height, 0.0f, HEIGHT);
					ImGui::SliderFloat("Near", &persp_near, 0.0f, 1200.0f);
					ImGui::SliderFloat("Far", &persp_far, 0.0f, 2000.0f);

				}

			}

			// homework 7
			if (choose_index == 7) {
				//cout << "hw7" << endl;
				ImGui::TextWrapped("Please select a projection model");
				const char* names2[] = { "Ortho", "Perspective" };
				if (ImGui::BeginPopup("my_select_popup2"))
				{
					ImGui::Text("Projection model");
					ImGui::Separator();
					for (int i = 0; i < IM_ARRAYSIZE(names2); i++)
						if (ImGui::Selectable(names2[i])) {
							selected_shadow = i;
						}
					ImGui::EndPopup();
				}
				if (ImGui::Button("Select projection model.."))
					ImGui::OpenPopup("my_select_popup2");
				ImGui::SameLine();
				ImGui::TextUnformatted(selected_shadow == -1 ? "<None>" : names2[selected_shadow]);

				// 进度条 更改因子
				ImGui::SliderFloat("bias", &bias, 0.0f, 1.0f);
				ImGui::SliderFloat("diffuseStrength", &diffuseStrength, 0.0f, 1.0f);
				ImGui::SliderFloat("specularStrength", &specularStrength, 0.0f, 1.0f);
				ImGui::SliderInt("nshiness", &nshiness, 0, 256);

				if (ImGui::Button("Front Culling")) {
					front_culling_flag = !front_culling_flag;
				}
				if (ImGui::Button("PCF")) {
					pcf_flag = abs(1 - pcf_flag);
				}
				if (ImGui::Button("Deal with over sample")) {
					deal_with_over_sample_flag = abs(1 - deal_with_over_sample_flag);
				}
				if (ImGui::Button("CSM")) {
					csm_flag = abs(1 - csm_flag);
				}
				if (ImGui::Button("debug")) {
					debug = !debug;
				}

			}
			// homework 6
			else if (choose_index == 6) {
				// 使光源在场景中来回移动
				lightPos.x = 1.0f + sin(glfwGetTime()) * 2.0f;
				lightPos.y = sin(glfwGetTime() / 2.0f) * 1.0f;
				// 选择Phong光照或者gouraud光照
				ImGui::TextWrapped("Please select a lighting model");
				const char* names2[] = { "Phong", "Gouraud" };
				if (ImGui::BeginPopup("my_select_popup2"))
				{
					ImGui::Text("Lighting model");
					ImGui::Separator();
					for (int i = 0; i < IM_ARRAYSIZE(names2); i++)
						if (ImGui::Selectable(names2[i])) {
							selected_shading = i;
						}
					ImGui::EndPopup();
				}
				if (ImGui::Button("Select lighting model.."))
					ImGui::OpenPopup("my_select_popup2");
				ImGui::SameLine();
				ImGui::TextUnformatted(selected_shading == -1 ? "<None>" : names2[selected_shading]);

				// 进度条 更改因子
				ImGui::SliderFloat("ambientStrength", &ambientStrength, 0.0f, 1.0f);
				ImGui::SliderFloat("diffuseStrength", &diffuseStrength, 0.0f, 1.0f);
				ImGui::SliderFloat("specularStrength", &specularStrength, 0.0f, 1.0f);
				ImGui::SliderInt("nshiness", &nshiness, 0, 256);

				// 参数设置
				// Phong
				if (selected_shading == 0) {



				}
				else if (selected_shading == 1) {



				}

			}

			ImGui::End();
			// ==========================================================================================================

			p1[0] = point1_x;
			p1[1] = point1_y;
			p2[0] = point2_x;
			p2[1] = point2_y;
			p3[0] = point3_x;
			p3[1] = point3_y;
		}
		// ========================================================================================================

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		// glClear接受一个缓冲位来指定要清空的缓冲 GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT, GL_STENCIL_BUFFER_BIT
		// 这里只关心颜色值，所以只清楚颜色缓冲
		//glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// record the time
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		// 渲染指令
		// 渲染迭代开始时我们要清屏，不然我们总是会看见上一次迭代的渲染结果
		//glClearColor(0.7f, 0.58f, 0.57f, 1.0f); // 设置清空屏幕所用的颜色，在下面调用glClear后，整个颜色缓冲会被填充为glClearColor里所设置的颜色


		// ========= homework 4 transform matrix =========
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);

		//Camera myCamera;

		//processInput(window);
		//processInput(window, myCamera);









		// ========= homework 4 各种transformation + 跳跃月球动画 =================================================================================

		// homework 7
		if (choose_index == 7) {
			shadowShader.Use();
			GLuint csm_flag_vs_loc = glGetUniformLocation(shadowShader.Program, "csm_flag_vs");
			GLuint csm_flag_fs_loc = glGetUniformLocation(shadowShader.Program, "csm_flag_fs");
			glUniform1i(csm_flag_fs_loc, csm_flag);
			glUniform1i(csm_flag_vs_loc, csm_flag);
			//glfwSetCursorPosCallback(window, mouse_callback);
			//processInputCameraClass(window, myCamera);
			glfwSetCursorPosCallback(window, mouse_callback_camera_class);
			processInputCameraClass(window, myCamera);
			glm::mat4 lightProjection, lightView;
			glm::mat4 lightSpaceMatrix;
			//float near_plane = 2.0f, far_plane = 20.5f;
			float near_plane = 2.0f, far_plane = 10.5f;
			if (csm_flag == 0) {
				
				if (selected_shadow == 0) {
					lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
				}
				else if (selected_shadow == 1) {
					lightProjection = glm::perspective(glm::radians(75.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane);
				}
				//lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
				/// ???
				//lightProjection = glm::perspective(glm::radians(75.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane);
				//glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
				//lightProjection = glm::perspective(glm::radians(45.0f), (float)SHADOW_WIDTH/(float)SHADOW_HEIGHT, near_plane, far_plane);
				lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
				lightSpaceMatrix = lightProjection * lightView;
				// render scene from light's point of view
				simpleDepthShader.Use();
				//simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
				GLuint lightSpaceMatrixLoc = glGetUniformLocation(simpleDepthShader.Program, "lightSpaceMatrix");
				glUniformMatrix4fv(lightSpaceMatrixLoc, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));


				glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
				glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
				glClear(GL_DEPTH_BUFFER_BIT);
				//glActiveTexture(GL_TEXTURE0);
				//glBindTexture(GL_TEXTURE_2D, woodTexture);
				//glCullFace(GL_FRONT);
				if (front_culling_flag) {
					glCullFace(GL_FRONT);
					cout << "Front" << endl;
				}
				renderScene(simpleDepthShader);
				//glCullFace(GL_BACK);

				glBindFramebuffer(GL_FRAMEBUFFER, 0);

				// reset viewport
				glViewport(0, 0, WIDTH, HEIGHT);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				// render Depth map to quad for visual debugging
				// ---------------------------------------------
				//debugDepthQuad.Use();
				shadowShader.Use();

				glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 1.0f, 100.0f);
				glm::mat4 view = myCamera.getViewMatrix();
				glUniformMatrix4fv(glGetUniformLocation(shadowShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
				glUniformMatrix4fv(glGetUniformLocation(shadowShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

				// light uniforms
				glUniform3fv(glGetUniformLocation(shadowShader.Program, "lightPos"), 1, &lightPos[0]);
				glUniform3fv(glGetUniformLocation(shadowShader.Program, "viewPos"), 1, glm::value_ptr(myCamera.getCameraPos()));
				glUniformMatrix4fv(glGetUniformLocation(shadowShader.Program, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

				// bias
				GLuint biasLoc = glGetUniformLocation(shadowShader.Program, "bias");
				glUniform1f(biasLoc, bias);

				// pcf
				GLuint pcfLoc = glGetUniformLocation(shadowShader.Program, "pcf_flag");
				glUniform1i(pcfLoc, pcf_flag);

				// over sample
				GLuint overSampleLoc = glGetUniformLocation(shadowShader.Program, "over_sample_flag");
				glUniform1i(overSampleLoc, deal_with_over_sample_flag);



				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, woodTexture);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, depthMap);
				//glCullFace(GL_FRONT);
				glCullFace(GL_BACK);
				renderScene(shadowShader);


				// visualize depth map by rendering it to plane
				debugDepthQuad.Use();
				GLuint nearPlaneLoc = glGetUniformLocation(debugDepthQuad.Program, "near_plane");
				GLuint farPlaneLoc = glGetUniformLocation(debugDepthQuad.Program, "far_plane");
				glUniform1f(nearPlaneLoc, near_plane);
				glUniform1f(farPlaneLoc, far_plane);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, depthMap);
				if (debug) {
					renderQuad();
				}

			}
			else {
				//glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
				glm::mat4 lightSpaceMatrix_CSM[NUM_CASCADES];
				simpleDepthShader.Use();
				GLuint lightSpaceMatrixLoc = glGetUniformLocation(simpleDepthShader.Program, "lightSpaceMatrix");
				
				

				//lightView = glm::lookAt(lightPos, glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0.0, 1.0, 0.0));
				lightView = glm::lookAt(lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0, 1.0, 0.0));
				CalcOrthoProjs(myCamera, lightView, (float)HEIGHT, (float)WIDTH, 90.0f);
				
				// 使用计算出的三个cascade做ortho投影
				for (int i = 0; i < NUM_CASCADES; i++) {
					lightProjection = glm::ortho(orthoArray[i].l, orthoArray[i].r, orthoArray[i].b, orthoArray[i].t, orthoArray[i].n, orthoArray[i].f);
					//cout << "1 " << lightProjection[0][0] << endl;
					lightSpaceMatrix = lightProjection * lightView;

					// 把每一个部分的lightSpaceMatrix储存在lightSpaceMatrix_CSM里面
					lightSpaceMatrix_CSM[i] = lightSpaceMatrix;

					glUniformMatrix4fv(lightSpaceMatrixLoc, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

					
					
					// bind
					glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
					glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO_CSM); 
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap_CSM[i], 0);
					
					glClear(GL_DEPTH_BUFFER_BIT);


					renderScene(simpleDepthShader);
					
					glBindFramebuffer(GL_FRAMEBUFFER, 0);
				}

				// RESET VIEWPORT
				glViewport(0, 0, WIDTH, HEIGHT);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


				shadowShader.Use();

				glm::mat4 projection = glm::perspective(glm::radians(75.0f), (float)WIDTH / (float)HEIGHT, 1.0f, 100.0f);
				glm::mat4 view = myCamera.getViewMatrix();
				glUniformMatrix4fv(glGetUniformLocation(shadowShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
				glUniformMatrix4fv(glGetUniformLocation(shadowShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
				
				glUniform1fv(glGetUniformLocation(shadowShader.Program, "gCascadeEndClipSpace"), 4, m_cascadeEnd);
				// light uniforms
				glUniform3fv(glGetUniformLocation(shadowShader.Program, "lightPos"), 1, &lightPos[0]);
				glUniform3fv(glGetUniformLocation(shadowShader.Program, "viewPos"), 1, glm::value_ptr(myCamera.getCameraPos()));
				//glUniformMatrix4fv(glGetUniformLocation(shadowShader.Program, "lightSpaceMatrix_CSM"), NUM_CASCADES, GL_FALSE, glm::value_ptr(lightSpaceMatrix_CSM[0])); // 不知对不对
				glUniformMatrix4fv(glGetUniformLocation(shadowShader.Program, "lightSpaceMatrix_CSM[0]"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix_CSM[0]));
				glUniformMatrix4fv(glGetUniformLocation(shadowShader.Program, "lightSpaceMatrix_CSM[1]"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix_CSM[1]));
				glUniformMatrix4fv(glGetUniformLocation(shadowShader.Program, "lightSpaceMatrix_CSM[2]"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix_CSM[2]));

				// bias
				GLuint biasLoc = glGetUniformLocation(shadowShader.Program, "bias");
				glUniform1f(biasLoc, bias);

				// pcf
				GLuint pcfLoc = glGetUniformLocation(shadowShader.Program, "pcf_flag");
				glUniform1i(pcfLoc, pcf_flag);

				// over sample
				GLuint overSampleLoc = glGetUniformLocation(shadowShader.Program, "over_sample_flag");
				glUniform1i(overSampleLoc, deal_with_over_sample_flag);



				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, woodTexture);
				glActiveTexture(GL_TEXTURE2);
				glBindTexture(GL_TEXTURE_2D, depthMap_CSM[0]);
				glActiveTexture(GL_TEXTURE3);
				glBindTexture(GL_TEXTURE_2D, depthMap_CSM[1]);
				glActiveTexture(GL_TEXTURE4);
				glBindTexture(GL_TEXTURE_2D, depthMap_CSM[2]);
				//glCullFace(GL_FRONT);
				glCullFace(GL_BACK);
				renderScene(shadowShader);

				/*debugDepthQuad.Use();
				GLuint nearPlaneLoc = glGetUniformLocation(debugDepthQuad.Program, "near_plane");
				GLuint farPlaneLoc = glGetUniformLocation(debugDepthQuad.Program, "far_plane");
				glUniform1f(nearPlaneLoc, near_plane);
				glUniform1f(farPlaneLoc, far_plane);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, depthMap);*/
				debugDepthQuad.Use();
				GLuint nearPlaneLoc = glGetUniformLocation(debugDepthQuad.Program, "near_plane");
				GLuint farPlaneLoc = glGetUniformLocation(debugDepthQuad.Program, "far_plane");
				glUniform1f(nearPlaneLoc, 1.0f);
				glUniform1f(farPlaneLoc, 200.0f);
				

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, depthMap_CSM[0]);
				//glUniform1i(glGetUniformLocation(debugDepthQuad.Program, "depthMap"), 0);
				

				
				cout << depthMap_CSM[0] << endl;
				if (debug) {
					renderQuad();
				}
				

			}

			

		}
		// homework6 光照
		else if (choose_index == 6) {
			
			processInputCameraClass(window, myCamera);

			lightShader.Use();
			glm::mat4 lightModel = glm::mat4(1.0f);
			lightModel = glm::translate(lightModel, lightPos);
			lightModel = glm::scale(lightModel, glm::vec3(0.1f));
			glm::mat4 lightView = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -20.0f)); // 摄像机向z正方向移动 == 场景向z的负方向移动
			glm::mat4 lightProjection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

			unsigned int lightModelLoc = glGetUniformLocation(lightShader.Program, "model");
			unsigned int lightViewLoc = glGetUniformLocation(lightShader.Program, "view");
			unsigned int lightProjectionLoc = glGetUniformLocation(lightShader.Program, "projection");
			// 变换矩阵
			glUniformMatrix4fv(lightModelLoc, 1, GL_FALSE, glm::value_ptr(lightModel));
			glUniformMatrix4fv(lightViewLoc, 1, GL_FALSE, glm::value_ptr(lightView));
			glUniformMatrix4fv(lightProjectionLoc, 1, GL_FALSE, glm::value_ptr(lightProjection));

			glBindVertexArray(lightVAO);
			glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(lightSource), lightSource, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);

			glDrawArrays(GL_TRIANGLES, 0, 36);


			model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -20.0f)); // 摄像机向z正方向移动 == 场景向z的负方向移动
			//projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
			glm::vec3 cameraPos = myCamera.getCameraPos();
			glm::vec3 cameraFront = myCamera.getCameraFront();

			unsigned int chooseFragLoc = glGetUniformLocation(ourShader.Program, "chooseFrag");
			unsigned int chooseVsLoc = glGetUniformLocation(ourShader.Program, "chooseVs");
			//cout << chooseFragLoc << " " << chooseVsLoc << endl;

			if (selected_shading == 0) {
				//glfwSetCursorPosCallback(window, mouse_callback_camera_class);
				//glfwSetCursorPosCallback(window, mouse_callback);

				// 颜色

				///// 物体

				ourShader.Use(); // 特别注意着色器使用的位置！！！
				glUniform1i(chooseFragLoc, selected_shading);
				glUniform1i(chooseVsLoc, selected_shading);


				unsigned int modelLoc = glGetUniformLocation(ourShader.Program, "model");
				unsigned int viewLoc = glGetUniformLocation(ourShader.Program, "view");
				unsigned int projectionLoc = glGetUniformLocation(ourShader.Program, "projection");

				unsigned int lightPosLoc = glGetUniformLocation(ourShader.Program, "lightPos");
				unsigned int viewPosLoc = glGetUniformLocation(ourShader.Program, "viewPos");

				// factor
				unsigned int ambientStrengthLoc = glGetUniformLocation(ourShader.Program, "ambientStrength");
				unsigned int diffuseStrengthLoc = glGetUniformLocation(ourShader.Program, "diffuseStrength");
				unsigned int specularStrengthLoc = glGetUniformLocation(ourShader.Program, "specularStrength");
				unsigned int nshinessLoc = glGetUniformLocation(ourShader.Program, "nshiness");

				//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				//glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
				//glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
				glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
				glUniform3f(viewPosLoc, cameraPos.x, cameraPos.y, cameraPos.z);
				//glUniform3f(viewPosLoc, cameraFront.x, cameraFront.y, cameraFront.z);
				glUniform1f(ambientStrengthLoc, ambientStrength);
				glUniform1f(diffuseStrengthLoc, diffuseStrength);
				glUniform1f(specularStrengthLoc, specularStrength);
				glUniform1i(nshinessLoc, nshiness);



				GLint objectColorLoc = glGetUniformLocation(ourShader.Program, "objectColor");
				GLint lightColorLoc = glGetUniformLocation(ourShader.Program, "lightColor");
				glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);
				glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);

				// 设置VAO VBO
				glBindVertexArray(VAO);
				glBindBuffer(GL_ARRAY_BUFFER, VBO);
				glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_normal), vertices_normal, GL_STATIC_DRAW);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
				glEnableVertexAttribArray(1);


				glDrawArrays(GL_TRIANGLES, 0, 36);

				//glBindVertexArray(0);


			}
			else if (selected_shading == 1) { // gouraud shading
				ourShader.Use(); // 特别注意着色器使用的位置！！！
				glUniform1i(chooseFragLoc, selected_shading);
				glUniform1i(chooseVsLoc, selected_shading);


				unsigned int modelLoc = glGetUniformLocation(ourShader.Program, "model");
				unsigned int viewLoc = glGetUniformLocation(ourShader.Program, "view");
				unsigned int projectionLoc = glGetUniformLocation(ourShader.Program, "projection");

				unsigned int lightPosLoc = glGetUniformLocation(ourShader.Program, "lightPosG");
				unsigned int viewPosLoc = glGetUniformLocation(ourShader.Program, "viewPosG");

				// factor
				unsigned int ambientStrengthLoc = glGetUniformLocation(ourShader.Program, "ambientStrengthG");
				unsigned int diffuseStrengthLoc = glGetUniformLocation(ourShader.Program, "diffuseStrengthG");
				unsigned int specularStrengthLoc = glGetUniformLocation(ourShader.Program, "specularStrengthG");
				unsigned int nshinessLoc = glGetUniformLocation(ourShader.Program, "nshinessG");

				//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				//glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
				//glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
				glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
				glUniform3f(viewPosLoc, cameraPos.x, cameraPos.y, cameraPos.z);
				//glUniform3f(viewPosLoc, cameraFront.x, cameraFront.y, cameraFront.z);
				glUniform1f(ambientStrengthLoc, ambientStrength);
				glUniform1f(diffuseStrengthLoc, diffuseStrength);
				glUniform1f(specularStrengthLoc, specularStrength);
				glUniform1i(nshinessLoc, nshiness);



				GLint objectColorLoc = glGetUniformLocation(ourShader.Program, "objectColor");
				GLint lightColorLoc = glGetUniformLocation(ourShader.Program, "lightColorG");
				glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);
				glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);

				// 设置VAO VBO
				glBindVertexArray(VAO);
				glBindBuffer(GL_ARRAY_BUFFER, VBO);
				glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_normal), vertices_normal, GL_STATIC_DRAW);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
				glEnableVertexAttribArray(1);


				glDrawArrays(GL_TRIANGLES, 0, 36);
				/*newShader.Use(); // 特别注意着色器使用的位置！！！

				unsigned int modelLoc = glGetUniformLocation(newShader.Program, "model");
				unsigned int viewLoc = glGetUniformLocation(newShader.Program, "view");
				unsigned int projectionLoc = glGetUniformLocation(newShader.Program, "projection");

				unsigned int lightPosLoc = glGetUniformLocation(newShader.Program, "lightPos");
				unsigned int viewPosLoc = glGetUniformLocation(newShader.Program, "viewPos");

				// factor
				unsigned int ambientStrengthLoc = glGetUniformLocation(newShader.Program, "ambientStrength");
				unsigned int diffuseStrengthLoc = glGetUniformLocation(newShader.Program, "diffuseStrength");
				unsigned int specularStrengthLoc = glGetUniformLocation(newShader.Program, "specularStrength");
				unsigned int nshinessLoc = glGetUniformLocation(newShader.Program, "nshiness");

				//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				//glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
				//glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
				glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
				glUniform3f(viewPosLoc, cameraPos.x, cameraPos.y, cameraPos.z);
				//glUniform3f(viewPosLoc, cameraFront.x, cameraFront.y, cameraFront.z);
				glUniform1f(ambientStrengthLoc, ambientStrength);
				glUniform1f(diffuseStrengthLoc, diffuseStrength);
				glUniform1f(specularStrengthLoc, specularStrength);
				glUniform1i(nshinessLoc, nshiness);



				GLint objectColorLoc = glGetUniformLocation(newShader.Program, "objectColor");
				GLint lightColorLoc = glGetUniformLocation(newShader.Program, "lightColor");
				glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);
				glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);

				// 设置VAO VBO
				glBindVertexArray(VAO);
				glBindBuffer(GL_ARRAY_BUFFER, VBO);
				glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_normal), vertices_normal, GL_STATIC_DRAW);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
				glEnableVertexAttribArray(1);


				glDrawArrays(GL_TRIANGLES, 0, 36);

				//glBindVertexArray(0);*/


















				//gouraudShader.Use(); // 特别注意着色器使用的位置！！！

				/*gouraudShader.Use();

				unsigned int modelLoc = glGetUniformLocation(gouraudShader.Program, "model");
				unsigned int viewLoc = glGetUniformLocation(gouraudShader.Program, "view");
				unsigned int projectionLoc = glGetUniformLocation(gouraudShader.Program, "projection");

				unsigned int lightPosLoc = glGetUniformLocation(gouraudShader.Program, "lightPos");
				unsigned int viewPosLoc = glGetUniformLocation(gouraudShader.Program, "viewPos");

				// factor
				unsigned int ambientStrengthLoc = glGetUniformLocation(gouraudShader.Program, "ambientStrength");
				unsigned int diffuseStrengthLoc = glGetUniformLocation(gouraudShader.Program, "diffuseStrength");
				unsigned int specularStrengthLoc = glGetUniformLocation(gouraudShader.Program, "specularStrength");
				unsigned int nshinessLoc = glGetUniformLocation(gouraudShader.Program, "nshiness");

				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				//glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
				//glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
				glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
				glUniform3f(viewPosLoc, cameraPos.x, cameraPos.y, cameraPos.z);
				//glUniform3f(viewPosLoc, cameraFront.x, cameraFront.y, cameraFront.z);
				glUniform1f(ambientStrengthLoc, ambientStrength);
				glUniform1f(diffuseStrengthLoc, diffuseStrength);
				glUniform1f(specularStrengthLoc, specularStrength);
				glUniform1i(nshinessLoc, nshiness);



				GLint objectColorLoc = glGetUniformLocation(gouraudShader.Program, "objectColor");
				GLint lightColorLoc = glGetUniformLocation(gouraudShader.Program, "lightColor");
				glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);
				glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);

				// 设置VAO VBO
				glBindVertexArray(gouraudVAO);
				glBindBuffer(GL_ARRAY_BUFFER, gouraudVBO);
				glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_normal), vertices_normal, GL_STATIC_DRAW);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
				glEnableVertexAttribArray(1);

				gouraudShader.Use();
				glDrawArrays(GL_TRIANGLES, 0, 36);*/
			}



		}
		// 正方体动画
		else if (homework5_projection) {


			model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			view = glm::translate(view, glm::vec3(0.0f, 0.0f, -20.0f)); // 摄像机向z正方向移动 == 场景向z的负方向移动
			projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

			// orthographic
			if (selected_projection == 0) {
				view = glm::translate(view, glm::vec3(0.0f, 0.0f, -20.0f));
				projection = glm::ortho(ortho_left, ortho_right, ortho_bottom, ortho_top, ortho_near, ortho_far);
				view = glm::translate(view, glm::vec3(0.0f, 0.0f, 0.0f)); // 摄像机向z正方向移动 == 场景向z的负方向移动
			}

			else if (selected_projection == 1) {
				projection = glm::perspective(glm::radians(persp_radians), (float)persp_width / (float)persp_height, persp_near, persp_far);

			}

			unsigned int modelLoc = glGetUniformLocation(ourShader.Program, "model");
			unsigned int viewLoc = glGetUniformLocation(ourShader.Program, "view");
			unsigned int projectionLoc = glGetUniformLocation(ourShader.Program, "projection");

			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

			// 设置VAO VBO
			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_hw5), vertices_hw5, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
			glEnableVertexAttribArray(1);

			glDrawArrays(GL_TRIANGLES, 0, 36);

		}
		else if (homework5_view_changing) {

			glm::vec3 test(1.0f, 2.0f, 3.0f);

			glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 20.0f); // position

			glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
			glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);  // direction

			glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
			glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection)); // right
			model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -20.0f)); // 摄像机向z正方向移动 == 场景向z的负方向移动
			projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
			float radius = 50.0f;
			float camPosX = sin(glfwGetTime()) * radius;
			float camPosZ = cos(glfwGetTime()) * radius;


			view = glm::lookAt(glm::vec3(camPosX, 0.0f, camPosZ), cameraTarget, up);


			unsigned int modelLoc = glGetUniformLocation(ourShader.Program, "model");
			unsigned int viewLoc = glGetUniformLocation(ourShader.Program, "view");
			unsigned int projectionLoc = glGetUniformLocation(ourShader.Program, "projection");

			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
			glEnableVertexAttribArray(1);
			glDrawArrays(GL_TRIANGLES, 0, 36);


		}
		else if (homework5_move_freely) {
			glfwSetCursorPosCallback(window, mouse_callback);
			processInput(window);
			//cout << "freely" << endl;
			view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
			model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -20.0f)); // 摄像机向z正方向移动 == 场景向z的负方向移动
			projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);


			unsigned int modelLoc = glGetUniformLocation(ourShader.Program, "model");
			unsigned int viewLoc = glGetUniformLocation(ourShader.Program, "view");
			unsigned int projectionLoc = glGetUniformLocation(ourShader.Program, "projection");

			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
			glEnableVertexAttribArray(1);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		else if (homework5_camera_class) {
			glfwSetCursorPosCallback(window, mouse_callback_camera_class);
			processInputCameraClass(window, myCamera);

			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
			glEnableVertexAttribArray(1);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		else if (homework4) {
			basetime = glfwGetTime();
			// 静止
			if (selected_transformation == -1 || selected_transformation == 3) {
				model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				view = glm::translate(view, glm::vec3(0.0f, 0.0f, -20.0f)); // 摄像机向z正方向移动 == 场景向z的负方向移动
				projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
			}
			// 平移
			else if (selected_transformation == 0) {
				model = glm::mat4(1.0f);
				float moveAmount = 8 * sin(glfwGetTime());
				model = glm::translate(model, glm::vec3(moveAmount, 0.0f, 0.0f));

				view = glm::mat4(1.0f);
				view = glm::translate(view, glm::vec3(0.0f, 0.0f, -20.0f)); // 摄像机向z正方向移动 == 场景向z的负方向移动

				projection = glm::mat4(1.0f);
				projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
			}
			// 旋转
			else if (selected_transformation == 1) {
				model = glm::mat4(1.0f);
				float rotateAmount = 40 * glfwGetTime();
				model = glm::rotate(model, glm::radians(rotateAmount), glm::vec3(1.0f, 0.0f, 1.0f));

				view = glm::mat4(1.0f);
				view = glm::translate(view, glm::vec3(0.0f, 0.0f, -20.0f)); // 摄像机向z正方向移动 == 场景向z的负方向移动

				projection = glm::mat4(1.0f);
				projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
			}

			// 缩放
			else if (selected_transformation == 2) {
				model = glm::mat4(1.0f);
				model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				float scaleAmount = abs(sin(glfwGetTime()));
				model = glm::scale(model, glm::vec3(scaleAmount, scaleAmount, scaleAmount));

				view = glm::mat4(1.0f);
				view = glm::translate(view, glm::vec3(0.0f, 0.0f, -20.0f)); // 摄像机向z正方向移动 == 场景向z的负方向移动

				projection = glm::mat4(1.0f);
				projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
			}

			// 给uniform赋值
			unsigned int modelLoc = glGetUniformLocation(ourShader.Program, "model");
			unsigned int viewLoc = glGetUniformLocation(ourShader.Program, "view");
			unsigned int projectionLoc = glGetUniformLocation(ourShader.Program, "projection");

			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

			// 设置VAO VBO
			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
			glEnableVertexAttribArray(1);

			glDrawArrays(GL_TRIANGLES, 0, 36);

		}
		// 跳跃月球动画
		else if (moon) {
			float realtime = glfwGetTime() - basetime;
			float scaleAmount = sin(realtime);
			float rotateAmount = 40 * realtime;
			float translateAmountY = sin(realtime);
			float translateAmountZ = 2 * realtime;


			model = glm::translate(model, glm::vec3(0.0f, translateAmountY, -30.0f + translateAmountZ));
			model = glm::rotate(model, glm::radians(rotateAmount), glm::vec3(1.0f, 0.0f, 0.0f));

			// 离屏幕较近时减缓Z方向的移动
			if (translateAmountZ >= 35.0f) {
				model = glm::mat4(1.0f);
				translateAmountY = (recordtime / (3 * realtime)) * sin(realtime); // y方向移动随时间增加 幅度减小
				translateAmountZ = 2 * realtime * (recordtime / realtime);      // z方向移动随时间增加 幅度减小
				rotateAmount = 40 * ((recordtime + 0.6) / realtime) * realtime; // rotate随时间增加 幅度减小
				model = glm::translate(model, glm::vec3(0.0f, translateAmountY, -30.0f + recordZ));
				model = glm::rotate(model, glm::radians(rotateAmount), glm::vec3(1.0f, 0.0f, 0.0f));
			}
			else {
				modelstore = model;
				recordZ = translateAmountZ;
				recordtime = realtime;
			}


			// 记住顺序是从后向前看的！！！
			view = glm::translate(view, glm::vec3(0.0f, -0.8f, -10.0f)); // 摄像机向z正方向移动 == 场景向z的负方向移动
			projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.01f, 100.0f);

			// 给uniform赋值
			unsigned int modelLoc = glGetUniformLocation(ourShader.Program, "model");
			unsigned int viewLoc = glGetUniformLocation(ourShader.Program, "view");
			unsigned int projectionLoc = glGetUniformLocation(ourShader.Program, "projection");


			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

			// 设置VAO VBO
			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sphereVerticesData_len2, sphereVerticesData2, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndicesData_len2, sphereIndicesData2, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GL_FLOAT)));
			glEnableVertexAttribArray(2);

			// bind texture using texture units
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture1);
			glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture1"), 0);


			glDrawElements(GL_TRIANGLES, test_index.size(), GL_UNSIGNED_INT, (void*)0);

		}
		// ==========================================================================================


		// ========= homework 3 =======================================================================
		else if (choose == 1) {
			auto vec1 = calculateLinePoints(p1, p2);
			float* data1 = vec1.data();
			size_t data_len1 = vec1.size() * sizeof(float);
			int draw_size1 = vec1.size() / 2;
			//cout << "line1 " << data_len1 << endl;

			auto vec2 = calculateLinePoints(p1, p3);
			float* data2 = vec2.data();
			size_t data_len2 = vec2.size() * sizeof(float);
			int draw_size2 = vec2.size() / 2;
			//cout << "line2 " << data_len2 << endl;

			auto vec3 = calculateLinePoints(p2, p3);
			float* data3 = vec3.data();
			size_t data_len3 = vec3.size() * sizeof(float);
			int draw_size3 = vec3.size() / 2;
			//cout << "line3  jjjj " << vec3.size() << endl;

			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, data_len1, data1, GL_STATIC_DRAW);
			glPointSize(5);
			glDrawArrays(GL_POINTS, 0, draw_size1);


			glBufferData(GL_ARRAY_BUFFER, data_len2, data2, GL_STATIC_DRAW);
			glDrawArrays(GL_POINTS, 0, draw_size2);



			glBufferData(GL_ARRAY_BUFFER, data_len3, data3, GL_STATIC_DRAW);
			glDrawArrays(GL_POINTS, 0, draw_size3);

			if (ras_flag) {
				if (!ras_walk) {  // edge walk
					auto vec4 = edgeEquationRasterization(p1, p2, p3);
					float* data4 = vec4.data();
					size_t data_len4 = vec4.size() * sizeof(float);
					int draw_size4 = vec4.size() / 2;
					//cout << "Draw size " << draw_size4 << endl;

					glBufferData(GL_ARRAY_BUFFER, data_len4, data4, GL_STATIC_DRAW);
					glDrawArrays(GL_POINTS, 0, draw_size4);
				}
				else {   // edge equation
					auto vec5 = rasterization(p1, p2, p3);
					float* data5 = vec5.data();
					size_t data_len5 = vec5.size() * sizeof(float);
					int draw_size5 = vec5.size() / 2;

					glBufferData(GL_ARRAY_BUFFER, data_len5, data5, GL_STATIC_DRAW);
					glDrawArrays(GL_POINTS, 0, draw_size5);
				}
			}
			// rasterization
			/*auto vec4 = edgeEquationRasterization(p1, p2, p3);
			float* data4 = vec4.data();
			size_t data_len4 = vec4.size() * sizeof(float);
			int draw_size4 = vec4.size() / 2;
			cout << "Draw size " << draw_size4 << endl;

			glBufferData(GL_ARRAY_BUFFER, data_len4, data4, GL_STATIC_DRAW);
			glDrawArrays(GL_POINTS, 0, draw_size4);*/

			// rasterization2
			/*auto vec5 = rasterization(p1, p2, p3);
			float* data5 = vec5.data();
			size_t data_len5 = vec5.size() * sizeof(float);
			int draw_size5 = vec5.size() / 2;

			glBufferData(GL_ARRAY_BUFFER, data_len5, data5, GL_STATIC_DRAW);
			glDrawArrays(GL_POINTS, 0, draw_size5);*/




		}
		else {
			GLint p1[] = { 800, 600 };

			auto vecpp = calculateCirclePoints(p1, r);


			float* datapp = vecpp.data();
			size_t data_lenpp = vecpp.size() * sizeof(float);
			int draw_sizepp = vecpp.size() / 2;


			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, data_lenpp, datapp, GL_STATIC_DRAW);
			glPointSize(5);
			glDrawArrays(GL_POINTS, 0, draw_sizepp);


			/*glBufferData(GL_ARRAY_BUFFER, sizeof(ppp1), ppp1, GL_STATIC_DRAW);
			glPointSize(5);
			glDrawArrays(GL_POINTS, 0, 1);
			GLfloat pp[] = { 0.0f, 0.0f };
			glBufferData(GL_ARRAY_BUFFER, sizeof(pp), pp, GL_STATIC_DRAW);
			glPointSize(5);
			glDrawArrays(GL_POINTS, 0, 1);*/
		}
		// ============================================================================================


		//glBindVertexArray(0); // 解绑

		// HERE!!!
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window); // 交换颜色缓冲（是一个储存着GLFW窗口每一个像素颜色的大缓冲）-》迭代中被用来绘制，作为输出显示在屏幕上
		glfwPollEvents(); // 检查函数有没有什么触发事件（键盘输入、鼠标移动）
	}
	// HERE!!!
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &planeVAO);
	glDeleteBuffers(1, &planeVBO);

	// 游戏循环结束之后-》释放/删除之前分配的所有资源
	glfwTerminate();


	// 注册回调
	//glfwSetKeyCallback(window, key_callback);

	return 0;
}

/*
	GLFWwindow* window -> window指针
	int key -> 按下的按键
	int  scancode -> ？？ 未知
	int action -> 按键是按下还是释放
	int mode -> 表示是否有Ctrl、Shift、Alt、Super等按钮的操作
*/
void key_callback(GLFWwindow * window, int key, int  scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE); // 设置为true，跳出while循环
	}
}

vector<float> calculateCirclePoints(GLint * p, GLint r) {
	vector<float> points;

	int point_num = 1;

	int xc = p[0];
	int yc = p[1];

	int x = 0;
	int y = r;

	int d = 3 - 2 * r;


	calculate8points(points, x, y, xc, yc);
	while (x < y) {
		if (d < 0) {
			d = d + 4 * x + 6;
		}
		else {
			d = d + 4 * (x - y) + 10;
			y--;
		}
		x++;
		calculate8points(points, x, y, xc, yc);
	}

	return points;

}

void calculate8points(vector<float> & points, int x, int y, int xc, int yc) {

	// (x,y)
	points.push_back((float)(x - WIDTH / 2 + xc) / (WIDTH / 2));
	//cout << points[0] << endl;
	points.push_back((float)(y - HEIGHT / 2 + yc) / (HEIGHT / 2));

	// (x, -y)
	points.push_back((float)(x - WIDTH / 2 + xc) / (WIDTH / 2));
	points.push_back((float)(-y - HEIGHT / 2 + yc) / (HEIGHT / 2));

	// (y, x)
	points.push_back((float)(y - WIDTH / 2 + xc) / (WIDTH / 2));
	points.push_back((float)(x - HEIGHT / 2 + yc) / (HEIGHT / 2));

	// (y, -x)
	points.push_back((float)(y - WIDTH / 2 + xc) / (WIDTH / 2));
	points.push_back((float)(-x - HEIGHT / 2 + yc) / (HEIGHT / 2));

	// (-y, x)
	points.push_back((float)(-y - WIDTH / 2 + xc) / (WIDTH / 2));
	points.push_back((float)(x - HEIGHT / 2 + yc) / (HEIGHT / 2));

	// (-y, -x)
	points.push_back((float)(-y - WIDTH / 2 + xc) / (WIDTH / 2));
	points.push_back((float)(-x - HEIGHT / 2 + yc) / (HEIGHT / 2));

	// (-x, y)
	points.push_back((float)(-x - WIDTH / 2 + xc) / (WIDTH / 2));
	points.push_back((float)(y - HEIGHT / 2 + yc) / (HEIGHT / 2));

	// (-x, -y)
	points.push_back((float)(-x - WIDTH / 2 + xc) / (WIDTH / 2));
	points.push_back((float)(-y - HEIGHT / 2 + yc) / (HEIGHT / 2));



}


vector<float> calculateLinePoints(GLint * p1, GLint * p2) {


	int x1 = p1[0];
	int y1 = p1[1];
	int x2 = p2[0];
	int y2 = p2[1];


	int delta_x = x2 - x1;
	int delta_y = y2 - y1;
	//cout << "delta_x " << delta_x << endl;
	//cout << "delta_y " << delta_y << endl;
	int size = 0;

	vector<float> points;




	float m = abs((float)delta_y / delta_x);
	//cout << "m " << m << endl;

	float tmpx = (float)(x1 - WIDTH / 2) / (WIDTH / 2);
	float tmpy = (float)(y1 - HEIGHT / 2) / (HEIGHT / 2);

	points.push_back(tmpx);
	points.push_back(tmpy);

	int i = 1;
	int stepx = 1;
	int stepy = 1;
	if (delta_x < 0) stepx = -1;
	if (delta_y < 0) stepy = -1;

	if (m <= 1) {
		size = (abs(delta_x) + 1) * 2;
		//size = abs((delta_x + 1) * 2);


		int p = 2 * delta_y * stepy - delta_x * stepx;

		while (x1 != x2) {
			x1 += stepx;
			if (p <= 0) {

				p = p + 2 * delta_y * (stepy);

			}
			else {
				y1 += stepy;
				p = p + 2 * (stepy * delta_y - delta_x * stepx);

			}


			float tmp_x1 = (float)(x1 - WIDTH / 2) / (WIDTH / 2);
			float tmp_y1 = (float)(y1 - HEIGHT / 2) / (HEIGHT / 2);
			points.push_back(tmp_x1);
			points.push_back(tmp_y1);
			i++;
		}

	}
	else {
		size = (abs(delta_y) + 1) * 2;

		int p = 2 * delta_x * stepx - delta_y * stepy;

		while (y1 != y2) {
			y1 += stepy;
			if (p <= 0) {

				p = p + 2 * delta_x * stepx;

			}
			else {
				x1 += stepx;
				p = p + 2 * (delta_x * stepx - delta_y * stepy);

			}

			float tmp_x1 = (float)(x1 - WIDTH / 2) / (WIDTH / 2);
			float tmp_y1 = (float)(y1 - HEIGHT / 2) / (HEIGHT / 2);
			points.push_back(tmp_x1);
			points.push_back(tmp_y1);
			i++;
		}

	}

	return points;


}

bool compareByY(const tri_point & a, const tri_point & b) {
	return a.y < b.y;
}

vector<float> rasterization(GLint * p1, GLint * p2, GLint * p3) {
	vector<float> points;
	tri_point tmp1(p1[0], p1[1]);
	tri_point tmp2(p2[0], p2[1]);
	tri_point tmp3(p3[0], p3[1]);

	vector<tri_point> vec;
	vec.push_back(tmp1);
	vec.push_back(tmp2);
	vec.push_back(tmp3);



	sort(vec.begin(), vec.end(), compareByY);



	if (vec[0].y == vec[1].y) {
		rasterization_uptri(points, vec[0], vec[1], vec[2]);
	}
	else if (vec[1].y == vec[2].y) {

		rasterization_downtri(points, vec[0], vec[1], vec[2]);
	}
	else {
		int x3 = floor(vec[2].x - (float)(vec[2].y - vec[1].y) / (vec[2].y - vec[0].y) * (vec[2].x - vec[0].x));
		tri_point tmp4(x3, vec[1].y);
		rasterization_downtri(points, vec[0], vec[1], tmp4);
		rasterization_uptri(points, vec[1], tmp4, vec[2]);
	}


	return points;
}

void rasterization_downtri(vector<float> & points, tri_point p1, tri_point p2, tri_point p3) {

	float delta_p1p2_x = p2.x - p1.x;
	float delta_p1p2_y = p2.y - p1.y;

	float m_p1p2 = delta_p1p2_x / delta_p1p2_y;

	float delta_p1p3_x = p3.x - p1.x;
	float delta_p1p3_y = p3.y - p1.y;

	float m_p1p3 = delta_p1p3_x / delta_p1p3_y;

	float m_left = (m_p1p2 >= 0) ? m_p1p3 : m_p1p2;
	float m_right = (m_p1p2 >= 0) ? m_p1p2 : m_p1p3;

	int xl = p1.x;
	int xr = p1.x;
	int y = p1.y;

	points.push_back((float)(xl - WIDTH / 2) / (WIDTH / 2));
	points.push_back((float)(y - HEIGHT / 2) / (HEIGHT / 2));

	while (y < p2.y) {
		xl = floor(xl + m_left);
		xr = ceil(xr + m_right);

		y++;
		for (int i = xl; i <= xr; i++) {
			points.push_back((float)(i - WIDTH / 2) / (WIDTH / 2));
			points.push_back((float)(y - HEIGHT / 2) / (HEIGHT / 2));
		}

	}

}
void rasterization_uptri(vector<float> & points, tri_point p1, tri_point p2, tri_point p3) {
	float delta_p1p3_x = p1.x - p3.x;
	float delta_p1p3_y = p1.y - p3.y;

	float m_p1p3 = delta_p1p3_x / delta_p1p3_y;

	float delta_p2p3_x = p2.x - p3.x;
	float delta_p2p3_y = p2.y - p3.y;

	float m_p2p3 = delta_p2p3_x / delta_p2p3_y;

	float m_left = ((m_p1p3 >= 0) ? m_p1p3 : m_p2p3);
	float m_right = ((m_p1p3 >= 0) ? m_p2p3 : m_p1p3);

	int xl = p3.x;
	int xr = p3.x;
	int y = p3.y;

	points.push_back((float)(xl - WIDTH / 2) / (WIDTH / 2));
	points.push_back((float)(y - HEIGHT / 2) / (HEIGHT / 2));

	while (y > p2.y) {
		xl = round(xl - m_left);
		xr = round(xr - m_right);
		y--;
		for (int i = xl; i <= xr; i++) {
			points.push_back((float)(i - WIDTH / 2) / (WIDTH / 2));
			points.push_back((float)(y - HEIGHT / 2) / (HEIGHT / 2));
		}

	}

}
vector<float> edgeEquationRasterization(GLint * p1, GLint * p2, GLint * p3) {
	vector<float> points;

	// 1. find the boundary
	int min_x = min(min(p1[0], p2[0]), p3[0]);
	int max_x = max(max(p1[0], p2[0]), p3[0]);
	int min_y = min(min(p1[1], p2[1]), p3[1]);
	int max_y = max(max(p1[1], p2[1]), p3[1]);



	for (int x = min_x; x <= max_x; x++) {
		for (int y = min_y; y <= max_y; y++) {
			int w1 = edgeEquation(x, y, p2, p3);
			int w2 = edgeEquation(x, y, p3, p1);
			int w3 = edgeEquation(x, y, p1, p2);
			//	cout << w1 << w2 << w3 << endl;

			if (w1 >= 0 && w2 >= 0 && w3 >= 0 || w1 <= 0 && w2 <= 0 && w3 <= 0) {
				float tmp_x = (float)(x - WIDTH / 2) / (WIDTH / 2);
				float tmp_y = (float)(y - HEIGHT / 2) / (HEIGHT / 2);
				points.push_back(tmp_x);
				points.push_back(tmp_y);
			}
		}
	}

	return points;


}

int edgeEquation(int x, int y, GLint * p1, GLint * p2) {
	return (x - p1[0]) * (p2[1] - p1[1]) - (y - p1[1]) * (p2[0] - p1[0]);
}
vector<float> calculateSphereParam(float radius, int sectorCount, int stackCount) {
	vector<float> vertices;

	float x, y, z, xy; // vector position
	float s, t;

	float sectorStep = 2 * glm::pi<float>() / sectorCount; // sector 是指一圈（纬度方向）上面的长方形
	float stackStep = glm::pi<float>() / stackCount;

	float sectorAngle, stackAngle;

	for (int i = 0; i <= stackCount; i++) {
		stackAngle = glm::pi<float>() / 2 - i * stackStep;
		xy = radius * cosf(stackAngle);
		z = radius * sinf(stackAngle);

		for (int j = 0; j <= sectorCount; j++) {
			sectorAngle = j * sectorStep;

			x = xy * cosf(sectorAngle);
			y = xy * sinf(sectorAngle);
			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);
			s = (float)j / sectorCount;
			t = (float)i / stackCount;
			vertices.push_back(s);
			vertices.push_back(t);
		}

	}
	return vertices;
}

vector<int> calculateIndex(int stackCount, int sectorCount) {
	vector<int> indices;
	int k1, k2;
	for (int i = 0; i < stackCount; i++) {
		k1 = i * (sectorCount + 1);
		k2 = k1 + sectorCount + 1;

		for (int j = 0; j < sectorCount; j++, k1++, k2++) {
			// why exclude the first and last stacks???
			if (i != 0) {
				indices.push_back(k1);
				indices.push_back(k2);
				indices.push_back(k1 + 1);
			}

			if (i != (stackCount - 1)) {
				indices.push_back(k1 + 1);
				indices.push_back(k2);
				indices.push_back(k2 + 1);
			}
		}
	}

	return indices;
}

// ============= homework5 =====================================
void processInputCameraClass(GLFWwindow * window, Camera & myCamera) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	float cameraSpeed = 10.0f * deltaTime;
	//cout << cameraSpeed << endl;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		myCamera.moveForward(cameraSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		myCamera.moveBack(cameraSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		myCamera.moveLeft(cameraSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		myCamera.moveRight(cameraSpeed);
	}
}
void processInput(GLFWwindow * window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	float cameraSpeed = 2.0f * deltaTime;
	//cout << cameraSpeed << endl;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cameraPos += cameraSpeed * cameraFront;   /// ???????????????????

	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cameraPos -= cameraSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}

}

void mouse_callback(GLFWwindow * window, double xpos, double ypos) {
	if (firstMouse) { // 记得如果不是选择free可能要重置一下
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.05;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f) {
		pitch = 89.0f;
	}
	if (pitch < -89.0f) {
		pitch = -89.0f;
	}

	glm::vec3 front;
	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	cameraFront = glm::normalize(front);




}

void mouse_callback_camera_class(GLFWwindow * window, double xpos, double ypos) {
	if (firstMouse) { // 记得如果不是选择free可能要重置一下
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.05;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f) {
		pitch = 89.0f;
	}
	if (pitch < -89.0f) {
		pitch = -89.0f;
	}
	void* obj = glfwGetWindowUserPointer(window);
	Camera* ptr = (Camera*)obj;
	//ptr->rotate(pitch, yaw);
	//ptr->rotate_quaternion_glmfunc(yoffset, xoffset);
	ptr->rotate_quaternion_class(yoffset, xoffset);
	//ptr->rotate_quaterion(pitch, yaw);

	//myCamera.rotate(pitch, yaw);





}
// renders the 3D scene
// --------------------
void renderScene(const Shader & shader)
{
	// floor
	glm::mat4 model = glm::mat4(1.0f);
	//shader.setMat4("model", model);
	GLuint modelLoc = glGetUniformLocation(shader.Program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glBindVertexArray(planeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	// cubes
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(2.0f, 1.5f, 0.0));
	model = glm::scale(model, glm::vec3(0.5f));
	//shader.setMat4("model", model);
	//GLuint modelLoc = glGetUniformLocation(shader.Program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	renderCube();
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(5.0f, 0.0f, 1.0));
	model = glm::scale(model, glm::vec3(0.5f));
	//shader.setMat4("model", model);
	//GLuint modelLoc = glGetUniformLocation(shader.Program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	renderCube();
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-2.0f, 4.0f, -1.0));
	//model = glm::rotate(model, glm::radians(45.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
	model = glm::scale(model, glm::vec3(0.25));
	//shader.setMat4("model", model);
	//GLuint modelLoc = glGetUniformLocation(shader.Program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	//renderCube();
}


// renderCube() renders a 1x1 3D cube in NDC.
// -------------------------------------------------
unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;
void renderCube()
{
	// 初始化
	if (cubeVAO == 0)
	{
		float vertices[] = {
			// back face
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
			// front face
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			// left face
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			// right face
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
			// bottom face
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			// top face
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			 1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
			 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
		};
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);

		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindVertexArray(cubeVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

// renderQuad() renders a 1x1 XY quad in NDC
// -----------------------------------------
unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

GLuint loadTexture(string path) {
	GLuint textureID;
	glGenTextures(1, &textureID);
	int width, height;
	unsigned char* image = SOIL_load_image(path.c_str(), &width, &height, 0, SOIL_LOAD_RGB);

	// Assign texture to ID
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);
	return textureID;


}

void CalcOrthoProjs(Camera myCamera, glm::mat4 lightViewMatrix, float Height, float Width, float FOV) {
	// get the inverse of view transform
	glm::mat4 camInv = glm::inverse(myCamera.getViewMatrix());

	float ar = Height / Width;
	float tanHalfHFOV = tanf(glm::radians(FOV / 2.0f));
	float tanHalfVFOV = tanf(glm::radians((FOV * ar)/ 2.0f));

	

	for (int i = 0; i < NUM_CASCADES; i++) {
		float xn = m_cascadeEnd[i] * tanHalfHFOV; // x of near plane
		float xf = m_cascadeEnd[i + 1] * tanHalfHFOV; // x of far plane
		float yn = m_cascadeEnd[i] * tanHalfVFOV;
		float yf = m_cascadeEnd[i + 1] * tanHalfVFOV;
		cout << "xn " << xn << " xf " << xf << " yn " << yn << " yf " << yf << endl;
		glm::vec4 frustumCorners[NUM_FRUSTUM_CORNERS] = {
			// near face
			glm::vec4(xn, yn, m_cascadeEnd[i], 1.0),
			glm::vec4(-xn, yn, m_cascadeEnd[i], 1.0),
			glm::vec4(xn, -yn, m_cascadeEnd[i], 1.0),
			glm::vec4(-xn, -yn, m_cascadeEnd[i], 1.0),

			// far face
			glm::vec4(xf, yf, m_cascadeEnd[i + 1], 1.0),
			glm::vec4(-xf, yf, m_cascadeEnd[i + 1], 1.0),
			glm::vec4(xf, -yf, m_cascadeEnd[i + 1], 1.0),
			glm::vec4(-xf, -yf, m_cascadeEnd[i + 1], 1.0)

		};

		glm::vec4 frustumCornersL[NUM_FRUSTUM_CORNERS];

		float minX = std::numeric_limits<float>::max();
		float maxX = std::numeric_limits<float>::min();
		float minY = std::numeric_limits<float>::max();
		float maxY = std::numeric_limits<float>::min();
		float minZ = std::numeric_limits<float>::max();
		float maxZ = std::numeric_limits<float>::min();

		for (int j = 0; j < NUM_FRUSTUM_CORNERS; j++) {
			// 变回到世界坐标
			glm::vec4 vW = camInv * frustumCorners[j];
			// 再变到light space下
			frustumCornersL[j] = lightViewMatrix * vW;

			// 取边界
			minX = min(minX, frustumCornersL[j].x);
			maxX = max(maxX, frustumCornersL[j].x);
			minY = min(minY, frustumCornersL[j].y);
			maxY = max(maxY, frustumCornersL[j].y);
			minZ = min(minZ, frustumCornersL[j].z);
			maxZ = max(maxZ, frustumCornersL[j].z);
		}

		// 可以输出看一下
		orthoArray[i].r = maxX;
		orthoArray[i].l = minX;
		orthoArray[i].b = minY;
		orthoArray[i].t = maxY;
		orthoArray[i].f = maxZ;
		orthoArray[i].n = minZ;
		cout << "--------------------------" << endl;
		cout << "i " << i << endl;
		cout << "left " << orthoArray[i].l << endl;
		cout << "right " << orthoArray[i].r << endl;
		cout << "bottom " << orthoArray[i].b << endl;
		cout << "top " << orthoArray[i].t << endl;
		cout << "near " << orthoArray[i].n << endl;
		cout << "far " << orthoArray[i].f << endl;

	}

}