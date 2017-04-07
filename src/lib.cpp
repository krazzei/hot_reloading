#include <iostream>
#include <GL\glew.h>
#include <gl\gl.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "template.h"

const char* vertexSource = "#version 120\n"
"uniform mat4 transform;\n"
"attribute vec3 position;\n"
"void main() {\n"
"	gl_Position = transform * vec4(position, 1.0);\n"
"}";

const char* fragmentSource = "#version 120\n"
"void main() {\n"
"	gl_FragColor = vec4(1, 0, 0, 0);\n"
"}";

GLuint program;
GLuint attribPosition;
GLuint vertexArray;
GLuint transformPosition;
glm::mat4 transform;

bool CompileShader(void)
{
	GLint compile_ok = GL_FALSE;
	GLint link_ok = GL_FALSE;

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertexSource, NULL);
	glCompileShader(vs);
	glGetShaderiv(vs, GL_COMPILE_STATUS, &compile_ok);
	if (!compile_ok)
	{
		GLint maxLength = 0;
		glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &maxLength);

		GLchar *errorLog = new GLchar[maxLength];
		glGetShaderInfoLog(vs, maxLength, &maxLength, errorLog);
		std::cerr << "Error in vertex shader: " << errorLog << std::endl;
		return false;
	}

	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragmentSource, NULL);
	glCompileShader(fs);
	glGetShaderiv(fs, GL_COMPILE_STATUS, &compile_ok);
	if (!compile_ok)
	{
		std::cerr << "Error in fragment shader" << std::endl;
		return false;
	}

	program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
	if (!link_ok)
	{
		std::cerr << "Error in glLinkProgram" << std::endl;
		return false;
	}

	attribPosition = glGetAttribLocation(program, "position");
	if (attribPosition == -1)
	{
		std::cerr << "Could not bind attribute position" << std::endl;
		return false;
	}

	transformPosition = glGetUniformLocation(program, "transform");
	if (transformPosition == -1)
	{
		std::cerr << "Could not bind attribute transform" << std::endl;
	}

	return true;
}

extern "C" GAME_INIT(InitGame)
{
	float vertex[9] = { -0.8f, -0.8f, 0,
		0, 0.8f, 0,
		0.8f, -0.8f, 0 };

	transform = glm::mat4();
	transform = glm::translate(transform, glm::vec3(0, 0, 0));

	// TODO: check openGL version
	vertexArray;
	glCreateBuffers(1, &vertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, vertexArray);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 9, &vertex, GL_STATIC_DRAW);

	if (!CompileShader())
	{
		return false;
	}

	glClearColor(0.39f, 0.58f, 0.92f, 1.0);

	return true;
}

extern "C" GAME_RENDER_AND_UPDATE(RenderAndUpdate)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(program);
	glUniformMatrix4fv(transformPosition, 1, GL_FALSE, &transform[0][0]);

	glBindBuffer(GL_ARRAY_BUFFER, vertexArray);
	glEnableVertexAttribArray(attribPosition);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, 0, (char*)NULL);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
