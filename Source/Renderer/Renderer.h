#pragma once

#include <string>
#include <GL/glew.h>
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

  // draw cube but sample the provided texture (bound to GL_TEXTURE0)
  void drawTexturedCube(const glm::mat4& model, GLuint texture);
  // draw semi-transparent particle (approximated sphere as cube)
  void drawParticle(const glm::mat4& model, const glm::vec3& color, float alpha);

  // approximate hollow cylinder by a ring of thin quads
  void drawHollowCylinderAt(const glm::vec3& center, float radius, float height, float thickness, int segments, const glm::vec3& color);

  // draw a hollow box (open at the top) centered at `center` with full width/height/depth
  // thickness is wall thickness in world units
  void drawHollowBoxAt(const glm::vec3& center, float width, float height, float depth, float thickness, const glm::vec3& color);

  // model loading & drawing (simple OBJ support)
  int loadOBJModel(const std::string& path);
  void drawModel(int modelId, const glm::mat4& model, const glm::vec3& color);

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

  // optional lamp parameters set by application
  glm::vec3 lampPos_ = glm::vec3(0.0f);
  glm::vec3 lampColor_ = glm::vec3(1.0f, 0.0f, 0.0f);
  float lampIntensity_ = 0.0f;
  bool lampEnabled_ = false;

public:
  void setLampLight(const glm::vec3& pos, const glm::vec3& color, float intensity, bool enabled);
};
