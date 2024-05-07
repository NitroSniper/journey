#ifndef SHADER_H
#define SHADER_H

#include <filesystem>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Diagnose.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stb_include.h>
#include <string>

class Shader {
public:
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

  unsigned int ID;
  // constructor generates the shader on the fly
  // ------------------------------------------------------------------------
  Shader(std::filesystem::path vert, std::filesystem::path frag) {
    auto vertex = create_shader(vert, GL_VERTEX_SHADER);
    auto fragment = create_shader(frag, GL_FRAGMENT_SHADER);
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    GLint linked = 0;
    glGetProgramiv(ID, GL_LINK_STATUS, &linked);
    if (!linked) {
      glGetProgramInfoLog(ID, 512, nullptr, Diagnose::callback());
      std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                << Diagnose::callback() << std::endl;
    }
    glDeleteShader(vertex);
    glDeleteShader(fragment);
  }

  // activate the shader
  // ------------------------------------------------------------------------
  void use() const { glUseProgram(ID); }
  // utility uniform functions
  // ------------------------------------------------------------------------
  void setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
  }
  // ------------------------------------------------------------------------
  void setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
  }
  // ------------------------------------------------------------------------
  void setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
  }
  // ------------------------------------------------------------------------
  void setVec2(const std::string &name, const glm::vec2 &value) const {
    glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
  }
  void setVec2(const std::string &name, float x, float y) const {
    glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
  }
  // ------------------------------------------------------------------------
  void setVec3(const std::string &name, const glm::vec3 &value) const {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
  }
  void setVec3(const std::string &name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
  }
  // ------------------------------------------------------------------------
  void setVec4(const std::string &name, const glm::vec4 &value) const {
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
  }
  void setVec4(const std::string &name, float x, float y, float z,
               float w) const {
    glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
  }
  // ------------------------------------------------------------------------
  void setMat2(const std::string &name, const glm::mat2 &mat) const {
    glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE,
                       &mat[0][0]);
  }
  // ------------------------------------------------------------------------
  void setMat3(const std::string &name, const glm::mat3 &mat) const {
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE,
                       &mat[0][0]);
  }
  // ------------------------------------------------------------------------
  void setMat4(const std::string &name, const glm::mat4 &mat) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE,
                       &mat[0][0]);
  }

private:
  // utility function for checking shader compilation/linking errors.
  // ------------------------------------------------------------------------
  void checkCompileErrors(GLuint shader, std::string type) {
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
      glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
      if (!success) {
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        std::cout
            << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
            << infoLog
            << "\n -- --------------------------------------------------- -- "
            << std::endl;
      }
    } else {
      glGetProgramiv(shader, GL_LINK_STATUS, &success);
      if (!success) {
        glGetProgramInfoLog(shader, 1024, NULL, infoLog);
        std::cout
            << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
            << infoLog
            << "\n -- --------------------------------------------------- -- "
            << std::endl;
      }
    }
  }
};
#endif
