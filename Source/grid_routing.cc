#include "../Headers/grid.h"
#include "../Headers/edge.h"
#include "../Headers/claim.h"
#include "../Headers/problem_object.h"
#include <math.h>



void Utilities::grid_routing::initialize_map()    /* initialzie the map with block */
{
      this->num_connections = problem_object->get_connections().size();
      int height = problem_object->get_height();
      int width = problem_object->get_width();
      for(int y = 0; y < height; y++) {
		  vector<Node*> temp_row;
	      for(int x = 0; x < width; x++) {
			  Node* new_node = new Node(x,y,0);
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
}


void Utilities::grid_routing::grid_routing(ProblemObject* problem_object)
{
        this ->problem_object = problem_object;
}

void Utilities::grid_routing::~grid_routing(){
}





void Utilities::grid_routing::Lee_algorithm1()             /* no intersections */
{
    initialize_map();
    vector<Path*> paths;
    vector<Connection> source_sink_connections = this->problem_object -> get_connections();
    int num_paths = this -> get_num_connections();
    for(int i = 0; i < num_paths ; i++)
    {
        Path* new_path = new Path();
        int source_x = source_sink_connections.at(i).source.x;
        int source_y = source_sink_connections.at(i).source.y;
        int sink_x = source_sink_connections.at(i).sink.x;
        int sink_y = source_sink_connections.at(i).sink.y;
        int height = this -> get_height();
        int width = this -> get_width();
        //grid.at(source_y).at(source_x) -> set_cost(0);     // COST AT SOURCE=0
        int distance = 0;     // STEPS FROM SOURCE
        
        // EXPANSION STARTS HERE
        bool flag = true;     // FLAG = FALSE WHEN SINK IS REACHED
        while(flag)
        {
            distance = distance + 1;
            for(int j = -distance ; j< distance; j++)
            {
                int l = distance - abs(j);
                if(source_y + l>=0 && source_y + l<= height-1 && source_x + j >=0 && source_x + j <= width -1)
                {
                    if(grid.at(source_y + l).at(source_x + j)-> get_cost() == 0)     //FOUND EMPTY NODE
                        grid.at(source_y + l).at(source_x + j) -> set_cost(distance);      //SET COST
                    if(grid.at(source_y + l).at(source_x + j)-> get_cost() == -3)    //STOP WHEN SINK IS REACHED
                        flag = false;
                }
                if(source_y - l>=0 && source_y - l<= height-1 && source_x - j >=0 && source_x - j <= width -1)
                {
                    if(grid.at(source_y - l).at(source_x - j)-> get_cost() == 0)     //FOUND EMPTY NODE
                        grid.at(source_y - l).at(source_x - j) -> set_cost(distance);      //SET COST
                    if(grid.at(source_y - l).at(source_x - j)-> get_cost() == -3)    //STOP WHEN SINK IS REACHED
                        flag = false;
                }
            }
        }
        // EXPANSION ENDS HERE
        
        // BACKTRACING STARTS HERE
        int x = sink_x;
        int y = sink_y;
        for(int step = distance; step > 0 ; step--)     // MOVE TO SOURCE IN THE DECREASING ORDER OF DISTANCE
	{
	    bool move = false;    // WHENEVER MAKE A MOVE
	    
	    if(x+1 >= 0 && x+1 <= width-1)     // RIGHT STEP IS NOT OUT OF RANGE
	    {
	        if(grid.at(y).at(x+1)-> get_cost() == step - 1)    // COST OF THIS STEP DECREASE BY 1
	        {
	            Point head(x ,y);
                    x++;
                    Point tail(x ,y);
                    move = true;     // AVOID FURTHER MOVE IN THE SAME STEP
                    PathSegment* path_segment = new PathSegment(head, tail);
                    new_path -> add_segment(path_segment);      // ADD CURRENT MOVE TO NEW PATH
	        }
	    }

	    if(x-1 >= 0 && x-1 <= width-1 && !move)     // LEFT STEP IS NOT OUT OF RANGE
	    {
	        if(grid.at(y).at(x-1)-> get_cost() == step - 1)    // COST OF THIS STEP DECREASE BY 1
	        {
	            Point head(x ,y);
	            x--;
	            Point tail(x ,y);
	            move = true;     // AVOID FURTHER MOVE IN THE SAME STEP
	            PathSegment* path_segment = new PathSegment(head, tail);
	            new_path -> add_segment(path_segment);      // ADD CURRENT MOVE TO NEW PATH
	        }
	    }

	    if(y+1 >= 0 && y+1 <= height-1 && !move)     // UP STEP IS NOT OUT OF RANGE
	    {
	        if(grid.at(y+1).at(x)-> get_cost() == step - 1)    // COST OF THIS STEP DECREASE BY 1
	        {
	            Point head(x ,y);
	            y++;
	            Point tail(x ,y);
	            move = true;     // AVOID FURTHER MOVE IN THE SAME STEP
	            PathSegment* path_segment = new PathSegment(head, tail);
	            new_path -> add_segment(path_segment);      // ADD CURRENT MOVE TO NEW PATH
	        }
	    }
		
	    if(y-1 >= 0 && y-1 <= height-1 && !move)     // DOWN STEP IS NOT OUT OF RANGE
	    {
	        if(grid.at(y-1).at(x)-> get_cost() == step - 1)    // COST OF THIS STEP DECREASE BY 1
	        {
	            Point head(x ,y);
	            y--;
	            Point tail(x ,y);
	            move = true;     // AVOID FURTHER MOVE IN THE SAME STEP
	            PathSegment* path_segment = new PathSegment(head, tail);
	            new_path -> add_segment(path_segment);      // ADD CURRENT MOVE TO NEW PATH
	        }
	    }
	}
	// BACK TRACING ENDS HERE
	
        paths.push_back(new_path);
    }
    print();
}




void Utilities::grid_routing::Lee_algorithm2()        /* unlimited intersections */
{
        
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

