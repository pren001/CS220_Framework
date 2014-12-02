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
            
        public:
            /* Constructors/Destructors */
            grid_routing(ProblemObject* problem_object);
            ~grid_routing();
            
            void Lee_algorithm(ProblemObject* problem_object);
    }
}

#endif  //_GRID_ROUTING_H
