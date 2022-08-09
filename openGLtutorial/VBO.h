#ifndef VBO_CLASS_H
#define	VBO_CLASS_H

#include<glad/glad.h>

class VBO {

	public:
	// Reference ID of the Vertex Buffer Object
		GLuint ID;
	//Constructor that generates a Vertex Buffer Object and links it to verticies
		VBO(GLfloat* vertices, GLsizeiptr size);
	//binds VBO
		void Bind();
	//Unbinds VBO
		void Unbind();
	//Deletes VBO
		void Delete();
};

#endif

