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

  // Create a simple white 1x1 texture
  glGenTextures(1, &defaultTex_);
  glBindTexture(GL_TEXTURE_2D, defaultTex_);
  unsigned char white[4] = {255,255,255,255};
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, white);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glBindTexture(GL_TEXTURE_2D, 0);

  // Create cube geometry (positions, normals, texcoords) - 36 vertices
  float verts[] = {
    // positions         normals           tex
    // front
    -0.5f, -0.5f,  0.5f,  0,0,1,  0,0,
     0.5f, -0.5f,  0.5f,  0,0,1,  1,0,
     0.5f,  0.5f,  0.5f,  0,0,1,  1,1,
     0.5f,  0.5f,  0.5f,  0,0,1,  1,1,
    -0.5f,  0.5f,  0.5f,  0,0,1,  0,1,
    -0.5f, -0.5f,  0.5f,  0,0,1,  0,0,
    // back
    -0.5f, -0.5f, -0.5f,  0,0,-1, 0,0,
    -0.5f,  0.5f, -0.5f,  0,0,-1, 0,1,
     0.5f,  0.5f, -0.5f,  0,0,-1, 1,1,
     0.5f,  0.5f, -0.5f,  0,0,-1, 1,1,
     0.5f, -0.5f, -0.5f,  0,0,-1, 1,0,
    -0.5f, -0.5f, -0.5f,  0,0,-1, 0,0,
    // left
    -0.5f,  0.5f,  0.5f, -1,0,0,  1,0,
    -0.5f,  0.5f, -0.5f, -1,0,0,  1,1,
    -0.5f, -0.5f, -0.5f, -1,0,0,  0,1,
    -0.5f, -0.5f, -0.5f, -1,0,0,  0,1,
    -0.5f, -0.5f,  0.5f, -1,0,0,  0,0,
    -0.5f,  0.5f,  0.5f, -1,0,0,  1,0,
    // right
     0.5f,  0.5f,  0.5f, 1,0,0,  1,0,
     0.5f, -0.5f, -0.5f, 1,0,0,  0,1,
     0.5f,  0.5f, -0.5f, 1,0,0,  1,1,
     0.5f, -0.5f, -0.5f, 1,0,0,  0,1,
     0.5f,  0.5f,  0.5f, 1,0,0,  1,0,
     0.5f, -0.5f,  0.5f, 1,0,0,  0,0,
    // top
    -0.5f,  0.5f, -0.5f, 0,1,0,  0,1,
    -0.5f,  0.5f,  0.5f, 0,1,0,  0,0,
     0.5f,  0.5f,  0.5f, 0,1,0,  1,0,
     0.5f,  0.5f,  0.5f, 0,1,0,  1,0,
     0.5f,  0.5f, -0.5f, 0,1,0,  1,1,
    -0.5f,  0.5f, -0.5f, 0,1,0,  0,1,
    // bottom
    -0.5f, -0.5f, -0.5f, 0,-1,0, 0,1,
     0.5f, -0.5f, -0.5f, 0,-1,0, 1,1,
     0.5f, -0.5f,  0.5f, 0,-1,0, 1,0,
     0.5f, -0.5f,  0.5f, 0,-1,0, 1,0,
    -0.5f, -0.5f,  0.5f, 0,-1,0, 0,0,
    -0.5f, -0.5f, -0.5f, 0,-1,0, 0,1
  };
  glGenVertexArrays(1, &cubeVao_);
  glGenBuffers(1, &cubeVbo_);
  glBindVertexArray(cubeVao_);
  glBindBuffer(GL_ARRAY_BUFFER, cubeVbo_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
  // pos
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0));
  // normal
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  // tex
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glBindVertexArray(0);
  cubeVboCount_ = 36;

  return true;
}

void Renderer::render() {
  // Placeholder render call; actual draw calls will be implemented later.
}

void Renderer::drawCube(const glm::mat4& model, const glm::vec3& color) {
  if (phongProgram_ == 0) return;
  glUseProgram(phongProgram_);

  GLint locModel = glGetUniformLocation(phongProgram_, "model");
  if (locModel >= 0) glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(model));
  GLint locMat = glGetUniformLocation(phongProgram_, "materialDiffuse");
  if (locMat >= 0) glUniform3f(locMat, color.r, color.g, color.b);
  GLint locSpec = glGetUniformLocation(phongProgram_, "materialSpecular");
  if (locSpec >= 0) glUniform3f(locSpec, 0.3f, 0.3f, 0.3f);
  GLint locSh = glGetUniformLocation(phongProgram_, "shininess");
  if (locSh >= 0) glUniform1f(locSh, 32.0f);

  // bind default texture
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, defaultTex_);
  GLint texLoc = glGetUniformLocation(phongProgram_, "tex");
  if (texLoc >= 0) glUniform1i(texLoc, 0);

  glBindVertexArray(cubeVao_);
  glDrawArrays(GL_TRIANGLES, 0, cubeVboCount_);
  glBindVertexArray(0);
  glUseProgram(0);
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
