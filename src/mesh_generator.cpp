#include "mesh_generator.h"
#include <iostream>
#include <fstream>
#include <map>
#include <utility>
#include <limits>

using namespace std;

// Сгенерировать тетраэдральную сетку из файла
void mesh_generator::triangulate(const string & filename)
{
    generate(filename);
    triangulate();
    cubes.clear();
}

// Сгенерировать базовую параллелепипедальную сетку
void mesh_generator::generate(const string & filename)
{
    cout << "Reading area ..." << endl;
    ifstream ifs(filename.c_str());
    if(!ifs.good())
    {
        cerr << "Error: " << filename << " is not good." << endl;
#if defined _WIN32
        system("pause");
#endif
        exit(EXIT_FAILURE);
    }
    double x_min, x_max, y_min, y_max, z_min, z_max;
    ifs >> x_min >> x_max >> y_min >> y_max >> z_min >> z_max;
    size_t x_num, y_num, z_num;
    ifs >> x_num >> y_num >> z_num;

    if(!ifs.good())
    {
        cerr << "Error: " << filename << " is not good." << endl;
#if defined _WIN32
        system("pause");
#endif
        exit(EXIT_FAILURE);
    }

    ifs >> phys_vol >> geom_vol;
    if(!ifs.good())
    {
        phys_vol = 27;
        geom_vol = 26;
    }

    for(size_t i = 0; i < 6; i++)
        ifs >> phys_surf[i] >> geom_surf[i];
    if(!ifs.good())
    {
        phys_surf[0] = 30;
        geom_surf[0] = 18;
        phys_surf[1] = 29;
        geom_surf[1] = 16;
        phys_surf[2] = 32;
        geom_surf[2] = 22;
        phys_surf[3] = 33;
        geom_surf[3] = 24;
        phys_surf[4] = 28;
        geom_surf[4] = 14;
        phys_surf[5] = 31;
        geom_surf[5] = 20;
    }
    ifs.close();

    cout << "Generating nodes ..." << endl;
    double hx = (x_max - x_min) / static_cast<double>(x_num),
           hy = (y_max - y_min) / static_cast<double>(y_num),
           hz = (z_max - z_min) / static_cast<double>(z_num);
    nodes.resize((x_num + 1) * (y_num + 1) * (z_num + 1));
    for(size_t i = 0, m = 0; i <= x_num; i++)
    {
        double x_curr = x_min + hx * static_cast<double>(i);
        for(size_t j = 0; j <= y_num; j++)
        {
            double y_curr = y_min + hy * static_cast<double>(j);
            for(size_t k = 0; k <= z_num; k++)
            {
                double z_curr = z_min + hz * static_cast<double>(k);
                nodes[m].x = x_curr;
                nodes[m].y = y_curr;
                nodes[m].z = z_curr;
                m++;
            }
        }
    }

    cout << "Generating cubes ..." << endl;
    cubes.resize(x_num * y_num * z_num);
    for(size_t i = 0 , m = 0; i < x_num; i++)
    {
        for(size_t j = 0; j < y_num; j++)
        {
            for(size_t k = 0; k < z_num; k++)
            {
                cubes[m].nodes[0] = (i * (y_num + 1) + j) * (z_num + 1) + k;
                cubes[m].nodes[1] = ((i + 1) * (y_num + 1) + j) * (z_num + 1) + k;
                cubes[m].nodes[2] = (i * (y_num + 1) + j + 1) * (z_num + 1) + k;
                cubes[m].nodes[3] = ((i + 1) * (y_num + 1) + j + 1) * (z_num + 1) + k;
                cubes[m].nodes[4] = (i * (y_num + 1) + j) * (z_num + 1) + k + 1;
                cubes[m].nodes[5] = ((i + 1) * (y_num + 1) + j) * (z_num + 1) + k + 1;
                cubes[m].nodes[6] = (i * (y_num + 1) + j + 1) * (z_num + 1) + k + 1;
                cubes[m].nodes[7] = ((i + 1) * (y_num + 1) + j + 1) * (z_num + 1) + k + 1;
                cubes[m].neighbor[0] = i > 0 ? true : false;
                cubes[m].neighbor[1] = i < x_num - 1 ? true : false;
                cubes[m].neighbor[2] = j > 0 ? true : false;
                cubes[m].neighbor[3] = j < y_num - 1 ? true : false;
                cubes[m].neighbor[4] = k > 0 ? true : false;
                cubes[m].neighbor[5] = k < z_num - 1 ? true : false;
                m++;
            }
        }
    }
}

