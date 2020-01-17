// vcglibからデータを渡して、CGALでPolyhedron作成、結果をもう一度vcglibに返す

#include<vcg/complex/algorithms/create/platonic.h>
#include<vcg/complex/allocate.h>

class MyFace;
class MyVertex;
struct MyUsedTypes : public vcg::UsedTypes<	vcg::Use<MyVertex>::AsVertexType, vcg::Use<MyFace>::AsFaceType> {};
class MyVertex : public vcg::Vertex< MyUsedTypes, vcg::vertex::Coord3f, vcg::vertex::BitFlags, vcg::vertex::VFAdj> {};
class MyFace : public vcg::Face< MyUsedTypes, vcg::face::VertexRef, vcg::face::VFAdj> {};
class VCGMesh : public vcg::tri::TriMesh<vector<MyVertex>, vector<MyFace> > {};


#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Polygon_mesh_processing/repair_polygon_soup.h>
#include <CGAL/Polygon_mesh_processing/orient_polygon_soup.h>
#include <CGAL/Polygon_mesh_processing/polygon_soup_to_polygon_mesh.h>
#include <iostream>
#include <fstream>
#include <vector>
typedef CGAL::Exact_predicates_inexact_constructions_kernel     K;
typedef K::Point_3                                              Point_3;
typedef std::vector<std::size_t>                                Polygon;
typedef CGAL::Surface_mesh<Point_3>                             CGALMesh;
namespace PMP = CGAL::Polygon_mesh_processing;


int main(){
	VCGMesh vcgmesh;
	vcg::tri::Octahedron(vcgmesh); // 正八面体のデータを作成
	
	// convert VCG mesh to CGAL mesh
	std::vector<Point_3> points;
	std::vector<Polygon> polygons;
	// vcglibオブジェクトの頂点を順番にCGALのオブジェクトにpushしていく
	for(auto &&v: vcgmesh.vert){
		points.push_back(Point_3(v.P()[0], v.P()[1], v.P()[2]));
	}
	for(auto &&f: vcgmesh.face){
		Polygon p;
		for(int i=0; i<f.VN(); ++i){
			unsigned int idx = vcg::tri::Index(vcgmesh, f.V(i));
			p.push_back(idx);
		}
		polygons.push_back(p);
	}
	//PMP::repair_polygon_soup(points, polygons);
	PMP::orient_polygon_soup(points, polygons);
	CGALMesh mesh;
	PMP::polygon_soup_to_polygon_mesh(points, polygons, mesh);
	std::cout << "CGALMesh has " << num_vertices(mesh) << " vertices and " << num_faces(mesh) << " faces" << std::endl;
	std::ofstream out("CGAL_mesh.off");
	out << mesh;
	out.close();
	
	// convert reverse
	VCGMesh vcgmesh2;
	unsigned int cgal_vn = num_vertices(mesh);
	unsigned int cgal_fn = num_faces(mesh);
	auto vi = vcg::tri::Allocator<VCGMesh>::AddVertices(vcgmesh2, cgal_vn); // 最初に追加する頂点数を丸ごと確保してから、座標値を代入
	unsigned int i;
	
	CGALMesh::Vertex_range::iterator v;
	for (v=mesh.vertices().begin(),i=0; v!=mesh.vertices().end(); ++v,++i){
		CGALMesh::Vertex_index vi = *v;
		Point_3 p3 = mesh.point(vi);
		vcgmesh2.vert[i].P() = vcg::Point3f(p3.x(),p3.y(),p3.z());
	}
	
	auto fi = vcg::tri::Allocator<VCGMesh>::AddFaces(vcgmesh2, cgal_fn); // fiは追加する開始点から始まるなので、追加元のデータインデックスをそのまま使える
	
	CGALMesh::Face_range::iterator f;
	for(f=mesh.faces().begin(),i=0; f!=mesh.faces().end(); ++f,++i){
		CGALMesh::Face_index fi = *f;
		CGALMesh::Halfedge_index hi = mesh.halfedge(fi);
		for(int j=0; j<3; ++j){
			unsigned int idx = mesh.source(hi).idx();
			vcgmesh2.face[i].V(j) = &vi[idx];
			hi=mesh.next(hi);
		}
	}
	std::cout << "VCGMesh has " << vcgmesh2.VN() << " vertices and " << vcgmesh2.FN() << " faces" << std::endl;
	
	return 1;
}



