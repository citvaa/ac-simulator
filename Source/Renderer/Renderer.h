#pragma once

#include <string>

class Renderer {
public:
  Renderer();
  ~Renderer();

  bool init();
  void render();

private:
  std::string loadShaderSource(const char* path);
  unsigned int createShaderProgram(const char* vertPath, const char* fragPath);
  unsigned int phongProgram_ = 0;
  unsigned int blinnProgram_ = 0;
};