// Разбить параллелепипедальную сетку на тетраэдры
void mesh_generator::triangulate()
{
    cout << "Generating tetrahedrons ..." << endl;
    for(size_t i = 0; i < nodes.size(); i++)
        nodes[i].num = i;

    // В 1 кубе 6 согласованных тетраэдров
    // http://files.school-collection.edu.ru/dlrstore/ee64f7d3-c90b-77da-e7bb-46ffc40c53c1/cube-6-t.html
    // 0126 - фиолетовый
    // 0146 - синий
    // 1236 - оранжевый
    // 1367 - желтый
    // 1456 - красный
    // 1567 - зеленый
    for(vector<cube>::const_iterator it = cubes.begin(); it != cubes.end(); ++it)
    {
        const cube * q = &(* it);
        tetrahedron tmp[6];

        tmp[0].nodes[0] = q->nodes[0];
        tmp[0].nodes[1] = q->nodes[1];
        tmp[0].nodes[2] = q->nodes[2];
        tmp[0].nodes[3] = q->nodes[6];

        tmp[1].nodes[0] = q->nodes[0];
        tmp[1].nodes[1] = q->nodes[1];
        tmp[1].nodes[2] = q->nodes[4];
        tmp[1].nodes[3] = q->nodes[6];

        tmp[2].nodes[0] = q->nodes[1];
        tmp[2].nodes[1] = q->nodes[2];
        tmp[2].nodes[2] = q->nodes[3];
        tmp[2].nodes[3] = q->nodes[6];

        tmp[3].nodes[0] = q->nodes[1];
        tmp[3].nodes[1] = q->nodes[3];
        tmp[3].nodes[2] = q->nodes[6];
        tmp[3].nodes[3] = q->nodes[7];

        tmp[4].nodes[0] = q->nodes[1];
        tmp[4].nodes[1] = q->nodes[4];
        tmp[4].nodes[2] = q->nodes[5];
        tmp[4].nodes[3] = q->nodes[6];

        tmp[5].nodes[0] = q->nodes[1];
        tmp[5].nodes[1] = q->nodes[5];
        tmp[5].nodes[2] = q->nodes[6];
        tmp[5].nodes[3] = q->nodes[7];

        for(size_t i = 0; i < 6; i++)
            tets.push_back(tmp[i]);

        if(!q->neighbor[0])
        {
            triangle tr;
            // Синий слева
            tr.nodes[0] = q->nodes[0];
            tr.nodes[1] = q->nodes[4];
            tr.nodes[2] = q->nodes[6];
            trs[0].push_back(tr);
            // Фиолетовый слева
            tr.nodes[1] = q->nodes[2];
            trs[0].push_back(tr);
        }

        if(!q->neighbor[1])
        {
            triangle tr;
            // Желтый справа
            tr.nodes[0] = q->nodes[1];
            tr.nodes[1] = q->nodes[3];
            tr.nodes[2] = q->nodes[7];
            trs[1].push_back(tr);
            // Зеленый справа
            tr.nodes[1] = q->nodes[5];
            trs[1].push_back(tr);
        }

        if(!q->neighbor[2])
        {
            triangle tr;
            // Синий спереди
            tr.nodes[0] = q->nodes[0];
            tr.nodes[1] = q->nodes[1];
            tr.nodes[2] = q->nodes[4];
            trs[2].push_back(tr);
            // Красный спереди
            tr.nodes[0] = q->nodes[1];
            tr.nodes[1] = q->nodes[4];
            tr.nodes[2] = q->nodes[5];
            trs[2].push_back(tr);
        }

        if(!q->neighbor[3])
        {
            triangle tr;
            // Оранжевый сзади
            tr.nodes[0] = q->nodes[2];
            tr.nodes[1] = q->nodes[3];
            tr.nodes[2] = q->nodes[6];
            trs[3].push_back(tr);
            // Желтый сзади
            tr.nodes[0] = q->nodes[3];
            tr.nodes[1] = q->nodes[6];
            tr.nodes[2] = q->nodes[7];
            trs[3].push_back(tr);
        }

        if(!q->neighbor[4])
        {
            triangle tr;
            // Фиолетоый снизу
            tr.nodes[0] = q->nodes[0];
            tr.nodes[1] = q->nodes[1];
            tr.nodes[2] = q->nodes[2];
            trs[4].push_back(tr);
            // Оранжевый снизу
            tr.nodes[0] = q->nodes[1];
            tr.nodes[1] = q->nodes[2];
            tr.nodes[2] = q->nodes[3];
            trs[4].push_back(tr);
        }

        if(!q->neighbor[5])
        {
            triangle tr;
            // Красный сверху
            tr.nodes[0] = q->nodes[4];
            tr.nodes[1] = q->nodes[5];
            tr.nodes[2] = q->nodes[6];
            trs[5].push_back(tr);
            // Зеленый сверху
            tr.nodes[0] = q->nodes[5];
            tr.nodes[1] = q->nodes[6];
            tr.nodes[2] = q->nodes[7];
            trs[5].push_back(tr);
        }
    }
}

