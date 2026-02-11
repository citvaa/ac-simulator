#pragma once

#include <string>
#include <glm/glm.hpp>

class Renderer {
public:
  Renderer();
  ~Renderer();

  bool init();
  void render();

  void setViewProjection(const glm::mat4& view, const glm::mat4& proj);

  // Simple mesh draw helpers
  void drawCube(const glm::mat4& model, const glm::vec3& color);

private:
  std::string loadShaderSource(const char* path);
  unsigned int createShaderProgram(const char* vertPath, const char* fragPath);
  unsigned int phongProgram_ = 0;
  unsigned int blinnProgram_ = 0;

  // cube mesh
  unsigned int cubeVao_ = 0;
  unsigned int cubeVbo_ = 0;
  unsigned int cubeVboCount_ = 0;

  unsigned int defaultTex_ = 0;
};
