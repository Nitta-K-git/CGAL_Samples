#include <iostream>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <sstream>
typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel; // 完全浮動小数点
typedef Kernel::Point_2 Point_2;
int main(){
	Point_2 p(0, 0.3), q, r(2, 0.9);
	{
		q  = Point_2(1, 0.6); // 入力時点で浮動小数点に変換されているため、not
		std::cout << (CGAL::collinear(p,q,r) ? "collinear\n" : "not collinear\n");
	}
	
	{
		std::istringstream input("0 0.3   1 0.6   2 0.9"); // テキストから入力されるため、完全精度になる
		input >> p >> q >> r;
		std::cout << (CGAL::collinear(p,q,r) ? "collinear\n" : "not collinear\n");
	}
	
	{
		q = CGAL::midpoint(p,r); // 完全精度型の戻り値になる
		std::cout << (CGAL::collinear(p,q,r) ? "collinear\n" : "not collinear\n");   
	}
	
	return 0;
}
