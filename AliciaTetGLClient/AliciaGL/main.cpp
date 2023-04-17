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
/*VAO is an object that represents the vertex fetch stage of the
OpenGL pipeline and is used to supply input to the vertex shader.*/

float triangle_vertices[] = {
	-0.05f, 0.0f,
	 0.05f, 0.0f,
	 0.0f,  0.05f,
};

void SetUpShader() 
{
	shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);
	colorLocation = glGetUniformLocation(shaderProgram, "uColor");
	translationLocation = glGetUniformLocation(shaderProgram, "uTranslation");
}

void CreateTetWindow4()
{
	Clock Time;
	Display display(SCREEN_WIDTH, SCREEN_HEIGHT, PROJECT_NAME);
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);


	createBuffers(vao, vbo, triangle_vertices, sizeof(triangle_vertices));
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	SetUpShader();

	translation.y = 1.0f;
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
		if (Time.currentFrameTime >= Time.INTERVAL && translation.y > -1.0f) {
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
	GLuint shader = glCreateShader(shaderType); //creates an empty shader object, ready to accept source code and be compiled.
	glShaderSource(shader, 1, &shaderSource, NULL); //hands shader source code to the shader object so that it can keep a copy of it.
	glCompileShader(shader); //compiles whatever source code is contained in the shader object.
	return shader;
}

GLuint createShaderProgram(const GLchar* vertexShaderSource, const GLchar* fragmentShaderSource) {
	// Create program, attach shaders to it, and link it
	GLuint shaderProgram = glCreateProgram(); //creates a program object to which you can attach shader objects.
	GLuint vertexShader = createShader(GL_VERTEX_SHADER, vertexShaderSource);
	GLuint fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

	//attaches a shader object to a program object.
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	//links all of the shader objects attached to a program object together.
	glLinkProgram(shaderProgram);

	/*deletes a shader object.Once a shader has been linked into
	a program object, the program contains the binary codeand the shader is no longer
		needed.*/
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