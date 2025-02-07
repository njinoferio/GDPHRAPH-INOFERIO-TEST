#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"


float radius = 0.5f;
float angle = 0.0f;
float rabbitRotationSpeed = 0.5f;
float circleRotationSpeed = 0.08f;

float scale_x = 1;
float scale_y = 1;
float scale_z = 1;


int main(void) {
	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	window = glfwCreateWindow(640, 640, "INOFERIO, NAOMI ", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	gladLoadGL();


	std::fstream vertSrc("Shaders/sample.vert");
	std::stringstream vertBuff;

	vertBuff << vertSrc.rdbuf();

	std::string vertS = vertBuff.str();
	const char* v = vertS.c_str();

	std::fstream fragSrc("Shaders/sample.frag");
	std::stringstream fragBuff;

	fragBuff << fragSrc.rdbuf();

	std::string fragS = fragBuff.str();
	const char* f = fragS.c_str();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &v, NULL);

	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShader, 1, &f, NULL);

	glCompileShader(fragmentShader);

	GLuint shaderProg = glCreateProgram();

	glAttachShader(shaderProg, vertexShader);
	glAttachShader(shaderProg, fragmentShader);

	glLinkProgram(shaderProg);

	std::string path = "3D/bunny.obj";
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> material;
	std::string warning, error;

	tinyobj::attrib_t attributes;

	bool success = tinyobj::LoadObj(
		&attributes,
		&shapes,
		&material,
		&warning,
		&error,
		path.c_str()
	);

	std::vector<GLuint> mesh_indices;
	for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
		mesh_indices.push_back(shapes[0].mesh.indices[i].vertex_index);
	}

	GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(GLfloat) * attributes.vertices.size(),
		&attributes.vertices[0],
		GL_STATIC_DRAW
	);

	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		3 * sizeof(GLfloat),
		(void*)0
	);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		sizeof(GLuint) * mesh_indices.size(),
		mesh_indices.data(),
		GL_STATIC_DRAW
	);

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

        angle += circleRotationSpeed;

        glUseProgram(shaderProg);

        for(int i = 0; i < 3; i++){
        float rabbitAngle = (i * 8 * 3.14)/3;
        float newX = radius * cos(angle + rabbitAngle);
        float newY = radius * sin(angle + rabbitAngle);
		float rabbitRotationAngle = glfwGetTime() * 0.5 + (sin(10 * 0.5) * 0.2);

		glm::mat4 identity_matrix4 = glm::mat4(1.0f);

		glm::mat4 transformation_matrix = glm::translate(
			identity_matrix4,
			glm::vec3(newX, newY, 0.1)
		);
		transformation_matrix = glm::scale(
			transformation_matrix,
			glm::vec3(scale_x, scale_y, scale_z)
		);
		transformation_matrix = glm::rotate(
			transformation_matrix,rabbitRotationAngle,
			glm::vec3(0.0, 0.0, 1.0)
		);

		unsigned int transformLoc = glGetUniformLocation(shaderProg, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformation_matrix));
		

		glBindVertexArray(VAO);
		glUseProgram(shaderProg);
		glBindVertexArray(VAO);

		glDrawElements(GL_TRIANGLES, mesh_indices.size(), GL_UNSIGNED_INT, 0);
        }
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}