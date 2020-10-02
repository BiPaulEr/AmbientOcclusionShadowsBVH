#include "Vec3.h"
#include "Mesh.h"
#include "BVH.h"
#include <limits>

class Ray{
  private:
    Vec3f origin;
    Vec3f direction;
    float radius;

    bool isIntersectedWithTriangle(const Vec3f & v0, const Vec3f & v1, const Vec3f & v2) const;
    bool isIntersectedWithCube(const Vec3f & min_p, const Vec3f & max_p) const;
    void isIntersected(const Mesh & mesh, BVH * const bvh, int & result) const;

  public:
    Ray(){}
    Ray(Vec3f origin, Vec3f direction_): origin(origin){
      this->radius = length(direction_);
      direction_.normalize();
      this->direction = direction_;
    }
    ~Ray(){}
    bool isIntersected(const Mesh & mesh) const;
    bool isIntersected(const Mesh & mesh, BVH * const bvh) const;
};
