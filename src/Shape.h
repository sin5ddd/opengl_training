//
// Created by kitayama on 2021/11/19.
//

#ifndef UNTITLED2_SHAPE_H
#define UNTITLED2_SHAPE_H

#include <memory>
#include "Object.h"

class Shape {
	std::shared_ptr<const Object> object;
protected:
	const GLsizei vertexCount;
public:
	// Shape
	Shape(GLint size, GLsizei vertexCount, const Object::Vertex* vertex) : object(new Object(size, vertexCount, vertex)),
	                                                                       vertexCount(vertexCount) {

	}

	void draw() const {
		object->bind();
		execute();
	}

	virtual void execute() const {
		glDrawArrays(GL_LINE_LOOP, 0, vertexCount);
	}
};

#endif //UNTITLED2_SHAPE_H
