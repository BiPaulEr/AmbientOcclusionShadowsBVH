
#include "Mesh.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <cmath>

#define M_PI 3.1451

using namespace std;

void Mesh::clear () {
    m_positions.clear ();
    m_normals.clear ();
    m_triangles.clear ();
    m_adj.clear();
}

void Mesh::loadOFF (const std::string & filename) {
    clear ();
	ifstream in (filename.c_str ());
    if (!in)
        exit (12);
	string offString;
    unsigned int sizeV, sizeT, tmp;
    in >> offString >> sizeV >> sizeT >> tmp;
    m_positions.resize (sizeV);
    m_triangles.resize (sizeT);
    for (unsigned int i = 0; i < sizeV; i++)
        in >> m_positions[i];
    int s;
    for (unsigned int i = 0; i < sizeT; i++) {
        in >> s;
        for (unsigned int j = 0; j < 3; j++)
            in >> m_triangles[i][j];
    }
    in.close ();
    centerAndScaleToUnit ();
    recomputeNormals ();
}

void Mesh::recomputeNormals () {
    m_normals.clear ();
    m_normals.resize (m_positions.size (), Vec3f (0.f, 0.f, 0.f));
    for (unsigned int i = 0; i < m_triangles.size (); i++) {
        Vec3f e01 = m_positions[m_triangles[i][1]] -  m_positions[m_triangles[i][0]];
        Vec3f e02 = m_positions[m_triangles[i][2]] -  m_positions[m_triangles[i][0]];
        Vec3f n = cross (e01, e02);
        n.normalize ();
        for (unsigned int j = 0; j < 3; j++)
            m_normals[m_triangles[i][j]] += n;
    }
    for (unsigned int i = 0; i < m_normals.size (); i++)
        m_normals[i].normalize ();
}

void Mesh::centerAndScaleToUnit () {
    Vec3f c;
    for  (unsigned int i = 0; i < m_positions.size (); i++)
        c += m_positions[i];
    c /= m_positions.size ();
    float maxD = dist (m_positions[0], c);
    for (unsigned int i = 0; i < m_positions.size (); i++){
        float m = dist (m_positions[i], c);
        if (m > maxD)
            maxD = m;
    }
    for  (unsigned int i = 0; i < m_positions.size (); i++)
        m_positions[i] = (m_positions[i] - c) / maxD;
}
vector<int> Mesh::getNeighbours(Vec3f vertex){
        std::vector<int> neighbours;
        for (unsigned int j=0;j<m_triangles.size();j++){
            if (m_positions[m_triangles[j][0]]==vertex){
                neighbours.push_back(m_triangles[j][1]);
                neighbours.push_back(m_triangles[j][2]);
            }
            else if (m_positions[m_triangles[j][1]]==vertex){
                neighbours.push_back(m_triangles[j][0]);
                neighbours.push_back(m_triangles[j][2]);
            }
            else if (m_positions[m_triangles[j][2]]==vertex){
                neighbours.push_back(m_triangles[j][1]);
                neighbours.push_back(m_triangles[j][0]);
            }
    }
    sort( neighbours.begin(), neighbours.end());
    neighbours.erase( unique( neighbours.begin(), neighbours.end() ), neighbours.end() );

    return neighbours;
}

void Mesh::computeAdj(){
    m_adj.clear();
    m_adj.resize(m_positions.size());
    for (unsigned int i=0;i<m_triangles.size();i++){
        for(unsigned int j=0;j<3;j++){
            m_adj[m_triangles[i][j]].push_back(m_triangles[i][(j+1)%3]);
            m_adj[m_triangles[i][j]].push_back(m_triangles[i][(j+2)%3]);
        }
    }
}
void Mesh::computeTriadj(){
        m_triadj.clear();
        m_triadj.resize(m_positions.size());
        for (unsigned int i=0;i<m_triangles.size();i++){
            for(unsigned int j=0;j<3;j++){
                m_triadj[m_triangles[i][j]].push_back(i);

                }

        }

}


void Mesh::compute_cotangent_weights(){
  cotangent_weight.clear();
  cotangent_weight.resize( positions().size() );

  for(unsigned int i=0;i<m_triangles.size();i++){
    int i0 =m_triangles[i][0];
    int i1 =m_triangles[i][1];
    int i2 =m_triangles[i][2];

    Vec3f v1=m_positions[i0];
    Vec3f v2=m_positions[i1];
    Vec3f v3=m_positions[i2];

    Vec3f edge1= v2-v1;
    Vec3f edge2= v3-v2;
    Vec3f edge3= v1-v3;


    float cos1=-dot(edge1,edge3);
    float cos2=-dot(edge1,edge2);
    float cos3=-dot(edge2,edge3);

    float sin1=cross(edge1,edge3).length();
    float sin2=cross(edge1,edge2).length();
    float sin3=cross(edge2,edge3).length();


    cotangent_weight[i0][i1]+=0.5*cos3/sin3;
    cotangent_weight[i1][i0]+=0.5*cos3/sin3;

    cotangent_weight[i0][i2]+=0.5*cos2/sin2;
    cotangent_weight[i2][i0]+=0.5*cos2/sin2;

    cotangent_weight[i2][i1]+=0.5*cos1/sin1;
    cotangent_weight[i1][i2]+=0.5*cos1/sin1;


  }
}

