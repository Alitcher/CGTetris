#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Display.h"
#include "Colors.h"
#include "Clocks.h"
#include "Config.h"
#include "GameConfig.h"
#include <vector>
using namespace std;

wstring shapes[7];
wstring board;

void setupTetrominos();
void setBoard();

void deleteBuffers(GLuint& vao, GLuint& vbo);
void drawSquare(glm::vec4 color);
void drawSquare(glm::vec4 color, glm::vec2 squareTranslation);
void SetUpShader();

GLuint createShaderProgram(const GLchar* vertexShaderSource, const GLchar* fragmentShaderSource);
void createBuffers(GLuint& vao, GLuint& vbo, float* vertices, GLsizei verticesSize);
GLuint vao, vbo;
GLuint shaderProgram;
GLint translationLocation;
GLint colorLocation;
/*VAO is an object that represents the vertex fetch stage of the
OpenGL pipeline and is used to supply input to the vertex shader.*/



void drawBoard()
{
	for (int i = 0; i < board.size(); ++i) {
		if (board.at(i) == L'X') {
			int row = i / 12; // calculate row
			int col = i % 12; // calculate column
			glm::vec2 squareTranslation(-0.9f + col * 0.10f, 0.9f - row * 0.10f); // set translation based on row and column
			drawSquare(COLOR_NAVY, squareTranslation);
		}
	}
}

void generateRandomTetrimino(int randomIndex) {
	wstring shape = shapes[randomIndex];
	float offsetX = -0.9f;
	//float offsetY = 1.2f;
	for (int j = 0; j < 16; j++) {
		if (shape[j] == L'X') {
			int row = j / 4; // calculate row
			int col = j % 4; // calculate column
			glm::vec2 squareTranslation(offsetX + col * 0.10f + randomIndex * gap, TopPosY - row * 0.10f); // set translation based on row and column
			drawSquare(colors[randomIndex], squareTranslation);
		}
	}
}

vector<glm::vec2> generateTetriminoT(int shapeIndex) {
	vector<glm::vec2> squareTranslations;
	float offsetX = -0.4f;
	float offsetY = 1.0f;
	float gap = 0.1f;

	if (shapeIndex == 0) {
		// T-shaped tetrimino
		squareTranslations.push_back(glm::vec2(offsetX, offsetY));
		squareTranslations.push_back(glm::vec2(offsetX - gap, offsetY - gap));
		squareTranslations.push_back(glm::vec2(offsetX, offsetY - gap));
		squareTranslations.push_back(glm::vec2(offsetX + gap, offsetY - gap));
	}

	return squareTranslations;
}
//
//void drawTetriminoSet() {
//	float offsetX = -0.9f;
//	float offsetY = 0.7f;
//	float gap = 0.2f;
//	for (int i = 0; i < 1; i++) {
//		wstring shape = shapes[i];
//		for (int j = 0; j < 16; j++) {
//			if (shape[j] == L'X') {
//				int row = j / 4; // calculate row
//				int col = j % 4; // calculate column
//				glm::vec2 squareTranslation(offsetX + col * 0.10f + i * gap, offsetY - row * 0.10f); // set translation based on row and column
//				drawSquare(colors[i], squareTranslation);
//			}
//		}
//	}
//}

int stepsCount;
vector<glm::vec2> squareTranslations;

