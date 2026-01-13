#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

class Circle
{
	GLuint VAO, VBO, EBO;
	int index;
	float x, y, z;
	float radius;
	float r, g, b;

public:
	Circle(float x1, float y1, float rad, int segmentov = 36, float r1 = 1.0f, float g1 = 1.0f, float b1 = 1.0f) : x(x1), y(y1), radius(rad), r(r1), g(g1), b(b1)
	{
		std::vector<float> vertices;
		std::vector<unsigned int> indices;

		vertices.push_back(0.0f);
		vertices.push_back(0.0f);
		vertices.push_back(0.0f);

		for (int i = 0; i < segmentov; i++) {
			float treta = 2.0f * 3.1415926f * float(i) / float(segmentov);
			float x = radius * cosf(treta);
			float y = radius * sinf(treta);
			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(0.0f);
		}
		for (int i = 0; i <= segmentov; i++) {
			indices.push_back(0);
			indices.push_back(i);
			indices.push_back(i == segmentov ? 1 : i+1);
		}

		index = indices.size();

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glBindVertexArray(0);
	}
	void draw(GLuint shaderProfram) {
		glUseProgram(shaderProfram);

		GLuint local = glGetUniformLocation(shaderProfram, "translation");
		glUniform2f(local, x, y);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, index, GL_UNSIGNED_INT, 0);
	}
	~Circle() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}
};

const char* vertexShaderSourse = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
uniform vec2 translation;

out vec3 Color;
void  main()
{
gl_Position = vec4(aPos.x + translation.x, aPos.y + translation.y, aPos.z, 1.0);
Color = aColor;
}
)";

const char* fragmentShaderSourse = R"(
#version 330 core
out vec4 FragColor;
in vec3 Color;
void main(){
FragColor = vec4(Color, 1.0f);
}
)";

GLuint compileShader(GLenum type, const char* sourse)
{
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &sourse, nullptr);
	glCompileShader(shader);
	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infLog[512];
		glGetShaderInfoLog(shader, 512, nullptr, infLog);
		std::cerr << "Error compile\n" << infLog;
	}

	return shader;
}

int main() {
	if (!glfwInit()) {
		std::cout << "glfw not\n";
		return -1;
	}
	GLFWwindow* window = glfwCreateWindow(1600, 900, "Program1", NULL, NULL);
	if (!window) {
		std::cout << "not window\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "glew not\n";
		return -1;
	}
	std::cout << "Great, OpenGL ver " << glGetString(GL_VERSION) << '\n';

	GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSourse);
	GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSourse);
	GLuint shaderProfram = glCreateProgram();

	glAttachShader(shaderProfram, vertexShader);
	glAttachShader(shaderProfram, fragmentShader);

	glLinkProgram(shaderProfram);

	GLint s;
	glGetProgramiv(shaderProfram, GL_LINK_STATUS, &s);

	if (!s) {
		std::cerr << "ERROR link!\n";
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	float vertices[] = {
		//x , y , z
		0.5f, 0.5f, 0.0f,
		0.2f, 0.2f, 0.8f, // color
		0.0f, 0.5f, 0.0f,
		0.2f, 0.2f, 0.8f, // color
		0.5f, 0.0f, 0.0f,
		0.2f, 0.2f, 0.8f, // color
		0.0f, 0.5f, 0.0f,
		0.2f, 0.2f, 0.8f, // color
		0.5f, 0.0f, 0.0f,
		0.2f, 0.2f, 0.8f, // color
		0.0f, 0.0f, 0.0f,
		0.2f, 0.2f, 0.8f, // color
		0.6f, 0.5f, 0.0f,
		0.2f, 0.2f, 0.0f, // color
		-0.1f, 0.5f, 0.0f,
		0.2f, 0.2f, 0.0f, // color
		0.25f, 0.8f, 0.0f,
		0.2f, 0.2f, 0.0f  // color

	};

	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VAO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	float triagleX = 0.0f;
	float triagleY = 0.0f;
	GLint local = glGetUniformLocation(shaderProfram, "translation");

	Circle circle(0.4f, 0.4f, 0.2, 4, 1.0f, 1.0f, 1.0f);

	while (!glfwWindowShouldClose(window)) {

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, true);
		}

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			triagleX -= 0.01;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			triagleX += 0.01;
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			triagleY -= 0.01;
		}
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			triagleY += 0.01;
		}

		if (triagleX < -0.9f) {
			triagleX = -0.9;
		}
		if (triagleX > 0.4f) {
			triagleX = 0.4;
		}
		if (triagleY < -1.0f) {
			triagleY = -1.0;
		}
		if (triagleY > 0.2f) {
			triagleY = 0.2;
		}
		glClearColor(120/255.0, 0.0f, 120/255.0 , 0.0f);

		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProfram);

		glUniform2f(local, triagleX, triagleY);

		glBindVertexArray(VAO);

		glDrawArrays(GL_TRIANGLES, 0, 9);

		circle.draw(shaderProfram);

		glfwPollEvents();

		glfwSwapBuffers(window);
	}
	glfwTerminate();
	return 0;
}