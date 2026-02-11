#include "Renderer.h"
#include <fstream>
#include <sstream>
#include <iostream>

Renderer::Renderer() {}
Renderer::~Renderer() {}

bool Renderer::init() {
  // Placeholder: create shader program from Shaders/phong.vert and Shaders/phong.frag
  // Actual GL setup will be added later.
  phongProgram_ = 0;
  return true;
}

void Renderer::render() {
  // Placeholder render call; actual draw calls will be implemented later.
}

std::string Renderer::loadShaderSource(const char* path) {
  std::ifstream in(path);
  if(!in) return std::string();
  std::stringstream ss; ss << in.rdbuf();
  return ss.str();
}

unsigned int Renderer::createShaderProgram(const char* vertPath, const char* fragPath) {
  // Placeholder: shader compilation/ linking will be implemented later.
  return 0;
}
