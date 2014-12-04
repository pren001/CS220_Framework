#include "../Headers/grid.h"
#include "../Headers/edge.h"
#include "../Headers/claim.h"
#include "../Headers/problem_object.h"
#include <math.h>

using Utilities::map_Lee;
using Utilities::map_Rubin;

void Utilities::grid_routing::initialize_map();
{
      this->num_connections = problem_object->get_connections().size();
      int height = problem_object->get_height();
      int width = problem_object->get_width();
      for(int y = 0; y < height; y++) {
		  vector<Node*> temp_row;
	      for(int x = 0; x < width; x++) {
			  Node* new_node = new Node(x,y);
			  if (x > 0) {
				  Edge* east = new Edge(new_node,temp_row.at(temp_row.size()-1));
				  new_node->add_connection(east);
			  }
			  if (y > 0) {
				  Edge* north = new Edge(new_node,grid.at(y-1).at(x));
				  new_node->add_connection(north);
			  }
			  temp_row.push_back(new_node);
          }
          this->grid.push_back(temp_row);
      }
      
      vector<Blocker> blockers = this->problem_object -> get_blockers();
      int block_num = problem_object -> get_blockers().size();
      for(int i = 0; i< block_num; i++)
      {
      		unsigned int block_width = blockers.at(i).width;
      		unsigned int block_height = blockers.at(i).height;
      		int block_x = blockers.at(i).location.x;
      		int block_y = blockers.at(i).location.y;
      		for( unsigned int h = block_x; h < block_x + block_height ; h++ )
      		{
      			for(unsigned int w = block_y; w < block_y + block_width ; w++)
      			{
      				if(h>=0 &&h<height && w>=0 && w<width) grid.at(h).at(w) -> set_cost(-1);
      			}
      		}
      }
      
      vector<Connection> source_sink_connections = this->problem_object -> get_connections();
      num_connections = problem_object -> get_connections().size(); ///////第一题晚上改错了，grid本身有num_connections，不需要额外设
      for(int i = 0; i < num_connections ; i++)
      {
          int source_x = source_sink_connections.at(i).source.x;
          int source_y = source_sink_connections.at(i).source.y;
          int sink_x = source_sink_connections.at(i).sink.x;
          int sink_y = source_sink_connections.at(i).sink.y;
          if(source_x>=0 && source_x<width && source_y >=0 && source_y<height && sink_x>=0 &&sink_x<width && sink_y>=0&&sink_y<height)
          {
          	grid.at(source_y).at(source_x) -> set_cost(-2);
          	grid.at(sink_y).at(sink_x) -> set_cost(-3);
          }
      }
}


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

