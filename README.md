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

## ファイルの入出力

fstreamで開いたファイルポインタをストリームでMeshオブジェクトに流し込むと読み込む(offファイルのみ対応?)

参考：https://doc.cgal.org/latest/Surface_mesh/index.html#Chapter_3D_Surface_mesh

### PLYファイルの入出力

CGAL::read_PLY()で読み込めるが、頂点、面、面の色、頂点の色がすべて別々に読み込まれる。

面と頂点の統合は別に行う必要があるが、データの状態によって処理を分ける必要がある。

- 点群のみ
- Non Manifoldのメッシュデータ
- 三角形以外のポリゴンを含むデータ
- 三角形のみのメッシュを持つmanifoldなデータ



# データアクセス方法

## 面、頂点、エッジの各要素にアクセスする方法

vcglibと同じような方法で出来る。

ストリーム演算子が全ての型に定義されているので、情報みたい場合は<<でストリームに流せばとりあえず何某かか表示される。

## イテレータ

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

CMakeLists.txt

```cmake
cmake_minimum_required( VERSION 3.6 )

# Create Project
get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId} CXX)
message(${ProjectId})

set( CGALLIB_DIR C:/CGAL-4.13.1)
find_package(CGAL REQUIRED COMPONENTS Qt5)

if(CGAL_Qt5_FOUND)
  add_definitions(-DCGAL_USE_BASIC_VIEWER -DQT_NO_KEYWORDS)
endif()

if ( CGAL_FOUND )
  include( ${CGAL_USE_FILE} )
else()
    message(STATUS "This program requires the CGAL library, and will not be compiled.")
endif()

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

set(SOURCE_FILES
   main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} 
   CGAL::CGAL
   CGAL::CGAL_Qt5
)
```

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

---

# まとめ

