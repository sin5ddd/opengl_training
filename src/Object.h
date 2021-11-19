//
// Created by kitayama on 2021/11/19.
//

#ifndef UNTITLED2_OBJECT_H
#define UNTITLED2_OBJECT_H

#include <GL/glew.h>

class Object {
	GLuint vao;
	GLuint vbo;
public:
	struct Vertex {
			GLfloat position[2];
	};

	Object(GLint size, GLsizei vertexCount, const Vertex* vertex) {
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex), vertex, GL_STATIC_DRAW);

		// link to vertex shader
		glVertexAttribPointer(0, size, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);
	}

	virtual ~Object() {
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
	}

private:
	Object(const Object& o);

	Object& operator=(const Object& o);

public:
	void bind() const{
		glBindVertexArray(vao);
	}
};

#endif //UNTITLED2_OBJECT_H
