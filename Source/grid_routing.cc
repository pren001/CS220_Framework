#include "../Headers/grid.h"
#include "../Headers/edge.h"
#include "../Headers/claim.h"
#include "../Headers/problem_object.h"
#include "../Headers/grid_routing.h"
#include <math.h>


Utilities::grid_routing::grid_routing(ProblemObject* problem_object)
{
        this->problem_object = problem_object;
	
}

Utilities::grid_routing::~grid_routing(){
      int width = this->get_width();
      int height = this ->get_height();
      for(int x = 0; x < width; x++) {
            for(int y = 0; y < width; y++) {
                  delete grid.at(y).at(x);
            }
      }
}



void Utilities::grid_routing::initialize_map()    /* initialzie the map with block, sink and source*/
{
      this->num_connections = problem_object->get_connections().size();
      
      int height = problem_object->get_height();
      int width = problem_object ->get_width();
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
	
      
      vector<Blocker> blockers = problem_object -> get_blockers();
      int block_num = this->problem_object -> get_blockers().size();
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
      				if(h<height&& w<width) grid.at(h).at(w) -> set_cost(-1);
      			}
      		}
      }
      
      /*
	//SOURCE & SINK LABEL START HERE
      vector<Connection> source_sink_connections = problem_object -> get_connections();
      int connections_num = problem_object -> get_connections().size();
      for(int i = 0; i < connections_num ; i++)
      {
            int source_x = source_sink_connections.at(i).source.x;
            int source_y = source_sink_connections.at(i).source.y;
            int sink_x = source_sink_connections.at(i).sink.x;
            int sink_y = source_sink_connections.at(i).sink.y;
            if(source_x >= 0 && source_x < width && source_y >= 0 && source_y < height && sink_x >= 0 && sink_x < width && sink_y >= 0 && sink_y < height)
            {
                  grid.at(source_y).at(source_x) -> set_cost(-2); //LABEL NODE AS COST=-2 WHEN THE NODE IS IN A SOURCE
                  grid.at(sink_y).at(sink_x) -> set_cost(-3); //LABEL NODE AS COST=-3 WHEN THE NODE IS IN A SINK
	         
            }
      }
   
      //SOURCE & SINK LABEL END HERE
      */
}








