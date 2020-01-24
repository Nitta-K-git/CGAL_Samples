#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/IO/Polyhedron_iostream.h>
#include <CGAL/Polygon_mesh_processing/measure.h>
#include <fstream>
 
typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;
typedef CGAL::Polyhedron_3<Kernel> Polyhedron;
 
int main(int argc, char* argv[]){
  if(argc < 2){
    std::cout << "usage: filename1" << std::endl;
    return EXIT_FAILURE;
  }
  Polyhedron P1;
  std::ifstream in1(argv[1]);
  in1 >> P1;
 
  std::cout << "P1: volume:" << CGAL::Polygon_mesh_processing::volume(P1) << std::endl;
  return EXIT_SUCCESS;
}
