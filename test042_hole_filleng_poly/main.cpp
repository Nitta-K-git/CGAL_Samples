#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_mesh_processing/triangulate_hole.h>
#include <CGAL/utility.h>
#include <vector>
#include <iterator>
typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::Point_3 Point;
int main()
{
  std::vector<Point> polyline;
  polyline.push_back(Point( 10.,0.,0.));
  polyline.push_back(Point( 0.,1.,0.));
  polyline.push_back(Point(-1.,0.,0.));
  polyline.push_back(Point( 1.,10.,0.));
  // repeating first point (i.e. polyline.push_back(Point(1.,0.,0.)) ) is optional
  // any type, having Type(int, int, int) constructor available, can be used to hold output triangles
  typedef CGAL::Triple<int, int, int> Triangle_int;
  std::vector<Triangle_int> patch;
  patch.reserve(polyline.size() -2); // there will be exactly n-2 triangles in the patch
  CGAL::Polygon_mesh_processing::triangulate_hole_polyline(
          polyline,
          std::back_inserter(patch));
  for(std::size_t i = 0; i < patch.size(); ++i)
  {
    std::cout << "Triangle " << i << ": "
      << patch[i].first << " " << patch[i].second << " " << patch[i].third
      << std::endl;
  }
  // note that no degenerate triangles are generated in the patch 縮退メッシュは生成されない
  std::vector<Point> polyline_collinear;
  polyline_collinear.push_back(Point(1.,0.,0.));
  polyline_collinear.push_back(Point(2.,0.,0.));
  polyline_collinear.push_back(Point(3.,0.,0.));
  polyline_collinear.push_back(Point(4.,0.,0.));
  std::vector<Triangle_int> patch_will_be_empty;
  CGAL::Polygon_mesh_processing::triangulate_hole_polyline(
          polyline_collinear,
          back_inserter(patch_will_be_empty));
  CGAL_assertion(patch_will_be_empty.empty());
  return 0;
}

// #include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
// #include <CGAL/Surface_mesh.h>
// #include <CGAL/Polygon_mesh_processing/triangulate_hole.h>
// #include <iostream>
// #include <fstream>
// #include <vector>
// typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
// typedef CGAL::Surface_mesh<Kernel::Point_3>                  Mesh;

// typedef Mesh::Halfedge_index halfedge_descriptor;
// typedef Mesh::Face_index face_descriptor;
// typedef Mesh::Vertex_index vertex_descriptor;

// int main(int argc, char* argv[])
// {
//   const char* filename = (argc > 1) ? argv[1] : "data/mech-holes-shark.off";
//   std::ifstream input(filename);
//   Mesh mesh;
//   if ( !input || !(input >> mesh) || mesh.is_empty() ) {
//     std::cerr << "Not a valid off file." << std::endl;
//     return 1;
//   }
//   // Incrementally fill the holes
//   unsigned int nb_holes = 0;
//   for(halfedge_descriptor h : halfedges(mesh))
//   {
//     if(mesh.is_border(h))
//     {
//         std::vector<face_descriptor>  patch_facets;
//         std::vector<vertex_descriptor> patch_vertices;
//         CGAL::Polygon_mesh_processing::triangulate_and_refine_hole(
//                 mesh,
//                 h,
//                 std::back_inserter(patch_facets),
//                 std::back_inserter(patch_vertices),
//                 CGAL::Polygon_mesh_processing::parameters::vertex_point_map(get(CGAL::vertex_point, mesh)).
//                 geom_traits(Kernel()));
//       std::cout << " Number of facets in constructed patch: " << patch_facets.size() << std::endl;
//       std::cout << " Number of vertices in constructed patch: " << patch_vertices.size() << std::endl;
//       ++nb_holes;
//     }
//   }
//   std::cout << std::endl;
//   std::cout << nb_holes << " holes have been filled" << std::endl;
  
//   std::ofstream out("filled.off");
//   out.precision(17);
//   out << mesh << std::endl;
//   return 0;
// }