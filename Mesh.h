// --------------------------------------------------------------------------
// Copyright(C) 2009-2016
// Tamy Boubekeur
//
// Permission granted to use this code only for teaching projects and
// private practice.
//
// Do not distribute this code outside the teaching assignements.
// All rights reserved.
// --------------------------------------------------------------------------

#pragma once
#include <cmath>
#include <vector>
#include <map>
#include "Vec3.h"
#include "Triangle.h"

/// A Mesh class, storing a list of vertices and a list of triangles indexed over it.
class Mesh {
  std::vector< std::map< unsigned int , float > > cotangent_weight; // cotangent_weight[i][j] = le poids cotangent de l'arete orientee i->j
  std::vector< float > vertex_area;
  std::vector< std::map< unsigned int , int > > new_indexes; // new_indexs[i][j]=index of the new vertex on the edje i->j (and j->i)

public:
    inline Mesh () {}
    inline virtual ~Mesh () {}

    inline std::vector<Vec3f> & positions () { return m_positions; }
    inline const std::vector<Vec3f> & positions () const { return m_positions; }
    inline  std::vector<Vec3f> & normals () { return m_normals; }
    inline const std::vector<Vec3f> & normals () const { return m_normals; }
    inline std::vector<Triangle> & triangles () { return m_triangles; }
    inline const std::vector<Triangle> & triangles () const { return m_triangles; }

    void compute_cotangent_weights();
    void compute_areas();

    /// Empty the positions, normals and triangles arrays.
    void clear ();

	/// Loads the mesh from a <file>.off
	void loadOFF (const std::string & filename);

    /// Compute smooth per-vertex normals
    void recomputeNormals ();

    /// scale to the unit cube and center at original
    void centerAndScaleToUnit ();

    void laplacianFilter();
    std::vector<int> getNeighbours(Vec3f vertex);
    void computeAdj();
    void computeTriadj();
    void GeomFilter(float alpha);
    void simplify(unsigned int resolution);
    void setmesh();
    void init_sub();
    void subdivide();
	void GeomFilterOnColor(std::vector<float>& ambientOcclusionPerVertex, std::vector<float>& ambientOcclusionPerVertexFiltered);
private:
    std::vector<Vec3f> m_positions;
    std::vector<Vec3f> m_normals;
    std::vector<Triangle> m_triangles;
    std::vector<std::vector<int>> m_adj;
    std::vector<std::vector<int>>m_triadj;
    std::vector<Vec3f> new_positions;
    Vec3f computeTriangleContrib(Triangle t,Vec3f vi);


    typedef struct {
        Vec3f position=Vec3f(0.0f,0.0f,0.0f);
        int weight = 0;
    } GridCell;


    float x_max = -100.;
    float x_min = 100.;
    float y_max = -100.;
    float y_min = 100.;
    float z_max = -100.;
    float z_min = 100.;

    std::vector<Vec3f> simplify_positions;
    std::vector<Triangle> simplify_triangles;

    float x_segment;
    float y_segment;
    float z_segment;
    int resolution;

    void getBox();

    unsigned int getCell(unsigned int index);

};
