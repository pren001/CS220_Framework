#ifndef _GRID_ROUTING_H
#define _GRID_ROUTING_H

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
    Class grid_routing {
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
            
            void Lee_algorithm1();  /*no intersections*/
            void Lee_algorithm2();   /*ultimate intersections*/
            
            void 2bit_Lee1();    /*no intersections*/
            void 2bit_Lee2();    /*ultimate intersections*/
            void 3bit_Lee1();    /*no intersections*/
            void 3bit_Lee2();    /*ultimate intersections*/
            
            
            void Rubin_algorithm1();   /*no intersections*/
            void Rubin_algorithm2();   /*ultimate intersections*/
            void Korn_algorithm1();    /*no intersections*/
            void Korn_algorithm2();    /*ultimate intersections*/
            void Hadlock_algorithm1();   /*no intersections*/
            void Hadlock_algorithm2();   /*ultimate intersections*/
    }
}

#endif  //_GRID_ROUTING_H
