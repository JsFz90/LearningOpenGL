#pragma once
#include <glad/glad.h>
#include <string>
#include <iostream>
#include <fstream> // files
#include <sstream>

struct vector4
{
	float x, y, z, w;
};

class Shader
{

public:
	// constructor generates the shader on the fly
	// constructor reads and builds the shader
	Shader(const char* vertexPath, const char* fragmentPath) : m_ID(0)
	{
		// 1. retrieve the vertex/fragment source code from filePath
		std::string vertexSource, fragmentSource;
		std::ifstream vShaderFile, fShaderFile;
		// ensure ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			//open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			// read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// close file handlers
			vShaderFile.close();
			fShaderFile.close();
			// convert stream into string
			vertexSource = vShaderStream.str();
			fragmentSource = fShaderStream.str();
		}
		catch (const std::ifstream::failure& error)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << error.what() << std::endl;
		}
		// 2. Compile shaders
		const char* vShaderSource = vertexSource.c_str();
		const char* fShaderSource = fragmentSource.c_str();
		// Compile Vertex Shader
		unsigned int vertexID = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexID, 1, &vShaderSource, NULL);
		glCompileShader(vertexID);
		checkErrors(vertexID, GL_COMPILE_STATUS);

		// Compile Fragment Shader
		unsigned int fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentID, 1, &fShaderSource, NULL);
		glCompileShader(fragmentID);
		checkErrors(fragmentID, GL_COMPILE_STATUS);

		//3. Link Shader Program
		unsigned int programID = glCreateProgram();
		glAttachShader(programID, vertexID);
		glAttachShader(programID, fragmentID);
		glLinkProgram(programID);
		checkErrors(programID, GL_LINK_STATUS);

		// Delete the shaders as they're linked into our program now and no longer necessary
		glDeleteShader(vertexID);
		glDeleteShader(fragmentID);

		m_ID = programID;  // Save ID 
	}

	~Shader() { glDeleteProgram(m_ID); }

	// activate the shader
	// ------------------
	void Bind() const
	{
		glUseProgram(m_ID);
	}

	// utility uniform functions
   // ------------------------------------------------------------------------
	void SetFloat4(const std::string& name, const vector4& value)
	{
		int location = glGetUniformLocation(m_ID, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

private:
	// utility function for checking shader compilation/linking errors.
	// ------------------------------------------------------------------------
	void checkErrors(unsigned int ID, int statusType)
	{
		int success;
		char infoLog[1024];

		if (statusType == GL_COMPILE_STATUS)
		{
			glGetShaderiv(ID, statusType, &success);
			if (!success)
			{
				glGetShaderInfoLog(ID, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER::COMPILATION_FAILED: " << infoLog << std::endl;
			}
		}
		else if (statusType == GL_LINK_STATUS)
		{
			glGetProgramiv(ID, statusType, &success);
			if (!success)
			{
				glGetProgramInfoLog(ID, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
			}
		}
		else 
		{ 
			std::cout << "ERROR::STATUS_NOT_SUPPORTED " << std::endl;
		}
	}

private:
	unsigned int m_ID; // the program ID (Shader Program ID)
};