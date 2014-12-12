/* Filename: grid_routing.h
 * Auther: Ping Ren
 * Date: 12.10.2014
 * Description: grid_routing is a class inherited form class Grid
 */
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
            ProblemObject* problem_object;
            vector<Path*> paths;
	
            void print();
            void set_path_block(int i,int block_cost,bool set);
	    void reset_map(int node_initial_cost,int block_cost);
            
            
        public:
            /* Constructors/Destructors */
            grid_routing(ProblemObject* problem_object);
            ~grid_routing();
            
            void initialize_map(int node_cost, int block_cost);
            
              
            void Lee(bool set);   
            
            void Two_bits(bool set);    
            
            void Three_bits(bool set);    
            
            
            void Rubin_DFS(bool set);   
	    void Rubin_BFS(bool set);
            
            
            //void Korn(bool set, double overpull);    
            
            void Hadlock(bool set);   
            

	    
    };
}

#endif  //_GRID_ROUTING_H_
