#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Display.h"
#include "Colors.h"
#include "Clocks.h"
#include "RenderConfig.h"
#include "GameConfig.h"
#include <vector>
#include <iomanip>

using namespace std;


void deleteBuffers(GLuint& vao, GLuint& vbo);
void drawSquare(glm::vec4 color, glm::vec2 squareTranslation);
void SetUpShader();

glm::mat4 projectionMatrix;
GLuint createShaderProgram(const GLchar* vertexShaderSource, const GLchar* fragmentShaderSource);
void createBuffers(GLuint& vao, GLuint& vbo, float* vertices, GLsizei verticesSize);
GLuint vao, vbo;
GLuint shaderProgram;
GLint translationLocation;
GLint colorLocation;
GLint projectionLocation;

vector<glm::vec2> CurrentTetrominoTranslations;

/*VAO is an object that represents the vertex fetch stage of the
OpenGL pipeline and is used to supply input to the vertex shader.*/

//void rotateTetromino(int rotation) {
//	int rotatedTetrominoGrid[TET_GRID_COUNT];
//	bool validRotation = true;
//
//	for (int i = 0; i < TET_GRID_COUNT; i++) {
//		int px = i % 4;
//		int py = i / 4;
//		int newPosIndex = RotateTet(px, py, rotation);
//
//		rotatedTetrominoGrid[newPosIndex] = tetrominoBitGrid[i];
//	}
//
//	// Check for collisions with the temporary board
//	for (int i = 0; i < TET_GRID_COUNT; i++) {
//		if (rotatedTetrominoGrid[i] == 1) {
//			int newRow = i / 4;
//			int newCol = i % 4;
//			glm::vec2 rotatedPosition = CurrentTetrominoTranslations[0] +
//				glm::vec2(newCol * 0.1f, -newRow * 0.1f);
//
//			int row = int(round((TopPosY - rotatedPosition.y) / 0.1f));
//			int col = int(round((rotatedPosition.x - LeftPos) / 0.1f));
//
//			if (row < 0 || row >= ROW_COUNT || col < 0 || col >= COL_COUNT || boardBitTmp[row * COL_COUNT + col] == 1) {
//				validRotation = false;
//				break;
//			}
//		}
//	}
//
//	// Apply the rotation if it's valid
//	if (validRotation) {
//		vector<glm::vec2> newTetrominoTranslations;
//		newTetrominoTranslations.push_back(CurrentTetrominoTranslations[0]);
//
//		for (int i = 0; i < TET_GRID_COUNT; i++) {
//			tetrominoBitGrid[i] = rotatedTetrominoGrid[i];
//
//			if (tetrominoBitGrid[i] == 1) {
//				int row = i / 4;
//				int col = i % 4;
//				glm::vec2 position = CurrentTetrominoTranslations[0] +
//					glm::vec2(col * 0.1f, -row * 0.1f);
//
//				newTetrominoTranslations.push_back(position);
//
//				int boardRow = int(round((TopPosY - position.y) / 0.1f));
//				int boardCol = int(round((position.x - LeftPos) / 0.1f));
//				boardBitTmp[boardRow * COL_COUNT + boardCol] = 1;
//			}
//		}
//
//		CurrentTetrominoTranslations = newTetrominoTranslations;
//	}
//}




void drawBoard()
{
	for (int i = 0; i < BoardSize; ++i) {
		int row = i / COL_COUNT; // calculate row
		int col = i % COL_COUNT; // calculate column
		glm::vec2 squareTranslation(LeftPos + col * 0.10f, 0.9f - row * 0.10f); // set translation based on row and column
		if (board.at(i) == L'X' /*|| */ ) {

			drawSquare(COLOR_NAVY, squareTranslation);
			boardBit[i] = 1;
			boardBitTmp[i] = 1;
		}
		if (boardBit[i] != 0)
		{
			drawSquare(COLOR_NAVY, squareTranslation);
		}

	}
}

void printCurrentTetrominoBoardPositions() {
	int i = 0;
	for (const glm::vec2& blockPos : CurrentTetrominoTranslations) {
		int row = int(round((TopPosY - blockPos.y) / 0.1f));
		int col = int(round((blockPos.x - LeftPos) / 0.1f));

		cout << i << "Block at (" << blockPos.x << ", " << blockPos.y
			<< ") is in row " << row << " and column " << col
			<< " on the board, with boardBit value: " << boardBit[row * COL_COUNT + col] << endl;
		i++;
	}
}



bool canMoveDown() {
	for (int i = TET_GRID_COUNT - 1; i >= 0; i--) {
		glm::vec2 blockPos = CurrentTetrominoTranslations[i];
		int row = int(round((TopPosY - blockPos.y) / 0.1f));
		int col = int(round((blockPos.x - LeftPos) / 0.1f));
		if (tetrominoBitGrid[i] == 1) {
			// Check for static blocks only (value == 1)
			if (boardBit[(row + 1) * COL_COUNT + col] > 0 ) { //&& tetrominoBitGrid[i] != boardBit[(row + 1) * COL_COUNT + col]
				return false;
			}
		}

	}
	return true;
}

