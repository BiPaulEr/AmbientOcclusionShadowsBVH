#include <cmath>
#include <vector>
#include "Triangle.h"
#include "Ray.h"

using namespace std;

bool Ray::isIntersected(const Mesh& mesh) const {
  float epsilon = 0.0001;
  bool intersected = false;

  vector<Triangle> triangle_indices = mesh.triangles ();
  vector<Vec3f> vertex_list = mesh.positions ();

  for(unsigned int i = 0; i < triangle_indices.size(); i++){
    Triangle threeIndices = triangle_indices[i];
    Vec3f v0 = vertex_list[threeIndices[0]];
    Vec3f v1 = vertex_list[threeIndices[1]];
    Vec3f v2 = vertex_list[threeIndices[2]];

    Vec3f v0v1 = v1 - v0;
    Vec3f v0v2 = v2 - v0;
    Vec3f pvec = cross(direction, v0v2);
    float det = dot(v0v1, pvec);

    // ray and triangle are parallel if det is close to 0
    if (fabs(det) < epsilon) continue;

    float invDet = 1.0 / det;

    Vec3f tvec = origin - v0;
    float u = dot(tvec, pvec) * invDet;
    if (u < 0.0 || u > 1.0) continue;

    Vec3f qvec = cross(tvec, v0v1);
    float v = dot(direction, qvec) * invDet;
    if (v < 0.0 || u + v > 1.0) continue;

    // positive t means intersection is in front of origin point,
    // negative t means intersection is behind the origin point.
    // The absolute value of t means the distance between intersection point and origin point.
    float t = dot(v0v2, qvec) * invDet;

    if(t > 0.0 && t <= radius){
      intersected = true;
      break;
    }
  }
  return intersected;
}

bool Ray::isIntersectedWithTriangle(const Vec3f & v0, const Vec3f & v1, const Vec3f & v2) const{
  float epsilon = 0.0001;
  Vec3f v0v1 = v1 - v0;
  Vec3f v0v2 = v2 - v0;
  Vec3f pvec = cross(direction, v0v2);
  float det = dot(v0v1, pvec);
  // ray and triangle are parallel if det is close to 0
  if (fabs(det) < epsilon) return false;
  float invDet = 1.0 / det;
  Vec3f tvec = origin - v0;
  float u = dot(tvec, pvec) * invDet;
  if (u < 0.0 || u > 1.0) return false;
  Vec3f qvec = cross(tvec, v0v1);
  float v = dot(direction, qvec) * invDet;
  if (v < 0.0 || u + v > 1.0) return false;
  // positive t means intersection is in front of origin point,
  // negative t means intersection is behind the origin point.
  // The absolute value of t means the distance between intersection point and origin point.
  float t = dot(v0v2, qvec) * invDet;
  if(t > 0.0 && t <= radius) return true;
  else return false;
}

bool Ray::isIntersectedWithCube(const Vec3f & min_p, const Vec3f & max_p) const {
  Vec3f dirInv = Vec3f(1.0f / direction[0], 1.0f / direction[1], 1.0f / direction[2]);
  float t1 = (min_p[0] - origin[0]) * dirInv[0];
  float t2 = (max_p[0] - origin[0]) * dirInv[0];
  float t3 = (min_p[1] - origin[1]) * dirInv[1];
  float t4 = (max_p[1] - origin[1]) * dirInv[1];
  float t5 = (min_p[2] - origin[2]) * dirInv[2];
  float t6 = (max_p[2] - origin[2]) * dirInv[2];
  float tmin = max( max( min(t1, t2), min(t3, t4) ), min(t5, t6) );
  float tmax = min( min( max(t1, t2), max(t3, t4) ), max(t5, t6) );

  // if tmax < 0, ray is intersecting AABB, but the whole AABB is behind the origin
  if (tmax < 0) return false;

  // if tmin > tmax, ray doesn't intersect AABB
  if (tmin > tmax) return false;
  else return true;
}

void Ray::isIntersected(const Mesh & mesh, BVH * const bvh, int & result) const {
  Vec3f min_p = ( bvh->getAABB() ).getMinPoint();
  Vec3f max_p = ( bvh->getAABB() ).getMaxPoint();
  if( this->isIntersectedWithCube(min_p, max_p) ){
    if( bvh->isALeaf() ){
      Triangle threeIndices = bvh->getTriangle();
      Vec3f v0 = mesh.positions ()[threeIndices[0]];
      Vec3f v1 = mesh.positions ()[threeIndices[1]];
      Vec3f v2 = mesh.positions ()[threeIndices[2]];
	  if (this->origin != v0 && this->origin != v1 && this->origin != v2)
	  {
		  if (this->isIntersectedWithTriangle(v0, v1, v2)) {
			  result *= 0;
		  }
	  }
    }else{
      if( bvh->getLeftChild() != nullptr ) isIntersected( mesh, bvh->getLeftChild(), result );
      if( bvh->getRightChild() != nullptr ) isIntersected( mesh, bvh->getRightChild(), result );
    }
  }
}

bool Ray::isIntersected(const Mesh & mesh, BVH * const bvh) const {
  int result = 1;
  this->isIntersected(mesh, bvh, result);
  if( result == 1 ) return false;
  else return true;
}
