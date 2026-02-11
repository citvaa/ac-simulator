#include "Renderer.h"
#include <GL/glew.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <glm/gtc/type_ptr.hpp>

Renderer::Renderer() {}
Renderer::~Renderer() {
  if (phongProgram_ != 0) glDeleteProgram(phongProgram_);
  if (blinnProgram_ != 0) glDeleteProgram(blinnProgram_);
}

bool Renderer::init() {
  // Compile and link shaders
  phongProgram_ = createShaderProgram("Shaders/phong.vert", "Shaders/phong.frag");
  blinnProgram_ = createShaderProgram("Shaders/phong.vert", "Shaders/blinn.frag");
  if (phongProgram_ == 0) {
    std::cerr << "Failed to create Phong shader program" << std::endl;
    return false;
  }
  // blinnProgram_ is optional; warn if missing
  if (blinnProgram_ == 0) {
    std::cerr << "Warning: Blinn-Phong shader failed to compile (blinn optional)" << std::endl;
  }
  return true;
}

void Renderer::render() {
  // Placeholder render call; actual draw calls will be implemented later.
}

void Renderer::setViewProjection(const glm::mat4& view, const glm::mat4& proj) {
  if (phongProgram_ != 0) {
    glUseProgram(phongProgram_);
    GLint loc = glGetUniformLocation(phongProgram_, "view");
    if (loc >= 0) glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(view));
    loc = glGetUniformLocation(phongProgram_, "projection");
    if (loc >= 0) glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(proj));
  }
  if (blinnProgram_ != 0) {
    glUseProgram(blinnProgram_);
    GLint loc = glGetUniformLocation(blinnProgram_, "view");
    if (loc >= 0) glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(view));
    loc = glGetUniformLocation(blinnProgram_, "projection");
    if (loc >= 0) glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(proj));
  }
  glUseProgram(0);
}

std::string Renderer::loadShaderSource(const char* path) {
  std::vector<std::string> candidates = { std::string(path), std::string("../") + path, std::string("./") + path };
  for (const auto& p : candidates) {
    std::ifstream in(p);
    if (in) {
      std::stringstream ss; ss << in.rdbuf();
      std::cerr << "Loaded shader from: " << p << std::endl;
      return ss.str();
    }
  }

  std::string basename(path);
  auto pos = basename.find_last_of("/\\");
  if (pos != std::string::npos) basename = basename.substr(pos + 1);
  std::vector<std::string> more = { std::string("Shaders/") + basename, std::string("../Shaders/") + basename };
  for (const auto& p : more) {
    std::ifstream in(p);
    if (in) {
      std::stringstream ss; ss << in.rdbuf();
      std::cerr << "Loaded shader from: " << p << std::endl;
      return ss.str();
    }
  }

  return std::string();
}

static unsigned int compileShader(GLenum type, const std::string& src) {
  unsigned int shader = glCreateShader(type);
  const char* cstr = src.c_str();
  glShaderSource(shader, 1, &cstr, nullptr);
  glCompileShader(shader);
  GLint ok = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
  if (!ok) {
    GLint len = 0; glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
    std::string log(len, '\0');
    glGetShaderInfoLog(shader, len, nullptr, &log[0]);
    std::cerr << "Shader compile error: " << log << std::endl;
    glDeleteShader(shader);
    return 0;
  }
  return shader;
}

unsigned int Renderer::createShaderProgram(const char* vertPath, const char* fragPath) {
  std::string vertSrc = loadShaderSource(vertPath);
  std::string fragSrc = loadShaderSource(fragPath);
  if (vertSrc.empty() || fragSrc.empty()) {
    std::cerr << "Failed to load shader sources: " << vertPath << ", " << fragPath << std::endl;
    return 0;
  }

  unsigned int vert = compileShader(GL_VERTEX_SHADER, vertSrc);
  if (vert == 0) return 0;
  unsigned int frag = compileShader(GL_FRAGMENT_SHADER, fragSrc);
  if (frag == 0) { glDeleteShader(vert); return 0; }

  unsigned int prog = glCreateProgram();
  glAttachShader(prog, vert);
  glAttachShader(prog, frag);
  glLinkProgram(prog);
  GLint ok = 0; glGetProgramiv(prog, GL_LINK_STATUS, &ok);
  if (!ok) {
    GLint len = 0; glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
    std::string log(len, '\0');
    glGetProgramInfoLog(prog, len, nullptr, &log[0]);
    std::cerr << "Program link error: " << log << std::endl;
    glDeleteProgram(prog);
    prog = 0;
  }

  // shaders can be deleted after linking
  glDetachShader(prog, vert);
  glDetachShader(prog, frag);
  glDeleteShader(vert);
  glDeleteShader(frag);

  return prog;
}
