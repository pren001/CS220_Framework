
#include "../Headers/grid.h"
#include "../Headers/problem_object.h"
#include "../Headers/grid_routing.h"
#include <time.h>
#include <cstdlib>
#include <iostream>

using std::cerr;
using std::cout;
using std::endl;

int main(int argc,char* argv[]) {

	// DO NOT CHANGE THIS SECTION OF CODE
	if(argc < 2) { 
		cout << "Usage: ./grid_router <test_file>" << endl; 
		exit(1);
	}
	Utilities::ProblemObject* first_problem = new Utilities::ProblemObject(std::string(argv[1]));
	// EDIT FROM HERE DOWN

	//Create your problem map object (in our example, we use a simple grid, you should create your own)
	Utilities::grid_routing g(first_problem);

	/*
	Note: we do not take into account the connections or blockers that exist in the Project Object
	You should be accouting for these in your problem map objects (you should not be using Grid). You
	should be using both the Node and Edge classes as the background for whatever problem map object
	you create.
	*/

	/*
	Run your algorithm after creating your problem map object. You should be returning from your algorithm 
	either a Path or a Netlist

	Path: a series of straight line segments, with a single source and a single sink
	Netlist: a series of stright line segments, with a single source and more than one sink
	*/
	
	
	//g.Lee_algorithm();
	//g.Two_bits();
	//g.Three_bits();
	//g.Rubin1();
	//g.Hadlock();
    
    int n = atoi(argv[2]);
    switch(n){
        case 0:
            g.Lee();     
            break;
        case 1:
            g.Three_bits();      
            break;
        case 2:
            g.Two_bits();       
            break;
	case 3:
            g.Rubin_DFS();
	    break;
	case 4:
            g.Rubin_BFS();
	    break;
        case 5:
            g.Hadlock();
        
    }
    
    
    

	
	
	//vector<Path*> paths = g.test_algorithm();
	
	//Print the paths/netlists that you return from your algorithm
	/*
	for(unsigned i = 0; i < paths.size(); i++) {
		cout << "Path " << i << ": ";
		paths.at(i)->print();
		Path* temp = paths.at(i);
		delete temp;
	}

	paths.clear();
	*/
	

	delete first_problem;

	return 0;
}
