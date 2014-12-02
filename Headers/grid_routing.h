#ifndef _GRID_ROUTING_H
#define _GRID_ROUTING_H

#include "node.h"
#include "path.h"
#include "problem_object.h"
#include <vector>

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
            vector<Path*> paths;
            
            void print();
            
        public:
            /* Constructors/Destructors */
            grid_routing(ProblemObject* problem_object);
            ~grid_routing();
            
            void Lee_algorithm1(ProblemObject* problem_object);  /*no intersections*/
            void Lee_algorithm2ProblemObject* problem_object);   /*ultimate intersections*/
            
            void 2bit_Lee1(ProblemObject* problem_object);    /*no intersections*/
            void 2bit_Lee2(ProblemObject* problem_object);    /*ultimate intersections*/
            void 3bit_Lee1(ProblemObject* problem_object);    /*no intersections*/
            void 3bit_Lee2(ProblemObject* problem_object);    /*ultimate intersections*/
            
            
            void Rubin(ProblemObject* problem_object);
            void Korn(ProblemObject* problem_object);
            void Hadlock(ProblemObject* problem_object);
    }
}

#endif  //_GRID_ROUTING_H