// Вывод сетки в файл в формате gmsh
void mesh_generator::out_gmsh(const string & filename)
{
    const char newl = '\n';
    cout << "Writing mesh ..." << newl;
    ofstream ofs(filename.c_str());
    ofs << "$MeshFormat" << newl
        << "2.2 0 8" << newl
        << "$EndMeshFormat" << newl
        << "$Nodes" << newl;
    ofs << nodes.size() << newl;
    for(size_t i = 0; i < nodes.size(); i++)
        ofs << i + 1 << " " << nodes[i].x << " " << nodes[i].y << " " << nodes[i].z << newl;
    ofs << "$EndNodes" << newl
        << "$Elements" << newl;
    size_t num = tets.size();
    for(size_t i = 0; i < 6; i++)
        num += trs[i].size();
    ofs << num << newl;
    num = 1;
    for(size_t i = 0; i < tets.size(); i++)
    {
        ofs << num << " 4 2 " << phys_vol << " " << geom_vol;
        for(size_t j = 0; j < 4; j++)
            ofs << " " << tets[i].nodes[j] + 1;
        ofs << newl;
        num++;
    }
    for(size_t k = 0; k < 6; k++)
    {
        for(size_t i = 0; i < trs[k].size(); i++)
        {
            ofs << num << " 2 2 " << phys_surf[k] << " " << geom_surf[k];
            for(size_t j = 0; j < 3; j++)
                ofs << " " << trs[k][i].nodes[j] + 1;
            ofs << newl;
            num++;
        }
    }
    ofs << "$EndElements" << newl << newl << flush;
    ofs.close();
}

