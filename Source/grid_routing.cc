#include "../Headers/grid.h"
#include "../Headers/edge.h"
#include "../Headers/claim.h"
#include "../Headers/problem_object.h"
#include <math.h>

using Utilities::map_Lee;
using Utilities::map_Rubin;

Utilities::grid_routing::grid_routing(ProblemObject* problem_object)
{
        this ->problem_object = problem_object;
}

Utilities::grid_routing::~grid_routing(){
}


Utilities::grid_routing::Lee_algorithm1()             /* no intersections */
{
        map_Lee *map_Lee = new map_Lee(problem_object);
        paths = map_Lee -> Lee1();
        map_Lee ->print();
}

Utilities::grid_routing::Lee_algorithm2()        /* unlimited intersections */
{
        map_Lee *map_Lee = new map_Lee(problem_object);
        paths = map_Lee -> Lee2();
        map_Lee ->print();
}

Utilities::grid_routing::Rubin_algorithm1()
