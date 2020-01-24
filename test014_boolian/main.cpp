#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/IO/Polyhedron_iostream.h>
#include <CGAL/Nef_polyhedron_3.h>

#include <CGAL/Polygon_mesh_processing/measure.h>

#include <fstream>
 
typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;
typedef CGAL::Polyhedron_3<Kernel> Polyhedron;
typedef CGAL::Nef_polyhedron_3<Kernel> Nef_polyhedron;
 
int main(int argc, char* argv[]){
  if(argc < 3){
    std::cout << "usage: filename1 filename2" << std::endl;
    return EXIT_FAILURE;
  }
  Polyhedron P1;
  std::ifstream in1(argv[1]);
  in1 >> P1;
 
  Polyhedron P2;
  std::ifstream in2(argv[2]);
  in2 >> P2;
 
  Nef_polyhedron N1(P1);
  Nef_polyhedron N2(P2);
  
  std::cout << "P1: volume:" << CGAL::Polygon_mesh_processing::volume(P1) << std::endl;
  std::cout << "P2: volume:" << CGAL::Polygon_mesh_processing::volume(P2) << std::endl;
 
  N1 -= N2;
 
  Polyhedron P;
  if(N1.is_simple()) {
    N1.convert_to_Polyhedron(P);
	std::cout << "P: volume:" << CGAL::Polygon_mesh_processing::volume(P) << std::endl;
    std::ofstream out("diff.off");
    out << P;
  }
  else
  {
    std::cerr << "N1 is not a 2-manifold." << std::endl;
    return EXIT_FAILURE;
  }
 
  return EXIT_SUCCESS;
}
