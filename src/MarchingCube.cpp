//
// Created by Danmeng on 2026/3/26.
//

#include "MarchingCube.h"

#include <iostream>
#include <ostream>


#include "glm/ext/scalar_constants.hpp"

static const int edgeTable[256]=
{
    0x0  , 0x109, 0x203, 0x30a, 0x406, 0x50f, 0x605, 0x70c,
    0x80c, 0x905, 0xa0f, 0xb06, 0xc0a, 0xd03, 0xe09, 0xf00,
    0x190, 0x99 , 0x393, 0x29a, 0x596, 0x49f, 0x795, 0x69c,
    0x99c, 0x895, 0xb9f, 0xa96, 0xd9a, 0xc93, 0xf99, 0xe90,
    0x230, 0x339, 0x33 , 0x13a, 0x636, 0x73f, 0x435, 0x53c,
    0xa3c, 0xb35, 0x83f, 0x936, 0xe3a, 0xf33, 0xc39, 0xd30,
    0x3a0, 0x2a9, 0x1a3, 0xaa , 0x7a6, 0x6af, 0x5a5, 0x4ac,
    0xbac, 0xaa5, 0x9af, 0x8a6, 0xfaa, 0xea3, 0xda9, 0xca0,
    0x460, 0x569, 0x663, 0x76a, 0x66 , 0x16f, 0x265, 0x36c,
    0xc6c, 0xd65, 0xe6f, 0xf66, 0x86a, 0x963, 0xa69, 0xb60,
    0x5f0, 0x4f9, 0x7f3, 0x6fa, 0x1f6, 0xff , 0x3f5, 0x2fc,
    0xdfc, 0xcf5, 0xfff, 0xef6, 0x9fa, 0x8f3, 0xbf9, 0xaf0,
    0x650, 0x759, 0x453, 0x55a, 0x256, 0x35f, 0x55 , 0x15c,
    0xe5c, 0xf55, 0xc5f, 0xd56, 0xa5a, 0xb53, 0x859, 0x950,
    0x7c0, 0x6c9, 0x5c3, 0x4ca, 0x3c6, 0x2cf, 0x1c5, 0xcc ,
    0xfcc, 0xec5, 0xdcf, 0xcc6, 0xbca, 0xac3, 0x9c9, 0x8c0,
    0x8c0, 0x9c9, 0xac3, 0xbca, 0xcc6, 0xdcf, 0xec5, 0xfcc,
    0xcc , 0x1c5, 0x2cf, 0x3c6, 0x4ca, 0x5c3, 0x6c9, 0x7c0,
    0x950, 0x859, 0xb53, 0xa5a, 0xd56, 0xc5f, 0xf55, 0xe5c,
    0x15c, 0x55 , 0x35f, 0x256, 0x55a, 0x453, 0x759, 0x650,
    0xaf0, 0xbf9, 0x8f3, 0x9fa, 0xef6, 0xfff, 0xcf5, 0xdfc,
    0x2fc, 0x3f5, 0xff , 0x1f6, 0x6fa, 0x7f3, 0x4f9, 0x5f0,
    0xb60, 0xa69, 0x963, 0x86a, 0xf66, 0xe6f, 0xd65, 0xc6c,
    0x36c, 0x265, 0x16f, 0x66 , 0x76a, 0x663, 0x569, 0x460,
    0xca0, 0xda9, 0xea3, 0xfaa, 0x8a6, 0x9af, 0xaa5, 0xbac,
    0x4ac, 0x5a5, 0x6af, 0x7a6, 0xaa , 0x1a3, 0x2a9, 0x3a0,
    0xd30, 0xc39, 0xf33, 0xe3a, 0x936, 0x83f, 0xb35, 0xa3c,
    0x53c, 0x435, 0x73f, 0x636, 0x13a, 0x33 , 0x339, 0x230,
    0xe90, 0xf99, 0xc93, 0xd9a, 0xa96, 0xb9f, 0x895, 0x99c,
    0x69c, 0x795, 0x49f, 0x596, 0x29a, 0x393, 0x99 , 0x190,
    0xf00, 0xe09, 0xd03, 0xc0a, 0xb06, 0xa0f, 0x905, 0x80c,
    0x70c, 0x605, 0x50f, 0x406, 0x30a, 0x203, 0x109, 0x0
};

