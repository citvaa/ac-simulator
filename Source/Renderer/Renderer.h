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

private:
  std::string loadShaderSource(const char* path);
  unsigned int createShaderProgram(const char* vertPath, const char* fragPath);
  unsigned int phongProgram_ = 0;
  unsigned int blinnProgram_ = 0;
};
