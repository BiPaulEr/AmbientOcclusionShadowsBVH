#include "Vec3.h"
#include "Triangle.h"

class AABB{
private:
  Vec3f min_point, max_point;
public:
  AABB(Vec3f min_point, Vec3f max_point): min_point(min_point), max_point(max_point) {}
  Vec3f getMinPoint() {return min_point;}
  Vec3f getMaxPoint() {return max_point;}
};
