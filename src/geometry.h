#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <cstdlib>

// Класс точка с номером
class point
{
public:
    double x, y, z;
    size_t num;
    point(double x = 0, double y = 0, double z = 0, size_t num = 0 )
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->num = num;
    }
};


/*
 *      /|\3
 *     / | \
 *    /  |  \
 *   /   |   \
 *  /-  -| - -\
 * 0\_   |   _/2
 *    \_ | _/
 *       \/1
 */

// Структура для описания узлов тетраэдра
struct tetrahedron
{
    size_t nodes[4];
};

/*
 *      /\2
 *     /  \
 *   4/----\5
 *   / \  / \
 * 0/___\/___\1
 *      3
 */

// Структура для описания узлов треугольника
struct triangle
{
    size_t nodes[3];
};


/*   _____________
 *  /|6          /|7    ^ z
 * /____________/ |     |  ^ y
 * |4|          |5|     | /
 * | |          | |     |/----> x
 * | |          | |
 * | |          | |
 * | |__________|_|
 * | /2         | /3
 * |/___________|/
 * 0            1
 */

/* Список соседей:
 * 0 - слева
 * 1 - справа
 * 2 - спереди
 * 3 - сзади
 * 4 - снизу
 * 5 - сверху
 */

// Класс куб с флагом соседства
class cube
{
public:
    size_t nodes[8];
    bool neighbor[6];
};

#endif // GEOMETRY_H
