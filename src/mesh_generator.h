#ifndef MESH_GENERATOR_H
#define MESH_GENERATOR_H

#include <vector>
#include <string>
#include "geometry.h"

using namespace std;

// Формат входного файла
// x_min x_max          // Минимальная и максимальная координата по оси X
// y_min y_max          // Минимальная и максимальная координата по оси Y
// z_min z_max          // Минимальная и максимальная координата по оси Z
// num_x num_y num_z    // Число отрезков по каждой из осей
// vol_phys vol_geom    // Физическая и геометрическая области для объема       (опционально)
// s_phys_l s_geom_l    // Физическая и геометрическая области левой грани      (опционально)
// s_phys_r s_geom_r    // Физическая и геометрическая области правой грани     (опционально)
// s_phys_f s_geom_f    // Физическая и геометрическая области передней грани   (опционально)
// s_phys_b s_geom_b    // Физическая и геометрическая области задней грани     (опционально)
// s_phys_d s_geom_d    // Физическая и геометрическая области нижней грани     (опционально)
// s_phys_u s_geom_u    // Физическая и геометрическая области верхней грани    (опционально)

// Класс генератора сеток
class mesh_generator
{
public:
    // Сгенерировать тетраэдральную сетку из файла
    void triangulate(const string & filename);
    // Вывод сетки в файл в формате gmsh
    void out_gmsh(const string & filename);
    // Построить вложенную сетку
    void split();

protected:
    // Сгенерировать базовую параллелепипедальную сетку
    void generate(const string & filename);
    // Разбить параллелепипедальную сетку на тетраэдры
    void triangulate();
    // Вектор из узлов сетки
    vector<point> nodes;
    // Вектор из параллелепипедов
    vector<cube> cubes;
    // Вектор из тетраэдров
    vector<tetrahedron> tets;
    // Вектор из треугольников
    vector<triangle> trs[6];

    // Физические области
    size_t phys_surf[6], phys_vol;
    // Геометрические области
    size_t geom_surf[6], geom_vol;
};

#endif // MESH_GENERATOR_H