bool canMoveLeft() {
	for (int i = 0; i < CurrentTetrominoTranslations.size(); i++) {
		glm::vec2 blockPos = CurrentTetrominoTranslations[i];
		int row = int(round((TopPosY - blockPos.y) / 0.1f));
		int col = int(round((blockPos.x - LeftPos) / 0.1f));

		if (tetrominoBitGrid[i] == 1) {
			if (col == 0 || boardBit[row * COL_COUNT + (col - 1)] > 0) {
				return false;
			}
		}
	}
	return true;
}

bool canMoveRight() {
	for (int i = 0; i < CurrentTetrominoTranslations.size(); i++) {
		glm::vec2 blockPos = CurrentTetrominoTranslations[i];
		int row = int(round((TopPosY - blockPos.y) / 0.1f));
		int col = int(round((blockPos.x - LeftPos) / 0.1f));

		if (tetrominoBitGrid[i] == 1) {
			if (col == (COL_COUNT - 1) || boardBit[row * COL_COUNT + (col + 1)] > 0) {
				return false;
			}
		}
	}
	return true;
}

void moveTetromino(glm::vec2 direction) {


	std::vector<glm::vec2> newPositions(TET_GRID_COUNT);

	// Calculate new positions without updating CurrentTetrominoTranslations yet
	for (int i = 0; i < TET_GRID_COUNT; i++) {
		newPositions[i] = CurrentTetrominoTranslations[i] + direction;
	}

	// Clear the previous position of the tetromino in the boardBit array
	for (int i = 0; i < TET_GRID_COUNT; i++) {
		if (tetrominoBitGrid[i] == 1) {
			int row = int(round((TopPosY - CurrentTetrominoTranslations[i].y) / 0.1f));
			int col = int(round((CurrentTetrominoTranslations[i].x - LeftPos) / 0.1f));
			boardBitTmp[row * COL_COUNT + col] = 0;
		}
	}

	// Update the new positions in CurrentTetrominoTranslations and boardBit array
	for (int i = 0; i < TET_GRID_COUNT; i++) {
		glm::vec2 blockPos = newPositions[i];
		int row = int(round((TopPosY - blockPos.y) / 0.1f));
		int col = int(round((blockPos.x - LeftPos) / 0.1f));
		if (tetrominoBitGrid[i] == 1) {

			if (randomTetromino == 0) 
			{
				boardBitTmp[row * COL_COUNT + col] = 7;

			}
			else 
			{
				boardBitTmp[row * COL_COUNT + col] = randomTetromino + 1;

			}
		}
		if (tetrominoBitGrid[i] == 1 || CurrentTetrominoTranslations[i] != newPositions[i]) {
			CurrentTetrominoTranslations[i] = newPositions[i]; // Update the actual position
			glUniform2fv(translationLocation, 1, glm::value_ptr(CurrentTetrominoTranslations[i]));
		}
	}

	// Render the new positions
	for (int i = 0; i < TET_GRID_COUNT; i++) {
		glUniform2fv(translationLocation, 1, glm::value_ptr(CurrentTetrominoTranslations[i]));
	}

}

void moveTetDown() {
	stepsCount++;

	glm::vec2 direction(DOWN[0] * 0.1f, DOWN[1] * 0.1f);
	moveTetromino(direction);
}

void moveTetLeft() {
	glm::vec2 direction(LEFT[0] * 0.1f, LEFT[1] * 0.1f);
	moveTetromino(direction);
}

void moveTetRight() {
	glm::vec2 direction(RIGHT[0] * 0.1f, RIGHT[1] * 0.1f);
	moveTetromino(direction);
}

void clearPrevTet()
{
	for (int i = 0; i < TET_GRID_COUNT; i++)
	{
		tetrominoBitGrid[i] = 0;
	}
	CurrentTetrominoTranslations.clear();
	currentTetRotation = 0;

}


void handleInput(GLFWwindow* window, glm::vec2& translation, bool& isDownKeyPressed) {
	if (!isDownKeyPressed) {
		if (glfwGetKey(window, GLFW_KEY_RIGHT) || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS &&canMoveRight()) {
			moveTetRight();
			isDownKeyPressed = true;
		}
		else if (glfwGetKey(window, GLFW_KEY_LEFT) || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && canMoveLeft()) {
			moveTetLeft();
			isDownKeyPressed = true;
		}
		else if (glfwGetKey(window, GLFW_KEY_UP) || glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			//clearPrevTet();
			isDownKeyPressed = true;
			currentTetRotation++;
			//rotateTetromino(currentTetRotation);
		}
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		if (canMoveDown())
		{
			moveTetDown();
			printBoardGlobe();
		}
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE
		&& glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE
		&& glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE) {
		isDownKeyPressed = false;
	}
}


