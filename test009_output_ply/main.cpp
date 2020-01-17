#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/IO/PLY_writer.h>
#include <CGAL/IO/PLY_reader.h>

#include <fstream>

typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_3 Point_3;
typedef CGAL::Surface_mesh<Point_3> Mesh;

int main (int argc, char** argv)
{
  Mesh mesh;
  // std::ifstream (argv[1]) >> mesh;
  std::ifstream in (argv[1]);
  CGAL::read_PLY (in, mesh);

  std::ofstream out ("out.ply");
  CGAL::write_PLY (out, mesh);
  
  return EXIT_SUCCESS;
}