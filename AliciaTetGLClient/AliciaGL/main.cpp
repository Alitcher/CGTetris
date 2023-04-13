#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Display.h"
#include "Colors.h"
#include "Clocks.h"
#include "Config.h"

using namespace std;

wstring shapes[8];
wstring board;

void testDrawTriangle();
void deleteBuffers(GLuint& vao, GLuint& vbo);
GLuint createShaderProgram(const GLchar* vertexShaderSource, const GLchar* fragmentShaderSource);
void createBuffers(GLuint& vao, GLuint& vbo, float* vertices, GLsizei verticesSize);
GLuint vao, vbo;
GLuint shaderProgram;
GLint translationLocation;
GLint colorLocation;
glm::vec2 translation(0.05f, 0.0f);


float triangle_vertices[] = {
	-0.05f, 0.0f,
	 0.05f, 0.0f,
	 0.0f,  0.05f,
};
void CreateTetWindow4()
{
	Clock Time;
	Display display(SCREEN_WIDTH, SCREEN_HEIGHT, PROJECT_NAME);
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);


	createBuffers(vao, vbo, triangle_vertices, sizeof(triangle_vertices));
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);
	colorLocation = glGetUniformLocation(shaderProgram, "uColor");
	translationLocation = glGetUniformLocation(shaderProgram, "uTranslation");

	translation.y = 1.0f;
	translation.x = 1.0f;
	glUniform2fv(translationLocation, 1, glm::value_ptr(translation));

	bool shouldDrawTriangle = true;
	while (!display.shouldClose()) {

		Time.currentFrameTime = glfwGetTime();
		Time.deltaTime = Time.currentFrameTime - Time.lastFrameTime;
		Time.lastFrameTime = Time.currentFrameTime;
		glClear(GL_COLOR_BUFFER_BIT);

		if (shouldDrawTriangle) {
			testDrawTriangle();
		}

		display.swapBuffers();
		display.pollEvents();

		// update the translation vector every interval seconds
		if (Time.currentFrameTime >= Time.INTERVAL) {
			translation.y -= 0.05f;
			glUniform2fv(translationLocation, 1, glm::value_ptr(translation));
			Time.INTERVAL += 1.0f;
		}
	}
	deleteBuffers(vao, vbo);
	glDeleteProgram(shaderProgram);
}

int main() {

	CreateTetWindow4();
	return 0;
}

void createBuffers(GLuint& vao, GLuint& vbo, float* vertices, GLsizei verticesSize) {
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
}

void deleteBuffers(GLuint& vao, GLuint& vbo) {
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

GLuint createShader(GLenum shaderType, const GLchar* shaderSource) {
	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderSource, NULL);
	glCompileShader(shader);
	return shader;
}

GLuint createShaderProgram(const GLchar* vertexShaderSource, const GLchar* fragmentShaderSource) {
	GLuint shaderProgram = glCreateProgram();
	GLuint vertexShader = createShader(GL_VERTEX_SHADER, vertexShaderSource);
	GLuint fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

void drawTriangle(glm::vec4 color) {

	glUseProgram(shaderProgram);
	glUniform4fv(colorLocation, 1, glm::value_ptr(COLOR_ORANGE));
	glUniform2fv(translationLocation, 1, glm::value_ptr(translation));

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glUseProgram(0); // Unbind the shader program
	glBindVertexArray(0);


}

void testDrawTriangle() {

	drawTriangle(COLOR_ORANGE);
}