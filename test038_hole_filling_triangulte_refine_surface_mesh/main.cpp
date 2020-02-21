#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Polygon_mesh_processing/triangulate_hole.h>
#include <iostream>
#include <fstream>
#include <vector>
typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef CGAL::Surface_mesh<Kernel::Point_3>                  Mesh;

typedef Mesh::Halfedge_index Halfedge_handle;
typedef Mesh::Face_index Facet_handle;
typedef Mesh::Vertex_index Vertex_handle;

int main(int argc, char* argv[])
{
  const char* filename = (argc > 1) ? argv[1] : "data/mech-holes-shark.off";
  std::ifstream input(filename);
  Mesh mesh;
  if ( !input || !(input >> mesh) || mesh.is_empty() ) {
    std::cerr << "Not a valid off file." << std::endl;
    return 1;
  }
  // Incrementally fill the holes
  unsigned int nb_holes = 0;
  for(Halfedge_handle h : halfedges(mesh))
  {
    if(mesh.is_border(h))
    {
        std::vector<Facet_handle>  patch_facets;
        std::vector<Vertex_handle> patch_vertices;
        CGAL::Polygon_mesh_processing::triangulate_and_refine_hole(
                mesh,
                h,
                std::back_inserter(patch_facets),
                std::back_inserter(patch_vertices),
                CGAL::Polygon_mesh_processing::parameters::vertex_point_map(get(CGAL::vertex_point, mesh)).
                geom_traits(Kernel()));
      std::cout << " Number of facets in constructed patch: " << patch_facets.size() << std::endl;
      std::cout << " Number of vertices in constructed patch: " << patch_vertices.size() << std::endl;
      ++nb_holes;
    }
  }
  std::cout << std::endl;
  std::cout << nb_holes << " holes have been filled" << std::endl;
  
  std::ofstream out("filled.off");
  out.precision(17);
  out << mesh << std::endl;
  return 0;
}