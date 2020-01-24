#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Polygon_mesh_processing/distance.h>

#include <CGAL/Polyhedron_3.h>
#include <CGAL/IO/Polyhedron_iostream.h>
#include <fstream>

#if defined(CGAL_LINKED_WITH_TBB) // if you installed and linked TBB, you can use parallel calculation
#define TAG CGAL::Parallel_tag
#else
#define TAG CGAL::Sequential_tag // default
#endif
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_3                     Point;
typedef CGAL::Surface_mesh<K::Point_3> Mesh;
typedef CGAL::Polyhedron_3<K> Polyhedron;
namespace PMP = CGAL::Polygon_mesh_processing;
int main(int argc, char* argv[])
{
  Mesh tm1, tm2;
  Polyhedron P1, P2;

  if(argc < 3){
    std::cout << "usage: filename1 filename2" << std::endl;
    return EXIT_FAILURE;
  }
  std::ifstream in1(argv[1]);
  in1 >> P1;
  std::ifstream in2(argv[2]);
  in2 >> P2;

  std::cout << "Approximated Hausdorff distance: "
            << CGAL::Polygon_mesh_processing::approximate_Hausdorff_distance
                  <TAG>(P1, P2, PMP::parameters::number_of_points_per_area_unit(4000))
            << std::endl;
}