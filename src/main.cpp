#include "mesh_generator.h"

int main()
{
    mesh_generator ar;
    ar.triangulate("area.txt");
    ar.out_gmsh("cube_x1.msh");
    ar.split();
    ar.out_gmsh("cube_x2.msh");
    ar.split();
    ar.out_gmsh("cube_x4.msh");
#if defined _WIN32
    system("pause");
#endif
    return 0;
}

