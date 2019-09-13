/*
*/

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/draw_surface_mesh.h>
#include <fstream>

#include <CGAL/IO/PLY_reader.h>
#include <CGAL/IO/PLY_writer.h>
#include <CGAL/Polygon_mesh_processing/polygon_soup_to_polygon_mesh.h>

//#include <QFileInfo>
#include <QDebug>

#include <CGAL/draw_surface_mesh.h>

typedef CGAL::Simple_cartesian<double>                       Kernel;
typedef Kernel::Point_3                                      Point;
typedef CGAL::Surface_mesh<Point>                            Mesh;
int main(int argc, char* argv[])
{
	//	Mesh sm1;
	//	std::ifstream in1((argc>1)?argv[1]:"data/triangle.off");
	//	in1 >> sm1;
	
	
	
	
	//	QFileInfo fileinfo;
	//	std::ifstream in(fileinfo.filePath().toUtf8(), std::ios_base::binary);
	std::ifstream in("data/octahedron.ply", std::ios_base::binary);
//	std::ifstream in("data/non_manifold_same_vertex.ply", std::ios_base::binary);
//	std::ifstream in("data/non_manifold_sharing_edge.ply", std::ios_base::binary);
	
	if(!in)
		std::cerr << "Error!\n";
	
	std::vector<Kernel::Point_3> points;
	std::vector<std::vector<std::size_t> > polygons;
	std::vector<CGAL::Color> fcolors;
	std::vector<CGAL::Color> vcolors;
	
	if(true){
		if (!(CGAL::read_PLY (in, points, polygons, fcolors, vcolors))){
			qDebug("read_PLY error.");
			return NULL;
		}
		
		if (CGAL::Polygon_mesh_processing::is_polygon_soup_a_polygon_mesh (polygons)){ // check manifold
			//			SMesh *surface_mesh = new SMesh();
			Mesh *mesh = new Mesh();
			CGAL::Polygon_mesh_processing::polygon_soup_to_polygon_mesh (points, polygons, *mesh);
			qDebug("read_PLY face success.");
			
			Mesh::Face_range::iterator f;
			Mesh::Vertex_range::iterator v;
			for (f=mesh->faces().begin(); f!=mesh->faces().end(); ++f){
				if (*f!=boost::graph_traits<Mesh>::null_face()){
					Mesh::Face_index fi = *f;
					std::cout << fi << "::";
					Mesh::Halfedge_index hi = mesh->halfedge(fi);
					do{
						std::cout << mesh->point(mesh->source(hi)) << ", ";
//						add_point_in_face(, get_vertex_normal(hd));
						hi=mesh->next(hi);
					}
					while(hi!=mesh->halfedge(fi));
					std::cout << std::endl;
				}
			}
			
			for (v=mesh->vertices().begin(); v!=mesh->vertices().end(); ++v){
				Mesh::Vertex_index vi = *v;
				std::cout << vi << "::" << mesh->point(vi) << ",";
			}
			std::cout << std::endl;
			
			CGAL::draw(*mesh);
			//			if(!(vcolors.empty()))
			//				set_vcolors(surface_mesh, vcolors);
			//			if(!(fcolors.empty()))
			//				set_fcolors(surface_mesh, fcolors);
			
			//			Scene_surface_mesh_item* sm_item = new Scene_surface_mesh_item(surface_mesh);
			//			sm_item->setName(fileinfo.completeBaseName());
			//			QApplication::restoreOverrideCursor();
			//			return sm_item;
			
		}else{
			qDebug("Non manifold");
			//			Scene_polygon_soup_item* soup_item = new Scene_polygon_soup_item;
			//			soup_item->setName(fileinfo.completeBaseName());
			//			soup_item->load (points, polygons, fcolors, vcolors); // 点群とメッシュデータをそのまま保持するだけ
			//			QApplication::restoreOverrideCursor();
			//			return soup_item;
		}
	}else{ // Open point set
		qDebug("Open point set");
		//		Scene_points_with_normal_item* item;
		//		item = new Scene_points_with_normal_item();
		//		if(!item->read_ply_point_set(in)){
		//			delete item;
		//			QApplication::restoreOverrideCursor();
		//			return NULL;
		//		}
		//		item->setName(fileinfo.completeBaseName());
		//		QApplication::restoreOverrideCursor();
		//		return item;
	}
	return EXIT_SUCCESS;
}
