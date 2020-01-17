# CGAL_Samples

# 概要

CGAL basic samples

公式サイト

- [The Computational Geometry Algorithms Library]([https://www.cgal.org](https://www.cgal.org/))
- [CGAL/cgal: The public CGAL repository, see the README below](https://github.com/CGAL/cgal)

参考サイト

- [CGAL入門](https://sites.google.com/site/introduction2cgal/)
  - 日本語の解説サイト

モジュールごとにライセンスが分かれているが、どれもGPLかLGPL。

- ライセンスについては明確に説明がある https://doc.cgal.org/latest/Manual/preliminaries.html#licensesGPL
- 基本的なデータ構造はLGPLで自由に使えて、高度なアルゴリズムはGPL
- GPLは商用ライセンスで購入して使う方法もあるらしい

マニュアルが充実している。アルゴリズムの解説もある https://doc.cgal.org/latest/Manual/packages.html



# 環境構築

## CGAL-5.0

setup版をインストール https://github.com/CGAL/cgal/releases/download/releases%2FCGAL-5.0/CGAL-5.0-Setup.exe

インストーラー起動するので、ライブラリと追加で必要なライブラリにチェック入れてインストール。

パスはデフォルトのC:/dev/CGAL-5.0で通した。環境変数も自動設定にした。

使うだけならこれで完了。

## CGAL-4.13.1

今回はコンパイラにVisual Studio 2017を使用した。

setup版をインストール https://github.com/CGAL/cgal/releases/download/releases%2FCGAL-4.13.1/CGAL-4.13.1-Setup.exe

"C:\CGAL-4.13.1\auxiliary\gmp\lib\" のパスを通せとwarning出る → 実行時に必要なdllがあるので、必ずパスを通すこと

setup版といってもcmakeするためのソースファイルが置かれるだけ。コンパイルは自分でやる

"C:\CGAL-4.13.1\build"のディレクトリ作成

buildディレクトリ内で

```
cmake .. -G"Visual Studio 15 2017 Win64"
cmake-gui ..
```

cmake-guiで変数の設定を行う。"C:\CGAL-4.13.1\INSTALL.md"の内容を参考に以下を設定
- CXX_FLAGSに /utf-8のオプション追加
- WITH_Eigen3, WITH_demos, WITH_examplesにチェック入れる
- MPFIが必要っぽいのでコンパイル済みのものをダウンロード http://vegas.loria.fr/rs/
  - MPFIのサイトの指示通り以下の環境変数を設定
    - MPFI_INC_DIR : C:\libs\librs_3.1.0\include
    - MPFI_LIB_DIR : C:\libs\librs_3.1.0\x86_64_WINDOWS\lib
    - RS_INC_DIR : C:\libs\librs_3.1.0\include
    - RS_LIB_DIR : C:\libs\librs_3.1.0\x86_64_WINDOWS\lib
- WITH_MPFI, WITH_RSにチェック入れる
- Eigenのパス設定。環境変数ではなくcmake-gui上に直接変数追加する。(無い場合はEigenのサイトからダウンロードしておく)
  - EIGEN3_INCLUDE_DIR : C:/eigen-3.3.7

cmake-guiでconfigureボタン押す

Boostの項目が出てくるので、Boostの各種libのパスを入力する。(Boostが無い場合はダウンロードする)

cmake-guiでconfigure,generateする。

vs2017でdebug,release各モードでALL_BUILDをビルドしてINSTALLをビルドする。

# 使い方

## cmake

- [How to use CGAL with CMake or your own build system · CGAL/cgal Wiki](https://github.com/CGAL/cgal/wiki/How-to-use-CGAL-with-CMake-or-your-own-build-system)

Qtのビューワーを使う場合と使わない場合で使うコンポーネントが異なる。

### Qtのビューワーを使わない場合

最小限のサンプル

```cmake
cmake_minimum_required( VERSION 3.6 )

project(sample_proj CXX)

find_package(CGAL)

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME}
  PRIVATE
    main.cpp
)
target_link_libraries(${PROJECT_NAME} 
   CGAL::CGAL
)
```

テンプレート用

```cmake
cmake_minimum_required( VERSION 3.6 )

# Create Project
get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId} CXX)
message(${ProjectId})

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

find_package(CGAL)

if(NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX d)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/../bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/../bin)

# Add Executable
add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME}
  PRIVATE
    main.cpp
)

set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

target_link_libraries(${PROJECT_NAME} 
   CGAL::CGAL
)
```

main.cppのサンプル

```c++
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh.h>
#include <fstream>

typedef CGAL::Simple_cartesian<double>                       Kernel;
typedef Kernel::Point_3                                      Point;
typedef CGAL::Surface_mesh<Point>                            Mesh;
int main(int argc, char* argv[]){
	Mesh mesh;
	std::ifstream in1((argc>1)?argv[1]:"data/triangle.off");
	//	std::ifstream in1((argc>1)?argv[1]:"data/octahedron.ply"); can't read
	
	in1 >> mesh;
	
	Mesh::Face_range::iterator f;
	Mesh::Vertex_range::iterator v;
	
	for (f=mesh.faces().begin(); f!=mesh.faces().end(); ++f){
		if (*f!=boost::graph_traits<Mesh>::null_face()){
			Mesh::Face_index fi = *f;
			std::cout << fi << "::"; // face index
			Mesh::Halfedge_index hi = mesh.halfedge(fi);
			do{
				std::cout << mesh.point(mesh.source(hi)) << ", "; // vertices index face has
				hi=mesh.next(hi);
			}while(hi!=mesh.halfedge(fi));
			std::cout << std::endl;
		}
	}
	
	for (v=mesh.vertices().begin(); v!=mesh.vertices().end(); ++v){
		Mesh::Vertex_index vi = *v;
		std::cout << vi << "::" << mesh.point(vi) << "," << std::endl; // vertex index and position
	}
	return EXIT_SUCCESS;
}
```

### Qtのビューワーを使う場合

最小サンプル

```cmake
cmake_minimum_required( VERSION 3.6 )

project(sample_proj CXX)

find_package(CGAL REQUIRED COMPONENTS Qt5 Core)

if(CGAL_FOUND AND CGAL_Qt5_FOUND)
  add_definitions(-DCGAL_USE_BASIC_VIEWER -DQT_NO_KEYWORDS) #required to use basic_viewer
else()
  message(FATAL_ERROR "ERROR: this program requires CGAL and CGAL_Qt5 and will not be compiled.")
endif()

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME}
  PRIVATE
    main.cpp
)

target_link_libraries(${PROJECT_NAME} 
  CGAL::CGAL
  CGAL::CGAL_Qt5
  CGAL::CGAL_Core
)
```

テンプレート用

```cmake
cmake_minimum_required( VERSION 3.6 )

# Create Project
get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId} CXX)
message(${ProjectId})

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

find_package(CGAL REQUIRED COMPONENTS Qt5 Core)

if(CGAL_FOUND AND CGAL_Qt5_FOUND)
  add_definitions(-DCGAL_USE_BASIC_VIEWER -DQT_NO_KEYWORDS) #required to use basic_viewer
else()
  message(FATAL_ERROR "ERROR: this program requires CGAL and CGAL_Qt5 and will not be compiled.")
endif()

if(NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX d)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/../bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/../bin)

# Add Executable
add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME}
  PRIVATE
    main.cpp
)

set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

target_link_libraries(${PROJECT_NAME} 
  CGAL::CGAL
  CGAL::CGAL_Qt5
  CGAL::CGAL_Core
)
```

main.cppのサンプル

```c++
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/boost/graph/helpers.h>
#include <CGAL/draw_surface_mesh.h>

#define CGAL_USE_BASIC_VIEWER 1
typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef K::Point_3 Point;
typedef CGAL::Surface_mesh<Point> Mesh;

int main()
{
  Mesh m;
  CGAL::make_icosahedron<Mesh, Point>(m);
  CGAL::draw(m);
  return 0;
}
```



## ファイルの入出力

fstreamで開いたファイルポインタをストリームでMeshオブジェクトに流し込むと読み込む(offファイルのみ対応?)

参考：https://doc.cgal.org/latest/Surface_mesh/index.html#Chapter_3D_Surface_mesh

より一般的な方法はpolygon soupを使う方法

- [CGAL 5.0 - Polygon Mesh Processing: Polygon_mesh_processing/orient_polygon_soup_example.cpp](https://doc.cgal.org/latest/Polygon_mesh_processing/Polygon_mesh_processing_2orient_polygon_soup_example_8cpp-example.html#a2)
  - offファイルを頂点と面で別々に読み込み、orientしてからPolyhedronに変換している。これはPLYファイルなどでも同じように使える

```cpp
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Polyhedron_items_with_id_3.h>
#include <CGAL/Polygon_mesh_processing/orient_polygon_soup.h>
#include <CGAL/Polygon_mesh_processing/polygon_soup_to_polygon_mesh.h>
#include <CGAL/Polygon_mesh_processing/orientation.h>
#include <CGAL/IO/OFF_reader.h>
#include <vector>
#include <fstream>
#include <iostream>
typedef CGAL::Exact_predicates_inexact_constructions_kernel          K;
typedef CGAL::Polyhedron_3<K, CGAL::Polyhedron_items_with_id_3>      Polyhedron;
int main(int argc, char* argv[])
{
  const char* filename = (argc > 1) ? argv[1] : "data/tet-shuffled.off";
  std::ifstream input(filename);
  std::vector<K::Point_3> points;
  std::vector<std::vector<std::size_t> > polygons;
  if(!input || !CGAL::read_OFF(input, points, polygons) || points.empty())
  {
    std::cerr << "Cannot open file " << std::endl;
    return EXIT_FAILURE;
  }
  CGAL::Polygon_mesh_processing::orient_polygon_soup(points, polygons);
  Polyhedron mesh;
  CGAL::Polygon_mesh_processing::polygon_soup_to_polygon_mesh(points, polygons, mesh);
  // Number the faces because 'orient_to_bound_a_volume' needs a face <--> index map
  int index = 0;
  for(Polyhedron::Face_iterator fb=mesh.facets_begin(), fe=mesh.facets_end(); fb!=fe; ++fb)
    fb->id() = index++;
  if(CGAL::is_closed(mesh))
    CGAL::Polygon_mesh_processing::orient_to_bound_a_volume(mesh);
  std::ofstream out("tet-oriented1.off");
  out << mesh;
  out.close();
  CGAL::Polygon_mesh_processing::reverse_face_orientations(mesh);
  std::ofstream out2("tet-oriented2.off");	// 面の向きをすべて反転
  out2 << mesh;
  out2.close();
  return EXIT_SUCCESS;
}
```

- [CGAL 5.0 - Polygon Mesh Processing: Polygon_mesh_processing/repair_polygon_soup_example.cpp](https://doc.cgal.org/latest/Polygon_mesh_processing/Polygon_mesh_processing_2repair_polygon_soup_example_8cpp-example.html#a3)
  - 頂点が重複していたり、使わないものがあった場合に修正してからPolyhedron作る方法

```cpp
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Polygon_mesh_processing/repair_polygon_soup.h>
#include <CGAL/Polygon_mesh_processing/orient_polygon_soup.h>
#include <CGAL/Polygon_mesh_processing/polygon_soup_to_polygon_mesh.h>
#include <iostream>
#include <vector>
typedef CGAL::Exact_predicates_inexact_constructions_kernel     K;
typedef K::Point_3                                              Point_3;
typedef std::vector<std::size_t>                                Polygon;
typedef CGAL::Surface_mesh<Point_3>                             Mesh;
namespace PMP = CGAL::Polygon_mesh_processing;
int main(int, char**)
{
  // First, construct a polygon soup with some problems
  std::vector<Point_3> points;
  std::vector<Polygon> polygons;
  points.push_back(Point_3(0,0,0));
  points.push_back(Point_3(1,0,0));
  points.push_back(Point_3(0,1,0));
  points.push_back(Point_3(-1,0,0));
  points.push_back(Point_3(0,-1,0));
  points.push_back(Point_3(0,1,0)); // duplicate point
  points.push_back(Point_3(0,-2,0)); // unused point
  Polygon p;
  p.push_back(0); p.push_back(1); p.push_back(2);
  polygons.push_back(p);
  // degenerate face
  p.clear();
  p.push_back(0); p.push_back(0); p.push_back(0);
  polygons.push_back(p);
  p.clear();
  p.push_back(0); p.push_back(1); p.push_back(4);
  polygons.push_back(p);
  // duplicate face with different orientation
  p.clear();
  p.push_back(0); p.push_back(4); p.push_back(1);
  polygons.push_back(p);
  p.clear();
  p.push_back(0); p.push_back(3); p.push_back(5);
  polygons.push_back(p);
  // degenerate face
  p.clear();
  p.push_back(0); p.push_back(3); p.push_back(0);
  polygons.push_back(p);
  p.clear();
  p.push_back(0); p.push_back(3); p.push_back(4);
  polygons.push_back(p);
  // pinched and degenerate face
  p.clear();
  p.push_back(0); p.push_back(1); p.push_back(2); p.push_back(3);
  p.push_back(4); p.push_back(3); p.push_back(2); p.push_back(1);
  polygons.push_back(p);
  PMP::repair_polygon_soup(points, polygons);
  PMP::orient_polygon_soup(points, polygons);
  Mesh mesh;
  PMP::polygon_soup_to_polygon_mesh(points, polygons, mesh);
  std::cout << "Mesh has " << num_vertices(mesh) << " vertices and " << num_faces(mesh) << " faces" << std::endl;
  assert(num_vertices(mesh) == 5);
  assert(num_faces(mesh) == 4);
  return 0;
}
```



### PLYファイルの入出力

- [How can we SIMPLY load a ply mesh to CGAL, apply some operators and SIMPLY export another ply mesh? · Issue #3344 · CGAL/cgal](https://github.com/CGAL/cgal/issues/3344)
- [Issue with Polyhedron and CGAL::write_PLY() function · Issue #4411 · CGAL/cgal](https://github.com/CGAL/cgal/issues/4411)

読み込むときは、PLYの構造上、Polyhedronとしてそのまま読むことはできない(orientable meshではないため)。

polygon soupとして読み込み→orientedなデータに変換する。

PLY入力例

[cgal/PLY_io_plugin.cpp at master · CGAL/cgal](https://github.com/CGAL/cgal/blob/master/Polyhedron/demo/Polyhedron/Plugins/IO/PLY_io_plugin.cpp)

上のリンクのファイルがそのまま参考になる。

PLYファイルは面の有無と面の頂点数が任意なので、パターン別に読み込む必要がある。

面があったら最初にSMesh(mesh型)で読み込んでみる→駄目ならpolygon soupで読み込み。

面が無い場合は点群として読み込み。

## 

PLY出力例(読み込みはOFFファイルとか)

```cpp
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/IO/PLY_writer.h>

#include <fstream>

typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_3 Point_3;
typedef CGAL::Surface_mesh<Point_3> Mesh;

int main (int argc, char** argv)
{
  Mesh mesh;
  std::ifstream (argv[1]) >> mesh;

  std::ofstream out ("out.ply");
  CGAL::write_PLY (out, mesh);
  
  return EXIT_SUCCESS;
}
```



## 頂点・面の追加

- [CGAL 5.0 - 3D Polyhedral Surface: CGAL::Polyhedron_incremental_builder_3< HDS > Class Template Reference](https://doc.cgal.org/latest/Polyhedron/classCGAL_1_1Polyhedron__incremental__builder__3.html)
- [じぇblog: Polyhedron_incremental_builder_3を使ってみる](http://geblog3.blogspot.com/2010/11/polyhedronincrementalbuilder3.html)

追加をするための専用のクラス`Polyhedron_incremental_builder_3`がある。

頂点数・面数・ハーフエッジ数(なしでも可)、相対インデックスの有無を指定して、頂点と面の追加をしていく。

面指定は頂点のインデックスで行う。

```c++
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Polyhedron_incremental_builder_3.h>
#include <CGAL/Polyhedron_3.h>
// A modifier creating a triangle with the incremental builder.
template <class HDS>
class Build_triangle : public CGAL::Modifier_base<HDS> {
public:
    Build_triangle() {}
    void operator()( HDS& hds) {
        // Postcondition: hds is a valid polyhedral surface.
        CGAL::Polyhedron_incremental_builder_3<HDS> B( hds, true);
        B.begin_surface( 3, 1, 6);		// 頂点、面、ハーフエッジ。openglみたいにbeginとendで挟む
        typedef typename HDS::Vertex   Vertex;
        typedef typename Vertex::Point Point;
        B.add_vertex( Point( 0, 0, 0));	//頂点の追加
        B.add_vertex( Point( 1, 0, 0));
        B.add_vertex( Point( 0, 1, 0));
        B.begin_facet();	//面の追加。これも面ごとにbegin-endで定義
        B.add_vertex_to_facet( 0);	//頂点はインデックス指定
        B.add_vertex_to_facet( 1);
        B.add_vertex_to_facet( 2);
        B.end_facet();
        B.end_surface();
    }
};
typedef CGAL::Simple_cartesian<double>     Kernel;
typedef CGAL::Polyhedron_3<Kernel>         Polyhedron;
typedef Polyhedron::HalfedgeDS             HalfedgeDS;
int main() {
    Polyhedron P;
    Build_triangle<HalfedgeDS> triangle;
    P.delegate( triangle);	// delegateでPolyhedronのデータが作成される。関数内で引数の関数呼び出し演算子が呼ばれる
    CGAL_assertion( P.is_triangle( P.halfedges_begin()));
    return 0;
}
```



## データアクセス方法

### 面、頂点、エッジの各要素にアクセスする方法

vcglibと同じような方法で出来る。

ストリーム演算子が全ての型に定義されているので、情報みたい場合は<<でストリームに流せばとりあえず何某かか表示される。

### イテレータ

CMakeLists.txt

```cmake
cmake_minimum_required( VERSION 3.6 )

# Create Project
get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId} CXX)
message(${ProjectId})

set( CGALLIB_DIR C:/CGAL-4.13.1)
find_package(CGAL REQUIRED COMPONENTS Core)

include( ${CGAL_USE_FILE} )

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

set(SOURCE_FILES
   main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} 
   CGAL::CGAL
)
```

main.cpp

```cpp
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh.h>
#include <fstream>

typedef CGAL::Simple_cartesian<double>                       Kernel;
typedef Kernel::Point_3                                      Point;
typedef CGAL::Surface_mesh<Point>                            Mesh;
int main(int argc, char* argv[]){
	Mesh mesh;
	std::ifstream in1((argc>1)?argv[1]:"data/triangle.off");
	//	std::ifstream in1((argc>1)?argv[1]:"data/octahedron.ply"); can't read
	
	in1 >> mesh;
	
	Mesh::Face_range::iterator f;
	Mesh::Vertex_range::iterator v;
	
	for (f=mesh.faces().begin(); f!=mesh.faces().end(); ++f){
		if (*f!=boost::graph_traits<Mesh>::null_face()){
			Mesh::Face_index fi = *f;
			std::cout << fi << "::"; // face index
			Mesh::Halfedge_index hi = mesh.halfedge(fi);
			do{
				std::cout << mesh.point(mesh.source(hi)) << ", "; // vertices index face has
				hi=mesh.next(hi);
			}while(hi!=mesh.halfedge(fi));
			std::cout << std::endl;
		}
	}
	
	for (v=mesh.vertices().begin(); v!=mesh.vertices().end(); ++v){
		Mesh::Vertex_index vi = *v;
		std::cout << vi << "::" << mesh.point(vi) << "," << std::endl; // vertex index and position
	}
	return EXIT_SUCCESS;
}
```



## データの可視化

形状の確認を一時的に行いたい場合はcmakeでCGALのQtコンポーネントを追加して、ソース内でCGAL::draw()呼ぶだけで簡易ビューワーが起動する。

main.cpp

```cpp
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/draw_surface_mesh.h>
#include <fstream>
typedef CGAL::Simple_cartesian<double>                       Kernel;
typedef Kernel::Point_3                                      Point;
typedef CGAL::Surface_mesh<Point>                            Mesh;
int main(int argc, char* argv[]){
	Mesh sm1;
	std::ifstream in1((argc>1)?argv[1]:"data/triangle.off");
	in1 >> sm1;
	CGAL::draw(sm1);
	return EXIT_SUCCESS;
}
```



## データ処理のサンプル

頂点の追加・削除



## ブーリアン演算

[表面メッシュのブーリアン演算(CGALライブラリ) - Mesh Wiki]([https://www.rccm.co.jp/icem/pukiwiki/index.php?%E8%A1%A8%E9%9D%A2%E3%83%A1%E3%83%83%E3%82%B7%E3%83%A5%E3%81%AE%E3%83%96%E3%83%BC%E3%83%AA%E3%82%A2%E3%83%B3%E6%BC%94%E7%AE%97%28CGAL%E3%83%A9%E3%82%A4%E3%83%96%E3%83%A9%E3%83%AA%29](https://www.rccm.co.jp/icem/pukiwiki/index.php?表面メッシュのブーリアン演算(CGALライブラリ)))

```c++
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/IO/Polyhedron_iostream.h>
#include <CGAL/Nef_polyhedron_3.h> 
#include <fstream>

typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;
typedef CGAL::Polyhedron_3<Kernel> Polyhedron;
typedef CGAL::Nef_polyhedron_3<Kernel> Nef_polyhedron;

int main() {
  Polyhedron P1;
  std::ifstream in1("box.off");
  in1 >> P1;
 
  Polyhedron P2;
  std::ifstream in2("cyl.off");
  in2 >> P2;
 
  Nef_polyhedron N1(P1);
  Nef_polyhedron N2(P2);
 
  N1 -= N2;
 
  Polyhedron P;
  if(N1.is_simple()){
    N1.convert_to_Polyhedron(P);
    std::ofstream out("diff.off");
    out << P;
  }
  else{
    std::cerr << "N1 is not a 2-manifold." << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
```

## Package

### deformation

http://sites.fas.harvard.edu/~cs277/papers/deformation_survey.pdf



---

# CGALの基礎知識

- [c++ - CGAL tutorial for beginners - Stack Overflow](https://stackoverflow.com/questions/17409037/cgal-tutorial-for-beginners)
- [CGAL - the Computational Geometry Algorithms Library](https://cel.archives-ouvertes.fr/file/index/docid/340448/filename/whole-course.pdf)
  - 基本的な概念が分かりやすく説明されている
- [CGAL Videos](https://www.cgal.org/videos.html)
  - [CGAL: The Open Source Computational Geometry Algorithms Library - YouTube](https://www.youtube.com/watch?v=3DLfkWWw_Tg)
- 

predicatesとconstructionsの2つの要素がある。

predicatesは状態のこと。e.g. 円の中にあるかないか、orientationの向き。

constructionsは作図のこと。交点を求めたり、重心を求めたり。



[CGAL入門](https://sites.google.com/site/introduction2cgal/)

```c++
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/convex_hull_2.h>
#include <vector>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point_2;
typedef std::vector<Point_2> Points;

int main(){
  Points points, result;
  points.push_back(Point_2(0,0));
  points.push_back(Point_2(10,0));
  points.push_back(Point_2(10,10));
  points.push_back(Point_2(6,5));
  points.push_back(Point_2(4,1));

  CGAL::convex_hull_2( points.begin(), points.end(), std::back_inserter(result) );
  std::cout << result.size() << " points on the convex hull" << std::endl;
  return 0;
}
```

```c++
#include <iostream>
#include <iterator>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/convex_hull_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point_2;

int main(){
  std::istream_iterator< Point_2 >  input_begin( std::cin );
  std::istream_iterator< Point_2 >  input_end;
  std::ostream_iterator< Point_2 >  output( std::cout, "\n" );
  CGAL::convex_hull_2( input_begin, input_end, output );
  return 0;
}
```

## 構造の説明

http://dosei.hatenadiary.jp/category/CGAL





CGALのライブラリはconceptとmodelに分かれている

# 数学の基礎知識

CGALは有限体や環の概念がベースになっている。これをC++のSTLで実現している。

[代数学 - [物理のかぎしっぽ]](http://hooktail.org/misc/index.php?%C2%E5%BF%F4%B3%D8)

# CGAL Ipelets

数学的な作図ソフトらしい

[CGAL 5.0 - CGAL Ipelets: User Manual](https://doc.cgal.org/latest/CGAL_ipelets/index.html)

[Ipe のインストール - さてもちブログ](http://satemochi.blog.fc2.com/blog-entry-126.html)