void Mesh::compute_areas(){
  vertex_area.clear();
  vertex_area.resize( positions().size() );

  for(unsigned int i=0;i<m_triangles.size();i++){
    int i0 =m_triangles[i][0];
    int i1 =m_triangles[i][1];
    int i2 =m_triangles[i][2];

    Vec3f v1=m_positions[i0];
    Vec3f v2=m_positions[i1];
    Vec3f v3=m_positions[i2];

    Vec3f edge1= v2-v1;
    Vec3f edge2= v3-v2;

    float area=fabs(0.5*(cross(edge1,edge2)).length());
    vertex_area[i0]+=area;
    vertex_area[i1]+=area;
    vertex_area[i2]+=area;
  }

}

void Mesh::laplacianFilter(){
  computeAdj();

    for(unsigned int i=0;i<m_adj.size();i++){
        Vec3f bary=Vec3f(0.0f,0.0f,0.0f);
        for (unsigned int j=0;j<m_adj[i].size();j++){
            int index=m_adj[i][j];
            bary+=m_positions[index];
        }
        bary*=(1.f/(m_adj[i].size()));
        m_positions[i]=bary;

    }

    recomputeNormals();
}


void Mesh::GeomFilterOnColor(std::vector<float>& ambientOcclusionPerVertex, std::vector<float>& ambientOcclusionPerVertexFiltered)
{
	computeAdj();
	ambientOcclusionPerVertexFiltered.resize(ambientOcclusionPerVertex.size(), 0.0f);
	for (unsigned int i = 0;i < m_adj.size();i++) 
	{
		for (unsigned int j = 0;j < m_adj[i].size();j++) 
		{
			int index = m_adj[i][j];
			ambientOcclusionPerVertexFiltered[i] += ambientOcclusionPerVertex[index];
		}
		ambientOcclusionPerVertexFiltered[i] *= (1.f / (m_adj[i].size()));
	}
}


void Mesh::GeomFilter(float alpha){
	compute_areas();
	 compute_cotangent_weights();

    for(unsigned int i=0;i<m_positions.size();i++){
    std::map<unsigned int,float> voisins_i=cotangent_weight[i];
    for(std::map<unsigned int,float>::iterator it=voisins_i.begin();it!=voisins_i.end();++it){
      int j =it->first;
      float wij=it->second;
      m_positions[i]+=alpha*(vertex_area[i]*wij*(m_positions[j]-m_positions[i])/6);
    }
  }
  recomputeNormals();

}

void Mesh::getBox() {
    for (const auto position : m_positions) {
        x_min = x_min > position[0] ? position[0] : x_min;
        x_max = x_max < position[0] ? position[0] : x_max;
        y_min = y_min > position[1] ? position[1] : y_min;
        y_max = y_max < position[1] ? position[1] : y_max;
        z_min = z_min > position[2] ? position[2] : z_min;
        z_max = z_max < position[2] ? position[2] : z_max;
    }

    x_min -= (x_max - x_min) / 1000.0;
    x_max += (x_max - x_min) / 1001.0;
    y_min -= (y_max - y_min) / 1000.0;
    y_max += (y_max - y_min) / 1001.0;
    z_min -= (z_max - z_min) / 1000.0;
    z_max += (z_max - z_min) / 1001.0;
}

void Mesh::simplify(unsigned resolution) {
    simplify_triangles.clear();
    simplify_positions.clear();

    getBox();

    this->resolution = resolution;
    std::vector<GridCell> gridCells(resolution * resolution * resolution);

    this->x_segment = (x_max - x_min) / (resolution - 1);
    this->y_segment = (y_max - y_min) / (resolution - 1);
    this->z_segment = (z_max - z_min) / (resolution - 1);


    for (unsigned int i = 0; i < m_positions.size() ;i++) {
        if(m_positions[i]==m_positions[i]){
        int cellIndex = getCell(i);
        gridCells[cellIndex].position += m_positions[i];
        gridCells[cellIndex].weight++;
    }
    }
    for (auto triangle : m_triangles) {
        unsigned int p1_index = getCell(triangle[0]);
        unsigned int p2_index = getCell(triangle[1]);
        unsigned int p3_index = getCell(triangle[2]);

        if (p1_index != p2_index && p2_index != p3_index && p3_index != p1_index) {
            simplify_triangles.push_back(Triangle(p1_index, p2_index, p3_index));
        }
    }

    for (auto& gcell : gridCells) {
        simplify_positions.push_back(gcell.position / (gcell.weight*1.0));
    }
}


