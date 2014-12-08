#ifndef _GRID_ROUTING_H_
#define _GRID_ROUTING_H_

#include "node.h"
#include "path.h"
#include "problem_object.h"
#include <vector>
#include <iostream>

using std::cerr;
using std::endl;
using std::string;
using Utilities::Node;
using Utilities::Path;

namespace Utilities{
    class grid_routing : public Grid{
        private:
            vector<vector<Node*> > grid;
            int num_connections;
            ProblemObject *problem_object;
            vector<Path*> paths;
	
            void print();
            
            
        public:
            /* Constructors/Destructors */
            grid_routing(ProblemObject* problem_object);
            ~grid_routing();
            
            void initialize_map();
            
              
            void Lee_algorithm1();   /*unlimited intersections*/
            
            void Two_bit1();    /*unlimited intersections*/
            
            void There_bit1();    /*unlimited intersections*/
            
            
            void Rubin1();   /*unlimited intersections*/
            
            
            //void Korn_algorithm1();    /*no intersections*/
            //void Korn_algorithm2();    /*unlimited intersections*/
            
            void Hadlock1();   /*no intersections*/
            //void Hadlock_algorithm2();   /*unlimited intersections*/

	    
    };
}

#endif  //_GRID_ROUTING_H_