void CreateTetWindow4()
{
	glm::vec2 translation(0.05f, 0.0f);

	Clock Time;
	Display display(SCREEN_WIDTH, SCREEN_HEIGHT, PROJECT_NAME);
	GLFWwindow* window = display.getWindow();
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_WIDTH);

	srand(time(NULL));
	createBuffers(vao, vbo, triangle_vertices, sizeof(triangle_vertices));
	createBuffers(vao, vbo, square_vertices, sizeof(square_vertices));

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	SetUpShader();
	glUniform2fv(translationLocation, 1, glm::value_ptr(translation));

	translation.y = 1.0f;
	translation.x = -0.8f;
	bool isGameover = false;
	int randomIndex = rand() % shapesLength;
	bool isDownKeyPressed = false;

	while (!display.shouldClose()) {

		// Game Timing =======================
		Time.currentFrameTime = static_cast<float>(glfwGetTime());
		Time.deltaTime = Time.currentFrameTime - Time.lastFrameTime;
		Time.lastFrameTime = Time.currentFrameTime;
		glClear(GL_COLOR_BUFFER_BIT);

		// INPUT  =======================
		if (!isDownKeyPressed)
		{
			if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && translation.x < 0.1f) {
				translation.x += 0.1f;
				isDownKeyPressed = true;
			}
			else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && translation.x > -0.8f) {
				translation.x -= 0.1f;
				isDownKeyPressed = true;
			}

			else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
				// Rotate tetrimino
				isDownKeyPressed = true;
			}
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && translation.y > -0.8f) {
			translation.y -= 0.1f;
		}

		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_RELEASE //glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_RELEASE && 
			&& glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE) {
			isDownKeyPressed = false;
		}


		// LOGOC - Collision detection, falling, score, rotate

		if (!isGameover)
		{
			// RENDER
			drawBoard();
			generateRandomTetrimino(randomIndex);
			drawSquare(COLOR_ORANGE, translation);
		}


		display.swapBuffers();
		display.pollEvents();

		// update the translation vector every interval seconds
		if (Time.currentFrameTime >= Time.INTERVAL && translation.y > -0.8f) {
			translation.y -= 0.1f;
			stepsCount++;
			glUniform2fv(translationLocation, 1, glm::value_ptr(translation));
			Time.INTERVAL += 1.0f;
		}
		else
		{
			generateRandomTetrimino(randomIndex);
		}
	}
	cout << stepsCount << " steps" << endl;
	deleteBuffers(vao, vbo);
	glDeleteProgram(shaderProgram);
}

int main() {

	setupTetrominos();
	setBoard();
	CreateTetWindow4();

	return 0;
}
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

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

void SetUpShader()
{
	shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);
	colorLocation = glGetUniformLocation(shaderProgram, "uColor");
	translationLocation = glGetUniformLocation(shaderProgram, "uTranslation");
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

void drawTriangle(glm::vec4 color, glm::vec2 triTranslation) {

	glUseProgram(shaderProgram);
	glUniform4fv(colorLocation, 1, glm::value_ptr(color));
	glUniform2fv(translationLocation, 1, glm::value_ptr(triTranslation));

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glUseProgram(0); // Unbind the shader program
	glBindVertexArray(0);


}

void drawSquare(glm::vec4 color, glm::vec2 squareTranslation) {
	glUseProgram(shaderProgram);
	glUniform4fv(colorLocation, 1, glm::value_ptr(color));
	glUniform2fv(translationLocation, 1, glm::value_ptr(squareTranslation));

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glUseProgram(0);
	glBindVertexArray(0);
}


void setBoard()
{

	board.append(L"X..........X");
	board.append(L"X..........X");
	board.append(L"X..........X");
	board.append(L"X..........X");
	board.append(L"X..........X");
	board.append(L"X..........X");
	board.append(L"X..........X");
	board.append(L"X..........X");
	board.append(L"X..........X");
	board.append(L"X..........X");
	board.append(L"X..........X");
	board.append(L"X..........X");
	board.append(L"X..........X");
	board.append(L"X..........X");
	board.append(L"X..........X");
	board.append(L"X..........X");
	board.append(L"X..........X");
	board.append(L"X..........X");
	board.append(L"XXXXXXXXXXXX");

	cout << board.size() << endl;

}

void setupTetrominos() {
	shapes[0].append(L"..X.");
	shapes[0].append(L"..X.");
	shapes[0].append(L"..X.");
	shapes[0].append(L"..X.");

	shapes[1].append(L"..X.");
	shapes[1].append(L".XX.");
	shapes[1].append(L"..X.");
	shapes[1].append(L"....");

	shapes[2].append(L"....");
	shapes[2].append(L".XX.");
	shapes[2].append(L".XX.");
	shapes[2].append(L"....");

	shapes[3].append(L"..X.");
	shapes[3].append(L".XX.");
	shapes[3].append(L".X..");
	shapes[3].append(L"....");

	shapes[4].append(L".X..");
	shapes[4].append(L".XX.");
	shapes[4].append(L"..X.");
	shapes[4].append(L"....");

	shapes[5].append(L".X..");
	shapes[5].append(L".X..");
	shapes[5].append(L".XX.");
	shapes[5].append(L"....");

	shapes[6].append(L"..X.");
	shapes[6].append(L"..X.");
	shapes[6].append(L".XX.");
	shapes[6].append(L"....");

}
