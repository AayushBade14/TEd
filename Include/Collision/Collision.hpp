#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

/*
 ================================================================
 SHAPE-DEFINATION
 ================================================================
 */

typedef struct{
  glm::vec3 origin;
  glm::vec3 size;
}Rectangle;

/*
 ================================================================
 AXIS ALIGNED RECT-RECT COLLISION
 ================================================================
 */

bool overlapping(float minA,float maxA,float minB,float maxB);

bool rect_rect_collide(Rectangle a,Rectangle b);

/*
 ================================================================
 POINT-RECT COLLISION
 ================================================================
 */

bool point_rect_collide(glm::vec3 &p,Rectangle r);
