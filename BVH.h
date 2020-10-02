#include <algorithm>

#include "Mesh.h"
#include "AABB.h"

class BVH{
private:
  AABB bbox;
  BVH * leftChild = nullptr;
  BVH * rightChild = nullptr;
  bool isLeaf = false;
  Triangle triangle;

  BVH(AABB bbox, Triangle triangle): bbox(bbox), isLeaf(true), triangle(triangle) {}
  BVH(AABB bbox, BVH * l, BVH * r): bbox(bbox), leftChild(l), rightChild(r) {}

  static int chooseAxis(float x_distance, float y_distance, float z_distance);
  static void calculateMinMax(Vec3f & min_p, Vec3f & max_p, const std::vector<Triangle> & t, const Mesh & mesh);
  static void redistributeTriangles(std::vector<Triangle> & triangles_left, std::vector<Triangle> & triangles_right,
                                const std::vector<Triangle> & t, const int & axis, const Vec3f & max_p, const Vec3f & min_p, const Mesh & mesh);
  static void drawCube(const Vec3f & min_p, const Vec3f & max_p);
  static void drawCube(BVH * const bvh);
  static unsigned int deepToNumberOfNodes(unsigned int currentDeep);

public:
  static std::vector<Vec3f> bvh_positions;
  static std::vector<unsigned int> bvh_indices;
  static unsigned int deep_count;

  static BVH * buildBVH(const std::vector<Triangle> & t, const Mesh & mesh, unsigned int deep_count1);

  BVH * getLeftChild() {return leftChild;}
  BVH * getRightChild() {return rightChild;}
  AABB getAABB() {return bbox;}
  bool isALeaf() {return isLeaf;}
  Triangle getTriangle() {return triangle;}
  void drawBVH(unsigned int currentDeep);
};
