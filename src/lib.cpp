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

float vertex[9] = { -0.8f, -0.8f, 0,
0, 0.8f, 0,
0.8f, -0.8f, 0 };

const GLchar *positionName = "position";
const GLchar *transformName = "transform";

bool CompileShader(render_stuff *RenderStuff)
{
	GLint compile_ok = GL_FALSE;
	GLint link_ok = GL_FALSE;

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertexSource, NULL);
	glCompileShader(vs);
	glGetShaderiv(vs, GL_COMPILE_STATUS, &compile_ok);
	if (!compile_ok)
	{
		glDeleteShader(vs);
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
		glDeleteShader(vs);
		glDeleteShader(fs);
		std::cerr << "Error in fragment shader" << std::endl;
		return false;
	}

	RenderStuff->program = glCreateProgram();
	glAttachShader(RenderStuff->program, vs);
	glAttachShader(RenderStuff->program, fs);
	glLinkProgram(RenderStuff->program);
	glGetProgramiv(RenderStuff->program, GL_LINK_STATUS, &link_ok);
	if (!link_ok)
	{
		glDeleteProgram(RenderStuff->program);
		glDeleteShader(vs);
		glDeleteShader(fs);
		std::cerr << "Error in glLinkProgram" << std::endl;
		return false;
	}

	RenderStuff->attribPosition = glGetAttribLocation(RenderStuff->program, positionName);
	if (RenderStuff->attribPosition == -1)
	{
		std::cerr << "Could not bind attribute position" << std::endl;
		return false;
	}

	RenderStuff->transformPosition = glGetUniformLocation(RenderStuff->program, transformName);
	if (RenderStuff->transformPosition == -1)
	{
		std::cerr << "Could not bind attribute transform" << std::endl;
	}

	glDeleteShader(vs);
	glDeleteShader(fs);

	return true;
}

extern "C" GAME_INIT(InitGame)
{
	State->triangle->transform = glm::mat4();
	State->triangle->transform = glm::translate(State->triangle->transform, glm::vec3(0, 0, 0));

	// TODO: check openGL version
	glCreateBuffers(1, &State->triangle->vertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, State->triangle->vertexArray);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 9, &vertex, GL_STATIC_DRAW);

	if (!CompileShader(State->triangle))
	{
		return false;
	}

	glClearColor(0.39f, 0.58f, 0.92f, 1.0);

	return true;
}

extern "C" GAME_RENDER_AND_UPDATE(RenderAndUpdate)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(State->triangle->program);
	glUniformMatrix4fv(State->triangle->transformPosition, 1, GL_FALSE, &State->triangle->transform[0][0]);

	glBindBuffer(GL_ARRAY_BUFFER, State->triangle->vertexArray);
	glEnableVertexAttribArray(State->triangle->attribPosition);
	glVertexAttribPointer(State->triangle->attribPosition, 3, GL_FLOAT, GL_FALSE, 0, (char*)NULL);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray(State->triangle->attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

extern "C" GAME_TEARDOWN(Teardown)
{
	glDeleteBuffers(1, &State->triangle->vertexArray);
	glDeleteProgram(State->triangle->program);
}
