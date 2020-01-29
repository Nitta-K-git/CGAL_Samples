#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Polygon_mesh_processing/corefinement.h>
#include <CGAL/Polygon_mesh_processing/remesh.h>
#include <CGAL/boost/graph/selection.h>
#include <fstream>
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Surface_mesh<K::Point_3>             Mesh;
typedef boost::graph_traits<Mesh>::edge_descriptor edge_descriptor;
typedef boost::graph_traits<Mesh>::face_descriptor face_descriptor;
typedef boost::graph_traits<Mesh>::halfedge_descriptor halfedge_descriptor;
namespace PMP = CGAL::Polygon_mesh_processing;
namespace params = PMP::parameters;
struct Vector_pmap_wrapper{
  std::vector<bool>& vect;
  Vector_pmap_wrapper(std::vector<bool>& v) : vect(v) {}
  friend bool get(const Vector_pmap_wrapper& m, face_descriptor f)
  {
    return m.vect[f];
  }
  friend void put(const Vector_pmap_wrapper& m, face_descriptor f, bool b)
  {
    m.vect[f]=b;
  }
};
int main(int argc, char* argv[])
{
  const char* filename1 = (argc > 1) ? argv[1] : "data/blobby.off";
  const char* filename2 = (argc > 2) ? argv[2] : "data/eight.off";
  std::ifstream input(filename1);
  Mesh mesh1, mesh2, mesh_dif, mesh_and, mesh_or;
  if (!input || !(input >> mesh1))
  {
    std::cerr << "First mesh is not a valid off file." << std::endl;
    return 1;
  }
  input.close();
  input.open(filename2);
  if (!input || !(input >> mesh2))
  {
    std::cerr << "Second mesh is not a valid off file." << std::endl;
    return 1;
  }
  // Precondition
    if(CGAL::Polygon_mesh_processing::does_self_intersect(mesh1)){
        std::cerr << "CGAL::Polygon_mesh_processing::does_self_intersect(mesh1)" << std::endl;
        return 1;
    }
    if(CGAL::Polygon_mesh_processing::does_self_intersect(mesh2)){
        std::cerr << "CGAL::Polygon_mesh_processing::does_self_intersect(mesh2)" << std::endl;
        return 1;
    }
    if(!CGAL::Polygon_mesh_processing::does_bound_a_volume(mesh1)){
        std::cerr << "CGAL::Polygon_mesh_processing::does_bound_a_volume(mesh1)" << std::endl;
        return 1;
    }
    if(!CGAL::Polygon_mesh_processing::does_bound_a_volume(mesh2)){
        std::cerr << "CGAL::Polygon_mesh_processing::does_bound_a_volume(mesh2)" << std::endl;
        return 1;
    }
  //create a property on edges to indicate whether they are constrained
  Mesh::Property_map<edge_descriptor,bool> is_constrained_map = mesh_dif.add_property_map<edge_descriptor,bool>("e:is_constrained", false).first;
  // update mesh1 to contain the mesh bounding the difference
  // of the two input volumes.
  bool valid_difference =
    PMP::corefine_and_compute_difference(mesh1,
                                         mesh2,
                                         mesh_dif,
                                         params::all_default(), // default parameters for mesh1
                                         params::all_default(), // default parameters for mesh2
                                         params::edge_is_constrained_map(is_constrained_map));
  if (valid_difference)
  {
    std::cout << "Difference was successfully computed\n";
    std::ofstream output("difference.off");
    output << mesh_dif;
  }
  else{
    std::cout << "Difference could not be computed\n";
    return 1;
  }

    is_constrained_map = mesh_and.add_property_map<edge_descriptor,bool>("e:is_constrained", false).first;
  bool valid_and =
    PMP::corefine_and_compute_intersection(mesh1,
                                         mesh2,
                                         mesh_and,
                                         params::all_default(), // default parameters for mesh1
                                         params::all_default(), // default parameters for mesh2
                                         params::edge_is_constrained_map(is_constrained_map));
  if (valid_and)
  {
    std::cout << "Intersection was successfully computed\n";
    std::ofstream output("intersect.off");
    output << mesh_and;
  }
  else{
    std::cout << "Intersection could not be computed\n";
    return 1;
  }



  return 0;
}