void generateRandomTetromino(int randomIndex) {
	wstring shape = shapes[randomIndex];
	float offsetX = LeftPos + 0.4f;
	//float offsetY = 1.2f;
	for (int j = 0; j < TET_GRID_COUNT; j++) {
		int row = j / 4; // calculate row
		int col = j % 4; // calculate column


		if (CurrentTetrominoTranslations.size() < TET_GRID_COUNT) {
			// set translation based on row and column
			glm::vec2 squareTranslation(offsetX + col * 0.10f, (TopPosY - row * 0.10f));
			CurrentTetrominoTranslations.push_back(squareTranslation);
		}

		if (shape[j] == L'X') {
			tetrominoBitGrid[j] = 1;
			drawSquare(colors[randomIndex], CurrentTetrominoTranslations[j]);
		}
	}
}

void CreateTetWindow4()
{
	glm::vec2 translation(0.05f, 0.0f);

	Clock Time;
	Display display(SCREEN_WIDTH, SCREEN_HEIGHT, PROJECT_NAME);
	GLFWwindow* window = display.getWindow();
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	float aspectRatio = static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT);
	projectionMatrix = glm::ortho(-1.0f * aspectRatio, 1.0f * aspectRatio, -1.0f, 1.0f);
	SetUpShader();


	srand(time(NULL));
	createBuffers(vao, vbo, triangle_vertices, sizeof(triangle_vertices));
	createBuffers(vao, vbo, square_vertices, sizeof(square_vertices));

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUniform2fv(translationLocation, 1, glm::value_ptr(translation));

	bool isGameover = false;
	randomTetromino = rand() % shapesLength;

	cout << randomTetromino<< endl;
	bool isDownKeyPressed = false;

	while (!display.shouldClose()) {

		// Game Timing =======================
		Time.currentFrameTime = static_cast<float>(glfwGetTime());
		Time.deltaTime = Time.currentFrameTime - Time.lastFrameTime;
		Time.lastFrameTime = Time.currentFrameTime;
		glClear(GL_COLOR_BUFFER_BIT);

		// INPUT  =======================
		handleInput(window, translation, isDownKeyPressed);


		// LOGOC - Collision detection, falling, score, rotate

		if (!isGameover)
		{
			// RENDER
			drawBoard();
			generateRandomTetromino(randomTetromino);

		}


		display.swapBuffers();
		display.pollEvents();

		// update the translation vector every interval seconds
		if (Time.currentFrameTime >= Time.INTERVAL && !isGameover) {
			if (canMoveDown()) {
				moveTetDown();
				printBoardGlobe();


			}
			else if(!canMoveDown() && stepsCount > 1)
			{
				checkAndClearRows();
				updateBoardFromTemporary();

				scoreCount += randomTetromino;
				clearPrevTet();
				randomTetromino = rand() % shapesLength;

				cout << randomTetromino << endl;
				stepsCount = 0;

				generateRandomTetromino(randomTetromino);
			}
			else if(!canMoveDown() && stepsCount == 0)
			{
				isGameover = true;
			}
			glUniform2fv(translationLocation, 1, glm::value_ptr(translation));
			Time.INTERVAL += 1.0f;
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
//////////////////////////////////////////////////////////////////////////////
///////////////////////RENDERING LOGICs BELOW/////////////////////////////////
//////////////////////////////////////////////////////////////////////////////



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
	projectionLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
	// Check for errors in getting the uniform locations
	if (colorLocation == -1 || translationLocation == -1 || projectionLocation == -1) {
		cout << "Error: Failed to get uniform locations" << endl;
	}
}

GLuint createShader(GLenum shaderType, const GLchar* shaderSource) {
	//creates an empty shader object, ready to accept source code and be compiled.
	GLuint shader = glCreateShader(shaderType);
	//hands shader source code to the shader object so that it can keep a copy of it.
	glShaderSource(shader, 1, &shaderSource, NULL);
	//compiles whatever source code is contained in the shader object.
	glCompileShader(shader);
	return shader;
}

GLuint createShaderProgram(const GLchar* vertexShaderSource, const GLchar* fragmentShaderSource) {
	// Create program, attach shaders to it, and link it

	//creates a program object to which you can attach shader objects.
	GLuint shaderProgram = glCreateProgram();
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


void drawSquare(glm::vec4 color, glm::vec2 squareTranslation) {
	glUseProgram(shaderProgram);
	glUniform4fv(colorLocation, 1, glm::value_ptr(color));
	glUniform2fv(translationLocation, 1, glm::value_ptr(squareTranslation));
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glUseProgram(0);
	glBindVertexArray(0);
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