// Построить вложенную сетку
void mesh_generator::split()
{
    cout << "Splitting mesh ..." << endl;
    // Кэш номеров точек для тех ребер, что уже были посчитаны
    map<pair<size_t, size_t>, size_t> point_map;

    // Новые тетраэдры
    vector<tetrahedron> tets_new;
    // Обходим по старым тетраэдрам, чтобы разбить каждый на 8 новых
    for(size_t i = 0; i < tets.size(); i++)
    {
        // Указатель на старый тетраэдр
        tetrahedron * tet = & tets[i];
        // Номера всех точек, старых и новых (для удобства)
        size_t nodes_all[10] =
        {
            tet->nodes[0],
            tet->nodes[1],
            tet->nodes[2],
            tet->nodes[3],
            numeric_limits<size_t>::max(),
            numeric_limits<size_t>::max(),
            numeric_limits<size_t>::max(),
            numeric_limits<size_t>::max(),
            numeric_limits<size_t>::max(),
            numeric_limits<size_t>::max()
        };
        // Новые точки, которые потом добавим ко всем остальным
        vector<point> nodes_new;
        nodes_new.reserve(6);
        // Итератор для поиска в кэше точек
        map<pair<size_t, size_t>, size_t>::const_iterator map_it;


        // Дальше будем смотреть, есть ли середина ребра в кэше, если нет - то добавим ее
        // в nodes_new, в nodes_all и в кэш, а если есть - то только в nodes_all
        // И так для всех ребер тетраэдра (6 штук)

        if((map_it = point_map.find(make_pair(tet->nodes[0], tet->nodes[1]))) == point_map.end())
        {
            nodes_new.push_back(point((nodes[nodes_all[0]].x + nodes[nodes_all[1]].x) / 2.0,
                                      (nodes[nodes_all[0]].y + nodes[nodes_all[1]].y) / 2.0,
                                      (nodes[nodes_all[0]].z + nodes[nodes_all[1]].z) / 2.0,
                                      nodes.size() + nodes_new.size()));
            point_map[make_pair(tet->nodes[0], tet->nodes[1])] = nodes_new[nodes_new.size() - 1].num;
            nodes_all[4] = nodes_new[nodes_new.size() - 1].num;
        }
        else
            nodes_all[4] = map_it->second;


        if((map_it = point_map.find(make_pair(tet->nodes[0], tet->nodes[2]))) == point_map.end())
        {
            nodes_new.push_back(point((nodes[nodes_all[0]].x + nodes[nodes_all[2]].x) / 2.0,
                                      (nodes[nodes_all[0]].y + nodes[nodes_all[2]].y) / 2.0,
                                      (nodes[nodes_all[0]].z + nodes[nodes_all[2]].z) / 2.0,
                                      nodes.size() + nodes_new.size()));
            point_map[make_pair(tet->nodes[0], tet->nodes[2])] = nodes_new[nodes_new.size() - 1].num;
            nodes_all[5] = nodes_new[nodes_new.size() - 1].num;
        }
        else
            nodes_all[5] = map_it->second;


        if((map_it = point_map.find(make_pair(tet->nodes[0], tet->nodes[3]))) == point_map.end())
        {
            nodes_new.push_back(point((nodes[nodes_all[0]].x + nodes[nodes_all[3]].x) / 2.0,
                                      (nodes[nodes_all[0]].y + nodes[nodes_all[3]].y) / 2.0,
                                      (nodes[nodes_all[0]].z + nodes[nodes_all[3]].z) / 2.0,
                                      nodes.size() + nodes_new.size()));
            point_map[make_pair(tet->nodes[0], tet->nodes[3])] = nodes_new[nodes_new.size() - 1].num;
            nodes_all[6] = nodes_new[nodes_new.size() - 1].num;
        }
        else
            nodes_all[6] = map_it->second;


        if((map_it = point_map.find(make_pair(tet->nodes[1], tet->nodes[2]))) == point_map.end())
        {
            nodes_new.push_back(point((nodes[nodes_all[1]].x + nodes[nodes_all[2]].x) / 2.0,
                                      (nodes[nodes_all[1]].y + nodes[nodes_all[2]].y) / 2.0,
                                      (nodes[nodes_all[1]].z + nodes[nodes_all[2]].z) / 2.0,
                                      nodes.size() + nodes_new.size()));
            point_map[make_pair(tet->nodes[1], tet->nodes[2])] = nodes_new[nodes_new.size() - 1].num;
            nodes_all[7] = nodes_new[nodes_new.size() - 1].num;
        }
        else
            nodes_all[7] = map_it->second;


        if((map_it = point_map.find(make_pair(tet->nodes[1], tet->nodes[3]))) == point_map.end())
        {
            nodes_new.push_back(point((nodes[nodes_all[1]].x + nodes[nodes_all[3]].x) / 2.0,
                                      (nodes[nodes_all[1]].y + nodes[nodes_all[3]].y) / 2.0,
                                      (nodes[nodes_all[1]].z + nodes[nodes_all[3]].z) / 2.0,
                                      nodes.size() + nodes_new.size()));
            point_map[make_pair(tet->nodes[1], tet->nodes[3])] = nodes_new[nodes_new.size() - 1].num;
            nodes_all[8] = nodes_new[nodes_new.size() - 1].num;
        }
        else
            nodes_all[8] = map_it->second;


        if((map_it = point_map.find(make_pair(tet->nodes[2], tet->nodes[3]))) == point_map.end())
        {
            nodes_new.push_back(point((nodes[nodes_all[2]].x + nodes[nodes_all[3]].x) / 2.0,
                                      (nodes[nodes_all[2]].y + nodes[nodes_all[3]].y) / 2.0,
                                      (nodes[nodes_all[2]].z + nodes[nodes_all[3]].z) / 2.0,
                                      nodes.size() + nodes_new.size()));
            point_map[make_pair(tet->nodes[2], tet->nodes[3])] = nodes_new[nodes_new.size() - 1].num;
            nodes_all[9] = nodes_new[nodes_new.size() - 1].num;
        }
        else
            nodes_all[9] = map_it->second;


        // Новый тетраэдр
        tetrahedron tet_new;

        // Будем собирать новые тетраэдры и запихивать их в вектор из новых тетраэдров

        tet_new.nodes[0] = nodes_all[0];
        tet_new.nodes[1] = nodes_all[4];
        tet_new.nodes[2] = nodes_all[5];
        tet_new.nodes[3] = nodes_all[6];
        tets_new.push_back(tet_new);

        tet_new.nodes[0] = nodes_all[1];
        tet_new.nodes[1] = nodes_all[4];
        tet_new.nodes[2] = nodes_all[7];
        tet_new.nodes[3] = nodes_all[8];
        tets_new.push_back(tet_new);

        tet_new.nodes[0] = nodes_all[2];
        tet_new.nodes[1] = nodes_all[5];
        tet_new.nodes[2] = nodes_all[7];
        tet_new.nodes[3] = nodes_all[9];
        tets_new.push_back(tet_new);

        tet_new.nodes[0] = nodes_all[3];
        tet_new.nodes[1] = nodes_all[6];
        tet_new.nodes[2] = nodes_all[8];
        tet_new.nodes[3] = nodes_all[9];
        tets_new.push_back(tet_new);

        tet_new.nodes[0] = nodes_all[5];
        tet_new.nodes[1] = nodes_all[6];
        tet_new.nodes[2] = nodes_all[7];
        tet_new.nodes[3] = nodes_all[9];
        tets_new.push_back(tet_new);

        tet_new.nodes[0] = nodes_all[4];
        tet_new.nodes[1] = nodes_all[5];
        tet_new.nodes[2] = nodes_all[6];
        tet_new.nodes[3] = nodes_all[7];
        tets_new.push_back(tet_new);

        tet_new.nodes[0] = nodes_all[6];
        tet_new.nodes[1] = nodes_all[7];
        tet_new.nodes[2] = nodes_all[8];
        tet_new.nodes[3] = nodes_all[9];
        tets_new.push_back(tet_new);

        tet_new.nodes[0] = nodes_all[4];
        tet_new.nodes[1] = nodes_all[6];
        tet_new.nodes[2] = nodes_all[7];
        tet_new.nodes[3] = nodes_all[8];
        tets_new.push_back(tet_new);

        // Теперь запихаем все новые точки в общий вектор точек
        for(size_t k = 0; k < nodes_new.size(); k++)
            nodes.push_back(nodes_new[k]);
    }

    // Перенесем тетраэдры
    tets.swap(tets_new);
    tets_new.clear();


    // Теперь пришло время треугольников
    vector<triangle> trs_new;
    for(size_t k = 0; k < 6; k++)
    {
        for(size_t i = 0; i < trs[k].size(); i++)
        {
            // Указатель на старый треугольник
            triangle * tr = & trs[k][i];
            // Номера всех точек, старых и новых (для удобства)
            size_t nodes_all[6] =
            {
                tr->nodes[0],
                tr->nodes[1],
                tr->nodes[2],
                numeric_limits<size_t>::max(),
                numeric_limits<size_t>::max(),
                numeric_limits<size_t>::max()
            };
            nodes_all[3] = point_map[make_pair(nodes_all[0], nodes_all[1])];
            nodes_all[4] = point_map[make_pair(nodes_all[0], nodes_all[2])];
            nodes_all[5] = point_map[make_pair(nodes_all[1], nodes_all[2])];

            // Новый треугольник
            triangle tr_new;

            // Будем собирать новые треугольники и запихивать их в вектор из новых треугольников

            tr_new.nodes[0] = nodes_all[0];
            tr_new.nodes[1] = nodes_all[3];
            tr_new.nodes[2] = nodes_all[4];
            trs_new.push_back(tr_new);

            tr_new.nodes[0] = nodes_all[1];
            tr_new.nodes[1] = nodes_all[3];
            tr_new.nodes[2] = nodes_all[5];
            trs_new.push_back(tr_new);

            tr_new.nodes[0] = nodes_all[2];
            tr_new.nodes[1] = nodes_all[4];
            tr_new.nodes[2] = nodes_all[5];
            trs_new.push_back(tr_new);

            tr_new.nodes[0] = nodes_all[3];
            tr_new.nodes[1] = nodes_all[4];
            tr_new.nodes[2] = nodes_all[5];
            trs_new.push_back(tr_new);
        }
        // Перенесем треугольники
        trs[k].swap(trs_new);
        trs_new.clear();
    }
}
