#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/boost/graph/graph_traits_Surface_mesh.h>
#include <CGAL/boost/graph/Face_filtered_graph.h>
#include <CGAL/Polygon_mesh_processing/measure.h>
#include <CGAL/boost/graph/copy_face_graph.h>
#include <CGAL/mesh_segmentation.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include <CGAL/property_map.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef CGAL::Surface_mesh<Kernel::Point_3> SM;
typedef boost::graph_traits<SM>::face_descriptor face_descriptor;
typedef boost::graph_traits<SM>::vertex_descriptor vertex_descriptor;
typedef Kernel::Point_3 Point_3;

int main(int argc, char** argv )
{
  SM mesh;
  if (argc >= 2){
    std::ifstream input(argv[1]);
    input >> mesh;
  } else {
    std::ifstream cactus("data/cactus.off");
    cactus >> mesh;
  }
  if (!CGAL::is_triangle_mesh(mesh)){
    std::cerr << "Input is not a triangle mesh" << std::endl;
    return EXIT_FAILURE;
  }
  typedef SM::Property_map<face_descriptor,double> Facet_double_map;
  Facet_double_map sdf_property_map;

  sdf_property_map = mesh.add_property_map<face_descriptor,double>("f:sdf").first;

//   CGAL::sdf_values(mesh, sdf_property_map);
  double cone_ratio = (argc>=3)?std::stod(argv[2]):2.0/3.0; // 大きい方が分割数は増えるわけでもない
  CGAL::sdf_values(mesh, sdf_property_map, cone_ratio*CGAL_PI, 25, true);

  // create a property-map for segment-ids
  typedef SM::Property_map<face_descriptor, std::size_t> Facet_int_map;
  Facet_int_map segment_property_map = mesh.add_property_map<face_descriptor,std::size_t>("f:sid").first;;

  // segment the mesh using default parameters for number of levels, and smoothing lambda
  // Any other scalar values can be used instead of using SDF values computed using the CGAL function
  std::size_t number_of_segments;
//   number_of_segments = CGAL::segmentation_from_sdf_values(mesh, sdf_property_map, segment_property_map);

  const std::size_t number_of_clusters = (argc>=4)?std::stoul(argv[3]):4;       // use 4 clusters in soft clustering 大きくすると分割数が増える
  const double smoothing_lambda = (argc>=5)?std::stod(argv[4]):0.3;  // importance of surface features, suggested to be in-between [0,1] 大きくすると分割数が減る
  // Note that we can use the same SDF values (sdf_property_map) over and over again for segmentation.
  // This feature is relevant for segmenting the mesh several times with different parameters.
  number_of_segments = CGAL::segmentation_from_sdf_values(mesh, sdf_property_map, segment_property_map, number_of_clusters, smoothing_lambda);

  std::cout << "Number of segments: " << number_of_segments << std::endl;
//   for(face_descriptor fd : faces(mesh)){
//       // ids are between [0, number_of_segments -1]
//       std::cout << segment_property_map[fd] << " ";
//   }
//   std::cout << std::endl;

  typedef CGAL::Face_filtered_graph<SM> Filtered_graph;
  //print area of each segment and then put it in a Mesh and print it in an OFF file
  Filtered_graph segment_mesh(mesh, 0, segment_property_map);
  for(std::size_t id = 0; id < number_of_segments; ++id)
  {
    if(id > 0)
      segment_mesh.set_selected_faces(id, segment_property_map);
    std::cout << "Segment "<<id<<"'s area is : "<<CGAL::Polygon_mesh_processing::area(segment_mesh)<<std::endl;
    SM out;
    CGAL::copy_face_graph(segment_mesh, out);
    std::ostringstream oss;
    oss << "Segment_" << id<<".off";
    std::ofstream os(oss.str().data());
    os<<out;
  }
}