void Utilities::grid_routing::Lee_algorithm1()             
{
 
    grid_routing::initialize_map();
    //vector<Path*> paths;
    vector<Connection> source_sink_connections = this->problem_object -> get_connections();
    int num_paths = this->problem_object -> get_connections().size();    
    
    for(int i = 0; i < num_paths ; i++)
    {
        Path* new_path = new Path();
        int source_x = source_sink_connections.at(i).source.x;
        int source_y = source_sink_connections.at(i).source.y;
        int sink_x = source_sink_connections.at(i).sink.x;
        int sink_y = source_sink_connections.at(i).sink.y;
        int height = problem_object->get_height();                     
        int width = problem_object->get_width();                       
        
        if(source_x >= 0 && source_x < width && source_y >= 0 && source_y < height && sink_x >= 0 && sink_x < width && sink_y >= 0 && sink_y < height)
            {
                  grid.at(source_y).at(source_x) -> set_cost(0); //LABEL NODE AS COST=0 WHEN THE NODE IS IN A SOURCE
                  grid.at(sink_y).at(sink_x) -> set_cost(-3); //LABEL NODE AS COST=-3 WHEN THE NODE IS IN A SINK
	         
            }
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
   
    //return paths;
    print();
}









void Utilities::grid_routing::There_bit1()
{
    
    grid_routing::initialize_map();
    vector<Connection> source_sink_connections = this->problem_object -> get_connections();
    int num_paths = this->problem_object -> get_connections().size();
    for(int i = 0; i < num_paths ; i++)
    {
        Path* new_path = new Path();
        int source_x = source_sink_connections.at(i).source.x;
        int source_y = source_sink_connections.at(i).source.y;
        int sink_x = source_sink_connections.at(i).sink.x;
        int sink_y = source_sink_connections.at(i).sink.y;
        int height = problem_object->get_height();
        int width = problem_object->get_width();
        
        if(source_x >= 0 && source_x < width && source_y >= 0 && source_y < height && sink_x >= 0 && sink_x < width && sink_y >= 0 && sink_y < height)
        {
            grid.at(source_y).at(source_x) -> set_cost(0); //LABEL NODE AS COST=0 WHEN THE NODE IS IN A SOURCE
            grid.at(sink_y).at(sink_x) -> set_cost(-3); //LABEL NODE AS COST=-3 WHEN THE NODE IS IN A SINK
        }
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
                    {
                        int yushu = (distance+2) % 3 + 1;
                        grid.at(source_y + l).at(source_x + j) -> set_cost(yushu);      //SET COST
                    }
                    if(grid.at(source_y + l).at(source_x + j)-> get_cost() == -3)    //STOP WHEN SINK IS REACHED
                        flag = false;
                }
                if(source_y - l>=0 && source_y - l<= height-1 && source_x - j >=0 && source_x - j <= width -1)
                {
                    if(grid.at(source_y - l).at(source_x - j)-> get_cost() == 0)     //FOUND EMPTY NODE
                    {
                        int yushu = (distance+2) % 3 + 1;
                        grid.at(source_y - l).at(source_x - j) -> set_cost(yushu);      //SET COST
                    }
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
            
            if (step == 1)
            {
                Point head(x,y);
                Point tail(source_x, source_y);
                PathSegment* path_segment = new PathSegment(head, tail);
                new_path -> add_segment(path_segment);
            }
            if ( x+1 >= 0 && x+1 <= width-1)     // RIGHT STEP IS NOT OUT OF RANGE
            {
                if(grid.at(y).at(x+1)-> get_cost() == (step+1)% 3 +1 )    // COST OF THIS STEP DECREASE BY 1
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
                if(grid.at(y).at(x-1)-> get_cost() == (step+1)% 3 +1)    // COST OF THIS STEP DECREASE BY 1
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
                if(grid.at(y+1).at(x)-> get_cost() == (step+1)% 3 +1)    // COST OF THIS STEP DECREASE BY 1
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
                if(grid.at(y-1).at(x)-> get_cost() == (step+1)% 3 +1)    // COST OF THIS STEP DECREASE BY 1
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
    //return paths;
    print();
}









void Utilities::grid_routing::Two_bit1()             
{
    grid_routing::initialize_map();
    vector<Connection> source_sink_connections = this->problem_object -> get_connections();
    int num_paths = this->problem_object -> get_connections().size();    
    
    for(int i = 0; i < num_paths ; i++)
    {
        Path* new_path = new Path();
        int source_x = source_sink_connections.at(i).source.x;
        int source_y = source_sink_connections.at(i).source.y;
        int sink_x = source_sink_connections.at(i).sink.x;
        int sink_y = source_sink_connections.at(i).sink.y;
        int height = problem_object->get_height();                     
        int width = problem_object->get_width();                       
        
        if(source_x >= 0 && source_x < width && source_y >= 0 && source_y < height && sink_x >= 0 && sink_x < width && sink_y >= 0 && sink_y < height)
        {
            grid.at(source_y).at(source_x) -> set_cost(0); //LABEL NODE AS COST=0 WHEN THE NODE IS IN A SOURCE
            grid.at(sink_y).at(sink_x) -> set_cost(-3); //LABEL NODE AS COST=-3 WHEN THE NODE IS IN A SINK
            
        }
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
                    {
                        int yushu = ((distance + 3) % 4 )/2 + 1;
                        grid.at(source_y + l).at(source_x + j) -> set_cost(yushu);      //SET COST
                    }
                    if(grid.at(source_y + l).at(source_x + j)-> get_cost() == -3)    //STOP WHEN SINK IS REACHED
                        flag = false;
                }
                if(source_y - l>=0 && source_y - l<= height-1 && source_x - j >=0 && source_x - j <= width -1)
                {
                    if(grid.at(source_y - l).at(source_x - j)-> get_cost() == 0)     //FOUND EMPTY NODE
                    {
                        int yushu = distance % 4;
                        grid.at(source_y - l).at(source_x - j) -> set_cost(yushu);      //SET COST
                    }
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
            
            if (step == 1)
            {
                Point head(x,y);
                Point tail(source_x, source_y);
                PathSegment* path_segment = new PathSegment(head, tail);
                new_path -> add_segment(path_segment);
            }
            if ( x+1 >= 0 && x+1 <= width-1)     // RIGHT STEP IS NOT OUT OF RANGE
            {
                if(grid.at(y).at(x+1)-> get_cost() == ((step+2)% 4)/2 +1 )    // COST OF THIS STEP DECREASE BY 1
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
                if(grid.at(y).at(x-1)-> get_cost() == ((step+2)% 4)/2 +1)    // COST OF THIS STEP DECREASE BY 1
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
                if(grid.at(y+1).at(x)-> get_cost() == ((step+2)% 4)/2 +1)    // COST OF THIS STEP DECREASE BY 1
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
                if(grid.at(y-1).at(x)-> get_cost() == ((step+2)% 4)/2 +1)    // COST OF THIS STEP DECREASE BY 1
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
    
    //return paths;
    print();
}



void Utilities::grid_routing::Rubin1()
{
    grid_routing::initialize_map();
    vector<Path*> paths;
    vector<Connection> source_sink_connections = problem_object -> get_connections();
    int num_paths = problem_object -> get_connections().size();
    for(int i = 0; i < num_paths ; i++)
    {
        Path* new_path = new Path();
        int source_x = source_sink_connections.at(i).source.x;
        int source_y = source_sink_connections.at(i).source.y;
        int sink_x = source_sink_connections.at(i).sink.x;
        int sink_y = source_sink_connections.at(i).sink.y;
        int height = problem_object -> get_height();
        int width = problem_object -> get_width();
        //grid.at(source_y).at(source_x) -> set_cost(0);     // COST AT SOURCE=0
        //int distance = 0;     // STEPS FROM SOURCE

//        int **C_distance;     // C_DISTANCE = CURRENT STEPS + MANHATTAN DISTANCE
//        C_distance = (int **)malloc(sizeof(int *) * height);
//        for(int i = 0; i < height; i++)
//        {
//        	C_distance[i] = (int *)malloc(sizeof(int) * width);
//        }
//        for(int i = 0; i < height; i++)
//        {
//        	for(int j = 0; j < width; j++)
//        	{
//        		C_distance[i][j] = 0;
//	       	}
//        }

        int level = abs(source_x - sink_x) + abs(source_y - sink_y);
		grid.at(source_y).at(source_x)->set_cost(level);
		        
        int **wavefront;     // WAVE FRONT IN THE FORM OF TIME STAMP
        wavefront = (int **)malloc(sizeof(int *) * height);
        for(int i = 0; i < height; i++)
        {
        	wavefront[i] = (int *)malloc(sizeof(int) * width);
        }
        for(int i = 0; i < height; i++)
        {
        	for(int j = 0; j < width; j++)
        	{
        		wavefront[i][j] = -1;	// UNVISITED CELL = -1
	       	}
        }
        wavefront[source_x][source_y] = 1;		// SOURCE IS STARTING WAVE FRONT
        int time_stamp = 1;
        int x = source_x;
		int y = source_y;
        
		// SETUP INITIAL STEP FROM SOURCE
        int east_cost = 0;
        int west_cost = 0;
        int north_cost = 0;
        int south_cost = 0;
        int min_cost = 0;
        int previous_step;
        time_stamp++;
        if(x + 1 <= width -1)
        {
        	if(grid.at(y).at(x + 1)->get_cost() == 0)
        	{
        		east_cost = 1 + abs(x + 1 - sink_x) + abs(y - sink_y);
        		grid.at(y).at(x + 1)->set_cost(east_cost);
        		wavefront[x + 1][y] = time_stamp;
        		time_stamp++;
        	}
        }
        if(x - 1 >= 0)
        {
        	if(grid.at(y).at(x - 1)->get_cost() == 0)
        	{
        		west_cost = 1 + abs(x - 1 - sink_x) + abs(y - sink_y);
        		grid.at(y).at(x - 1)->set_cost(west_cost);
        		wavefront[x - 1][y] = time_stamp;
        		time_stamp++;
        	}
        }
        if(y + 1 <= height -1)
        {
        	if(grid.at(y + 1).at(x)->get_cost() == 0)
        	{
        		north_cost = 1 + abs(x - sink_x) + abs(y + 1 - sink_y);
        		grid.at(y + 1).at(x)->set_cost(north_cost);
        		wavefront[x][y + 1] = time_stamp;
        		time_stamp++;
        	}
        }
        if(y - 1 >= 0)
        {
        	if(grid.at(y - 1).at(x)->get_cost() == 0)
        	{
        		south_cost = 1 + abs(x - sink_x) + abs(y - 1 - sink_y);
        		grid.at(y - 1).at(x)->set_cost(south_cost);
        		wavefront[x][y - 1] = time_stamp;
        		time_stamp++;
        	}
        }
   		if(wavefront[x + 1][y] <= 0)	east_cost = 9999999;
        if(wavefront[x - 1][y] <= 0)	west_cost = 9999999;
        if(wavefront[x][y + 1] <= 0)	north_cost = 9999999;
        if(wavefront[x][y - 1] <= 0)	south_cost = 9999999;
        min_cost = std::min(east_cost, std::min(west_cost, std::min(north_cost, south_cost)));
        if(min_cost > 0 && min_cost == east_cost)	// IF SINK IS LOCATED ON EAST
		{
			previous_step = 1;
			x++;
		}
        else if(min_cost > 0 && min_cost == west_cost)	// IF SINK IS LOCATED ON WEST
        {
        	previous_step = 2;
        	x--;
        }
        else if(min_cost > 0 && min_cost == north_cost)	// IF SINK IS LOCATED ON NORTH
        {
        	previous_step = 3;
        	y++;
        }
        else if(min_cost > 0 && min_cost == south_cost)	// IF SINK IS LOCATED ON SOUTH
        {
        	previous_step = 4;
        	y--;
        }
        wavefront[x][y] = 0;
        level = min_cost;
		// END OF SETUP INITIAL STEP FROM SOURCE

        // EXPANSION STARTS HERE
        bool flag = true;     // FLAG = FALSE WHEN SINK IS REACHED        
        while(flag)
        {
			// FIND MIN COST WITHIN WAVE FRONT
        	for(int i = 0; i < height; i++)
        	{
        		for(int j = 0; j < width; j++)
        		{
        			if(wavefront[i][j] > 0 && grid.at(j).at(i)->get_cost() <= level)
        			{
        				level = grid.at(j).at(i)->get_cost();
        			}
        		}
        	}
        	//	END OF FIND MIN COST WITHIN WAVE FRONT
        	
        	
//            int max_time_stamp = 0;
//            for(int i = 0; i < height; i++)
//            {
//            	for(int j = 0; j < width; j++)
//            	{
//            		if(C_distance[i][j] > 0 && C_distance[i][j] <= level && wavefront[i][j] >= max_time_stamp)     // FIND NEWEST WAVE FRONT WITH LOWEST C VALUE IN C_DISTANCE MATRIX
//            		{
//            			level = C_distance[i][j];
//            			max_time_stamp = wavefront[i][j];
//            			x = i;
//            			y = j;
//            		}
//            	}
//            }
            
//            if(x + 1 <= width -1)	// IF EAST CELL IS AVAILABLE
//            {
//            	int cost = grid.at(y).at(x + 1)->get_cost();
//            	int manhattan_distance = 1 + abs(x + 1 - sink_x) + abs(y - sink_y);
//            	C_distance[x + 1][y] = distance + manhattan_distance;
//            	
//            	if(cost = 0 || manhattan_distance > level)	// FOUND EMPTY NODE OR NODE MANHATTAN DISTANCE CAN BE DECREASED
//            	{
//            		grid.at(y).at(x + 1)->set_cost(distance);	// SET COST SAME AS LEE
//            		C_distance[x + 1][y] = manhattan_distance;
//            	}
//            }


			// SETTING UP CELL COST AROUND CURRENT CELL
	        east_cost = 0;
	        west_cost = 0;
	        north_cost = 0;
	        south_cost = 0;
	        min_cost = 0;
	        int current_step = grid.at(y).at(x)->get_cost() - (abs(x - sink_x) + abs(y - sink_y));	// CURRENT STEP FROM SOURCE
	        if(x + 1 <= width -1)
	        {
	        	if(grid.at(y).at(x + 1)->get_cost() == -3)
	        	{
	        		break;
	        	}
	        	else if(grid.at(y).at(x + 1)->get_cost() == 0 || (wavefront[x + 1][y] > 0 && grid.at(y).at(x + 1)->get_cost() > current_step + 1 + abs(x + 1 - sink_x) + abs(y - sink_y)))
	        	{
	        		east_cost = current_step + 1 + abs(x + 1 - sink_x) + abs(y - sink_y);
	        		grid.at(y).at(x + 1)->set_cost(east_cost);
	        		wavefront[x + 1][y] = time_stamp;
	        		time_stamp++;
	        	}
	        	else if(wavefront[x + 1][y] > 0)
	        	{
	        		east_cost = grid.at(y).at(x + 1)->get_cost();
	        	}
	        }
	        if(x - 1 >= 0)
	        {
	        	if(grid.at(y).at(x - 1)->get_cost() == -3)
	        	{
	        		break;
	        	}
	        	else if(grid.at(y).at(x - 1)->get_cost() == 0 || (wavefront[x - 1][y] > 0 && grid.at(y).at(x - 1)->get_cost() > current_step + 1 + abs(x - 1 - sink_x) + abs(y - sink_y)))
	        	{
	        		west_cost = current_step + 1 + abs(x - 1 - sink_x) + abs(y - sink_y);
	        		grid.at(y).at(x - 1)->set_cost(west_cost);
	        		wavefront[x - 1][y] = time_stamp;
	        		time_stamp++;
	        	}
	        	else if(wavefront[x - 1][y] > 0)
	        	{
	        		west_cost = grid.at(y).at(x - 1)->get_cost();
	        	}
	        }
	        if(y + 1 <= height -1)
	        {
	        	if(grid.at(y + 1).at(x)->get_cost() == -3)
	        	{
	        		break;
	        	}
	        	else if(grid.at(y + 1).at(x)->get_cost() == 0 || (wavefront[x][y + 1] > 0 && grid.at(y + 1).at(x)->get_cost() > current_step + 1 + abs(x - sink_x) + abs(y + 1 - sink_y)))
	        	{
	        		north_cost = current_step + 1 + abs(x - sink_x) + abs(y + 1 - sink_y);
	        		grid.at(y + 1).at(x)->set_cost(north_cost);
	        		wavefront[x][y + 1] = time_stamp;
	        		time_stamp++;
	        	}
	        	else if(wavefront[x][y + 1] > 0)
	        	{
	        		north_cost = grid.at(y + 1).at(x)->get_cost();
	        	}
	        }
	        if(y - 1 >= 0)
	        {
	        	if(grid.at(y - 1).at(x)->get_cost() == -3)
	        	{
	        		break;
	        	}
	        	else if(grid.at(y - 1).at(x)->get_cost() == 0  || (wavefront[x][y - 1] > 0 && grid.at(y - 1).at(x)->get_cost() > current_step + 1 + abs(x - sink_x) + abs(y - 1 - sink_y)))
	        	{
	        		south_cost = current_step + 1 + abs(x - sink_x) + abs(y - 1 - sink_y);
	        		grid.at(y - 1).at(x)->set_cost(south_cost);
	        		wavefront[x][y - 1] = time_stamp;
	        		time_stamp++;
	        	}
	        	else if(wavefront[x][y - 1] > 0)
	        	{
	        		south_cost = grid.at(y - 1).at(x)->get_cost();
	        	}
	        }
	        // END OF SETTING UP CELL COST AROUND CURRENT CELL
	        
			// DECIDE WHICI DIRECTION TO GO
	        if(wavefront[x + 1][y] <= 0)	east_cost = 9999999;
	        if(wavefront[x - 1][y] <= 0)	west_cost = 9999999;
	        if(wavefront[x][y + 1] <= 0)	north_cost = 9999999;
	        if(wavefront[x][y - 1] <= 0)	south_cost = 9999999;
	        min_cost = std::min(east_cost, std::min(west_cost, std::min(north_cost, south_cost)));
	        if(min_cost == level)	// IF THERE EXISTS A NEIGHBOR WHOSE COST DOES NOT INCREASE
	        {
	        	if(previous_step == 1 && min_cost == east_cost)
	        	{
	        		x++;
	        	}
	        	else if(previous_step == 2 && min_cost == west_cost)
	        	{
	        		x--;
	        	}	        	
	        	else if(previous_step == 3 && min_cost == north_cost)
	        	{
	        		y++;
	        	}
	        	else if(previous_step == 4 && min_cost == south_cost)
	        	{
	        		y--;
	        	}
	        	
	        	else if(min_cost == east_cost)
	        	{
	        		x++;
	        		previous_step = 1;
	        	}
	        	else if(min_cost == west_cost)
	        	{
	        		x--;
	        		previous_step = 2;
	        	}	        	
	        	else if(min_cost == north_cost)
	        	{
	        		y++;
	        		previous_step = 3;
	        	}
	        	else if(min_cost == south_cost)
	        	{
	        		y--;
	        		previous_step = 4;
	        	}
	        	wavefront[x][y] = 0;
	        }
	        else	// IF THERE IS NO NEIGHBOR WITH EQUAL OR LESS COST, JUMP BACK TO NEWEST WAVE FRONT WITH COST = LEVEL
	        {
	        	int time_stamp;
	        	for(int i = 0; i < height; i++)
	        	{
	        		for(int j = 0; j < width; j++)
	        		{
	        			if(wavefront[i][j] > 0 && grid.at(j).at(i)->get_cost() <= level)
	        			{
	        				level = grid.at(j).at(i)->get_cost();
	        				time_stamp = wavefront[i][j];	// INITIAL TIME STAMP TO AN EXISTING NUMBER
	        			}
	        		}
	        	}
	        	
	        	for(int i = 0; i < height; i++)
	        	{
	        		for(int j = 0; j < width; j++)
	        		{
	        			if(wavefront[i][j] > 0 && grid.at(j).at(i)->get_cost() == level && wavefront[i][j] >= time_stamp)
	        			{
	        				time_stamp = wavefront[i][j];
	        				x = i;
	        				y = j;
	        			}
	        		}
	        	}
	        }

		


        	
//        	case 1:     // IF PREVIOUS STEP IS EAST STEP
//        		{
//		           if(x + 1 <= width -1)	// IF EAST CELL IS AVAILABLE
//		           {
//		           		int next_cost = grid.at(y).at(x + 1)->get_cost();
//		           		int new_cost = distance[x][y] + 1 + abs(x + 1 - sink_x) + abs(y - sink_y);
//						if(next_cost == 0 || next_cost > new_cost)	// IF EAST CELL IS UNVISITED OR VISITED BUT COST CAN BE REDUCED
//		           		{
//		           			grid.at(y).at(x + 1)->set_cost(new_cost);
//		           			wavefront[x + 1][y] = wavefront[x][y] + 1;	// INCREASE TIME STAMP
//		           			wavefront[x][y] = 0;	// CURRENT CELL IS NO LONGER WAVE FRONT
//		           			previous_step = 1;
//		           		}
//		           		else if(next_cost = -3)	// FOUND SINK NODE
//		           		{
//		           			flag = false;
//		           		}
//		           }
//        		}
//        	break;
//
//        	case 2:     // IF PREVIOUS STEP IS WEST STEP
//        		{
//		           if(x - 1 >= 0)	// IF WEST CELL IS AVAILABLE
//		           {
//		           		int next_cost = grid.at(y).at(x + 1)->get_cost();
//		           		int new_cost = distance[x][y] + 1 + abs(x - 1 - sink_x) + abs(y - sink_y);
//						if(next_cost == 0 || next_cost > new_cost)	// IF WEST CELL IS UNVISITED OR VISITED BUT COST CAN BE REDUCED
//		           		{
//		           			grid.at(y).at(x - 1)->set_cost(new_cost);
//		           			wavefront[x - 1][y] = wavefront[x][y] + 1;	// INCREASE TIME STAMP
//		           			wavefront[x][y] = 0;	// CURRENT CELL IS NO LONGER WAVE FRONT
//		           			previous_step = 2;
//		           		}
//		           		else if(next_cost = -3)	// FOUND SINK NODE
//		           		{
//		           			flag = false;
//		           		}
//		           }            			
//        		}
//        	break;
//
//        	case 3:     // IF PREVIOUS STEP IS NORTH STEP
//        		{
//		           if(y + 1 <= height -1)	// IF NORTH CELL IS AVAILABLE
//		           {
//		           		int next_cost = grid.at(y + 1).at(x)->get_cost();
//		           		int new_cost = distance[x][y] + 1 + abs(x - sink_x) + abs(y + 1 - sink_y);
//						if(next_cost == 0 || next_cost > new_cost)	// IF NORTH CELL IS UNVISITED OR VISITED BUT COST CAN BE REDUCED
//		           		{
//		           			grid.at(y + 1).at(x)->set_cost(new_cost);
//		           			wavefront[x][y + 1] = wavefront[x][y] + 1;	// INCREASE TIME STAMP
//		           			wavefront[x][y] = 0;	// CURRENT CELL IS NO LONGER WAVE FRONT
//		           			previous_step = 3;
//		           		}
//		           		else if(next_cost = -3)	// FOUND SINK NODE
//		           		{
//		           			flag = false;
//		           		}
//		           }
//        		}
//        	break;
//        	
//        	case 4:     // IF PREVIOUS STEP IS SOUTH STEP
//        		{
//		           if(y - 1 >= 0)	// IF SOUTH CELL IS AVAILABLE
//		           {
//		           		int next_cost = grid.at(y - 1).at(x)->get_cost();
//		           		int new_cost = distance[x][y] + 1 + abs(x - sink_x) + abs(y - 1 - sink_y);
//						if(next_cost == 0 || next_cost > new_cost)	// IF SOUTH CELL IS UNVISITED OR VISITED BUT COST CAN BE REDUCED
//		           		{
//		           			grid.at(y - 1).at(x)->set_cost(new_cost);
//		           			wavefront[x][y - 1] = wavefront[x][y] + 1;	// INCREASE TIME STAMP
//		           			wavefront[x][y] = 0;	// CURRENT CELL IS NO LONGER WAVE FRONT
//		           			previous_step = 4;
//		           		}
//		           		else if(next_cost = -3)	// FOUND SINK NODE
//		           		{
//		           			flag = false;
//		           		}
//		           }
//        		}
//        	break;            	



        }
        // EXPANSION ENDS HERE
grid.at(source_y).at(source_x)->set_cost(-2);



			  
			  for(int j=height-1;j>=0;j--)
			  {
				  for(int i=0;i<width;i++)
				  {
					  int cost=grid.at(j).at(i)->get_cost();
					  if(cost<10&&cost>=0) std::cout<<"  "<<cost;
					  else std::cout<<" "<<cost;
					  //if(cos==-2) cout<<"SOURCE("<<i<<","<<j<<")"<<endl;
					  //if(cos==-3) cout<<"SINK("<<i<<","<<j<<")"<<endl;
				  }
			  std::cout<<std::endl;
			  }


			  std::cout<<std::endl<<std::endl<<"WAVE FRONT"<<std::endl;
			  for(int j=height-1;j>=0;j--)
			  {
				  for(int i=0;i<width;i++)
				  {
					  int wf=wavefront[i][j];
					  if(wf<10&&wf>=0) std::cout<<"  "<<wf;
					  else if(wf>=10&&wf<100) std::cout<<" "<<wf;
					  else if(wf==-1) std::cout<<" "<<wf;
					  else std::cout<<wf;
					  //if(cos==-2) cout<<"SOURCE("<<i<<","<<j<<")"<<endl;
					  //if(cos==-3) cout<<"SINK("<<i<<","<<j<<")"<<endl;
				  }
			  std::cout<<std::endl;
			  }




/*       
        // BACKTRACING STARTS HERE
        x = sink_x;
        y = sink_y;
        for(int step = distance; step > 0 ; step--)     // MOVE TO SOURCE IN THE DECREASING ORDER OF DISTANCE
		{
		    bool move = false;     // WHENEVER MAKE A MOVE
		    
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
*/
    }
    print();
}








//void Utilities::grid_routing::Lee_algorithm2()        /* unlimited intersections */
//{
        
//}

//void Utilities::grid_routing::Rubin_algorithm1()
//{
//}

void Utilities::grid_routing::print()
{
        
	if(paths.empty())
        {
                std::cout << "There is no path.";
                return;
        }
	
        for(unsigned i = 0; i < paths.size(); i++) {
		std::cout << "Path " << i << ": ";
		paths.at(i)->print();
		Path* temp = paths.at(i);
		delete temp;
	}
	paths.clear();
}

