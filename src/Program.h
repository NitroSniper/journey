//
// Created by mahie on 23/02/2024.
//

#ifndef OPENGLTEMPL_PROGRAM_H
#define OPENGLTEMPL_PROGRAM_H

#include <filesystem>
#include <glad/glad.h>

#include "Diagnose.h"
#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>
#include <stb_include.h>
#include <string>
#include <string_view>

class Program {
private:
  GLuint id;
  GLuint create_shader(std::filesystem::path path, GLenum shader_type) {

    char *source = stb_include_file(
        const_cast<char *>(path.c_str()), nullptr,
        const_cast<char *>(path.parent_path().c_str()), Diagnose::callback());
    Diagnose::unwrap();
    GLuint shader = glCreateShader(shader_type);
    // OpenGL does that automatically.
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    // check for shader compile errors
    GLint compiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
      glGetShaderInfoLog(shader, 512, nullptr, Diagnose::callback());
      // Should Only be Vert or Frag Shader passed in
      std::string_view shader_type_str{
          (shader_type == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT"};
      std::cout << "ERROR::SHADER::" << shader_type_str
                << "::COMPILATION_FAILED\n"
                << Diagnose::callback() << std::endl;
    }

    free(source);
    return shader;
  }

public:
  Program(std::filesystem::path vert, std::filesystem::path frag) {
    auto vertex = create_shader(vert, GL_VERTEX_SHADER);
    auto fragment = create_shader(frag, GL_FRAGMENT_SHADER);
    id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);
    GLint linked = 0;
    glGetProgramiv(id, GL_LINK_STATUS, &linked);
    if (!linked) {
      glGetProgramInfoLog(id, 512, nullptr, Diagnose::callback());
      std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                << Diagnose::callback() << std::endl;
    }
    glDeleteShader(vertex);
    glDeleteShader(fragment);
  }

  operator GLuint() const { return id; }
};

struct Attribute {
  GLuint attrib_index;
  size_t offset;
  std::pair<GLenum, GLint> type_size;
};

#endif // OPENGLTEMPL_PROGRAM_H
