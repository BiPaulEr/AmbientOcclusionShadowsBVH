#include <queue>
#include <cmath>
#include "BVH.h"

using namespace std;

int BVH::chooseAxis(float x_distance, float y_distance, float z_distance){
	// 0 x, 1 y, 2 z
	if( x_distance >= y_distance && x_distance >= z_distance ) return 0;
	else if ( y_distance >= x_distance && y_distance >= z_distance ) return 1;
	else return 2;
}

void BVH::calculateMinMax(Vec3f & min_p, Vec3f & max_p, const vector<Triangle> & t, const Mesh & mesh){
	for(unsigned int i = 0; i < t.size(); i++){
		if( max_p[0] < mesh.positions()[ t[i][0] ][0] ) max_p[0] = mesh.positions()[ t[i][0] ][0];
		if( max_p[1] < mesh.positions()[ t[i][0] ][1] ) max_p[1] = mesh.positions()[ t[i][0] ][1];
		if( max_p[2] < mesh.positions()[ t[i][0] ][2] ) max_p[2] = mesh.positions()[ t[i][0] ][2];
		if( min_p[0] > mesh.positions()[ t[i][0] ][0] ) min_p[0] = mesh.positions()[ t[i][0] ][0];
		if( min_p[1] > mesh.positions()[ t[i][0] ][1] ) min_p[1] = mesh.positions()[ t[i][0] ][1];
		if( min_p[2] > mesh.positions()[ t[i][0] ][2] ) min_p[2] = mesh.positions()[ t[i][0] ][2];

		if( max_p[0] < mesh.positions()[ t[i][1] ][0] ) max_p[0] = mesh.positions()[ t[i][1] ][0];
		if( max_p[1] < mesh.positions()[ t[i][1] ][1] ) max_p[1] = mesh.positions()[ t[i][1] ][1];
		if( max_p[2] < mesh.positions()[ t[i][1] ][2] ) max_p[2] = mesh.positions()[ t[i][1] ][2];
		if( min_p[0] > mesh.positions()[ t[i][1] ][0] ) min_p[0] = mesh.positions()[ t[i][1] ][0];
		if( min_p[1] > mesh.positions()[ t[i][1] ][1] ) min_p[1] = mesh.positions()[ t[i][1] ][1];
		if( min_p[2] > mesh.positions()[ t[i][1] ][2] ) min_p[2] = mesh.positions()[ t[i][1] ][2];

		if( max_p[0] < mesh.positions()[ t[i][2] ][0] ) max_p[0] = mesh.positions()[ t[i][2] ][0];
		if( max_p[1] < mesh.positions()[ t[i][2] ][1] ) max_p[1] = mesh.positions()[ t[i][2] ][1];
		if( max_p[2] < mesh.positions()[ t[i][2] ][2] ) max_p[2] = mesh.positions()[ t[i][2] ][2];
		if( min_p[0] > mesh.positions()[ t[i][2] ][0] ) min_p[0] = mesh.positions()[ t[i][2] ][0];
		if( min_p[1] > mesh.positions()[ t[i][2] ][1] ) min_p[1] = mesh.positions()[ t[i][2] ][1];
		if( min_p[2] > mesh.positions()[ t[i][2] ][2] ) min_p[2] = mesh.positions()[ t[i][2] ][2];
	}
}

void BVH::redistributeTriangles(vector<Triangle> & triangles_left, vector<Triangle> & triangles_right, const vector<Triangle> & t,
        const int & axis, const Vec3f & max_p, const Vec3f & min_p, const Mesh & mesh){
	vector<float> centroids;
	for(unsigned int i = 0; i < t.size(); i++){
		float p0 = mesh.positions()[ t[i][0] ][axis];
		float p1 = mesh.positions()[ t[i][1] ][axis];
		float p2 = mesh.positions()[ t[i][2] ][axis];
		float centroid = ( p0 + p1 + p2 ) / 3.0f;
		centroids.push_back(centroid);
	}
	sort( centroids.begin(), centroids.end() );
	unsigned int median_index = centroids.size() / 2;
	for(unsigned int i = 0; i < t.size(); i++){
		if( i < median_index ) triangles_left.push_back( t[i] );
		else triangles_right.push_back( t[i] );
	}
}

