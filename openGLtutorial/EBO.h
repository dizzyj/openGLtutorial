#ifndef EBO_CLASS_H
#define	EBO_CLASS_H

#include<glad/glad.h>

class EBO {

public:
	// Reference ID of the Vertex Buffer Object
	GLuint ID;
	//Constructor that generates a Vertex Buffer Object and links it to verticies
	EBO(GLuint* indices, GLsizeiptr size);
	//binds VBO
	void Bind();
	//Unbinds VBO
	void Unbind();
	//Deletes VBO
	void Delete();
};

#endif

