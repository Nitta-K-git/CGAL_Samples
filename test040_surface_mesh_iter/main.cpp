#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>
#include <iostream>
#include <fstream>
#include <vector>
typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef CGAL::Surface_mesh<Kernel::Point_3>                  Mesh;

int main(int argc, char* argv[])
{
    const char* filename = (argc > 1) ? argv[1] : "in.off";
    std::ifstream input(filename);
    Mesh mesh;
    if ( !input || !(input >> mesh) || mesh.is_empty() ) {
        std::cerr << "Not a valid off file." << std::endl;
        return 1;
    }

    std::cout << "Halfedge_index: ";
    for(Mesh::Halfedge_index h : halfedges(mesh))
        std::cout << h << ',';
    std::cout << std::endl;

    std::cout << "Edge_index: ";
    for(Mesh::Edge_index e : edges(mesh))
        std::cout << e << ',';
    std::cout << std::endl;

    std::cout << "vertex_index: ";
    for(Mesh::Vertex_index v : vertices(mesh))
        std::cout << v << ',';
    std::cout << std::endl;

    std::cout << "face_index: ";
    for(Mesh::Face_index f : faces(mesh))
        std::cout << f << ',';
    std::cout << std::endl;

    std::cout << "Halfedge_index: ";
    for(Mesh::Halfedge_index h : halfedges(mesh))
        std::cout << h.idx() << ',';
    std::cout << std::endl;

    std::cout << "Edge_index: ";
    for(Mesh::Edge_index e : edges(mesh))
        std::cout << e.idx() << ',';
    std::cout << std::endl;

    std::cout << "vertex_index: ";
    for(Mesh::Vertex_index v : vertices(mesh))
        std::cout << v.idx() << ',';
    std::cout << std::endl;

    std::cout << "face_index: ";
    for(Mesh::Face_index f : faces(mesh))
        std::cout << f.idx() << ',';
    std::cout << std::endl;

    // get connectivity
	Mesh::Vertex_index v0(0);
    Mesh::Halfedge_index h = mesh.halfedge(v0); //v0に入ってくるハーフエッジの一つを返す
    Mesh::Vertex_index v = mesh.source(h);
    std::cout << "v: " << v.idx() << std::endl;

    std::ofstream out("out.off");
    out.precision(17);
    out << mesh << std::endl;
    return 0;
}