unsigned int Mesh::getCell(unsigned int index) {
    int x_index = (m_positions[index][0] - x_min) / x_segment;
    int y_index = (m_positions[index][1] - y_min) / y_segment;
    int z_index = (m_positions[index][2] - z_min) / z_segment;
    int res = x_index * resolution * resolution + y_index * resolution + z_index;
    if (abs(res) >= resolution * resolution * resolution) {
        throw overflow_error(" Over range of grid");
    }
    return res;
}

void Mesh::setmesh(){
    m_positions.clear();
    m_triangles.clear();

    m_positions.resize(simplify_positions.size());
    m_triangles.resize(simplify_triangles.size());


        m_positions=simplify_positions;
        m_triangles=simplify_triangles;
        recomputeNormals();

}

void Mesh::init_sub(){
  new_indexes.clear();
  new_positions.clear();  
  new_indexes.clear();

  computeAdj();
  computeTriadj();

  new_indexes.resize(m_positions.size());
  new_positions.resize(m_positions.size());


  for (unsigned int i=0;i<m_triangles.size();i++){
    unsigned int index1=m_triangles[i][0];
    unsigned int index2=m_triangles[i][1];
    unsigned int index3=m_triangles[i][2];

    Vec3f v1=m_positions[index1];
    Vec3f v2=m_positions[index2];
    Vec3f v3=m_positions[index3];
    

    unsigned int len=new_positions.size();

    new_indexes[index1][index2]=len;
    new_indexes[index2][index1]=len;

    new_indexes[index1][index3]=len+1;
    new_indexes[index3][index1]=len+1;

    new_indexes[index3][index2]=len+2;
    new_indexes[index2][index3]=len+2;


    new_positions.push_back(Vec3f(0.f,0.f,0.f));
    new_positions.push_back(Vec3f(0.f,0.f,0.f));
    new_positions.push_back(Vec3f(0.f,0.f,0.f));

    int n1=m_adj[index1].size();
    int n2=m_adj[index2].size();
    int n3=m_adj[index3].size();

    int m1=m_triadj[index1].size();
    int m2=m_triadj[index2].size();
    int m3=m_triadj[index3].size();

    float alpha_n1= (3.f/8)+pow(((3.f/8)+0.25*cos(2*M_PI/n1)),2);
    float alpha_n2 = (3.f/8)+pow(((3.f/8)+0.25*cos(2*M_PI/n2)),2);
    float alpha_n3 = (3.f/8)+pow(((3.f/8)+0.25*cos(2*M_PI/n3)),2);

    Vec3f sum_n1 = Vec3f();
    Vec3f sum_n2 = Vec3f();
    Vec3f sum_n3 = Vec3f();

    for (auto& n : m_adj[index1])
        sum_n1 += m_positions[n];
    
    for (auto& n : m_adj[index2])
        sum_n2 += m_positions[n];
    
    for (auto& n : m_adj[index3])
        sum_n3 += m_positions[n];

    
    new_positions[index1]+=(1.f/m1)*(alpha_n1*v1 + ((1.f-alpha_n1)/n1)*sum_n1);
    new_positions[index2]+=(1.f/m2)*(alpha_n2*v2 + ((1.f-alpha_n2)/n2)*sum_n2);
    new_positions[index3]+=(1.f/m3)*(alpha_n3*v3 + ((1.f-alpha_n3)/n3)*sum_n3);

    }
}


void Mesh::subdivide(){
    std::vector<Triangle> new_triangles;
    init_sub();
    for (unsigned int i=0;i<m_triangles.size();i++){

        unsigned int index1=m_triangles[i][0];
        unsigned int index2=m_triangles[i][1];
        unsigned int index3=m_triangles[i][2];

        unsigned int index_e1 = new_indexes[index1][index2];
        unsigned int index_e2 = new_indexes[index1][index3];
        unsigned int index_e3 = new_indexes[index3][index2];

        Vec3f v1=m_positions[index1];
        Vec3f v2=m_positions[index2];
        Vec3f v3=m_positions[index3];

        new_positions[index_e1] += (3.f/16.f)*(v1+v2)+(1.f/8.f)*v3;
        new_positions[index_e2] += (3.f/16.f)*(v1+v2)+(1.f/8.f)*v3;
        new_positions[index_e3] += (3.f/16.f)*(v1+v2)+(1.f/8.f)*v3;

        new_triangles.push_back(Triangle(index1,index_e1,index_e2));
        new_triangles.push_back(Triangle(index_e2,index_e1,index_e3));
        new_triangles.push_back(Triangle(index_e1,index2,index_e3));
        new_triangles.push_back(Triangle(index_e2,index_e3,index3));
    }

    m_positions.clear();
    m_positions=new_positions;
    m_triangles.clear();
    m_triangles=new_triangles;
    recomputeNormals();

}
