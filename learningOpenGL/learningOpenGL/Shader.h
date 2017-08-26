#ifndef SHADER_H
#define SHADER_H

#include<string>
#include <fstream>
#include  <sstream>
#include  <iostream>

#include <GL/glew.h>; // Include glew to get all the required OpenGL headers

class Shader //Whole class in header file for learning/portability reasons
{
public:
	// The program ID
	unsigned int ID;
	// Constructor read and builds shader from file paths
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
	{
		// 1. Retrieve the vertex/fragment source code from the filePaths
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;

		// Ensure ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			// Open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			
			// Read each file's contents into streams
			std::stringstream vShaderStream, fShaderStream; // to hold stream of characters of source code
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			
			// Close file handles
			vShaderFile.close();
			fShaderFile.close();

			// Convert streamm into GLchar array
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch(std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}

		// Turn c-string into GLchar array
		const GLchar* vShaderCode = vertexCode.c_str();
		const GLchar* fShaderCode = fragmentCode.c_str();


		// 2. Compile shaders
		GLuint vertex, fragment;
		GLint success;
		GLchar infoLog[512];

		// Vertex Shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL); //replaces source code of shader
		glCompileShader(vertex); // compile the shader
		// print compile errors if any
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success); //query shader for given parameter (compile status)
		if(!success)
		{
			glGetShaderInfoLog(vertex, 512, NULL, infoLog); //returns info log of shader which contains all info relating to compilation
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		// Fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL); //replaces source code of shader
		glCompileShader(fragment); // compile the shader
		// print compile errors if any
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success); //query shader for given parameter (compile status)
		if (!success)
		{
			glGetShaderInfoLog(fragment, 512, NULL, infoLog); //returns info log of shader which contains all info relating to compilation
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		// Create and Set-up Shader Program
		ID = glCreateProgram(); // Create program object and return reference
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID); // Links all attached shaders into one object and matches outputs and inputs of different shaders
		// Print linking errors is any
		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(ID, 512, NULL, infoLog); //returns info log of Program
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}

		// Delete the shaders as they're linked into our program now and no longer needed
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	// Use the program
	void Use() { glUseProgram(ID); }
};

#endif


