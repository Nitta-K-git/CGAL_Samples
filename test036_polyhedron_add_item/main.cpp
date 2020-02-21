#include <CGAL/Simple_cartesian.h>
#include <CGAL/HalfedgeDS_vector.h>
#include <CGAL/Polyhedron_3.h>
#include <iostream>
typedef CGAL::Simple_cartesian<double>                 Kernel;
typedef Kernel::Point_3                                Point_3;
typedef CGAL::Polyhedron_3< Kernel,
                            CGAL::Polyhedron_items_3,
                            CGAL::HalfedgeDS_vector>   Polyhedron;
int main() {
    Point_3 p( 1.0, 0.0, 0.0);
    Point_3 q( 0.0, 1.0, 0.0);
    Point_3 r( 0.0, 0.0, 1.0);
    Point_3 s( 0.0, 0.0, 0.0);
    Polyhedron P;    // alternative constructor: Polyhedron P(4,12,4);
    // P.make_tetrahedron( p, q, r, s);
    P.make_triangle(p,q,r); // 面を頂点指定して追加
    P.make_triangle(p,q,s); // 頂点の重複はそのままになる
    //make_triangleを空の引数で作成。戻り値のHandleから頂点を追加する(やり方不明)

    CGAL::set_ascii_mode( std::cout);
    std::copy( P.points_begin(), P.points_end(),
           std::ostream_iterator<Point_3>( std::cout, "\n"));
    return 0;
}