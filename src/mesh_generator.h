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
};

#endif // MESH_GENERATOR_H