BVH * BVH::buildBVH(const vector<Triangle> & t, const Mesh & mesh, unsigned int deep_count1){
	if( deep_count < deep_count1 ) deep_count = deep_count1;
	if( t.size() == 0 ) return nullptr;
	float max_float = numeric_limits<float>::max();
	float min_float =  - ( numeric_limits<float>::max() - 1);
	Vec3f max_p = Vec3f(min_float, min_float, min_float);
	Vec3f min_p = Vec3f(max_float, max_float, max_float);
	calculateMinMax(min_p, max_p, t, mesh);
	AABB aabb(min_p, max_p);
	if( t.size() == 1 ){
		BVH * bvh = new BVH(aabb, t[0]);
		return bvh;
	}else{
		int axis = chooseAxis(max_p[0] - min_p[0], max_p[1] - min_p[1], max_p[2] - min_p[2]);
		vector<Triangle> triangles_left, triangles_right;
		redistributeTriangles(triangles_left, triangles_right, t, axis, max_p, min_p, mesh);
		BVH * left_c = buildBVH(triangles_left, mesh, deep_count1 + 1);
		BVH * right_c = buildBVH(triangles_right, mesh, deep_count1 + 1);
		BVH * bvh = new BVH(aabb, left_c, right_c );
		return bvh;
	}
}

void BVH::drawBVH(unsigned int currentDeep){
  unsigned int k = 0;
  bvh_positions.clear();
  bvh_indices.clear();

	unsigned int numberOfNodes = deepToNumberOfNodes(currentDeep);
	if( numberOfNodes == 0 ) return;
	unsigned int numberOfNodesOfPreviousLayer = deepToNumberOfNodes(currentDeep - 1);

  //BFS
  std::queue<BVH *> visited, unvisited;
  BVH * current;
  unvisited.push(this);
  while ( ! unvisited.empty() ){
    current = unvisited.front();
    if (current->getLeftChild()  != nullptr) unvisited.push( current->getLeftChild()  );
    if (current->getRightChild() != nullptr) unvisited.push( current->getRightChild() );
    visited.push(current);
		if( k >= numberOfNodesOfPreviousLayer ) drawCube(current);
    unvisited.pop();
    k++;
    if( k >= numberOfNodes ) break;
  }
}

void BVH::drawCube(BVH * const bvh){
  Vec3f min_p = ( bvh->getAABB() ).getMinPoint();
	Vec3f max_p = ( bvh->getAABB() ).getMaxPoint();
	drawCube(min_p, max_p);
}

unsigned int BVH::deepToNumberOfNodes(unsigned int currentDeep){
  unsigned int sum = 0;
  for(unsigned int i = 1; i <= currentDeep; i++){
    sum += pow(2, i - 1);
  }
  return sum;
}

void BVH::drawCube(const Vec3f & min_p, const Vec3f & max_p){
	int first_index = bvh_positions.size() - 1;
	Vec3f p1 = Vec3f(min_p[0], max_p[1], max_p[2]);
	Vec3f p2 = Vec3f(min_p[0], max_p[1], min_p[2]);
	Vec3f p3 = Vec3f(max_p[0], max_p[1], min_p[2]);
	Vec3f p4 = Vec3f(max_p[0], max_p[1], max_p[2]);
  Vec3f p5 = Vec3f(min_p[0], min_p[1], max_p[2]);
  Vec3f p6 = Vec3f(min_p[0], min_p[1], min_p[2]);
  Vec3f p7 = Vec3f(max_p[0], min_p[1], min_p[2]);
  Vec3f p8 = Vec3f(max_p[0], min_p[1], max_p[2]);

	bvh_positions.push_back(p1);
	bvh_positions.push_back(p2);
	bvh_positions.push_back(p3);
	bvh_positions.push_back(p4);
  bvh_positions.push_back(p5);
  bvh_positions.push_back(p6);
  bvh_positions.push_back(p7);
  bvh_positions.push_back(p8);

	bvh_indices.push_back(first_index + 1);
	bvh_indices.push_back(first_index + 2);
	bvh_indices.push_back(first_index + 2);
	bvh_indices.push_back(first_index + 3);
	bvh_indices.push_back(first_index + 3);
	bvh_indices.push_back(first_index + 4);
	bvh_indices.push_back(first_index + 4);
	bvh_indices.push_back(first_index + 1);

  bvh_indices.push_back(first_index + 5);
	bvh_indices.push_back(first_index + 6);
	bvh_indices.push_back(first_index + 6);
	bvh_indices.push_back(first_index + 7);
	bvh_indices.push_back(first_index + 7);
	bvh_indices.push_back(first_index + 8);
	bvh_indices.push_back(first_index + 8);
	bvh_indices.push_back(first_index + 5);

  bvh_indices.push_back(first_index + 1);
	bvh_indices.push_back(first_index + 5);
	bvh_indices.push_back(first_index + 2);
	bvh_indices.push_back(first_index + 6);
	bvh_indices.push_back(first_index + 3);
	bvh_indices.push_back(first_index + 7);
	bvh_indices.push_back(first_index + 4);
	bvh_indices.push_back(first_index + 8);
}
