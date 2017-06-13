//
//  Segment.hpp
//  ik-opengl
//
//  Created by Jesse Zhou on 6/1/17.
//  Copyright © 2017 Jesse and Robb. All rights reserved.
//


#include <stdio.h>

/////////////////////
// ModelObject.h
/////////////////////

#pragma once

#include "../Shader.h"
#include "../Model.h"

// Std includes
#include <stdio.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

// GLM includes
#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <GL/glew.h>

class Segment {
public:
  glm::vec3 position;
  glm::quat quat;
  float magnitude;
  
  // Functions
  Segment(glm::vec3 base, float magnitude, glm::quat dir);
  void Render(glm::mat4 view, glm::mat4 proj);
  void Set(glm::vec3 base, float magnitude, glm::quat dir);
  
private:
  
  /* Data */
  GLchar* vertexShaderPath = "./seg.vs";
  GLchar* fragShaderPath   = "./seg.frag";
  Shader objectShader;
  
};
