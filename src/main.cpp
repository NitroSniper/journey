// Copyright 2024 <Mahie Miah>

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Program.h"
#include <ostream>

struct Vertex {
  float position[2];
  float color[3];
};

int idk(int, char **) {
  GLFWwindow *window;
  if (!glfwInit()) {
    return -1;
  }
  window = glfwCreateWindow(960, 540, "I am a Window", NULL, NULL);
  glfwMakeContextCurrent(window);

  // Check if glad is loaded
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Couldn't load OpenGL" << std::endl;
    glfwTerminate();
    return 0;
  }

  Program shaderProgram{"shader/basic.vert", "shader/basic.frag"};

  Vertex vertex1 = Vertex{{-0.5, -0.5}, {1, 0, 0}};
  Vertex vertex2 = Vertex{{0, 0.5}, {0, 1, 0}};
  Vertex vertex3 = Vertex{{0.5, -0.25}, {0, 0, 1}};

  Vertex shape[] = {vertex1, vertex2, vertex3};
  //
  // generate buffers
  GLuint VAO, VBO /*, IBO */;

  // Vertex Array Object holds both the VAO and VBO in context

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  // glGenBuffers(1, &IBO);

  // Bind the Vertex Array Object, then Bind the Vertex Buffer
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(shape), shape, GL_STATIC_DRAW);

  glVertexAttribPointer(
      0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
      (void *)offsetof(Vertex, position)); // set location 0 to be position
  glEnableVertexAttribArray(0);
  glBindAttribLocation(shaderProgram, 0, "position");

  glVertexAttribPointer(
      1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
      (void *)offsetof(Vertex, color)); // set location 1 to be color
  glEnableVertexAttribArray(1);
  glBindAttribLocation(shaderProgram, 1, "color");

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0);

  glm::vec2 x = glm::vec2(1.0f, 1.0f);
  GLint x_location = glGetUniformLocation(shaderProgram, "x");

  glUniform2fv(x_location, 1, glm::value_ptr(x));

  // used to resize window
  auto resizing = [](GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
  };
  glfwSetFramebufferSizeCallback(window, resizing);

  glm::float32 t = 0;
  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0, 0, 0, 1);
    glUseProgram(shaderProgram);
    glBindVertexArray(
        VAO); // seeing as we only have a single VAO there's no need to bind it
              // every time, but we'll do so to keep things a bit more organized

    t += 0.005;
    x.x = glm::sin(5 * t);
    x.y = glm::sin(3 * t);
    glUniform2fv(x_location, 1, glm::value_ptr(x *= 0.5));
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwPollEvents();
    glfwSwapBuffers(window);
  }
  glfwTerminate();
  return 0;
}
