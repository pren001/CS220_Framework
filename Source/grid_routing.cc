#include "../Headers/grid.h"
#include "../Headers/edge.h"
#include "../Headers/claim.h"
#include "../Headers/problem_object.h"
#include <math.h>

using Utilities::map_Lee;
using Utilities::map_Rubin;

void Utilities::grid_routing::grid_routing(ProblemObject* problem_object)
{
        this ->problem_object = problem_object;
}

void Utilities::grid_routing::~grid_routing(){
}


void Utilities::grid_routing::Lee_algorithm1()             /* no intersections */
{
        map_Lee *map_Lee = new map_Lee(problem_object);
        paths = map_Lee -> Lee1();
        map_Lee ->print();
}

void Utilities::grid_routing::Lee_algorithm2()        /* unlimited intersections */
{
        map_Lee *map_Lee = new map_Lee(problem_object);
        paths = map_Lee -> Lee2();
        map_Lee ->print();
}

void Utilities::grid_routing::Rubin_algorithm1()
{
}

void Utilities::gird_routing::print()
{
        if(paths.empty())
        {
                std::cout << "There is no path."
                return;
        }
        for(unsigned i = 0; i < paths.size(); i++) {
		cout << "Path " << i << ": ";
		paths.at(i)->print();
		Path* temp = paths.at(i);
		delete temp;
	}
	paths.clear();
}

