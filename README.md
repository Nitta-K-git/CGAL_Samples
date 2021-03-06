# CGAL_Samples

# 概要

## official

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

## Python tool

open3dのようにpythonで幾何学データ処理を行うためのツールとして提供されている。

ベースにCGALのライブラリを使用している。

現状ではまだまだ開発途中の模様。

# 環境構築

## CGAL-5.0

setup版をインストール https://github.com/CGAL/cgal/releases/download/releases%2FCGAL-5.0/CGAL-5.0-Setup.exe

インストーラー起動するので、ライブラリと追加で必要なライブラリにチェック入れてインストール。

パスはデフォルトのC:/dev/CGAL-5.0で通した。環境変数も自動設定にした。

### install ceres

https://doc.cgal.org/latest/Manual/thirdparty.html

In CGAL, Ceres is used by the [Polygon Mesh Processing Reference](https://doc.cgal.org/latest/Polygon_mesh_processing/group__PkgPolygonMeshProcessingRef.html) package for mesh smoothing, which requires solving complex non-linear least squares problems.

install steps

ref

- [Installation — Ceres Solver](http://ceres-solver.org/installation.html)
- https://github.com/ceres-solver/ceres-solver/issues/371

1. download stable package
2. extract downloaded file anywhere
3. move to "ceres-solver-1.14.0"
4. make directory "ceres-bin" and move there
5. In command prompt, input below command
   1. if you use visual studio : `cmake .. -G"Visual Studio 15 2017 Win64" -DEIGEN3_INCLUDE_DIR="C:/eigen-3.3.7" -DEIGEN_INCLUDE_DIR_HINTS="C:/eigen-3.3.7" -DMINIGLOG=ON -DCMAKE_CXX_FLAGS="/utf-8 /EHsc"`
   2. other compiler(I don't do this) : `cmake .. -G"Other compiler name" -DEIGEN3_INCLUDE_DIR="C:/eigen-3.3.7" -DEIGEN_INCLUDE_DIR_HINTS="C:/eigen-3.3.7" -DMINIGLOG=ON`
6. build by release mode `cmake --build . --config release`
7. install liblary `cmake --build . --target install --config release` or Use Visual Studio by building INSTALL project

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

# Using with cmake

- [How to use CGAL with CMake or your own build system · CGAL/cgal Wiki](https://github.com/CGAL/cgal/wiki/How-to-use-CGAL-with-CMake-or-your-own-build-system)

Qtのビューワーを使う場合と使わない場合で使うコンポーネントが異なる。

## without Qt viewer

最小限のサンプル

```cmake
cmake_minimum_required( VERSION 3.6 )

project(sample_proj CXX)

find_package(CGAL)
set( EIGEN3_INCLUDE_DIR C:/eigen-3.3.7)
find_package(Eigen3) # you have to find Eigen3 after find CGAL(if you find Eigen3 before CGAL, Eigen is not installed correctly)

if(CGAL_FOUND AND EIGEN3_FOUND)
  include( ${EIGEN3_USE_FILE} )
else()
  message(FATAL_ERROR "ERROR: this program requires CGAL and Eigen and will not be compiled.")
endif()

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
set( EIGEN3_INCLUDE_DIR C:/eigen-3.3.7)
find_package(Eigen3) # you have to find Eigen3 after find CGAL(if you find Eigen3 before CGAL, Eigen is not installed correctly)

if(CGAL_FOUND AND EIGEN3_FOUND)
  include( ${EIGEN3_USE_FILE} )
else()
  message(FATAL_ERROR "ERROR: this program requires CGAL and Eigen and will not be compiled.")
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

## with Qt viewer

最小サンプル

```cmake
cmake_minimum_required( VERSION 3.6 )

project(sample_proj CXX)

find_package(CGAL REQUIRED COMPONENTS Qt5 Core)
set( EIGEN3_INCLUDE_DIR C:/eigen-3.3.7)
find_package(Eigen3) # you have to find Eigen3 after find CGAL(if you find Eigen3 before CGAL, Eigen is not installed correctly)

if(CGAL_FOUND AND EIGEN3_FOUND)
  include( ${EIGEN3_USE_FILE} )
else()
  message(FATAL_ERROR "ERROR: this program requires CGAL and Eigen and will not be compiled.")
endif()

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
set( EIGEN3_INCLUDE_DIR C:/eigen-3.3.7)
find_package(Eigen3) # you have to find Eigen3 after find CGAL(if you find Eigen3 before CGAL, Eigen is not installed correctly)

if(CGAL_FOUND AND EIGEN3_FOUND)
  include( ${EIGEN3_USE_FILE} )
else()
  message(FATAL_ERROR "ERROR: this program requires CGAL and Eigen and will not be compiled.")
endif()

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

## with Ceres

Ceres need when use smoothing library in Polygon Mesh Processing

- [Using Ceres with CMake](http://ceres-solver.org/installation.html#using-ceres-with-cmake)
- [How to link Ceres to CGAL to use Smoothing functions? - Stack Overflow](https://stackoverflow.com/questions/58830516/how-to-link-ceres-to-cgal-to-use-smoothing-functions)
- [勝手に作るCMake入門 その4 外部ライブラリを利用する - かみのメモ](https://kamino.hatenablog.com/entry/cmake_tutorial4#sec8_2)

add below command in CMakeLists.txt

```cmake
find_package(Ceres REQUIRED)
include_directories(${CERES_INCLUDE_DIRS})
target_compile_definitions(${PROJECT_NAME} PRIVATE CGAL_PMP_USE_CERES_SOLVER)

target_link_libraries(${PROJECT_NAME} 
   CGAL::CGAL
   ceres
)
```

## with vcglib

以下を追加するだけでよい

```cmake
target_include_directories(${PROJECT_NAME}
  PUBLIC
    C:/Users/nitta/Documents/GitLab/vcglib
)
```



# Manual

## Specializations of boost::graph_traits

[CGAL 5.0.1 - CGAL and the Boost Graph Library: Specializations of boost::graph_traits](https://doc.cgal.org/latest/BGL/group__PkgBGLTraits.html)

- vertex_descriptorとかの一覧表
- descriptor : pointerのようなもの

e.g.

```cpp
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>
typedef CGAL::Exact_predicates_inexact_constructions_kernel     Kernel;
typedef CGAL::Surface_mesh<Kernel::Point_3>               CGALMesh;

// boost使用
typedef boost::graph_traits<CGALMesh>::vertex_descriptor   vertex_descriptor;
typedef boost::graph_traits<CGALMesh>::vertex_iterator     vertex_iterator;
typedef boost::graph_traits<CGALMesh>::halfedge_descriptor halfedge_descriptor;
typedef boost::graph_traits<CGALMesh>::edge_descriptor     edge_descriptor;

// boost使わない版
typedef CGALMesh::Vertex_index        vertex_descriptor;
typedef CGALMesh::Vertex_iterator     vertex_iterator;
typedef CGALMesh::Halfedge_index      halfedge_descriptor;
typedef CGALMesh::Edge_index          edge_descriptor;
```

## typedef Kernel

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;の方が簡易版らしい。

具体的な違いはよく分からない

```cpp
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

#include <CGAL/Simple_cartesian.h>
typedef CGAL::Simple_cartesian<double> K;
```

---

```cpp
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polyhedron_3.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef CGAL::Polyhedron_3<Kernel> Polyhedron;
```

[CGAL 5.0.1 - 2D and 3D Linear Geometry Kernel: CGAL::Exact_predicates_inexact_constructions_kernel Class Reference](https://doc.cgal.org/latest/Kernel_23/classCGAL_1_1Exact__predicates__inexact__constructions__kernel.html)

- It uses Cartesian representation.
- It supports constructions of points from `double` Cartesian coordinates.
- It provides exact geometric predicates, but inexact geometric constructions.

内部でSimple_cartesian<double>を呼び出している。

```cpp
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh.h>

typedef CGAL::Simple_cartesian<double> Kernel;
typedef CGAL::Surface_mesh<Kernel::Point_3> CGALMesh;
```

[CGAL 5.0.1 - 2D and 3D Linear Geometry Kernel: CGAL::Simple_cartesian< FieldNumberType > Struct Template Reference](https://doc.cgal.org/latest/Kernel_23/structCGAL_1_1Simple__cartesian.html)

Simple_cartesianにはdouble以外にも[色々な拡張型](https://doc.cgal.org/latest/Algebraic_foundations/classFieldNumberType.html)が指定可能。doubleなどの組み込み型の場合は近似計算になる。



## Surface Mesh

Reference

- [CGAL 5.0.1 - Surface Mesh: User Manual](https://doc.cgal.org/latest/Surface_mesh/index.html)
- [CGAL 5.0.1 - Surface Mesh: CGAL::Surface_mesh< P > Class Template Reference](https://doc.cgal.org/latest/Surface_mesh/classCGAL_1_1Surface__mesh.html#aea5cd1027cca955472f6c7cb6fc6ffcb)

HalfedgeDS,Polyhedron_3の代わりに使える。

- MutableFaceGraphとFaceListGraphのコンセプトを持っているので、この条件を満たすライブラリで使用できる

違いは

- ポインタでなくインデックスベースでのアクセス
- 要素の追加メカニズムがシンプル、コンパイル時でなく実行時に行われる
- 要素の削除はフラグ方式であり、メモリ上から削除するためにはガベージコレクションを使う
- empty関数の仕様なども違うため、互換性があるわけではない

データ型 [CGAL 5.0.1 - CGAL and the Boost Graph Library: Specializations of boost::graph_traits](https://doc.cgal.org/latest/BGL/group__PkgBGLTraits.html)

- `Surface_mesh::Vertex_index`
- `Surface_mesh::Halfedge_index`
- `Surface_mesh::Face_index`
- `Surface_mesh::Edge_index`

### データ入出力と要素巡回

OFFファイル

```cpp
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

    std::ofstream out("out.off");
    out.precision(17);
    out << mesh << std::endl;
    return 0;
}
```

```
C:\Users\nitta\Documents\GitHub\CGAL_Samples\bin>test040_surface_mesh_iter.exe .\data\tetrahedron.off
Halfedge_index: h0,h1,h2,h3,h4,h5,h6,h7,h8,h9,h10,h11,
Edge_index: e0 on h0,e1 on h2,e2 on h4,e3 on h6,e4 on h8,e5 on h10,
vertex_index: v0,v1,v2,v3,
face_index: f0,f1,f2,f3,
Halfedge_index: 0,1,2,3,4,5,6,7,8,9,10,11,
Edge_index: 0,1,2,3,4,5,
vertex_index: 0,1,2,3,
face_index: 0,1,2,3,
```

### 頂点と面の追加

```cpp
typedef Surface_mesh<Point> Mesh;
Mesh m;
Mesh::Vertex_index u = m.add_vertex(Point(0,1,0));
Mesh::Vertex_index v = m.add_vertex(Point(0,0,0));
Mesh::Vertex_index w = m.add_vertex(Point(1,0,0));
m.add_face(u, v, w);
```

面を追加したときのチェック方法

- メッシュの構造が崩れるような場合は面の追加が失敗する
- 下記の例は辺を共有する面で表裏が逆になる場合(ハーフエッジの向きが同じ方向だと不可)

```cpp
/*
// カーネルはこっち使っても同じ
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
*/
#include <CGAL/Simple_cartesian.h>
typedef CGAL::Simple_cartesian<double> K;

#include <CGAL/Surface_mesh.h>
typedef CGAL::Surface_mesh<K::Point_3> Mesh;
typedef Mesh::Vertex_index vertex_descriptor;
typedef Mesh::Face_index face_descriptor;

int main()
{
  Mesh m;
  // Add the points as vertices
  vertex_descriptor u = m.add_vertex(K::Point_3(0,1,0));
  vertex_descriptor v = m.add_vertex(K::Point_3(0,0,0));
  vertex_descriptor w = m.add_vertex(K::Point_3(1,1,0));
  vertex_descriptor x = m.add_vertex(K::Point_3(1,0,0));
  m.add_face(u,v,w);
  face_descriptor f = m.add_face(u,v,x);
  if(f == Mesh::null_face())
  {
    std::cerr<<"The face could not be added because of an orientation error."<<std::endl;
    f = m.add_face(u,x,v);
    assert(f != Mesh::null_face());
  }
  return 0;
}
```

### Connectivity

ハーフエッジは自身の属する面ひとつと頂点ふたつを持つ。

使える関数は

- Surface_mesh::opposite()
- Surface_mesh::next()
- Surface_mesh::prev()
- Surface_mesh::target()
- Surface_mesh::face()
- Surface_mesh::halfedge()

#### indexの取得

頂点、辺、面、ハーフエッジすべて.idx()でindexが直接取得できる

#### 他の種類のdescriptorを取得

ある頂点に属するハーフエッジは複数ある場合がある。

ハーフエッジなら頂点2つ、辺とインデックスを使えば一意のポインタが取得できる。

- [ハーフエッジから頂点](https://doc.cgal.org/latest/Surface_mesh/classCGAL_1_1Surface__mesh.html#a25636a372cb22645aba7175c5b5af3fc)(ハーフエッジの出発点)
  - Vertex_index source(Halfedge_index h) const

```cpp
Mesh::Vertex_index v0(0);
Mesh::Halfedge_index h = mesh.halfedge(v0); //v0に入ってくるハーフエッジの一つを返す
Mesh::Vertex_index v = mesh.source(h);
std::cout << "v: " << v.idx() << std::endl; // v: 3
```

#### 頂点座標の取得

```cpp
Mesh::Property_map<vertex_descriptor, K::Point_3> location = m.points(); // returns the property for the string "v:point".
for(vertex_descriptor vd : m.vertices())
    std::cout << location[vd] << std::endl;
```



### 境界線の取得

```cpp
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>
#include <iostream>
#include <fstream>
#include <vector>
typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef CGAL::Surface_mesh<Kernel::Point_3>                  Mesh;
typedef Mesh::Halfedge_index halfedge_descriptor;

int main(int argc, char* argv[])
{
    const char* filename = (argc > 1) ? argv[1] : "in.off";
    std::ifstream input(filename);
    Mesh mesh;
    if ( !input || !(input >> mesh) || mesh.is_empty() ) {
        std::cerr << "Not a valid off file." << std::endl;
        return 1;
    }

    // add property_map
    Mesh::Property_map<halfedge_descriptor,bool> bVisit; // プロパティマップ用の変数定義
    bool created;
    boost::tie(bVisit, created) = mesh.add_property_map<halfedge_descriptor,bool>("h:visit",false); // 文字列キーと初期値指定
    assert(created);

    for(halfedge_descriptor h : halfedges(mesh)){
        if(bVisit[h])
            continue;
        bVisit[h] = true;
        if(mesh.is_border(h)){
            std::cout << "Halfedge_index: ";
            std::cout << h << ',';
            halfedge_descriptor hn = mesh.next(h);
            while(h!=hn){
                std::cout << hn << ',';
                bVisit[hn] = true;
                hn = mesh.next(hn);
            }
            std::cout << std::endl;
        }
    }
    return 0;
}

```



### [Property map](https://doc.cgal.org/latest/Surface_mesh/index.html#title7)

Ref : https://doc.cgal.org/latest/Surface_mesh/classCGAL_1_1Surface__mesh.html#a9d40ac48b19106cd3f59639b036471b1

頂点、辺、ハーフエッジ、面の要素に任意の型を割り当てることが可能。

デフォルト設定で

- 頂点には座標値が"v:point"の文字列をキーとして割り当てられている
  - points()関数を使うと、キーを指定する方法でなくてもプロパティマップが取得できる
- 頂点、辺、面にそれぞれ接続情報と削除フラグが文字列キーで割り当てられている
  - "v:connectivity", "h:connectivity", and "f:connectivity"
  - "v:removed", "e:removed", and "f:removed"

#### プロパティ一覧取得

```cpp
std::vector<std::string> props = m.properties<vertex_descriptor>();
for(std::string p : props){
    std::cout << p << std::endl;
}
```

#### プロパティ追加

```cpp
Mesh::Property_map<vertex_descriptor,std::string> name; // プロパティマップ用の変数定義
bool created;
boost::tie(name, created) = m.add_property_map<vertex_descriptor,std::string>("v:name",""); // 文字列キーと初期値指定
assert(created);
```

#### サンプル

```cpp
#include <string>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh.h>
typedef CGAL::Simple_cartesian<double> K;
typedef CGAL::Surface_mesh<K::Point_3> Mesh;
typedef Mesh::Vertex_index vertex_descriptor;
typedef Mesh::Face_index face_descriptor;
int main()
{
  Mesh m;
  vertex_descriptor v0 = m.add_vertex(K::Point_3(0,2,0));
  vertex_descriptor v1 = m.add_vertex(K::Point_3(2,2,0));
  vertex_descriptor v2 = m.add_vertex(K::Point_3(0,0,0));
  vertex_descriptor v3 = m.add_vertex(K::Point_3(2,0,0));
  vertex_descriptor v4 = m.add_vertex(K::Point_3(1,1,0));
  m.add_face(v3, v1, v4);
  m.add_face(v0, v4, v1);
  m.add_face(v0, v2, v4);
  m.add_face(v2, v3, v4);
  // give each vertex a name, the default is empty
  Mesh::Property_map<vertex_descriptor,std::string> name; // プロパティマップ用の変数定義
  bool created;
  boost::tie(name, created) = m.add_property_map<vertex_descriptor,std::string>("v:name",""); // 文字列キーと初期値指定
  assert(created);
  // add some names to the vertices
  name[v0] = "hello";
  name[v2] = "world";
  {
    // You get an existing property, and created will be false 同じプロパティ名での追加は不可
    Mesh::Property_map<vertex_descriptor,std::string> name;
    bool created;
    boost::tie(name, created) = m.add_property_map<vertex_descriptor,std::string>("v:name", "");
    assert(! created);
  }
  //  You can't get a property that does not exist 存在しないプロパティは取得できない
  Mesh::Property_map<face_descriptor,std::string> gnus;
  bool found;
  boost::tie(gnus, found) = m.property_map<face_descriptor,std::string>("v:gnus");
  assert(! found);
  // retrieve the point property for which exists a convenience function
  Mesh::Property_map<vertex_descriptor, K::Point_3> location = m.points(); // returns the property for the string "v:point".
  for(vertex_descriptor vd : m.vertices()) { 
    std::cout << name[vd] << " @ " << location[vd] << std::endl;
  }
  std::vector<std::string> props = m.properties<vertex_descriptor>();
  for(std::string p : props){
    std::cout << p << std::endl;
  }
  
  // delete the string property again
  m.remove_property_map(name);
  return 0;
}
```





## 3D Polyhedral Surface

Reference

- [CGAL 5.0.1 - 3D Polyhedral Surface: User Manual](https://doc.cgal.org/latest/Polyhedron/index.html)

Polyhedral : 頂点、ファセット、エッジ、それらの入射関係で構成。ハーフエッジデータ構造に基づく。2-manifoldに限定。

polyhedron : Polyhedralで境界線がない閉じたもの



## Polygon Mesh Processing

[CGAL 5.0 - Polygon Mesh Processing: User Manual](https://doc.cgal.org/latest/Polygon_mesh_processing/index.html)

- [この本](https://www.amazon.co.jp/Polygon-Mesh-Processing-Mario-Botsch/dp/1568814267)の内容をベースに作ってある。
- Polygon Meshは面と辺と頂点で構成される一般的なもの。自己交差含む。2-manifoldに限る。
- Polyhedron_3やSurface_meshなどのFaceGraphのコンセプトに基づくものなら使える
- 

# Sample

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

## Edge Collapse

[CGAL 5.0.1 - Triangulated Surface Mesh Simplification: User Manual](https://doc.cgal.org/latest/Surface_mesh_simplification/index.html)

エッジを1本消して両端点を統合することによりエッジに接触している面を消去できる。

## Triangulated Surface Mesh Segmentation

- [CGAL 5.0.1 - Triangulated Surface Mesh Segmentation: User Manual](https://doc.cgal.org/latest/Surface_mesh_segmentation/index.html)

各ファセットに対してSDF値を計算し、クラスタリング・分割を行う。

## Meshing

### Refine and Fair a Region on a Triangle Mesh

https://doc.cgal.org/latest/Polygon_mesh_processing/index.html#MeshingExample_1

refine:形状はそのままでmeshの再構成?をする。変換前後の違いはよく分からない

fair:linear bi-Laplacianで形状自体を修正する

### Triangulate a Polygon Mesh

https://doc.cgal.org/latest/Polygon_mesh_processing/index.html#MeshingExample_2

任意の多面体を三角形の面に分割する

### Isotropic Remeshing of a Region on a Polygon Mesh

https://doc.cgal.org/latest/Polygon_mesh_processing/index.html#RemeshingExample_1

不均一な三角形を正三角形になるように分割する。基本的にメッシュは細かくなる

## [Hole filling](https://doc.cgal.org/latest/Polygon_mesh_processing/index.html#title23)

## Predicates

### Self Intersections

ポリゴンが別のポリゴンにめり込んでる状態を判定する

```cpp
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Polygon_mesh_processing/self_intersections.h>
#include <fstream>
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Surface_mesh<K::Point_3>             Mesh;
typedef boost::graph_traits<Mesh>::face_descriptor face_descriptor;
namespace PMP = CGAL::Polygon_mesh_processing;
int main(int argc, char* argv[])
{
  const char* filename = (argc > 1) ? argv[1] : "data/pig.off";
  std::ifstream input(filename);
  Mesh mesh;
  if (!input || !(input >> mesh) || !CGAL::is_triangle_mesh(mesh))
  {
    std::cerr << "Not a valid input file." << std::endl;
    return 1;
  }
  bool intersecting = PMP::does_self_intersect(mesh,
      PMP::parameters::vertex_point_map(get(CGAL::vertex_point, mesh)));
  std::cout
    << (intersecting ? "There are self-intersections." : "There is no self-intersection.")
    << std::endl;
  std::vector<std::pair<face_descriptor, face_descriptor> > intersected_tris;
  PMP::self_intersections(mesh, std::back_inserter(intersected_tris));
  std::cout << intersected_tris.size() << " pairs of triangles intersect." << std::endl;
  
  return 0;
}
```

hole fillingの関数は4種類

- `triangulate_hole_polyline()` : given a sequence of points defining the hole, triangulates the hole.
  - 頂点のvector入れて、Tripleのvectorが返される
- `triangulate_hole()` : given a border halfedge on the boundary of the hole on a mesh, triangulates the hole.
  - 既存の頂点をつないで新しい面を生成する。生成された面の情報返す
- `triangulate_and_refine_hole()` : in addition to `triangulate_hole()` the generated patch is refined.
  - 上の処理に加えて、メッシュ分割を行う。生成された面と頂点の情報を返す
- `triangulate_refine_and_fair_hole()` : in addition to `triangulate_and_refine_hole()` the generated patch is also faired.
  - 上の処理に加えて、メッシュ形状の最適化を行う。生成された面と頂点と最適化が成功したかの結果を返す

### triangulate_hole_polyline

```cpp
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_mesh_processing/triangulate_hole.h>
#include <CGAL/utility.h>
#include <vector>
#include <iterator>
typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::Point_3 Point;
int main()
{
  std::vector<Point> polyline;
  polyline.push_back(Point( 1.,0.,0.));
  polyline.push_back(Point( 0.,1.,0.));
  polyline.push_back(Point(-1.,0.,0.));
  polyline.push_back(Point( 1.,1.,0.));
  // repeating first point (i.e. polyline.push_back(Point(1.,0.,0.)) ) is optional (1周してもしなくてもよい)
  // any type, having Type(int, int, int) constructor available, can be used to hold output triangles
  typedef CGAL::Triple<int, int, int> Triangle_int;
  std::vector<Triangle_int> patch;
  patch.reserve(polyline.size() -2); // there will be exactly n-2 triangles in the patch
  CGAL::Polygon_mesh_processing::triangulate_hole_polyline(
          polyline,
          std::back_inserter(patch));
  for(std::size_t i = 0; i < patch.size(); ++i)
  {
    std::cout << "Triangle " << i << ": "
      << patch[i].first << " " << patch[i].second << " " << patch[i].third
      << std::endl;
  }
  return 0;
}
```





## ブーリアン演算

- [CGAL 5.0 - Polygon Mesh Processing: User Manual](https://doc.cgal.org/latest/Polygon_mesh_processing/index.html#coref_bolop_subsec)
- [3D Boolean Operations on Nef Polyhedra](https://doc.cgal.org/latest/Manual/packages.html#PkgNef3)
- [表面メッシュのブーリアン演算(CGALライブラリ) - Mesh Wiki](https://www.rccm.co.jp/icem/pukiwiki/index.php?%E8%A1%A8%E9%9D%A2%E3%83%A1%E3%83%83%E3%82%B7%E3%83%A5%E3%81%AE%E3%83%96%E3%83%BC%E3%83%AA%E3%82%A2%E3%83%B3%E6%BC%94%E7%AE%97%28CGAL%E3%83%A9%E3%82%A4%E3%83%96%E3%83%A9%E3%83%AA%29)

Polygon_mesh_processingを使う方法とNef Polyhedraパッケージを使う方法がある。

Nefの方が高機能

- 演算子が定義されている(+,-)
- Manifoldでないデータも扱えるらしい(辺が共有されてるパターン)。

### Polygon Mesh Processing : Union

```cpp
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Polygon_mesh_processing/corefinement.h>
#include <fstream>
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Surface_mesh<K::Point_3>             Mesh;
namespace PMP = CGAL::Polygon_mesh_processing;
int main(int argc, char* argv[])
{
  const char* filename1 = (argc > 1) ? argv[1] : "data/blobby.off";
  const char* filename2 = (argc > 2) ? argv[2] : "data/eight.off";
  std::ifstream input(filename1);
  Mesh mesh1, mesh2;
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
  Mesh out;
  bool valid_union = PMP::corefine_and_compute_union(mesh1,mesh2, out);
  if (valid_union)
  {
    std::cout << "Union was successfully computed\n";
    std::ofstream output("union.off");
    output << out;
    return 0;
  }
  std::cout << "Union could not be computed\n";
  return 1;
}
```

### Polygon Mesh Processing : Difference and Remeshing

```cpp
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
  Mesh mesh1, mesh2;
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
  //create a property on edges to indicate whether they are constrained
  Mesh::Property_map<edge_descriptor,bool> is_constrained_map = mesh1.add_property_map<edge_descriptor,bool>("e:is_constrained", false).first;
  // update mesh1 to contain the mesh bounding the difference of the two input volumes.
  bool valid_difference =
    PMP::corefine_and_compute_difference(mesh1,
                                         mesh2,
                                         mesh1,
                                         params::all_default(), // default parameters for mesh1
                                         params::all_default(), // default parameters for mesh2
                                         params::edge_is_constrained_map(is_constrained_map));
  if (valid_difference)
  {
    std::cout << "Difference was successfully computed\n";
    std::ofstream output("difference.off");
    output << mesh1;
  }
  else{
    std::cout << "Difference could not be computed\n";
    return 1;
  }
  
  // collect faces incident to a constrained edge
  std::vector<face_descriptor> selected_faces;
  std::vector<bool> is_selected(num_faces(mesh1), false);
  for(edge_descriptor e : edges(mesh1))
    if (is_constrained_map[e])
    {
      // insert all faces incident to the target vertex
      for(halfedge_descriptor h :
                    halfedges_around_target(halfedge(e,mesh1),mesh1))
      {
        if (!is_border(h, mesh1) )
        {
          face_descriptor f=face(h, mesh1);
          if ( !is_selected[f] )
          {
            selected_faces.push_back(f);
            is_selected[f]=true;
          }
        }
      }
    }
  // increase the face selection
  CGAL::expand_face_selection(selected_faces, mesh1, 2,
    Vector_pmap_wrapper(is_selected), std::back_inserter(selected_faces));
  std::cout << selected_faces.size()
            << " faces were selected for the remeshing step\n";
  // remesh the region around the intersection polylines
  PMP::isotropic_remeshing(
    selected_faces,
    0.02,
    mesh1,
    params::edge_is_constrained_map(is_constrained_map) );
  std::ofstream output("difference_remeshed.off");
  output << mesh1;
  return 0;
}
```

### Nef Polyhedra : Difference

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

## 体積の計算

- [CGAL 5.0 - Polygon Mesh Processing: Geometric Measure Functions](https://doc.cgal.org/latest/Polygon_mesh_processing/group__measure__grp.html#ga85cebf8fbc7cb8930fd16aeee2878c7e)
- [How to calculate volume of a polyhedron/mesh? · Issue #2620 · CGAL/cgal](https://github.com/CGAL/cgal/issues/2620)

```cpp
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
```

## Hausdorff距離

- [CGAL 5.0 - Polygon Mesh Processing: Distance Functions](https://doc.cgal.org/latest/Polygon_mesh_processing/group__PMP__distance__grp.html#ga2b28867dc150931b2f12d17b77d15266)
- [CGAL 5.0 - Polygon Mesh Processing: Polygon_mesh_processing/hausdorff_distance_remeshing_example.cpp](https://doc.cgal.org/latest/Polygon_mesh_processing/Polygon_mesh_processing_2hausdorff_distance_remeshing_example_8cpp-example.html#a4)

tm1のサンプリング頂点数を決めて、tm2の各頂点との距離を計算する。

tm2でもサンプリング頂点数を決めることが可能。

ただし、vcglibとは異なり最大距離しか計算できない。

```cpp
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Polygon_mesh_processing/distance.h>
#include <CGAL/Polygon_mesh_processing/remesh.h>
#if defined(CGAL_LINKED_WITH_TBB) // if you installed and linked TBB, you can use parallel calculation
#define TAG CGAL::Parallel_tag
#else
#define TAG CGAL::Sequential_tag // default
#endif
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_3                     Point;
typedef CGAL::Surface_mesh<K::Point_3> Mesh;
namespace PMP = CGAL::Polygon_mesh_processing;
int main()
{
  Mesh tm1, tm2;
  CGAL::make_tetrahedron(Point(.0,.0,.0),
                         Point(2,.0,.0),
                         Point(1,1,1),
                         Point(1,.0,2),
                         tm1);
  tm2=tm1;
  CGAL::Polygon_mesh_processing::isotropic_remeshing(tm2.faces(),.05, tm2);
  std::cout << "Approximated Hausdorff distance: "
            << CGAL::Polygon_mesh_processing::approximate_Hausdorff_distance
                  <TAG>(tm1, tm2, PMP::parameters::number_of_points_per_area_unit(4000))
            << std::endl;
}
```

Polyhedronのデータ形式でも計算可能

```cpp
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
```



## CGAL with OpenMesh

OpenMeshのデータは割と簡単に連携できるらしい

- [CGAL 5.0 - Triangulated Surface Mesh Simplification: Surface_mesh_simplification/edge_collapse_OpenMesh.cpp](https://doc.cgal.org/latest/Surface_mesh_simplification/Surface_mesh_simplification_2edge_collapse_OpenMesh_8cpp-example.html)

## その他の計算 Polygon Mesh Processing

[CGAL 5.0 - Polygon Mesh Processing: User Manual](https://doc.cgal.org/latest/Polygon_mesh_processing/index.html)

- Boolean operation
- Clipping
- Intersections Detection
- Hole filling
- Remeshing
- Smoothing
- Polygon soup
- Slicer
- Hausdorff distance



## Triangulated Surface Mesh Deformation

- Reference : [CGAL 5.0.1 - Triangulated Surface Mesh Deformation: CGAL::Surface_mesh_deformation< TM, VIM, HIM, TAG, WC, ST, CR, VPM > Class Template Reference](https://doc.cgal.org/latest/Surface_mesh_deformation/classCGAL_1_1Surface__mesh__deformation.html)

3種類の手法が実装されている

- [The *As-Rigid-As-Possible* (*ARAP*) method](https://igl.ethz.ch/projects/ARAP/arap_web.pdf)
  - メッシュが鈍角の場合は使えない(鋭角に分割すれば使える)
- The *Spokes and Rims* method
  - メッシュが鈍角でも使える
  - ARAPに比べると歪みが発生しやすい
- The *Smoothed Rotation Enhanced As-Rigid-As-Possible* method

### How to use





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

## CGAL and Boost Property Maps

- [CGAL 5.0.1 - CGAL and Boost Property Maps: User Manual](CGAL 5.0.1 - CGAL and Boost Property Maps: User Manual)

CGALは頂点やfacetを操作するときにfor分でマニュアル的に対対処せずイテレータとメッシュを関数



CGALのライブラリはconceptとmodelに分かれている

# 数学の基礎知識

CGALは有限体や環の概念がベースになっている。これをC++のSTLで実現している。

[代数学 - [物理のかぎしっぽ]](http://hooktail.org/misc/index.php?%C2%E5%BF%F4%B3%D8)

# CGAL Ipelets

数学的な作図ソフトらしい

[CGAL 5.0 - CGAL Ipelets: User Manual](https://doc.cgal.org/latest/CGAL_ipelets/index.html)

[Ipe のインストール - さてもちブログ](http://satemochi.blog.fc2.com/blog-entry-126.html)