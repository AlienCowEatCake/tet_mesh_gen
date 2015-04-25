#include "mesh_generator.h"

int main()
{
    mesh_generator ar;
    ar.triangulate("area.txt");
    ar.out_gmsh("mesh.msh");
    ar.split();
    ar.out_gmsh("mesh2.msh");
    ar.split();
    ar.out_gmsh("mesh4.msh");
#if defined _WIN32
    system("pause");
#endif
    return 0;
}

