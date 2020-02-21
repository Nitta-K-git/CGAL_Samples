#include <CGAL/Surface_mesh_deformation.h>
#include <fstream>
#include <map>
#include <queue>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>
typedef CGAL::Exact_predicates_inexact_constructions_kernel     Kernel;
typedef CGAL::Surface_mesh<Kernel::Point_3>               CGALMesh;
// typedef boost::graph_traits<CGALMesh>::vertex_descriptor   vertex_descriptor;
// typedef boost::graph_traits<CGALMesh>::vertex_iterator     vertex_iterator;
// typedef boost::graph_traits<CGALMesh>::halfedge_descriptor halfedge_descriptor;
// typedef boost::graph_traits<CGALMesh>::edge_descriptor     edge_descriptor;

// boost使っても同じ
typedef CGALMesh::Vertex_index        vertex_descriptor;
typedef CGALMesh::Vertex_iterator     vertex_iterator;
typedef CGALMesh::Halfedge_index      halfedge_descriptor;
typedef CGALMesh::Edge_index          edge_descriptor;

namespace PMP = CGAL::Polygon_mesh_processing;

typedef CGAL::Surface_mesh_deformation<CGALMesh>     Surface_mesh_deformation;


std::vector<vertex_descriptor> extract_k_ring(const CGALMesh &P, vertex_descriptor v, int k) // descriptor:記述子
{
	std::map<vertex_descriptor, int>  D;
	std::vector<vertex_descriptor>    Q;
	Q.push_back(v); D[v] = 0;
	std::size_t current_index = 0;
	int dist_v;
	while( current_index < Q.size() && (dist_v = D[ Q[current_index] ]) < k ) {
		v = Q[current_index++];
		for(edge_descriptor e : out_edges(v, P))
		{
			halfedge_descriptor he = halfedge(e, P);
			vertex_descriptor new_v = target(he, P);
			if(D.insert(std::make_pair(new_v, dist_v + 1)).second) { // .second return true if insert success else return false
				Q.push_back(new_v);
			}
		}
	}
	return Q;
}

/*
 * 流れ
 * ROIとCONTROLの頂点をvectorに入れて指定
 * CONTROLの頂点を移動したり、回転させたりする
 * ROIの頂点を最適化する
*/
int main()
{
  CGALMesh mesh;
  std::ifstream input("data/plane.off");
  if ( !input || !(input >> mesh) || mesh.is_empty() ) {
    std::cerr<< "Cannot open data/plane.off";
    return 1;
  }
  // Init the indices of the halfedges and the vertices.
  // set_halfedgeds_items_id(mesh);
  // https://doc.cgal.org/latest/Surface_mesh_deformation/classCGAL_1_1Surface__mesh__deformation.html#a247c51e05bcd734cfb04d254677ee96c
  // Triangle_mesh 
  Surface_mesh_deformation deform_mesh(mesh);
  // Select and insert the vertices of the region of interest
  vertex_iterator vb, ve;
  boost::tie(vb,ve) = vertices(mesh); // vb,ve = mesh.vert.begin(), mesh.vert.end() と同じ
  std::vector<vertex_descriptor> roi = extract_k_ring(mesh, *std::next(vb, 47), 9); // std::next(vb, 47) -> offset 47 from vb, namely set as index
  // ROI頂点を追加
  deform_mesh.insert_roi_vertices(roi.begin(), roi.end());
  // Select and insert the control vertices
  std::vector<vertex_descriptor> cvertices_1 = extract_k_ring(mesh, *std::next(vb, 39), 1);
  std::vector<vertex_descriptor> cvertices_2 = extract_k_ring(mesh, *std::next(vb, 97), 1);
  // CONTROL頂点を追加
  deform_mesh.insert_control_vertices(cvertices_1.begin(), cvertices_1.end());
  deform_mesh.insert_control_vertices(cvertices_2.begin(), cvertices_2.end());
  // Apply a rotation to the control vertices
  Eigen::Quaternion<double> quad(0.92, 0, 0, -0.38);
  deform_mesh.rotate(cvertices_1.begin(), cvertices_1.end(), Eigen::Vector3d(0,0,0), quad);
  deform_mesh.rotate(cvertices_2.begin(), cvertices_2.end(), Eigen::Vector3d(0,0,0), quad);
  deform_mesh.deform();
  // Save the deformed mesh
  std::ofstream output("deform_1.off");
  output << mesh;
  output.close();
  // Restore the positions of the vertices
  deform_mesh.reset();
  // Apply a translation on the original positions of the vertices (reset() was called before)
  deform_mesh.translate(cvertices_1.begin(), cvertices_1.end(), Eigen::Vector3d(0,0.3,0));
  deform_mesh.translate(cvertices_2.begin(), cvertices_2.end(), Eigen::Vector3d(0,0.3,0));
  // Call the function deform() with one-time parameters:
  // iterate 10 times and do not use energy based termination criterion
  deform_mesh.set_iterations(10);
  deform_mesh.set_tolerance(0.0);
  deform_mesh.deform();
  // Save the deformed mesh
  output.open("deform_2.off");
  output << mesh;
}