static const int triTable[256][16] =
{{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 1, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 8, 3, 9, 8, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 2, 10, 0, 2, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{2, 8, 3, 2, 10, 8, 10, 9, 8, -1, -1, -1, -1, -1, -1, -1},
{3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 11, 2, 8, 11, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 9, 0, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 11, 2, 1, 9, 11, 9, 8, 11, -1, -1, -1, -1, -1, -1, -1},
{3, 10, 1, 11, 10, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 10, 1, 0, 8, 10, 8, 11, 10, -1, -1, -1, -1, -1, -1, -1},
{3, 9, 0, 3, 11, 9, 11, 10, 9, -1, -1, -1, -1, -1, -1, -1},
{9, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 3, 0, 7, 3, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 1, 9, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 1, 9, 4, 7, 1, 7, 3, 1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 4, 7, 3, 0, 4, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1},
{9, 2, 10, 9, 0, 2, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
{2, 10, 9, 2, 9, 7, 2, 7, 3, 7, 9, 4, -1, -1, -1, -1},
{8, 4, 7, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{11, 4, 7, 11, 2, 4, 2, 0, 4, -1, -1, -1, -1, -1, -1, -1},
{9, 0, 1, 8, 4, 7, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},
{4, 7, 11, 9, 4, 11, 9, 11, 2, 9, 2, 1, -1, -1, -1, -1},
{3, 10, 1, 3, 11, 10, 7, 8, 4, -1, -1, -1, -1, -1, -1, -1},
{1, 11, 10, 1, 4, 11, 1, 0, 4, 7, 11, 4, -1, -1, -1, -1},
{4, 7, 8, 9, 0, 11, 9, 11, 10, 11, 0, 3, -1, -1, -1, -1},
{4, 7, 11, 4, 11, 9, 9, 11, 10, -1, -1, -1, -1, -1, -1, -1},
{9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 5, 4, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 5, 4, 1, 5, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{8, 5, 4, 8, 3, 5, 3, 1, 5, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 0, 8, 1, 2, 10, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
{5, 2, 10, 5, 4, 2, 4, 0, 2, -1, -1, -1, -1, -1, -1, -1},
{2, 10, 5, 3, 2, 5, 3, 5, 4, 3, 4, 8, -1, -1, -1, -1},
{9, 5, 4, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 11, 2, 0, 8, 11, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
{0, 5, 4, 0, 1, 5, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},
{2, 1, 5, 2, 5, 8, 2, 8, 11, 4, 8, 5, -1, -1, -1, -1},
{10, 3, 11, 10, 1, 3, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1},
{4, 9, 5, 0, 8, 1, 8, 10, 1, 8, 11, 10, -1, -1, -1, -1},
{5, 4, 0, 5, 0, 11, 5, 11, 10, 11, 0, 3, -1, -1, -1, -1},
{5, 4, 8, 5, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1},
{9, 7, 8, 5, 7, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 3, 0, 9, 5, 3, 5, 7, 3, -1, -1, -1, -1, -1, -1, -1},
{0, 7, 8, 0, 1, 7, 1, 5, 7, -1, -1, -1, -1, -1, -1, -1},
{1, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 7, 8, 9, 5, 7, 10, 1, 2, -1, -1, -1, -1, -1, -1, -1},
{10, 1, 2, 9, 5, 0, 5, 3, 0, 5, 7, 3, -1, -1, -1, -1},
{8, 0, 2, 8, 2, 5, 8, 5, 7, 10, 5, 2, -1, -1, -1, -1},
{2, 10, 5, 2, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1},
{7, 9, 5, 7, 8, 9, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1},
{9, 5, 7, 9, 7, 2, 9, 2, 0, 2, 7, 11, -1, -1, -1, -1},
{2, 3, 11, 0, 1, 8, 1, 7, 8, 1, 5, 7, -1, -1, -1, -1},
{11, 2, 1, 11, 1, 7, 7, 1, 5, -1, -1, -1, -1, -1, -1, -1},
{9, 5, 8, 8, 5, 7, 10, 1, 3, 10, 3, 11, -1, -1, -1, -1},
{5, 7, 0, 5, 0, 9, 7, 11, 0, 1, 0, 10, 11, 10, 0, -1},
{11, 10, 0, 11, 0, 3, 10, 5, 0, 8, 0, 7, 5, 7, 0, -1},
{11, 10, 5, 7, 11, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 0, 1, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 8, 3, 1, 9, 8, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
{1, 6, 5, 2, 6, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 6, 5, 1, 2, 6, 3, 0, 8, -1, -1, -1, -1, -1, -1, -1},
{9, 6, 5, 9, 0, 6, 0, 2, 6, -1, -1, -1, -1, -1, -1, -1},
{5, 9, 8, 5, 8, 2, 5, 2, 6, 3, 2, 8, -1, -1, -1, -1},
{2, 3, 11, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{11, 0, 8, 11, 2, 0, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
{0, 1, 9, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
{5, 10, 6, 1, 9, 2, 9, 11, 2, 9, 8, 11, -1, -1, -1, -1},
{6, 3, 11, 6, 5, 3, 5, 1, 3, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 11, 0, 11, 5, 0, 5, 1, 5, 11, 6, -1, -1, -1, -1},
{3, 11, 6, 0, 3, 6, 0, 6, 5, 0, 5, 9, -1, -1, -1, -1},
{6, 5, 9, 6, 9, 11, 11, 9, 8, -1, -1, -1, -1, -1, -1, -1},
{5, 10, 6, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 3, 0, 4, 7, 3, 6, 5, 10, -1, -1, -1, -1, -1, -1, -1},
{1, 9, 0, 5, 10, 6, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
{10, 6, 5, 1, 9, 7, 1, 7, 3, 7, 9, 4, -1, -1, -1, -1},
{6, 1, 2, 6, 5, 1, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 5, 5, 2, 6, 3, 0, 4, 3, 4, 7, -1, -1, -1, -1},
{8, 4, 7, 9, 0, 5, 0, 6, 5, 0, 2, 6, -1, -1, -1, -1},
{7, 3, 9, 7, 9, 4, 3, 2, 9, 5, 9, 6, 2, 6, 9, -1},
{3, 11, 2, 7, 8, 4, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
{5, 10, 6, 4, 7, 2, 4, 2, 0, 2, 7, 11, -1, -1, -1, -1},
{0, 1, 9, 4, 7, 8, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1},
{9, 2, 1, 9, 11, 2, 9, 4, 11, 7, 11, 4, 5, 10, 6, -1},
{8, 4, 7, 3, 11, 5, 3, 5, 1, 5, 11, 6, -1, -1, -1, -1},
{5, 1, 11, 5, 11, 6, 1, 0, 11, 7, 11, 4, 0, 4, 11, -1},
{0, 5, 9, 0, 6, 5, 0, 3, 6, 11, 6, 3, 8, 4, 7, -1},
{6, 5, 9, 6, 9, 11, 4, 7, 9, 7, 11, 9, -1, -1, -1, -1},
{10, 4, 9, 6, 4, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 10, 6, 4, 9, 10, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1},
{10, 0, 1, 10, 6, 0, 6, 4, 0, -1, -1, -1, -1, -1, -1, -1},
{8, 3, 1, 8, 1, 6, 8, 6, 4, 6, 1, 10, -1, -1, -1, -1},
{1, 4, 9, 1, 2, 4, 2, 6, 4, -1, -1, -1, -1, -1, -1, -1},
{3, 0, 8, 1, 2, 9, 2, 4, 9, 2, 6, 4, -1, -1, -1, -1},
{0, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{8, 3, 2, 8, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1},
{10, 4, 9, 10, 6, 4, 11, 2, 3, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 2, 2, 8, 11, 4, 9, 10, 4, 10, 6, -1, -1, -1, -1},
{3, 11, 2, 0, 1, 6, 0, 6, 4, 6, 1, 10, -1, -1, -1, -1},
{6, 4, 1, 6, 1, 10, 4, 8, 1, 2, 1, 11, 8, 11, 1, -1},
{9, 6, 4, 9, 3, 6, 9, 1, 3, 11, 6, 3, -1, -1, -1, -1},
{8, 11, 1, 8, 1, 0, 11, 6, 1, 9, 1, 4, 6, 4, 1, -1},
{3, 11, 6, 3, 6, 0, 0, 6, 4, -1, -1, -1, -1, -1, -1, -1},
{6, 4, 8, 11, 6, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{7, 10, 6, 7, 8, 10, 8, 9, 10, -1, -1, -1, -1, -1, -1, -1},
{0, 7, 3, 0, 10, 7, 0, 9, 10, 6, 7, 10, -1, -1, -1, -1},
{10, 6, 7, 1, 10, 7, 1, 7, 8, 1, 8, 0, -1, -1, -1, -1},
{10, 6, 7, 10, 7, 1, 1, 7, 3, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 6, 1, 6, 8, 1, 8, 9, 8, 6, 7, -1, -1, -1, -1},
{2, 6, 9, 2, 9, 1, 6, 7, 9, 0, 9, 3, 7, 3, 9, -1},
{7, 8, 0, 7, 0, 6, 6, 0, 2, -1, -1, -1, -1, -1, -1, -1},
{7, 3, 2, 6, 7, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{2, 3, 11, 10, 6, 8, 10, 8, 9, 8, 6, 7, -1, -1, -1, -1},
{2, 0, 7, 2, 7, 11, 0, 9, 7, 6, 7, 10, 9, 10, 7, -1},
{1, 8, 0, 1, 7, 8, 1, 10, 7, 6, 7, 10, 2, 3, 11, -1},
{11, 2, 1, 11, 1, 7, 10, 6, 1, 6, 7, 1, -1, -1, -1, -1},
{8, 9, 6, 8, 6, 7, 9, 1, 6, 11, 6, 3, 1, 3, 6, -1},
{0, 9, 1, 11, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{7, 8, 0, 7, 0, 6, 3, 11, 0, 11, 6, 0, -1, -1, -1, -1},
{7, 11, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 0, 8, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 1, 9, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{8, 1, 9, 8, 3, 1, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
{10, 1, 2, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, 3, 0, 8, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},
{2, 9, 0, 2, 10, 9, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},
{6, 11, 7, 2, 10, 3, 10, 8, 3, 10, 9, 8, -1, -1, -1, -1},
{7, 2, 3, 6, 2, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{7, 0, 8, 7, 6, 0, 6, 2, 0, -1, -1, -1, -1, -1, -1, -1},
{2, 7, 6, 2, 3, 7, 0, 1, 9, -1, -1, -1, -1, -1, -1, -1},
{1, 6, 2, 1, 8, 6, 1, 9, 8, 8, 7, 6, -1, -1, -1, -1},
{10, 7, 6, 10, 1, 7, 1, 3, 7, -1, -1, -1, -1, -1, -1, -1},
{10, 7, 6, 1, 7, 10, 1, 8, 7, 1, 0, 8, -1, -1, -1, -1},
{0, 3, 7, 0, 7, 10, 0, 10, 9, 6, 10, 7, -1, -1, -1, -1},
{7, 6, 10, 7, 10, 8, 8, 10, 9, -1, -1, -1, -1, -1, -1, -1},
{6, 8, 4, 11, 8, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 6, 11, 3, 0, 6, 0, 4, 6, -1, -1, -1, -1, -1, -1, -1},
{8, 6, 11, 8, 4, 6, 9, 0, 1, -1, -1, -1, -1, -1, -1, -1},
{9, 4, 6, 9, 6, 3, 9, 3, 1, 11, 3, 6, -1, -1, -1, -1},
{6, 8, 4, 6, 11, 8, 2, 10, 1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, 3, 0, 11, 0, 6, 11, 0, 4, 6, -1, -1, -1, -1},
{4, 11, 8, 4, 6, 11, 0, 2, 9, 2, 10, 9, -1, -1, -1, -1},
{10, 9, 3, 10, 3, 2, 9, 4, 3, 11, 3, 6, 4, 6, 3, -1},
{8, 2, 3, 8, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1},
{0, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 9, 0, 2, 3, 4, 2, 4, 6, 4, 3, 8, -1, -1, -1, -1},
{1, 9, 4, 1, 4, 2, 2, 4, 6, -1, -1, -1, -1, -1, -1, -1},
{8, 1, 3, 8, 6, 1, 8, 4, 6, 6, 10, 1, -1, -1, -1, -1},
{10, 1, 0, 10, 0, 6, 6, 0, 4, -1, -1, -1, -1, -1, -1, -1},
{4, 6, 3, 4, 3, 8, 6, 10, 3, 0, 3, 9, 10, 9, 3, -1},
{10, 9, 4, 6, 10, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 9, 5, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, 4, 9, 5, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
{5, 0, 1, 5, 4, 0, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1},
{11, 7, 6, 8, 3, 4, 3, 5, 4, 3, 1, 5, -1, -1, -1, -1},
{9, 5, 4, 10, 1, 2, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1},
{6, 11, 7, 1, 2, 10, 0, 8, 3, 4, 9, 5, -1, -1, -1, -1},
{7, 6, 11, 5, 4, 10, 4, 2, 10, 4, 0, 2, -1, -1, -1, -1},
{3, 4, 8, 3, 5, 4, 3, 2, 5, 10, 5, 2, 11, 7, 6, -1},
{7, 2, 3, 7, 6, 2, 5, 4, 9, -1, -1, -1, -1, -1, -1, -1},
{9, 5, 4, 0, 8, 6, 0, 6, 2, 6, 8, 7, -1, -1, -1, -1},
{3, 6, 2, 3, 7, 6, 1, 5, 0, 5, 4, 0, -1, -1, -1, -1},
{6, 2, 8, 6, 8, 7, 2, 1, 8, 4, 8, 5, 1, 5, 8, -1},
{9, 5, 4, 10, 1, 6, 1, 7, 6, 1, 3, 7, -1, -1, -1, -1},
{1, 6, 10, 1, 7, 6, 1, 0, 7, 8, 7, 0, 9, 5, 4, -1},
{4, 0, 10, 4, 10, 5, 0, 3, 10, 6, 10, 7, 3, 7, 10, -1},
{7, 6, 10, 7, 10, 8, 5, 4, 10, 4, 8, 10, -1, -1, -1, -1},
{6, 9, 5, 6, 11, 9, 11, 8, 9, -1, -1, -1, -1, -1, -1, -1},
{3, 6, 11, 0, 6, 3, 0, 5, 6, 0, 9, 5, -1, -1, -1, -1},
{0, 11, 8, 0, 5, 11, 0, 1, 5, 5, 6, 11, -1, -1, -1, -1},
{6, 11, 3, 6, 3, 5, 5, 3, 1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, 9, 5, 11, 9, 11, 8, 11, 5, 6, -1, -1, -1, -1},
{0, 11, 3, 0, 6, 11, 0, 9, 6, 5, 6, 9, 1, 2, 10, -1},
{11, 8, 5, 11, 5, 6, 8, 0, 5, 10, 5, 2, 0, 2, 5, -1},
{6, 11, 3, 6, 3, 5, 2, 10, 3, 10, 5, 3, -1, -1, -1, -1},
{5, 8, 9, 5, 2, 8, 5, 6, 2, 3, 8, 2, -1, -1, -1, -1},
{9, 5, 6, 9, 6, 0, 0, 6, 2, -1, -1, -1, -1, -1, -1, -1},
{1, 5, 8, 1, 8, 0, 5, 6, 8, 3, 8, 2, 6, 2, 8, -1},
{1, 5, 6, 2, 1, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 3, 6, 1, 6, 10, 3, 8, 6, 5, 6, 9, 8, 9, 6, -1},
{10, 1, 0, 10, 0, 6, 9, 5, 0, 5, 6, 0, -1, -1, -1, -1},
{0, 3, 8, 5, 6, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{10, 5, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{11, 5, 10, 7, 5, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{11, 5, 10, 11, 7, 5, 8, 3, 0, -1, -1, -1, -1, -1, -1, -1},
{5, 11, 7, 5, 10, 11, 1, 9, 0, -1, -1, -1, -1, -1, -1, -1},
{10, 7, 5, 10, 11, 7, 9, 8, 1, 8, 3, 1, -1, -1, -1, -1},
{11, 1, 2, 11, 7, 1, 7, 5, 1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, 1, 2, 7, 1, 7, 5, 7, 2, 11, -1, -1, -1, -1},
{9, 7, 5, 9, 2, 7, 9, 0, 2, 2, 11, 7, -1, -1, -1, -1},
{7, 5, 2, 7, 2, 11, 5, 9, 2, 3, 2, 8, 9, 8, 2, -1},
{2, 5, 10, 2, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1},
{8, 2, 0, 8, 5, 2, 8, 7, 5, 10, 2, 5, -1, -1, -1, -1},
{9, 0, 1, 5, 10, 3, 5, 3, 7, 3, 10, 2, -1, -1, -1, -1},
{9, 8, 2, 9, 2, 1, 8, 7, 2, 10, 2, 5, 7, 5, 2, -1},
{1, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 7, 0, 7, 1, 1, 7, 5, -1, -1, -1, -1, -1, -1, -1},
{9, 0, 3, 9, 3, 5, 5, 3, 7, -1, -1, -1, -1, -1, -1, -1},
{9, 8, 7, 5, 9, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{5, 8, 4, 5, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1},
{5, 0, 4, 5, 11, 0, 5, 10, 11, 11, 3, 0, -1, -1, -1, -1},
{0, 1, 9, 8, 4, 10, 8, 10, 11, 10, 4, 5, -1, -1, -1, -1},
{10, 11, 4, 10, 4, 5, 11, 3, 4, 9, 4, 1, 3, 1, 4, -1},
{2, 5, 1, 2, 8, 5, 2, 11, 8, 4, 5, 8, -1, -1, -1, -1},
{0, 4, 11, 0, 11, 3, 4, 5, 11, 2, 11, 1, 5, 1, 11, -1},
{0, 2, 5, 0, 5, 9, 2, 11, 5, 4, 5, 8, 11, 8, 5, -1},
{9, 4, 5, 2, 11, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{2, 5, 10, 3, 5, 2, 3, 4, 5, 3, 8, 4, -1, -1, -1, -1},
{5, 10, 2, 5, 2, 4, 4, 2, 0, -1, -1, -1, -1, -1, -1, -1},
{3, 10, 2, 3, 5, 10, 3, 8, 5, 4, 5, 8, 0, 1, 9, -1},
{5, 10, 2, 5, 2, 4, 1, 9, 2, 9, 4, 2, -1, -1, -1, -1},
{8, 4, 5, 8, 5, 3, 3, 5, 1, -1, -1, -1, -1, -1, -1, -1},
{0, 4, 5, 1, 0, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{8, 4, 5, 8, 5, 3, 9, 0, 5, 0, 3, 5, -1, -1, -1, -1},
{9, 4, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 11, 7, 4, 9, 11, 9, 10, 11, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, 4, 9, 7, 9, 11, 7, 9, 10, 11, -1, -1, -1, -1},
{1, 10, 11, 1, 11, 4, 1, 4, 0, 7, 4, 11, -1, -1, -1, -1},
{3, 1, 4, 3, 4, 8, 1, 10, 4, 7, 4, 11, 10, 11, 4, -1},
{4, 11, 7, 9, 11, 4, 9, 2, 11, 9, 1, 2, -1, -1, -1, -1},
{9, 7, 4, 9, 11, 7, 9, 1, 11, 2, 11, 1, 0, 8, 3, -1},
{11, 7, 4, 11, 4, 2, 2, 4, 0, -1, -1, -1, -1, -1, -1, -1},
{11, 7, 4, 11, 4, 2, 8, 3, 4, 3, 2, 4, -1, -1, -1, -1},
{2, 9, 10, 2, 7, 9, 2, 3, 7, 7, 4, 9, -1, -1, -1, -1},
{9, 10, 7, 9, 7, 4, 10, 2, 7, 8, 7, 0, 2, 0, 7, -1},
{3, 7, 10, 3, 10, 2, 7, 4, 10, 1, 10, 0, 4, 0, 10, -1},
{1, 10, 2, 8, 7, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 9, 1, 4, 1, 7, 7, 1, 3, -1, -1, -1, -1, -1, -1, -1},
{4, 9, 1, 4, 1, 7, 0, 8, 1, 8, 7, 1, -1, -1, -1, -1},
{4, 0, 3, 7, 4, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 8, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 0, 9, 3, 9, 11, 11, 9, 10, -1, -1, -1, -1, -1, -1, -1},
{0, 1, 10, 0, 10, 8, 8, 10, 11, -1, -1, -1, -1, -1, -1, -1},
{3, 1, 10, 11, 3, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 11, 1, 11, 9, 9, 11, 8, -1, -1, -1, -1, -1, -1, -1},
{3, 0, 9, 3, 9, 11, 1, 2, 9, 2, 11, 9, -1, -1, -1, -1},
{0, 2, 11, 8, 0, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 2, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{2, 3, 8, 2, 8, 10, 10, 8, 9, -1, -1, -1, -1, -1, -1, -1},
{9, 10, 2, 0, 9, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{2, 3, 8, 2, 8, 10, 0, 1, 8, 1, 10, 8, -1, -1, -1, -1},
{1, 10, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 3, 8, 9, 1, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 9, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 3, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}
};

float MarchingCube::scalerField(glm::vec3 p)
{
    return p.x * p.x + p.y * p.y + p.z * p.z - 1.f;
}

glm::vec3 MarchingCube::vertexInterp(float isoLevel, const glm::vec3& p1, const glm::vec3& p2, float valp1, float valp2)
{

    if (glm::abs(isoLevel - valp1) < 0.00001f) return p1;

    if (glm::abs(isoLevel - valp2) < 0.00001f) return p2;

    float diff = valp2 - valp1;
    if (glm::abs(diff) < 0.00001f) return (p1 + p2) * 0.5f;

    float mu = (isoLevel - valp1) / diff;

    mu = glm::clamp(mu, 0.0f, 1.0f);

    auto p = p1 * ( 1.0f - mu ) + p2 * mu;

    return p;
}

void MarchingCube::generateMeshInfo()
{
    m_gridPoints.resize(m_dim * m_dim * m_dim);
    m_scalarValues.resize(m_dim * m_dim * m_dim);

    float step = 2.0f * m_size / static_cast<float>((m_resolution - 1));

    for (int i = 0; i < m_dim; ++i)
    {
        float x = -m_size + i * step;
        for (int j = 0; j < m_dim; ++j)
        {
            float y = -m_size + j * step;
            for (int k = 0; k < m_dim; ++k)
            {
                float z = -m_size + k * step;
                int idx = i + j * m_dim + k * m_dim * m_dim;
                m_gridPoints[idx] = glm::vec3(x, y, z);
                m_scalarValues[idx] = scalerField(m_gridPoints[idx]);
            }
        }
    }

    int numVoxels = m_dim - 1;

    Triangle triangles[5];

    for (int i = 0; i < numVoxels; ++i)
    {
        for (int j = 0; j < numVoxels; ++j)
        {
            for (int k = 0; k < numVoxels; ++k)
            {
                GridCell cell;
                int indexs[8];
                indexs[0] = (i  ) + (j  ) * m_dim + (k  ) * m_dim * m_dim;
                indexs[1] = (i+1) + (j  ) * m_dim + (k  ) * m_dim * m_dim;
                indexs[2] = (i+1) + (j+1) * m_dim + (k  ) * m_dim * m_dim;
                indexs[3] = (i  ) + (j+1) * m_dim + (k  ) * m_dim * m_dim;
                indexs[4] = (i  ) + (j  ) * m_dim + (k+1) * m_dim * m_dim;
                indexs[5] = (i+1) + (j  ) * m_dim + (k+1) * m_dim * m_dim;
                indexs[6] = (i+1) + (j+1) * m_dim + (k+1) * m_dim * m_dim;
                indexs[7] = (i  ) + (j+1) * m_dim + (k+1) * m_dim * m_dim;

                int cubeIndex = 0;
                for (int m = 0; m < 8; ++m)
                {
                    cell.p[m] = m_gridPoints[indexs[m]];
                    cell.val[m] = m_scalarValues[indexs[m]];
                }

                int numTri = polygonise(cell, m_isoLevel, triangles);

                for (int t = 0; t < numTri; ++t)
                {
                    glm::vec3 v0(triangles[t].p[0]);
                    glm::vec3 v1(triangles[t].p[1]);
                    glm::vec3 v2(triangles[t].p[2]);

                    glm::vec3 faceNormal = glm::normalize(glm::cross(v2 - v0, v1 - v0));

                    unsigned int idx0 = addVertex(v0, faceNormal);
                    unsigned int idx1 = addVertex(v1, faceNormal);
                    unsigned int idx2 = addVertex(v2, faceNormal);

                    m_indices.push_back(idx0);
                    m_indices.push_back(idx1);
                    m_indices.push_back(idx2);
                }
            }
        }
    }
    m_averageEdgeLength = computeAverageEdgeLength();
}

bool MarchingCube::canFlipEdge(size_t v1, size_t v2, size_t v3, size_t v4) const
{
    const auto& p1 = m_vertices[v1];
    const auto& p2 = m_vertices[v2];
    const auto& p3 = m_vertices[v3];
    const auto& p4 = m_vertices[v4];

    auto e12 = p2.position - p1.position;
    auto e13 = p3.position - p1.position;
    auto e14 = p4.position - p1.position;
    auto n1 = glm::cross(e12, e13);
    auto n2 = glm::cross(e12, e14);

    if (glm::dot(n1, n2) >= 0) return false;

    auto e34 = p4.position - p3.position;
    auto e31 = p1.position - p3.position;
    auto e32 = p2.position - p3.position;
    auto n3 = glm::cross(e34, e31);
    auto n4 = glm::cross(e34, e32);

    if (glm::dot(n3, n4) >= 0) return false;

    auto newTri1 = glm::cross(e13, (p4.position - p3.position));
    auto newTri2 = glm::cross(e32, (p4.position - p2.position));

    if (glm::dot(newTri1, newTri1) < 0.00000001 || glm::dot(newTri2, newTri2) < 0.00000001) return false;

    return true;

}

float MarchingCube::computeEdgeLength(size_t v1, size_t v2) const
{
    if (v1 >= m_vertices.size() || v2 >= m_vertices.size())
    {
        return 0.0f;
    }
    return glm::length(m_vertices[v1].position - m_vertices[v2].position);
}

Vertex MarchingCube::computeMergedVertex(int v0, int v1,
                                         const std::unordered_map<int, std::unordered_set<int>>& vertexToTriangles)
{
    Vertex merged;

    float weight0 = 1.0f;
    float weight1 = 1.0f;

    auto it0 = vertexToTriangles.find(v0);
    auto it1 = vertexToTriangles.find(v1);

    if (it0 != vertexToTriangles.end() && it1 != vertexToTriangles.end()) {
        weight0 = static_cast<float>(it0->second.size());
        weight1 = static_cast<float>(it1->second.size());
    }

    float totalWeight = weight0 + weight1;
    float t = weight1 / totalWeight; 

    merged.position = m_vertices[v0].position * (1.0f - t) + m_vertices[v1].position * t;
    merged.normal = glm::normalize(m_vertices[v0].normal * (1.0f - t) + m_vertices[v1].normal * t);

    return merged;
}

void MarchingCube::processEdgeForCollapse(size_t v0, size_t v1, float collapseThreshold, float preserveThreshold,
                                          const std::unordered_set<int>& boundaryVertices, std::vector<std::pair<int, int>>& edgesToCollapse,
                                          std::unordered_set<int64_t>& processedEdges)
{
    if (v0 > v1) std::swap(v0, v1);

    int64_t edgeKey = static_cast<int64_t>(v0) * m_vertices.size() + v1;

    if (processedEdges.find(edgeKey) != processedEdges.end()) return;

    if (boundaryVertices.find(v0) != boundaryVertices.end() &&
        boundaryVertices.find(v1) != boundaryVertices.end()) {
        processedEdges.insert(edgeKey);
        return;
        }

    float edgeLength = glm::length(m_vertices[v0].position - m_vertices[v1].position);

    if (edgeLength < collapseThreshold && edgeLength > preserveThreshold) {
        edgesToCollapse.push_back({v0, v1});
        processedEdges.insert(edgeKey);
    }
}


MarchingCube::MarchingCube() : m_dim(9), m_size(1.2f), m_resolution(9), m_isoLevel(0.f)
{
    generateMeshInfo();
}

void MarchingCube::isotropicalRemesh(float targetEdgeLength, int iterations)
{
    if (m_vertices.empty() || m_indices.empty()) return;

    std::cout << "MarchingCube::isotropicalRemesh" << std::endl;
    std::cout << "Starting remeshing..." << std::endl;
    std::cout << "Initial vertices: " << m_vertices.size() << std::endl;
    std::cout << "Initial indices: " << m_indices.size() << std::endl;

    auto splitThreshold = targetEdgeLength * 1.5f; 
    auto collapseThreshold = targetEdgeLength * 0.6f; 
    auto preserveThreshold = targetEdgeLength * 0.3f;

    auto prevVertexCount = static_cast<float>(m_vertices.size());
    int noImproveMentCount = 0;
    const int maxNoImproveMent = 3;

    for (int iter = 0; iter < iterations; ++iter)
    {
        std::cout << "Iteration: " << iter + 1 << "/" << iterations << std::endl;

        // 分割过长的边
        auto splitCount = splitLongEdges(splitThreshold);
        std::cout << "   Split " << splitCount << " edges" << std::endl;

        // 折叠过短的边
        auto collapseCount = collapseShortEdges(collapseThreshold, preserveThreshold);
        std::cout << "   Collapsed " << collapseCount << " edges" << std::endl;

        // 边翻转优化顶点度数
        auto flipCount = equalizeValences();
        std::cout << "   Flipped " << flipCount << " edges" << std::endl;

        // 切向松弛
        auto lambda = 0.5f * std::exp(-iter * 0.3f);
        tangentialRelaxation(3, lambda, true);

        // 投影回原始表面
        projectToSurface();

        updateNormals();

        float currentVertexCount = static_cast<float>(m_vertices.size());
        float vertexChange = std::abs(currentVertexCount - prevVertexCount) / prevVertexCount;

        std::cout << "Vertices: " << m_vertices.size() << std::endl;
        std::cout << "Triangles: " << m_indices.size() / 3 << std::endl;
        std::cout << "   Vertex change: " << (vertexChange * 100.0f) << "%" << std::endl;
    }

    for (auto& vertex : m_vertices)
    {
        vertex.normal = glm::normalize(vertex.position);
    }

    std::cout << "Remeshing finished. " << std::endl;
    checkMeshIntegrity();
}

void MarchingCube::checkMeshIntegrity() const
{
    std::cout << "Mesh Integrity Check:" << std::endl;
    std::cout << "  Vertices: " << m_vertices.size() << std::endl;
    std::cout << "  Triangles: " << m_indices.size() / 3 << std::endl;

    for (size_t i = 0; i < m_indices.size(); ++i) {
        if (m_indices[i] >= m_vertices.size()) {
            std::cerr << "  ERROR: Index " << m_indices[i] << " out of range!" << std::endl;
        }
    }

    float totalEdgeLength = 0.0f;
    int edgeCount = 0;
    std::unordered_set<Edge, EdgeHash> uniqueEdges;

    for (size_t i = 0; i < m_indices.size(); i += 3) {
        size_t v0 = m_indices[i];
        size_t v1 = m_indices[i + 1];
        size_t v2 = m_indices[i + 2];

        Edge edges[3] = {Edge(v0, v1), Edge(v1, v2), Edge(v2, v0)};

        for (int e = 0; e < 3; ++e) {
            if (uniqueEdges.find(edges[e]) == uniqueEdges.end()) {
                float length = computeEdgeLength(edges[e].v1, edges[e].v2);
                totalEdgeLength += length;
                edgeCount++;
                uniqueEdges.insert(edges[e]);
            }
        }
    }

    if (edgeCount > 0) {
        float avgLength = totalEdgeLength / edgeCount;
        m_averageEdgeLength = avgLength;
        std::cout << "  Average edge length: " << avgLength << std::endl;

        float variance = 0.0f;
        for (const auto& edge : uniqueEdges) {
            float length = computeEdgeLength(edge.v1, edge.v2);
            variance += (length - avgLength) * (length - avgLength);
        }
        variance /= edgeCount;
        std::cout << "  Edge length std dev: " << std::sqrt(variance) << std::endl;
    }
}

void MarchingCube::buildTopologyInfo(std::unordered_map<int, std::unordered_set<int>>& vertexToTriangles,
    std::unordered_set<int>& boundaryVertices) const
{
    vertexToTriangles.clear();
    boundaryVertices.clear();

    for (size_t i = 0; i < m_indices.size(); i += 3)
    {
        int triIndex = static_cast<int>(i / 3);
        vertexToTriangles[m_indices[i]].insert(triIndex);
        vertexToTriangles[m_indices[i + 1]].insert(triIndex);
        vertexToTriangles[m_indices[i + 2]].insert(triIndex);
    }

    std::unordered_map<uint64_t, int> edgeCount;
    for (size_t i = 0; i < m_indices.size(); i += 3)
    {
        size_t v0 = m_indices[i];
        size_t v1 = m_indices[i + 1];
        size_t v2 = m_indices[i + 2];

        uint64_t e0 = (v0 < v1) ? (v0 * m_vertices.size() + v1) : (v1 * m_vertices.size() + v0);
        uint64_t e1 = (v1 < v2) ? (v1 * m_vertices.size() + v2) : (v2 * m_vertices.size() + v1);
        uint64_t e2 = (v2 < v0) ? (v2 * m_vertices.size() + v0) : (v0 * m_vertices.size() + v2);

        edgeCount[e0]++;
        edgeCount[e1]++;
        edgeCount[e2]++;
    }

    for (const auto& [edgeKey, count] : edgeCount)
    {
        if (count == 1) 
        {
            size_t v0 = edgeKey / m_vertices.size();
            size_t v1 = edgeKey % m_vertices.size();
            boundaryVertices.insert(static_cast<int>(v0));
            boundaryVertices.insert(static_cast<int>(v1));
        }
    }
}

void MarchingCube::buildEdgeInfo(std::unordered_set<Edge, EdgeHash>& edges,
    std::vector<std::vector<size_t>>& adjacencies,
    std::unordered_map<Edge, std::vector<size_t>, EdgeHash>& edgeToTriangles) const
{
    edges.clear();
    adjacencies.assign(m_vertices.size(), std::vector<size_t>());
    edgeToTriangles.clear();

    for (size_t i = 0; i < m_indices.size(); i += 3)
    {
        size_t v0 = m_indices[i];
        size_t v1 = m_indices[i + 1];
        size_t v2 = m_indices[i + 2];

        size_t triIdx = i / 3;

        Edge e01(v0, v1), e12(v1, v2), e20(v2, v0);
        edges.insert(e01);
        edges.insert(e12);
        edges.insert(e20);

        edgeToTriangles[e01].push_back(triIdx);
        edgeToTriangles[e12].push_back(triIdx);
        edgeToTriangles[e20].push_back(triIdx);

        adjacencies[v0].push_back(v1);
        adjacencies[v0].push_back(v2);
        adjacencies[v1].push_back(v0);
        adjacencies[v1].push_back(v2);
        adjacencies[v2].push_back(v0);
        adjacencies[v2].push_back(v1);
    }

    for (auto& neighbors : adjacencies)
    {
        std::sort(neighbors.begin(), neighbors.end());
        neighbors.erase(std::unique(neighbors.begin(), neighbors.end()), neighbors.end());
    }
}

size_t MarchingCube::findThirdVertex(size_t triangleIdx, size_t v1, size_t v2) const
{
    size_t baseIdx = triangleIdx * 3;
    size_t a = m_indices[baseIdx];
    size_t b = m_indices[baseIdx + 1];
    size_t c = m_indices[baseIdx + 2];

    if ((a == v1 || a == v2) && (b == v1 || b == v2)) return c;
    if ((b == v1 || b == v2) && (c == v1 || c == v2)) return a;
    if ((c == v1 || c == v2) && (a == v1 || a == v2)) return b;

    return SIZE_MAX;
}

int MarchingCube::computeValenceCost(size_t v1, size_t v2, size_t v3, size_t v4,
    const std::vector<std::vector<size_t>>& adjacencies) const
{
    int cost = 0;
    cost += std::abs(static_cast<int>(adjacencies[v1].size()) - 6);
    cost += std::abs(static_cast<int>(adjacencies[v2].size()) - 6);
    cost += std::abs(static_cast<int>(adjacencies[v3].size()) - 6);
    cost += std::abs(static_cast<int>(adjacencies[v4].size()) - 6);
    return cost;
}

int MarchingCube::computeValenceCostAfterFlip(size_t v1, size_t v2, size_t v3, size_t v4,
    const std::vector<std::vector<size_t>>& adjacencies) const
{
    int cost = 0;
    cost += std::abs(static_cast<int>(adjacencies[v1].size()) - 1 - 6);
    cost += std::abs(static_cast<int>(adjacencies[v2].size()) - 1 - 6);
    cost += std::abs(static_cast<int>(adjacencies[v3].size()) + 1 - 6);
    cost += std::abs(static_cast<int>(adjacencies[v4].size()) + 1 - 6);
    return cost;
}

bool MarchingCube::performEdgeFlip(size_t tri1, size_t tri2, size_t v1, size_t v2, size_t v3, size_t v4)
{
    size_t idx1 = tri1 * 3;
    size_t idx2 = tri2 * 3;

    m_indices[idx1] = v1;
    m_indices[idx1 + 1] = v3;
    m_indices[idx1 + 2] = v4;

    m_indices[idx2] = v2;
    m_indices[idx2 + 1] = v4;
    m_indices[idx2 + 2] = v3;

    return true;
}

void MarchingCube::updateLocalAdjacency(size_t v1, size_t v2, size_t v3, size_t v4,
    std::vector<std::vector<size_t>>& adjacencies)
{
    auto removeFromAdjacency = [&](size_t v, size_t neighbor) {
        auto& neighbors = adjacencies[v];
        neighbors.erase(std::remove(neighbors.begin(), neighbors.end(), neighbor), neighbors.end());
    };

    removeFromAdjacency(v1, v2);
    removeFromAdjacency(v2, v1);

    adjacencies[v3].push_back(v4);
    adjacencies[v4].push_back(v3);

    adjacencies[v1].push_back(v4);
    adjacencies[v4].push_back(v1);

    adjacencies[v2].push_back(v3);
    adjacencies[v3].push_back(v2);

    for (size_t v : {v1, v2, v3, v4})
    {
        auto& neighbors = adjacencies[v];
        std::sort(neighbors.begin(), neighbors.end());
        neighbors.erase(std::unique(neighbors.begin(), neighbors.end()), neighbors.end());
    }
}

glm::vec3 MarchingCube::computeLaplacianSmooth(size_t vertexIdx, const std::vector<size_t>& neighbors) const
{
    if (neighbors.empty()) return m_vertices[vertexIdx].position;

    glm::vec3 sum(0.0f);
    for (size_t neighbor : neighbors)
    {
        sum += m_vertices[neighbor].position;
    }

    return sum / static_cast<float>(neighbors.size());
}

float MarchingCube::computeAverageEdgeLength() const
{
    if (m_vertices.empty() || m_indices.empty()) return 0.0f;

    float totalLength = 0.0f;
    int edgeCount = 0;
    std::unordered_set<Edge, EdgeHash> uniqueEdges;

    for (size_t i = 0; i < m_indices.size(); i += 3)
    {
        size_t v0 = m_indices[i];
        size_t v1 = m_indices[i + 1];
        size_t v2 = m_indices[i + 2];

        Edge edges[3] = {Edge(v0, v1), Edge(v1, v2), Edge(v2, v0)};

        for (int e = 0; e < 3; e++)
        {
            if (uniqueEdges.find(edges[e]) == uniqueEdges.end())
            {
                totalLength += computeEdgeLength(edges[e].v1, edges[e].v2);
                edgeCount++;
                uniqueEdges.insert(edges[e]);
            }
        }
    }

    return edgeCount > 0 ? totalLength / edgeCount : 0.0f;
}

const std::vector<Vertex>& MarchingCube::getVertices() const
{
    return m_vertices;
}

const std::vector<unsigned int>& MarchingCube::getIndices() const
{
    return m_indices;
}

const float MarchingCube::getAverageEdgeLength() const
{
    return m_averageEdgeLength;
}

MarchingCube::MarchingCube(unsigned int dim, int resolution, float size, float isoLevel)
{
    generateMeshInfo();
}

int MarchingCube::polygonise(GridCell grid, float isoLevel, Triangle* triangles)
{
    int cubeIndex = 0;
    glm::vec3 vertList[12];

    if (grid.val[0] < isoLevel) cubeIndex |= 1;
    if (grid.val[1] < isoLevel) cubeIndex |= 2;
    if (grid.val[2] < isoLevel) cubeIndex |= 4;
    if (grid.val[3] < isoLevel) cubeIndex |= 8;
    if (grid.val[4] < isoLevel) cubeIndex |= 16;
    if (grid.val[5] < isoLevel) cubeIndex |= 32;
    if (grid.val[6] < isoLevel) cubeIndex |= 64;
    if (grid.val[7] < isoLevel) cubeIndex |= 128;

    if (edgeTable[cubeIndex] == 0) return 0;

    if (edgeTable[cubeIndex] & 1)
    {
        vertList[0] = vertexInterp(isoLevel,grid.p[0], grid.p[1], grid.val[0], grid.val[1]);
    }
    if (edgeTable[cubeIndex] & 2)
    {
        vertList[1] = vertexInterp(isoLevel,grid.p[1], grid.p[2], grid.val[1], grid.val[2]);
    }
    if (edgeTable[cubeIndex] & 4)
    {
        vertList[2] = vertexInterp(isoLevel,grid.p[2], grid.p[3], grid.val[2], grid.val[3]);
    }
    if (edgeTable[cubeIndex] & 8)
    {
        vertList[3] = vertexInterp(isoLevel,grid.p[3], grid.p[0], grid.val[3], grid.val[0]);
    }
    if (edgeTable[cubeIndex] & 16)
    {
        vertList[4] = vertexInterp(isoLevel,grid.p[4], grid.p[5], grid.val[4], grid.val[5]);
    }
    if (edgeTable[cubeIndex] & 32)
    {
        vertList[5] = vertexInterp(isoLevel,grid.p[5], grid.p[6], grid.val[5], grid.val[6]);
    }
    if (edgeTable[cubeIndex] & 64)
    {
        vertList[6] = vertexInterp(isoLevel,grid.p[6], grid.p[7], grid.val[6], grid.val[7]);
    }
    if (edgeTable[cubeIndex] & 128)
    {
        vertList[7] = vertexInterp(isoLevel,grid.p[7], grid.p[4], grid.val[7], grid.val[4]);
    }
    if (edgeTable[cubeIndex] & 256)
    {
        vertList[8] = vertexInterp(isoLevel,grid.p[0], grid.p[4], grid.val[0], grid.val[4]);
    }
    if (edgeTable[cubeIndex] & 512)
    {
        vertList[9] = vertexInterp(isoLevel,grid.p[1], grid.p[5], grid.val[1], grid.val[5]);
    }
    if (edgeTable[cubeIndex] & 1024)
    {
        vertList[10] = vertexInterp(isoLevel,grid.p[2], grid.p[6], grid.val[2], grid.val[6]);
    }
    if (edgeTable[cubeIndex] & 2048)
    {
        vertList[11] = vertexInterp(isoLevel,grid.p[3], grid.p[7], grid.val[3], grid.val[7]);
    }

    int ntriang = 0;
    for (unsigned int i=0;triTable[cubeIndex][i]!=-1;i+=3)
    {
        triangles[ntriang].p[0] = vertList[triTable[cubeIndex][i]];
        triangles[ntriang].p[1] = vertList[triTable[cubeIndex][i+1]];
        triangles[ntriang].p[2] = vertList[triTable[cubeIndex][i+2]];
        ntriang++;
    }

    return (ntriang);
}

size_t MarchingCube::hashVertex(const glm::vec3& vertex) const
{
    const float epsilon = 0.0001f;
    size_t h1 = std::hash<float>()(std::round(vertex.x / epsilon) * epsilon);
    size_t h2 = std::hash<float>()(std::round(vertex.y / epsilon) * epsilon);
    size_t h3 = std::hash<float>()(std::round(vertex.z / epsilon) * epsilon);

    return h1 ^ (h2 << 1) ^ (h3 << 2);
}

unsigned int MarchingCube::addVertex(const glm::vec3& position, const glm::vec3& normal)
{
    auto hash = hashVertex(position);
    auto it = m_vertexMap.find(hash);

    if (it != m_vertexMap.end())
    {
        const Vertex& existing = m_vertices[it->second];
        if (glm::length(existing.position - position) < 0.0001f)
        {
            m_vertices[it->second].normal = glm::normalize(existing.normal + normal);
            return it->second;
        }
    }

    auto index = static_cast<unsigned int>(m_vertices.size());
    m_vertices.push_back({position, normal});
    m_vertexMap[hash] = index;
    return index;
}

int MarchingCube::splitLongEdges(float maxLength)
{
    if (m_vertices.empty() || m_indices.empty()) return 0;

    std::unordered_set<Edge, EdgeHash> processedEdges;
    std::vector<unsigned int> newIndices;
    std::vector<Vertex> newVertices = m_vertices;
    int splitCount = 0;

    std::unordered_map<Edge, std::vector<size_t>, EdgeHash> edgeToTriangles;
    for (size_t i = 0; i < m_indices.size(); i+=3)
    {
        size_t v0 = m_indices[i];
        size_t v1 = m_indices[i+1];
        size_t v2 = m_indices[i+2];

        edgeToTriangles[Edge(v0, v1)].push_back(i / 3);
        edgeToTriangles[Edge(v1, v2)].push_back(i / 3);
        edgeToTriangles[Edge(v2, v0)].push_back(i / 3);
    }

    std::vector<std::pair<Edge, float>> edgesToSplit;
    for (const auto& [edge, triangles] : edgeToTriangles)
    {
        if (triangles.size() != 2) continue; 

        float length = computeEdgeLength(edge.v1, edge.v2);
        if (length > maxLength)
        {
            edgesToSplit.emplace_back(edge, length);
        }

    }

    std::sort(edgesToSplit.begin(), edgesToSplit.end(),
        [](const auto& a, const auto& b)
        {return a.second > b.second;});

    for (const auto& [edge, length] : edgesToSplit)
    {
        if (processedEdges.contains(edge))
        {
            continue;
        }

        Vertex newVertex;
        newVertex.position = (m_vertices[edge.v1].position + m_vertices[edge.v2].position) * 0.5f;
        newVertex.normal = glm::normalize(m_vertices[edge.v1].normal + m_vertices[edge.v2].normal);

        size_t newIndex = newVertices.size();
        newVertices.push_back(newVertex);

        const auto& triangles = edgeToTriangles[edge];
        for (auto triIdx : triangles)
        {
            size_t baseIdx = triIdx * 3;
            size_t v0 = m_indices[baseIdx];
            size_t v1 = m_indices[baseIdx + 1];
            size_t v2 = m_indices[baseIdx + 2];

            if ((v0 == edge.v1 && v1 == edge.v2) || (v1 == edge.v1 && v0 == edge.v2))
            {
                size_t v3 = v2;
                newIndices.push_back(v0); newIndices.push_back(newIndex); newIndices.push_back(v3);
                newIndices.push_back(newIndex); newIndices.push_back(v1); newIndices.push_back(v3);
            }
            else if ((v1 == edge.v1 && v2 == edge.v2) || (v2 == edge.v1 && v1 == edge.v2))
            {
                size_t v3 = v0;
                newIndices.push_back(v1); newIndices.push_back(newIndex); newIndices.push_back(v3);
                newIndices.push_back(newIndex); newIndices.push_back(v2); newIndices.push_back(v3);
            }
            else 
            {
                size_t v3 = v1;
                newIndices.push_back(v2); newIndices.push_back(newIndex); newIndices.push_back(v3);
                newIndices.push_back(newIndex); newIndices.push_back(v0); newIndices.push_back(v3);
            }
        }

        processedEdges.insert(edge);
        splitCount++;
    }

    for (size_t i = 0; i < m_indices.size(); i += 3)
    {
        Edge e1(m_indices[i], m_indices[i + 1]);
        Edge e2(m_indices[i + 1], m_indices[i + 2]);
        Edge e3(m_indices[i + 2], m_indices[i]);

        if (!processedEdges.contains(e1) &&
            !processedEdges.contains(e2) &&
            !processedEdges.contains(e3))
        {
            newIndices.push_back(m_indices[i]);
            newIndices.push_back(m_indices[i + 1]);
            newIndices.push_back(m_indices[i + 2]);
        }
    }

    m_vertices = std::move(newVertices);
    m_indices = std::move(newIndices);

    return splitCount;
}

int MarchingCube::collapseShortEdges(float collapseThreshold, float preserveThreshold)
{
    if (m_vertices.empty() || m_indices.empty()) return 0;

    std::cout << "Collapsing short edges: collapseThreshold = " << collapseThreshold
    << ", preserveThreshold = " << preserveThreshold << std::endl;

    std::unordered_map<int, std::unordered_set<int>> vertexToTriangles;
    std::unordered_set<int> boundaryVertices;
    buildTopologyInfo(vertexToTriangles, boundaryVertices);

    std::vector<std::pair<int, int>> edgesToCollapse;
    std::unordered_set<int64_t> processedEdges;

    for (size_t i = 0; i < m_indices.size(); i += 3)
    {
        int v0 = m_indices[i];
        int v1 = m_indices[i + 1];
        int v2 = m_indices[i + 2];

        processEdgeForCollapse(v0, v1, collapseThreshold, preserveThreshold,
                              boundaryVertices, edgesToCollapse, processedEdges);
        processEdgeForCollapse(v1, v2, collapseThreshold, preserveThreshold,
                              boundaryVertices, edgesToCollapse, processedEdges);
        processEdgeForCollapse(v2, v0, collapseThreshold, preserveThreshold,
                              boundaryVertices, edgesToCollapse, processedEdges);
    }

    std::sort(edgesToCollapse.begin(), edgesToCollapse.end(),
        [&](const auto& a, const auto& b) {
            float lenA = glm::length(m_vertices[a.first].position - m_vertices[a.second].position);
            float lenB = glm::length(m_vertices[b.first].position - m_vertices[b.second].position);
            return lenA < lenB;
        });

    std::unordered_map<int, int> vertexRemap;
    std::vector<bool> vertexUsed(m_vertices.size(), false);
    std::vector<Vertex> newVertices;
    std::vector<unsigned int> newIndices;
    int collapseCount = 0;

    for (const auto& [v0, v1] : edgesToCollapse)
    {
        if (vertexRemap.find(v0) != vertexRemap.end() ||
            vertexRemap.find(v1) != vertexRemap.end())
        {
            continue;
        }

        if (!canCollapseEdge(v0, v1))
        {
            continue;
        }

        Vertex newVertex = computeMergedVertex(v0, v1, vertexToTriangles);
        int newIndex = static_cast<int>(newVertices.size());
        newVertices.push_back(newVertex);

        vertexRemap[v0] = newIndex;
        vertexRemap[v1] = newIndex;
        vertexUsed[v0] = true;
        vertexUsed[v1] = true;

        collapseCount++;
    }

    for (size_t i = 0; i < m_vertices.size(); i++)
    {
        if (!vertexUsed[i])
        {
            vertexRemap[i] = static_cast<int>(newVertices.size());
            newVertices.push_back(m_vertices[i]);
        }
    }

    for (size_t i = 0; i < m_indices.size(); i += 3)
    {
        int v0 = vertexRemap[m_indices[i]];
        int v1 = vertexRemap[m_indices[i + 1]];
        int v2 = vertexRemap[m_indices[i + 2]];

        if (v0 == v1 || v1 == v2 || v0 == v2)
        {
            continue; 
        }

        glm::vec3 normal = glm::normalize(glm::cross(
            newVertices[v1].position - newVertices[v0].position,
            newVertices[v2].position - newVertices[v0].position));

        if (glm::length(normal) < 0.001f)
        {
            continue;
        }

        newIndices.push_back(v0);
        newIndices.push_back(v1);
        newIndices.push_back(v2);
    }

    m_vertices = std::move(newVertices);
    m_indices = std::move(newIndices);

    return collapseCount;
}

int MarchingCube::equalizeValences()
{
    if (m_vertices.empty() || m_indices.empty()) return 0;

    std::unordered_set<Edge, EdgeHash> edges;
    std::vector<std::vector<size_t>> adjacencies(m_vertices.size());
    std::unordered_map<Edge, std::vector<size_t>, EdgeHash> edgeToTriangles;

    buildEdgeInfo(edges, adjacencies, edgeToTriangles);

    std::vector<FlipCandidate> candidates;
    int flipCount = 0;

    for (const auto& edge : edges)
    {
        auto it = edgeToTriangles.find(edge);
        if (it == edgeToTriangles.end() || it->second.size() != 2)
        {
            continue; 
        }

        size_t tri1 = it->second[0];
        size_t tri2 = it->second[1];

        size_t v1 = edge.v1, v2 = edge.v2;
        size_t v3 = findThirdVertex(tri1, v1, v2);
        size_t v4 = findThirdVertex(tri2, v1, v2);

        if (v3 == SIZE_MAX || v4 == SIZE_MAX)
        {
            continue;
        }

        if (!canFlipEdge(v1, v2, v3, v4))
        {
            continue;
        }

        int currentValence = computeValenceCost(v1, v2, v3, v4, adjacencies);
        int newValence = computeValenceCostAfterFlip(v1, v2, v3, v4, adjacencies);

        if (newValence < currentValence - 1) 
        {
            if (performEdgeFlip(tri1, tri2, v1, v2, v3, v4))
            {
                flipCount++;

                updateLocalAdjacency(v1, v2, v3, v4, adjacencies);
            }
        }
    }

    return flipCount;
}

void MarchingCube::tangentialRelaxation(int innerIterations, float lambda, bool preserveBoundary)
{
   if (m_vertices.empty() || m_indices.empty()) return;

    std::vector<bool> isBoundaryVertex;
    if (preserveBoundary)
    {
        detectBoundaryVertices(isBoundaryVertex);
    }
    else
    {
        isBoundaryVertex.assign(m_vertices.size(), false);
    }

    std::vector<std::vector<size_t>> adjacency(m_vertices.size());
    for (size_t i = 0; i < m_indices.size(); i += 3)
    {
        size_t v0 = m_indices[i];
        size_t v1 = m_indices[i + 1];
        size_t v2 = m_indices[i + 2];

        adjacency[v0].push_back(v1);
        adjacency[v0].push_back(v2);
        adjacency[v1].push_back(v0);
        adjacency[v1].push_back(v2);
        adjacency[v2].push_back(v0);
        adjacency[v2].push_back(v1);
    }

    for (auto& neighbors : adjacency)
    {
        std::sort(neighbors.begin(), neighbors.end());
        neighbors.erase(std::unique(neighbors.begin(), neighbors.end()), neighbors.end());
    }

    for (int iter = 0; iter < innerIterations; iter++)
    {
        std::vector<Vertex> newPositions = m_vertices;
        float maxDisplacement = 0.0f;

        for (size_t i = 0; i < m_vertices.size(); i++)
        {
            if (adjacency[i].empty()) continue;

            if (isBoundaryVertex[i])
            {
                float boundaryLambda = lambda * 0.3f;
                glm::vec3 avgPos = computeLaplacianSmooth(i, adjacency[i]);
                glm::vec3 displacement = avgPos - m_vertices[i].position;

                glm::vec3 normal = glm::normalize(m_vertices[i].normal);
                glm::vec3 tangential = displacement - glm::dot(displacement, normal) * normal;

                newPositions[i].position += boundaryLambda * tangential;
            }
            else
            {
                glm::vec3 avgPos = computeLaplacianSmooth(i, adjacency[i]);
                glm::vec3 displacement = avgPos - m_vertices[i].position;

                glm::vec3 normal = glm::normalize(m_vertices[i].normal);
                glm::vec3 tangential = displacement - glm::dot(displacement, normal) * normal;

                float adaptiveLambda = lambda * (6.0f / adjacency[i].size());
                adaptiveLambda = glm::clamp(adaptiveLambda, 0.1f, 1.0f);

                newPositions[i].position += adaptiveLambda * tangential;
            }

            float dispLength = glm::length(newPositions[i].position - m_vertices[i].position);
            if (dispLength > maxDisplacement)
            {
                maxDisplacement = dispLength;
            }
        }

        for (size_t i = 0; i < m_vertices.size(); i++)
        {
            m_vertices[i].position = newPositions[i].position;
        }

        if (maxDisplacement < 0.0001f)
        {
            break;
        }
    }

    updateNormals();
}

void MarchingCube::detectBoundaryVertices(std::vector<bool>& isBoundary)
{
    if (m_vertices.empty() || m_indices.empty()) return;

    isBoundary.assign(m_vertices.size(), false);

    std::unordered_map<uint64_t, int> edgeCount;

    for (size_t i = 0; i < m_indices.size(); i += 3) {
        size_t v0 = m_indices[i];
        size_t v1 = m_indices[i + 1];
        size_t v2 = m_indices[i + 2];

        uint64_t e0 = (v0 < v1) ? (v0 * m_vertices.size() + v1) : (v1 * m_vertices.size() + v0);
        uint64_t e1 = (v1 < v2) ? (v1 * m_vertices.size() + v2) : (v2 * m_vertices.size() + v1);
        uint64_t e2 = (v2 < v0) ? (v2 * m_vertices.size() + v0) : (v0 * m_vertices.size() + v2);

        edgeCount[e0]++;
        edgeCount[e1]++;
        edgeCount[e2]++;
    }

    for (size_t i = 0; i < m_indices.size(); i += 3) {
        size_t v0 = m_indices[i];
        size_t v1 = m_indices[i + 1];
        size_t v2 = m_indices[i + 2];

        uint64_t e0 = (v0 < v1) ? (v0 * m_vertices.size() + v1) : (v1 * m_vertices.size() + v0);
        uint64_t e1 = (v1 < v2) ? (v1 * m_vertices.size() + v2) : (v2 * m_vertices.size() + v1);
        uint64_t e2 = (v2 < v0) ? (v2 * m_vertices.size() + v0) : (v0 * m_vertices.size() + v2);

        if (edgeCount[e0] == 1) {
            isBoundary[v0] = true;
            isBoundary[v1] = true;
        }
        if (edgeCount[e1] == 1) {
            isBoundary[v1] = true;
            isBoundary[v2] = true;
        }
        if (edgeCount[e2] == 1) {
            isBoundary[v2] = true;
            isBoundary[v0] = true;
        }
    }
}

void MarchingCube::updateNormals()
{
    if (m_vertices.empty() || m_indices.empty()) return;

    for (auto& vertex : m_vertices)
    {
        vertex.normal = glm::vec3(0.0f);
    }

    for (size_t i = 0; i < m_indices.size(); i += 3)
    {
        int i0 = m_indices[i];
        int i1 = m_indices[i + 1];
        int i2 = m_indices[i + 2];

        glm::vec3 v0 = m_vertices[i0].position;
        glm::vec3 v1 = m_vertices[i1].position;
        glm::vec3 v2 = m_vertices[i2].position;

        glm::vec3 edge1 = v1 - v0;
        glm::vec3 edge2 = v2 - v0;
        glm::vec3 faceNormal = glm::normalize(glm::cross(edge1, edge2));

        m_vertices[i0].normal += faceNormal;
        m_vertices[i1].normal += faceNormal;
        m_vertices[i2].normal += faceNormal;
    }
}

void MarchingCube::projectToSurface()
{
    if (m_vertices.empty()) return;

    for (auto& vertex : m_vertices)
    {
        float distance = glm::length(vertex.position);
        if (distance > 0.001f)
        {
            vertex.position = glm::normalize(vertex.position);
            vertex.normal = vertex.position; 
        }
    }
}

bool MarchingCube::canCollapseEdge(size_t v1, size_t v2) const 
{
    if (v1 == v2) return false;

    const size_t nVerts = m_vertices.size();
    const size_t nTris = m_indices.size() / 3;

    std::vector<std::vector<size_t>> vertToTris(nVerts);
    for (size_t i = 0; i < nTris; ++i) {
        size_t i0 = m_indices[3 * i];
        size_t i1 = m_indices[3 * i + 1];
        size_t i2 = m_indices[3 * i + 2];
        vertToTris[i0].push_back(i);
        vertToTris[i1].push_back(i);
        vertToTris[i2].push_back(i);
    }

    std::vector<size_t> sharedTris;
    for (size_t t : vertToTris[v1]) {
        const size_t i0 = m_indices[3 * t];
        const size_t i1 = m_indices[3 * t + 1];
        const size_t i2 = m_indices[3 * t + 2];
        if ((i0 == v2 || i1 == v2 || i2 == v2))
            sharedTris.push_back(t);
    }
   
    if (sharedTris.size() != 1 && sharedTris.size() != 2)
        return false;   
    bool isBoundaryEdge = (sharedTris.size() == 1);

    std::unordered_set<size_t> neighV1, neighV2;
    for (size_t t : vertToTris[v1]) {
        const size_t i0 = m_indices[3 * t];
        const size_t i1 = m_indices[3 * t + 1];
        const size_t i2 = m_indices[3 * t + 2];
        if (i0 != v1) neighV1.insert(i0);
        if (i1 != v1) neighV1.insert(i1);
        if (i2 != v1) neighV1.insert(i2);
    }
    for (size_t t : vertToTris[v2]) {
        const size_t i0 = m_indices[3 * t];
        const size_t i1 = m_indices[3 * t + 1];
        const size_t i2 = m_indices[3 * t + 2];
        if (i0 != v2) neighV2.insert(i0);
        if (i1 != v2) neighV2.insert(i1);
        if (i2 != v2) neighV2.insert(i2);
    }

    std::vector<size_t> commonNeighbors;
    for (size_t w : neighV1) {
        if (w != v2 && neighV2.count(w))
            commonNeighbors.push_back(w);
    }

    if (!isBoundaryEdge && commonNeighbors.size() != 2)
        return false;
   
    if (isBoundaryEdge && commonNeighbors.size() != 1)
        return false;

    auto isVertexOnBoundary = [&](size_t v) -> bool {
        std::unordered_set<size_t> edgeNeighbors;
        for (size_t t : vertToTris[v]) {
            const size_t i0 = m_indices[3 * t];
            const size_t i1 = m_indices[3 * t + 1];
            const size_t i2 = m_indices[3 * t + 2];
            if (i0 != v) edgeNeighbors.insert(i0);
            if (i1 != v) edgeNeighbors.insert(i1);
            if (i2 != v) edgeNeighbors.insert(i2);
        }
        
        std::unordered_map<size_t, int> edgeCount;
        for (size_t t : vertToTris[v]) {
            const size_t i0 = m_indices[3 * t];
            const size_t i1 = m_indices[3 * t + 1];
            const size_t i2 = m_indices[3 * t + 2];
            if (i0 != v) edgeCount[i0]++;
            if (i1 != v) edgeCount[i1]++;
            if (i2 != v) edgeCount[i2]++;
        }
        for (auto& p : edgeCount) {
            if (p.second == 1) return true; 
        }
        return false;
        };

    bool v1Boundary = isVertexOnBoundary(v1);
    bool v2Boundary = isVertexOnBoundary(v2);

    if (isBoundaryEdge) 
    {
        if (!v1Boundary || !v2Boundary) return false;
    }
    else 
    {     
        if (v1Boundary || v2Boundary) return false;
    }

    glm::vec3 newPos = (m_vertices[v1].position + m_vertices[v2].position) * 0.5f;
   
    std::unordered_set<size_t> affectedTris;
    for (size_t t : vertToTris[v1]) affectedTris.insert(t);
    for (size_t t : vertToTris[v2]) affectedTris.insert(t);
    for (size_t t : sharedTris) affectedTris.erase(t); 

    auto triArea = [](const glm::vec3& a, const glm::vec3& b, const glm::vec3& c) -> float {
        glm::vec3 ab = b - a;
        glm::vec3 ac = c - a;
        return 0.5f * glm::length(glm::cross(ab, ac));
        };

    const float minArea = 1e-6f;
    const float minDot = 0.1f;  

    for (size_t t : affectedTris) 
    {
        size_t i0 = m_indices[3 * t];
        size_t i1 = m_indices[3 * t + 1];
        size_t i2 = m_indices[3 * t + 2];

        glm::vec3 p0 = (i0 == v1 || i0 == v2) ? newPos : m_vertices[i0].position;
        glm::vec3 p1 = (i1 == v1 || i1 == v2) ? newPos : m_vertices[i1].position;
        glm::vec3 p2 = (i2 == v1 || i2 == v2) ? newPos : m_vertices[i2].position;

        if (p0 == p1 || p1 == p2 || p2 == p0) return false;

        float area = triArea(p0, p1, p2);
        if (area < minArea) return false;

        glm::vec3 oldNormal = glm::normalize(glm::cross(
            m_vertices[i1].position - m_vertices[i0].position,
            m_vertices[i2].position - m_vertices[i0].position));
        glm::vec3 newNormal = glm::normalize(glm::cross(p1 - p0, p2 - p0));
        if (glm::dot(oldNormal, newNormal) < minDot) return false;
    }

    std::map<std::pair<size_t, size_t>, int> newEdgeCount;
    for (size_t t : affectedTris) 
    {
        size_t i0 = m_indices[3 * t];
        size_t i1 = m_indices[3 * t + 1];
        size_t i2 = m_indices[3 * t + 2];
       
        auto remap = [&](size_t v) -> size_t {
            if (v == v1 || v == v2) return nVerts; 
            return v;
            };
        size_t a = remap(i0), b = remap(i1), c = remap(i2);
        if (a == b || b == c || c == a) continue; 
        std::pair<size_t, size_t> e1 = { std::min(a,b), std::max(a,b) };
        std::pair<size_t, size_t> e2 = { std::min(b,c), std::max(b,c) };
        std::pair<size_t, size_t> e3 = { std::min(c,a), std::max(c,a) };
        newEdgeCount[e1]++; newEdgeCount[e2]++; newEdgeCount[e3]++;
    }
    for (auto& p : newEdgeCount)
    {
        if (p.second > 2) return false; 
    }

    return true;
}
