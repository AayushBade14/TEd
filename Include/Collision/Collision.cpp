#include "./Collision.hpp"

/*
 ================================================================
 AXIS ALIGNED RECT-RECT COLLISION
 ================================================================
 */

bool overlapping(float minA,float maxA,float minB,float maxB){
  return (minB <= maxA) && (minA <= maxB);
}

bool rect_rect_collide(Rectangle a,Rectangle b){
  float leftA = a.origin.x;
  float rightA = leftA + a.size.x;
  float bottomA = a.origin.y;
  float topA = bottomA + a.size.y;

  float leftB = b.origin.x;
  float rightB = leftB + b.size.x;
  float bottomB = b.origin.y;
  float topB = bottomB + b.size.y;

  return (overlapping(leftA,rightA,leftB,rightB) && overlapping(bottomA,topA,bottomB,topB));
}

/*
 ================================================================
 POINT-RECT COLLISION
 ================================================================
 */

bool point_rect_collide(glm::vec3 &p,Rectangle r){
  float left = r.origin.x;
  float right = left + r.size.x;
  float bottom = r.origin.y;
  float top = bottom + r.size.y;

  return ((p.x >= left) && (p.x <= right) && (p.y >= bottom) && (p.y <= top));
}

