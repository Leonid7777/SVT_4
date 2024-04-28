#include "inmost.h"
#include <stdio.h>


using namespace INMOST;


void 
make_rel_cells_tag(Mesh* m)
{

	Tag tag_rel_cells = m->CreateTag("Related cells count", DATA_INTEGER, NODE, NONE, 1);

	for (Mesh::iteratorNode i = m->BeginNode(); i != m->EndNode(); i++) {
		Node node = i->getAsNode();
		node.Integer(tag_rel_cells) = node.getCells().size();
	}
	
}

void 
make_vec_tag(Mesh *m, double x, double y)
{

	Tag tag_vec_cent = m->CreateTag("Vec to center coords", DATA_REAL, CELL, NONE, 3);

	for (Mesh::iteratorCell i = m->BeginCell(); i != m->EndCell(); i++) {
		Cell cell = i->getAsCell();
		double *c = new double[3];

		cell.Barycenter(c);
		cell.RealArray(tag_vec_cent)[0] = x - c[0];
		cell.RealArray(tag_vec_cent)[1] = y - c[1];
		cell.RealArray(tag_vec_cent)[2] = 0;

	}

}

double 
dist(Node f_n, Node s_n) 
{

	double x = f_n.Coords()[0] - s_n.Coords()[0];
	double y = f_n.Coords()[1] - s_n.Coords()[1];

	return sqrt(x * x + y * y);

}

double 
diameter(Mesh* m)
{

	double max = 0;

	for (Mesh::iteratorCell i = m->BeginCell(); i != m->EndCell(); i++) {

		Cell cell = i->getAsCell();
		ElementArray<Node> nodes = cell.getNodes();

		double x = dist(nodes[0], nodes[1]);
		double y = dist(nodes[0], nodes[2]);
		double z = dist(nodes[1], nodes[2]);

		double cur_diam = std::max(std::max(x, y), z);

		if (cur_diam > max) {
			max = cur_diam;
		}
	}

	return max;

}

int main(int argc, char *argv[])
{
	if (argc != 2) {
		std::cout << "Usage: mesh <mesh.vtk>" << std::endl;
		return 1;
	}

	double x = 6.052;
	double y = 2.348;

    Mesh *m = new Mesh;
	m->Load(argv[1]);
	make_vec_tag(m, x, y);
	make_rel_cells_tag(m);
	diameter(m);
	m->Save("res.vtk");
	delete m;
	std::cout << "Success!" << std::endl;

	return 0;
}