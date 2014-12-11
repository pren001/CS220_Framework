#include "../Headers/grid.h"
#include "../Headers/edge.h"
#include "../Headers/claim.h"
#include "../Headers/problem_object.h"
#include "../Headers/grid_routing.h"
#include <math.h>
#include <algorithm>
#include <vector>
#include <stack>
#include <queue>

using std::string;



Utilities::grid_routing::grid_routing(ProblemObject* problemobject)
{
      this->problem_object = problemobject;
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



void Utilities::grid_routing::initialize_map(int node_initial_cost, int block_cost)    /* initialzie the map with block, sink and source*/
{
      if(problem_object == NULL)
          claim("There is no problem object input", kError);
      this->problem_object = problem_object;
      this->num_connections = problem_object->get_connections().size();
      
      int height = problem_object->get_height();
      int width = problem_object ->get_width();
    
      for(int y = 0; y < height; y++) {
		  vector<Node*> temp_row;
	      for(int x = 0; x < width; x++) {
			  Node* new_node = new Node(x,y,node_initial_cost);
			  /*
              if (x > 0) {
				  Edge* east = new Edge(new_node,temp_row.at(temp_row.size()-1));
				  new_node->add_connection(east);
			  }
			  if (y > 0) {
				  Edge* north = new Edge(new_node,grid.at(y-1).at(x));
				  new_node->add_connection(north);
			  }
               */
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
      		for( unsigned int h = block_y; h < block_y + block_height ; h++ )
      		{
      			for(unsigned int w = block_x; w < block_x+ block_width ; w++)
      			{
      				if(h<height&& w<width) grid.at(h).at(w) -> set_cost(block_cost);
      			}
      		}
      }
	/*
     for(int m = height -1; m>=0; m--)
         for(int n = 0; n<width; n++ )
         {
             if(n == width -1)
                 std::cout << grid.at(m).at(n)->get_cost()<< std::endl;
             else
                 std::cout << grid.at(m).at(n)->get_cost()<< " ";
         }
      */
      
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
          
            if(grid.at(source_y).at(source_x) -> get_cost()== block_cost)
            {
                claim("Source is in blocks, please change the coordinate of source",kError);
                break;
            }
            if(grid.at(sink_y).at(source_x) -> get_cost()== block_cost)
            {
                claim("Sink is in blocks, please change the coordinate of sink",kError);
                break;
            }
      }
      //SOURCE & SINK LABEL END HERE
     */
}


/*
void Utilities::grid_routing::set_source_sink()
{
    vector< std::queue<Node*> > nodes_queue;
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

}
*/




void Utilities::grid_routing::Lee()
{
    
    initialize_map(0,-1);
    vector<Connection> source_sink_connections = this->problem_object -> get_connections();
    int num_paths = this->problem_object -> get_connections().size();    
    
    int height = problem_object->get_height();
    int width = problem_object->get_width();
    for(int i = 0; i < num_paths ; i++)
    {
        vector< std::queue<Node*> > nodes_queue;
        Path* new_path1 = new Path();
        Path* new_path2 = new Path();
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
        int distance;     // STEPS FROM SOURCE
        
        // EXPANSION STARTS HERE
        bool flag = true;     // FLAG = FALSE WHEN SINK IS REACHED
        
        int x = source_x;
        int y = source_y;
        
        std::queue<Node*> queue0;
        nodes_queue.push_back(queue0);
        while(flag)
        {
            distance = grid.at(y).at(x)->get_cost();
            /*
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
            */
            if(x+1>=0 && x+1<=width -1 )
            {
                if(grid.at(y).at(x+1)->get_cost()== 0)   /*  node never be waved */
                {
                    grid.at(y).at(x+1) -> set_cost(distance + 1);
                    if(nodes_queue.size()<distance+2)
                    {
                        std::queue<Node*> queue;
                        nodes_queue.push_back(queue);
                        nodes_queue.at(distance+1).push(grid.at(y).at(x+1));
                    }
                    else
                        nodes_queue.at(distance+1).push(grid.at(y).at(x+1));
                    
                }
                if(grid.at(y).at(x+1)->get_cost()== -3)   /* meets sink */
                {
                    flag = false;
                }
            }
            if(x-1>=0 && x-1<=width -1 )
            {
                if(grid.at(y).at(x-1)->get_cost()== 0)   /*  node never be waved */
                {
                    grid.at(y).at(x-1) -> set_cost(distance + 1);
                    if(nodes_queue.size()<distance+2)
                    {
                        std::queue<Node*> queue;
                        nodes_queue.push_back(queue);
                        nodes_queue.at(distance+1).push(grid.at(y).at(x-1));
                    }
                    else
                        nodes_queue.at(distance+1).push(grid.at(y).at(x-1));
                }
                if(grid.at(y).at(x-1)->get_cost()== -3)   /* meets sink */
                {
                    flag = false;
                }
            }
            if(y-1>=0 && y-1<=width -1 )
            {
                if(grid.at(y-1).at(x)->get_cost()== 0)   /*  node never be waved */
                {
                    grid.at(y-1).at(x) -> set_cost(distance + 1);
                    if(nodes_queue.size()<distance+2)
                    {
                        std::queue<Node*> queue;
                        nodes_queue.push_back(queue);
                        nodes_queue.at(distance+1).push(grid.at(y-1).at(x));
                    }
                    else
                        nodes_queue.at(distance+1).push(grid.at(y-1).at(x));
                }
                if(grid.at(y-1).at(x)->get_cost()== -3)   /* meets sink */
                {
                    flag = false;
                }

            }
            if(y+1>=0 && y+1<=width -1 )
            {
                if(grid.at(y+1).at(x)->get_cost()== 0)   /*  node never be waved */
                {
                    grid.at(y+1).at(x) -> set_cost(distance + 1);
                    if(nodes_queue.size()<distance+2)
                    {
                        std::queue<Node*> queue;
                        nodes_queue.push_back(queue);
                        nodes_queue.at(distance+1).push(grid.at(y+1).at(x));
                    }
                    else
                        nodes_queue.at(distance+1).push(grid.at(y+1).at(x));
                }
                if(grid.at(y+1).at(x)->get_cost()== -3)   /* meets sink */
                {
                    flag = false;
                }
            }
            if(!nodes_queue.at(distance).empty())
            {
                x = nodes_queue.at(distance).front() -> get_x();
                y = nodes_queue.at(distance).front() -> get_y();
                nodes_queue.at(distance).pop();
            }
            else
            {
                x = nodes_queue.at(distance+1).front() -> get_x();
                y = nodes_queue.at(distance+1).front() -> get_y();
                nodes_queue.at(distance+1).pop();
            }
        }
        grid.at(source_y).at(source_x)->set_cost(0);
        // EXPANSION ENDS HERE
        for(int m = height -1; m>=0; m--)
            for(int n = 0; n<width; n++ )
            {
                if(n == width -1)
                    std::cout << grid.at(m).at(n)->get_cost()<< std::endl;
                else
                    std::cout << grid.at(m).at(n)->get_cost()<< " ";
            }

        // BACKTRACING STARTS HERE
        x = sink_x;
        y = sink_y;
        
        int direction_before = 0;
        int direction_now = 0;
        int c_turn1 = 0;
        int c_turn2 = 0;
    
        for(int step = distance+1; step > 0 ; step--)     // MOVE TO SOURCE IN THE DECREASING ORDER OF DISTANCE
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
                    new_path1 -> add_segment(path_segment);      // ADD CURRENT MOVE TO NEW PATH
                    direction_now = 1;
                    if(direction_before!=direction_now)
                        c_turn1 ++;
                    direction_before = 1;
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
                    new_path1 -> add_segment(path_segment);      // ADD CURRENT MOVE TO NEW PATH
                    direction_now = 2;
                    if(direction_before!=direction_now)
                        c_turn1 ++;
                    direction_before = 2;
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
                    new_path1 -> add_segment(path_segment);      // ADD CURRENT MOVE TO NEW PATH
                    direction_now = 3;
                    if(direction_before!=direction_now)
                        c_turn1 ++;
                    direction_before = 3;
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
                    new_path1 -> add_segment(path_segment);      // ADD CURRENT MOVE TO NEW PATH
                    direction_now = 4;
                    if(direction_before!=direction_now)
                        c_turn1 ++;
                    direction_before = 4;
                }
            }
        }
        
        x = sink_x;
        y = sink_y;
    
        direction_before = 0;
        direction_now = 0;
        for(int step = distance+1; step > 0 ; step--)     // MOVE TO SOURCE IN THE DECREASING ORDER OF DISTANCE
        {
            bool move = false;    // WHENEVER MAKE A MOVE
            
            if(y-1 >= 0 && y-1 <= height-1 && !move)     // DOWN STEP IS NOT OUT OF RANGE
            {
                if(grid.at(y-1).at(x)-> get_cost() == step - 1)    // COST OF THIS STEP DECREASE BY 1
                {
                    Point head(x ,y);
                    y--;
                    Point tail(x ,y);
                    move = true;     // AVOID FURTHER MOVE IN THE SAME STEP
                    PathSegment* path_segment = new PathSegment(head, tail);
                    new_path2 -> add_segment(path_segment);      // ADD CURRENT MOVE TO NEW PATH
                    direction_now = 4;
                    if(direction_before!=direction_now)
                        c_turn2 ++;
                    direction_before = 4;
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
                    new_path2 -> add_segment(path_segment);      // ADD CURRENT MOVE TO NEW PATH
                    direction_now = 3;
                    if(direction_before!=direction_now)
                        c_turn2 ++;
                    direction_before = 3;
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
                    new_path2 -> add_segment(path_segment);      // ADD CURRENT MOVE TO NEW PATH
                    direction_now = 2;
                    if(direction_before!=direction_now)
                        c_turn2 ++;
                    direction_before = 2;
                }
            }
            if(x+1 >= 0 && x+1 <= width-1)     // RIGHT STEP IS NOT OUT OF RANGE
            {
                if(grid.at(y).at(x+1)-> get_cost() == step - 1)    // COST OF THIS STEP DECREASE BY 1
                {
                    Point head(x ,y);
                    x++;
                    Point tail(x ,y);
                    move = true;     // AVOID FURTHER MOVE IN THE SAME STEP
                    PathSegment* path_segment = new PathSegment(head, tail);
                    new_path2 -> add_segment(path_segment);      // ADD CURRENT MOVE TO NEW PATH
                    direction_now = 1;
                    if(direction_before!=direction_now)
                        c_turn2 ++;
                    direction_before = 1;
                }
            }
            
        }
        
        std::cout << c_turn1 <<std::endl;
        std::cout << c_turn2 <<std::endl;
        if(c_turn1 <  c_turn2)
            paths.push_back(new_path1);
        else
            paths.push_back(new_path2);
        // BACK TRACING ENDS HERE
    }
    //return paths;
    print();
    
}









void Utilities::grid_routing::Three_bits()
{
    
    initialize_map(0,-1);
    vector<Connection> source_sink_connections = this->problem_object -> get_connections();
    int num_paths = this->problem_object -> get_connections().size();
    for(int i = 0; i < num_paths ; i++)
    {
        vector<std::queue<Node*> > nodes_queue;
        Path* new_path = new Path();
        int source_x = source_sink_connections.at(i).source.x;
        int source_y = source_sink_connections.at(i).source.y;
        int sink_x = source_sink_connections.at(i).sink.x;
        int sink_y = source_sink_connections.at(i).sink.y;
        int height = problem_object->get_height();
        int width = problem_object->get_width();
        
        if(grid.at(source_y).at(source_x) -> get_cost()== -1)
        {
            claim("Source is in blocks, please change the coordinate of source",kError);
            break;
        }
        if(grid.at(sink_y).at(source_x) -> get_cost()== -1)
        {
            claim("Sink is in blocks, please change the coordinate of sink",kError);
            break;
        }

        
        if(source_x >= 0 && source_x < width && source_y >= 0 && source_y < height && sink_x >= 0 && sink_x < width && sink_y >= 0 && sink_y < height)
        {
            grid.at(source_y).at(source_x) -> set_cost(0); //LABEL NODE AS COST=0 WHEN THE NODE IS IN A SOURCE
            grid.at(sink_y).at(sink_x) -> set_cost(-3); //LABEL NODE AS COST=-3 WHEN THE NODE IS IN A SINK
        }
        //grid.at(source_y).at(source_x) -> set_cost(0);     // COST AT SOURCE=0
        int distance = 0;     // STEPS FROM SOURCE
        
        // EXPANSION STARTS HERE
        bool flag = true;     // FLAG = FALSE WHEN SINK IS REACHED
        
        std::queue<Node*> queue0;
        nodes_queue.push_back(queue0);
        
        int x = source_x;
        int y = source_y;
        while(flag)
        {
            
            /*
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
             */
            
            //int yushu = (distance+2) % 3 + 1;
            if(x+1>=0 && x+1<=width -1 )
            {
                if(grid.at(y).at(x+1)->get_cost()== 0)   //  node never be waved
                {
                    grid.at(y).at(x+1) -> set_cost((distance+1+2) % 3 + 1);
                    if(nodes_queue.size()<distance+2)
                    {
                        std::queue<Node*> queue;
                        nodes_queue.push_back(queue);
                        nodes_queue.at(distance+1).push(grid.at(y).at(x+1));
                    }
                    else
                        nodes_queue.at(distance+1).push(grid.at(y).at(x+1));
                    
                }
                if(grid.at(y).at(x+1)->get_cost()== -3)   // meets sink
                {
                    flag = false;
                }
            }
            if(x-1>=0 && x-1<=width -1)
            {
                if(grid.at(y).at(x-1)->get_cost()== 0)   //  node never be waved
                {
                    grid.at(y).at(x-1) -> set_cost((distance+1+2) % 3 + 1);
                    if(nodes_queue.size()<distance+2)
                    {
                        std::queue<Node*> queue;
                        nodes_queue.push_back(queue);
                        nodes_queue.at(distance+1).push(grid.at(y).at(x-1));
                    }
                    else
                        nodes_queue.at(distance+1).push(grid.at(y).at(x-1));
                }
                if(grid.at(y).at(x-1)->get_cost()== -3)   // meets sink
                {
                    flag = false;
                }
            }
            if(y-1>=0 && y-1<=width -1)
            {
                if(grid.at(y-1).at(x)->get_cost()== 0)   //  node never be waved
                {
                    grid.at(y-1).at(x) -> set_cost((distance+1+2) % 3 + 1);
                    if(nodes_queue.size()<distance+2)
                    {
                        std::queue<Node*> queue;
                        nodes_queue.push_back(queue);
                        nodes_queue.at(distance+1).push(grid.at(y-1).at(x));
                    }
                    else
                        nodes_queue.at(distance+1).push(grid.at(y-1).at(x));
                }
                if(grid.at(y-1).at(x)->get_cost()== -3)   // meets sink
                {
                    flag = false;
                }
                
            }
            if(y+1>=0 && y+1<=width -1)
            {
                if(grid.at(y+1).at(x)->get_cost()== 0)   // node never be waved
                {
                    grid.at(y+1).at(x) -> set_cost((distance+1+2) % 3 + 1);
                    if(nodes_queue.size()<distance+2)
                    {
                        std::queue<Node*> queue;
                        nodes_queue.push_back(queue);
                        nodes_queue.at(distance+1).push(grid.at(y+1).at(x));
                    }
                    else
                        nodes_queue.at(distance+1).push(grid.at(y+1).at(x));
                }
                if(grid.at(y+1).at(x)->get_cost()== -3)   // meets sink
                {
                    flag = false;
                }
            }
            if(!nodes_queue.at(distance).empty())
            {
                x = nodes_queue.at(distance).front() -> get_x();
                y = nodes_queue.at(distance).front() -> get_y();
                nodes_queue.at(distance).pop();
            }
            
            else if(nodes_queue.at(distance+1).empty())  //the case if cannot find sink
            {
                claim("Cannot find sink for the current source", kError);
                continue;
            }
            
            else
            {
                x = nodes_queue.at(distance+1).front() -> get_x();
                y = nodes_queue.at(distance+1).front() -> get_y();
                nodes_queue.at(distance+1).pop();
                distance++;
            }
            
        }
        grid.at(source_y).at(source_x)->set_cost(0);
        // EXPANSION ENDS HERE
        for(int m = height -1; m>=0; m--)
            for(int n = 0; n<width; n++ )
            {
                if(n == width -1)
                    std::cout << grid.at(m).at(n)->get_cost()<< std::endl;
                else
                    std::cout << grid.at(m).at(n)->get_cost()<< " ";
            }
        
        // BACKTRACING STARTS HERE
        x = sink_x;
        y = sink_y;
        for(int step = distance+1; step > 0 ; step--)     // MOVE TO SOURCE IN THE DECREASING ORDER OF DISTANCE
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









void Utilities::grid_routing::Two_bits()
{
    initialize_map(0,-1);
    vector<Connection> source_sink_connections = this->problem_object -> get_connections();
    int num_paths = this->problem_object -> get_connections().size();    
    
    for(int i = 0; i < num_paths ; i++)
    {
        vector<std::queue<Node*> > nodes_queue;
        Path* new_path = new Path();
        int source_x = source_sink_connections.at(i).source.x;
        int source_y = source_sink_connections.at(i).source.y;
        int sink_x = source_sink_connections.at(i).sink.x;
        int sink_y = source_sink_connections.at(i).sink.y;
        int height = problem_object->get_height();                     
        int width = problem_object->get_width();
        
        if(grid.at(source_y).at(source_x) -> get_cost()== -1)
        {
            claim("Source is in blocks, please change the coordinate of source",kError);
            break;
        }
        if(grid.at(sink_y).at(source_x) -> get_cost()== -1)
        {
            claim("Sink is in blocks, please change the coordinate of sink",kError);
            break;
        }

        
        if(source_x >= 0 && source_x < width && source_y >= 0 && source_y < height && sink_x >= 0 && sink_x < width && sink_y >= 0 && sink_y < height)
        {
            grid.at(source_y).at(source_x) -> set_cost(0); //LABEL NODE AS COST=0 WHEN THE NODE IS IN A SOURCE
            grid.at(sink_y).at(sink_x) -> set_cost(-3); //LABEL NODE AS COST=-3 WHEN THE NODE IS IN A SINK
            
        }
        //grid.at(source_y).at(source_x) -> set_cost(0);     // COST AT SOURCE=0
        int distance = 0;     // STEPS FROM SOURCE
        
        // EXPANSION STARTS HERE
        bool flag = true;     // FLAG = FALSE WHEN SINK IS REACHED
        
        int x = source_x;
        int y = source_y;
        
        std::queue<Node*> queue0;
        nodes_queue.push_back(queue0);
        while(flag)
        {
            /*
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
                        int yushu = ((distance + 3) % 4 )/2 + 1;
                        grid.at(source_y - l).at(source_x - j) -> set_cost(yushu);      //SET COST
                    }
                    if(grid.at(source_y - l).at(source_x - j)-> get_cost() == -3)    //STOP WHEN SINK IS REACHED
                        flag = false;
                }
            }
             */
            if(x+1>=0 && x+1<=width -1 )
            {
                if(grid.at(y).at(x+1)->get_cost()== 0)   //  node never be waved
                {
                    grid.at(y).at(x+1) -> set_cost(((distance+1 + 3) % 4 )/2 + 1);
                    if(nodes_queue.size()<distance+2)
                    {
                        std::queue<Node*> queue;
                        nodes_queue.push_back(queue);
                        nodes_queue.at(distance+1).push(grid.at(y).at(x+1));
                    }
                    else
                        nodes_queue.at(distance+1).push(grid.at(y).at(x+1));
                    
                }
                if(grid.at(y).at(x+1)->get_cost()== -3)   // meets sink
                {
                    flag = false;
                }
            }
            if(x-1>=0 && x-1<=width -1)
            {
                if(grid.at(y).at(x-1)->get_cost()== 0)   //  node never be waved
                {
                    grid.at(y).at(x-1) -> set_cost(((distance+1 + 3) % 4 )/2 + 1);
                    if(nodes_queue.size()<distance+2)
                    {
                        std::queue<Node*> queue;
                        nodes_queue.push_back(queue);
                        nodes_queue.at(distance+1).push(grid.at(y).at(x-1));
                    }
                    else
                        nodes_queue.at(distance+1).push(grid.at(y).at(x-1));
                }
                if(grid.at(y).at(x-1)->get_cost()== -3)   // meets sink
                {
                    flag = false;
                }
            }
            if(y-1>=0 && y-1<=width -1)
            {
                if(grid.at(y-1).at(x)->get_cost()== 0)   //  node never be waved
                {
                    grid.at(y-1).at(x) -> set_cost(((distance+1 + 3) % 4 )/2 + 1);
                    if(nodes_queue.size()<distance+2)
                    {
                        std::queue<Node*> queue;
                        nodes_queue.push_back(queue);
                        nodes_queue.at(distance+1).push(grid.at(y-1).at(x));
                    }
                    else
                        nodes_queue.at(distance+1).push(grid.at(y-1).at(x));
                }
                if(grid.at(y-1).at(x)->get_cost()== -3)   // meets sink
                {
                    flag = false;
                }
                
            }
            if(y+1>=0 && y+1<=width -1)
            {
                if(grid.at(y+1).at(x)->get_cost()== 0)   // node never be waved
                {
                    grid.at(y+1).at(x) -> set_cost(((distance+1 + 3) % 4 )/2 + 1);
                    if(nodes_queue.size()<distance+2)
                    {
                        std::queue<Node*> queue;
                        nodes_queue.push_back(queue);
                        nodes_queue.at(distance+1).push(grid.at(y+1).at(x));
                    }
                    else
                        nodes_queue.at(distance+1).push(grid.at(y+1).at(x));
                }
                if(grid.at(y+1).at(x)->get_cost()== -3)   // meets sink
                {
                    flag = false;
                }
            }
            if(!nodes_queue.at(distance).empty())
            {
                x = nodes_queue.at(distance).front() -> get_x();
                y = nodes_queue.at(distance).front() -> get_y();
                nodes_queue.at(distance).pop();
            }
            else if(nodes_queue.at(distance+1).empty())  //the case if cannot find sink
            {
                claim("Cannot find sink for the current source", kError);
                continue;
            }
            else
            {
                x = nodes_queue.at(distance+1).front() -> get_x();
                y = nodes_queue.at(distance+1).front() -> get_y();
                nodes_queue.at(distance+1).pop();
                distance++;
            }

        }
        grid.at(source_y).at(source_x)->set_cost(0);
        // EXPANSION ENDS HERE
        for(int m = height -1; m>=0; m--)
            for(int n = 0; n<width; n++ )
            {
                if(n == width -1)
                    std::cout << grid.at(m).at(n)->get_cost()<< std::endl;
                else
                    std::cout << grid.at(m).at(n)->get_cost()<< " ";
            }
        // BACKTRACING STARTS HERE
        x = sink_x;
        y = sink_y;
        
        
        for(int step = distance+1; step > 0 ; step--)     // MOVE TO SOURCE IN THE DECREASING ORDER OF DISTANCE
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











void Utilities::grid_routing::Hadlock()
{
    initialize_map(-9,-1);
    vector<Connection> source_sink_connections = this->problem_object -> get_connections();
    int num_paths = this->problem_object -> get_connections().size();
    
    for(int i = 0; i < num_paths ; i++)
    {
        vector< std::queue<Node*> > detour_queue;           /* each queue store the nodes with same detour value */
        //vector<Edge*> edges;
        Path* new_path1 = new Path();
        Path* new_path2 = new Path();
        int source_x = source_sink_connections.at(i).source.x;
        int source_y = source_sink_connections.at(i).source.y;
        int sink_x = source_sink_connections.at(i).sink.x;
        int sink_y = source_sink_connections.at(i).sink.y;
        
        int height = problem_object->get_height();
        int width = problem_object->get_width();
        /*
        if(grid.at(source_y).at(source_x) -> get_cost()== -1)
        {
            claim("Source is in blocks, please change the coordinate of source",kError);
            break;
        }
        if(grid.at(sink_y).at(source_x) -> get_cost()== -1)
        {
            claim("Sink is in blocks, please change the coordinate of sink",kError);
            break;
        }
        */

        
        if(source_x >= 0 && source_x < width && source_y >= 0 && source_y < height && sink_x >= 0 && sink_x < width && sink_y >= 0 && sink_y < height)
        {
            grid.at(source_y).at(source_x) -> set_cost(0); //LABEL NODE AS COST=0 WHEN THE NODE IS IN A SOURCE
            grid.at(sink_y).at(sink_x) -> set_cost(-3); //LABEL NODE AS COST=-3 WHEN THE NODE IS IN A SINK
            
        }
        int x = source_x;
        int y = source_y;
        int d;
        
        std::queue<Node*> queue0;                /* since source's detour value is 0, we can add a queue first */
        detour_queue.push_back(queue0);
        
        bool flag = true;
        while(flag)
        {
            d = grid.at(y).at(x)->get_cost();
            if(x+1>=0 && x+1<=width -1)
            {
                if(grid.at(y).at(x+1)->get_cost()== -9)          /* the grid never be waved */
                {
                    if(sink_x >= source_x && x+1 <= sink_x)
                    {
                        grid.at(y).at(x+1) -> set_cost(d);
                        detour_queue.at(d).push(grid.at(y).at(x+1));
                        Node* head = grid.at(y).at(x);
                        Node* tail = grid.at(y).at(x+1);
                        Edge* edge = new Edge(head, tail);
                        grid.at(y).at(x+1) -> add_connection(edge);
                        
                    }
                    else
                    {
                        grid.at(y).at(x+1) -> set_cost(d+1);
                        if(detour_queue.size()< d+2)
                        {
                            std::queue<Node*> queue;
                            detour_queue.push_back(queue);
                            detour_queue.at(d+1).push(grid.at(y).at(x+1));
                            Node* head = new Node(x, y);
                            Node* tail = new Node(x+1,y);
                            Edge* edge = new Edge(head, tail);
                            grid.at(y).at(x+1) -> add_connection(edge);
                            
                        }
                        else
                        {
                            Node* head = new Node(x, y);
                            Node* tail = new Node(x+1,y);
                            Edge* edge = new Edge(head, tail);
                            grid.at(y).at(x+1) -> add_connection(edge);
                            detour_queue.at(d+1).push(grid.at(y).at(x+1));
                        }
                    }
                }
                if(grid.at(y).at(x+1) ->get_cost() == -3)         /* meet source */
                {
                    Node* head = new Node(x, y);
                    Node* tail = new Node(x+1,y);
                    Edge* edge = new Edge(head, tail);
                    grid.at(y).at(x+1) -> add_connection(edge);
                    
                    flag = false;
                }
            }
            if(x-1>=0 && x-1<=width -1)
            {
                if(grid.at(y).at(x-1)->get_cost()== -9)          /* the grid never be waved */
                {
                    if(sink_x <= source_x && x-1 >= sink_x)
                    {
                        grid.at(y).at(x-1) -> set_cost(d);
                        detour_queue.at(d).push(grid.at(y).at(x-1));
                        
                        Node* head = new Node(x, y);
                        Node* tail = new Node(x-1,y);
                        Edge* edge = new Edge(head, tail);
                        grid.at(y).at(x-1) -> add_connection(edge);
                        
                    }
                    else
                    {
                        grid.at(y).at(x-1) -> set_cost(d+1);
                        if(detour_queue.size()< d+2)              /* check if the queue store (d+1) is created, if not, create it */
                        {
                            std::queue<Node*> queue;
                            detour_queue.push_back(queue);
                            detour_queue.at(d+1).push(grid.at(y).at(x-1));
                            Node* head = new Node(x, y);
                            Node* tail = new Node(x-1,y);
                            Edge* edge = new Edge(head, tail);
                            grid.at(y).at(x-1) -> add_connection(edge);
                            
                        }
                        else
                        {
                            detour_queue.at(d+1).push(grid.at(y).at(x-1));
                            Node* head = new Node(x, y);
                            Node* tail = new Node(x-1,y);
                            Edge* edge = new Edge(head, tail);
                            grid.at(y).at(x-1) -> add_connection(edge);
                        }
                    }
                }
                if(grid.at(y).at(x-1)->get_cost() == -3)          /* meet source */
                {
                    Node* head = new Node(x, y);
                    Node* tail = new Node(x-1,y);
                    Edge* edge = new Edge(head, tail);
                    grid.at(y).at(x-1) -> add_connection(edge);
                    flag = false;
                }
            }
            if(y-1>=0 && y-1<=height -1)
            {
                if(grid.at(y-1).at(x)->get_cost()== -9)        /* the grid never be waved */
                {
                    if(sink_y <= source_y && y-1 >= sink_y)
                    {
                        grid.at(y-1).at(x) -> set_cost(d);
                        detour_queue.at(d).push(grid.at(y-1).at(x));
                        Node* head = new Node(x, y);
                        Node* tail = new Node(x,y-1);
                        Edge* edge = new Edge(head, tail);
                        grid.at(y-1).at(x) -> add_connection(edge);
                    }
                    else
                    {
                        grid.at(y-1).at(x) -> set_cost(d+1);
                        if(detour_queue.size()< d+2)               /* check if the queue store (d+1) is created, if not, create it */
                        {
                            std::queue<Node*> queue;
                            detour_queue.push_back(queue);
                            detour_queue.at(d+1).push(grid.at(y-1).at(x));
                            
                            Node* head = new Node(x, y);
                            Node* tail = new Node(x,y-1);
                            Edge* edge = new Edge(head, tail);
                            grid.at(y-1).at(x) -> add_connection(edge);
                
                        }
                        else
                        {
                            detour_queue.at(d+1).push(grid.at(y-1).at(x));
                            Node* head = new Node(x, y);
                            Node* tail = new Node(x,y-1);
                            Edge* edge = new Edge(head, tail);
                            grid.at(y-1).at(x) -> add_connection(edge);
                            
                        }

                    }
                }
                if(grid.at(y-1).at(x)->get_cost()== -3)            /* meet source */
                {
                    Node* head = new Node(x, y);
                    Node* tail = new Node(x,y-1);
                    Edge* edge = new Edge(head, tail);
                    grid.at(y-1).at(x) -> add_connection(edge);
                    flag = false;

                }
            }
            if(y+1>=0 && y+1<=height -1)
            {
                if(grid.at(y+1).at(x)->get_cost()== -9)           /* the grid never be waved */
                {
                    if(sink_y >= source_y && y+1 <= sink_y)
                    {
                        grid.at(y+1).at(x) -> set_cost(d);
                        detour_queue.at(d).push(grid.at(y+1).at(x));
                        Node* head = new Node(x, y);
                        Node* tail = new Node(x,y+1);
                        Edge* edge = new Edge(head, tail);
                        grid.at(y+1).at(x) -> add_connection(edge);

                    }
                    else
                    {
                        grid.at(y+1).at(x) -> set_cost(d+1);
                        if(detour_queue.size()< d+2)              /* check if the queue store (d+1) is created, if not, create it */
                        {
                            std::queue<Node*> queue;
                            detour_queue.push_back(queue);
                            detour_queue.at(d+1).push(grid.at(y+1).at(x));
                            Node* head = new Node(x, y);
                            Node* tail = new Node(x,y+1);
                            Edge* edge = new Edge(head, tail);
                            grid.at(y+1).at(x) -> add_connection(edge);
    
                        }
                        else
                        {
                            detour_queue.at(d+1).push(grid.at(y+1).at(x));
                            Node* head = new Node(x, y);
                            Node* tail = new Node(x,y+1);
                            Edge* edge = new Edge(head, tail);
                            grid.at(y+1).at(x) -> add_connection(edge);
                        }
                    }
                }
                if(grid.at(y+1).at(x)->get_cost()== -3)          /* meet source */
                {
                    Node* head = new Node(x, y);
                    Node* tail = new Node(x,y+1);
                    Edge* edge = new Edge(head, tail);
                    grid.at(y+1).at(x) -> add_connection(edge);
                    flag = false;
                }
            }
            
            if(!detour_queue.at(d).empty())  /*if the queue store current detour value isn't empty, means the four nodes around has same detour, we can wave it */
            {
                x = detour_queue.at(d).front() -> get_x();
                y = detour_queue.at(d).front() -> get_y();
                detour_queue.at(d).pop();
            }
            else if(!detour_queue.at(d+1).empty())                /* if the queue store current detour is empty,means the four nodes around's detour is larger,we have to check the first node with same detour stored to the queue */
            {
                x = detour_queue.at(d+1).front() -> get_x();
                y = detour_queue.at(d+1).front() -> get_y();
                detour_queue.at(d+1).pop();
            }
            else  //the case if cannot find sink
            {
                claim("Cannot find sink for the current source", kError);
                continue;
            }
        }
        //wave over
        int md = abs(source_x - sink_x) + abs(source_y - sink_y);
        
        for(int m = height -1; m>=0; m--)
            for(int n = 0; n<width; n++ )
            {
                if(n == width -1)
                    std::cout << grid.at(m).at(n)->get_cost()<< std::endl;
                else
                    std::cout << grid.at(m).at(n)->get_cost()<< " ";
            }
        
        //start to backtacing
        x = sink_x;
        y = sink_y;
        /*
        flag = true;
        while(flag)
        {
            int x1 = grid.at(y).at(x)-> connections_at(0) -> get_head() -> get_x();
            int y1 = grid.at(y).at(x)-> connections_at(0) -> get_head() -> get_y();
            PathSegment* path_segment = new PathSegment(x,y,x1, y1);
            new_path -> add_segment( path_segment);
            x = x1;
            y = y1;
             
            if(x == source_x && y == source_y)
                flag = false;
        }
        paths.push_back(new_path);
         */
        x = sink_x;
        y = sink_y;
        
        int direction_before = 0;
        int direction_now = 0;
        int c_turn1 = 0;
        int c_turn2 = 0;
        int distance = md + d*2;
        std::cout<< distance <<std::endl;
        
        grid.at(y).at(x)->set_cost(d);
        
        for(int step = 0; step < distance ; step++)     // MOVE TO SOURCE IN THE DECREASING ORDER OF DISTANCE
        {
            bool move = false;    // WHENEVER MAKE A MOVE
            d = grid.at(y).at(x)->get_cost();
            if(x+1 >= 0 && x+1 <= width-1)     // RIGHT STEP IS NOT OUT OF RANGE
            {
                if(grid.at(y).at(x+1)-> get_cost() == d && abs(y-sink_y)+abs(x+1-sink_x) == step+1)    // COST OF THIS STEP DECREASE BY 1
                {
                    Point head(x ,y);
                    x++;
                    Point tail(x ,y);
                    move = true;     // AVOID FURTHER MOVE IN THE SAME STEP
                    PathSegment* path_segment = new PathSegment(head, tail);
                    new_path1 -> add_segment(path_segment);      // ADD CURRENT MOVE TO NEW PATH
                    direction_now = 1;
                    if(direction_before!=direction_now)
                        c_turn1 ++;
                    direction_before = 1;
                    
                }
                else if(grid.at(y).at(x+1)-> get_cost() == d-1)
                {
                    Point head(x ,y);
                    x++;
                    Point tail(x ,y);
                    move = true;     // AVOID FURTHER MOVE IN THE SAME STEP
                    PathSegment* path_segment = new PathSegment(head, tail);
                    new_path1 -> add_segment(path_segment);      // ADD CURRENT MOVE TO NEW PATH
                    direction_now = 1;
                    if(direction_before!=direction_now)
                        c_turn1 ++;
                    direction_before = 1;
                }
                else
                    move = false;

            }
            
            if(x-1 >= 0 && x-1 <= width-1 && !move)     // LEFT STEP IS NOT OUT OF RANGE
            {
                if(grid.at(y).at(x-1)-> get_cost() == d && abs(y-sink_y)+abs(x-1-sink_x) == step+1)    // COST OF THIS STEP DECREASE BY 1
                {
                    Point head(x ,y);
                    x--;
                    Point tail(x ,y);
                    move = true;     // AVOID FURTHER MOVE IN THE SAME STEP
                    PathSegment* path_segment = new PathSegment(head, tail);
                    new_path1 -> add_segment(path_segment);      // ADD CURRENT MOVE TO NEW PATH
                    direction_now = 2;
                    if(direction_before!=direction_now)
                        c_turn1 ++;
                    direction_before = 2;
                }
                else if(grid.at(y).at(x-1)-> get_cost() == d-1)
                {
                    Point head(x ,y);
                    x--;
                    Point tail(x ,y);
                    move = true;     // AVOID FURTHER MOVE IN THE SAME STEP
                    PathSegment* path_segment = new PathSegment(head, tail);
                    new_path1 -> add_segment(path_segment);      // ADD CURRENT MOVE TO NEW PATH
                    direction_now = 2;
                    if(direction_before!=direction_now)
                        c_turn1 ++;
                    direction_before = 2;
                }
                else
                    move = false;

            }
            
            if(y+1 >= 0 && y+1 <= height-1 && !move)     // UP STEP IS NOT OUT OF RANGE
            {
                if(grid.at(y+1).at(x)-> get_cost() == d && abs(y+1-sink_y)+abs(x-sink_x) == step+1)    // COST OF THIS STEP DECREASE BY 1
                {
                    Point head(x ,y);
                    y++;
                    Point tail(x ,y);
                    move = true;     // AVOID FURTHER MOVE IN THE SAME STEP
                    PathSegment* path_segment = new PathSegment(head, tail);
                    new_path1 -> add_segment(path_segment);      // ADD CURRENT MOVE TO NEW PATH
                    direction_now = 3;
                    if(direction_before!=direction_now)
                        c_turn1 ++;
                    direction_before = 3;
                    
                }
                else if(grid.at(y+1).at(x)-> get_cost() == d-1)
                {
                    Point head(x ,y);
                    y++;
                    Point tail(x ,y);
                    move = true;     // AVOID FURTHER MOVE IN THE SAME STEP
                    PathSegment* path_segment = new PathSegment(head, tail);
                    new_path1 -> add_segment(path_segment);      // ADD CURRENT MOVE TO NEW PATH
                    direction_now = 3;
                    if(direction_before!=direction_now)
                        c_turn1 ++;
                    direction_before = 3;
                    
                }
                else
                    move = false;

            }
            
            if(y-1 >= 0 && y-1 <= height-1 && !move)     // DOWN STEP IS NOT OUT OF RANGE
            {
                if(grid.at(y-1).at(x)-> get_cost() == d && abs(y-1-sink_y)+abs(x-sink_x) == step+1)    // COST OF THIS STEP DECREASE BY 1
                {
                    Point head(x ,y);
                    y--;
                    Point tail(x ,y);
                    move = true;     // AVOID FURTHER MOVE IN THE SAME STEP
                    PathSegment* path_segment = new PathSegment(head, tail);
                    new_path1 -> add_segment(path_segment);      // ADD CURRENT MOVE TO NEW PATH
                    direction_now = 4;
                    if(direction_before!=direction_now)
                        c_turn1 ++;
                    direction_before = 4;
                }
                else if(grid.at(y-1).at(x)-> get_cost() == d-1)
                {
                    Point head(x ,y);
                    y--;
                    Point tail(x ,y);
                    move = true;     // AVOID FURTHER MOVE IN THE SAME STEP
                    PathSegment* path_segment = new PathSegment(head, tail);
                    new_path1 -> add_segment(path_segment);      // ADD CURRENT MOVE TO NEW PATH
                    direction_now = 4;
                    if(direction_before!=direction_now)
                        c_turn1 ++;
                    direction_before = 4;
            
                }
                else
                    move = false;

            }
        }
        
        
        x = sink_x;
        y = sink_y;
        for(int step = 0; step < distance ; step++)     // MOVE TO SOURCE IN THE DECREASING ORDER OF DISTANCE
        {
            bool move = false;    // WHENEVER MAKE A MOVE
            d = grid.at(y).at(x)->get_cost();
            if(y-1 >= 0 && y-1 <= height-1 && !move)     // DOWN STEP IS NOT OUT OF RANGE
            {
                if(grid.at(y-1).at(x)-> get_cost() == d && abs(y-1-sink_y)+abs(x-sink_x) == step+1)    // COST OF THIS STEP DECREASE BY 1
                {
                    Point head(x ,y);
                    y--;
                    Point tail(x ,y);
                    move = true;     // AVOID FURTHER MOVE IN THE SAME STEP
                    PathSegment* path_segment = new PathSegment(head, tail);
                    new_path2 -> add_segment(path_segment);      // ADD CURRENT MOVE TO NEW PATH
                    direction_now = 4;
                    if(direction_before!=direction_now)
                        c_turn2 ++;
                    direction_before = 4;
                }
                else if(grid.at(y-1).at(x)-> get_cost() == d-1)
                {
                    Point head(x ,y);
                    y--;
                    Point tail(x ,y);
                    move = true;     // AVOID FURTHER MOVE IN THE SAME STEP
                    PathSegment* path_segment = new PathSegment(head, tail);
                    new_path2 -> add_segment(path_segment);      // ADD CURRENT MOVE TO NEW PATH
                    direction_now = 4;
                    if(direction_before!=direction_now)
                        c_turn2 ++;
                    direction_before = 4;
                    
                }
                else
                    move = false;
                
            }
            if(y+1 >= 0 && y+1 <= height-1 && !move)     // UP STEP IS NOT OUT OF RANGE
            {
                if(grid.at(y+1).at(x)-> get_cost() == d && abs(y+1-sink_y)+abs(x-sink_x) == step+1)    // COST OF THIS STEP DECREASE BY 1
                {
                    Point head(x ,y);
                    y++;
                    Point tail(x ,y);
                    move = true;     // AVOID FURTHER MOVE IN THE SAME STEP
                    PathSegment* path_segment = new PathSegment(head, tail);
                    new_path2 -> add_segment(path_segment);      // ADD CURRENT MOVE TO NEW PATH
                    direction_now = 3;
                    if(direction_before!=direction_now)
                        c_turn2 ++;
                    direction_before = 3;
                    
                }
                else if(grid.at(y+1).at(x)-> get_cost() == d-1)
                {
                    Point head(x ,y);
                    y++;
                    Point tail(x ,y);
                    move = true;     // AVOID FURTHER MOVE IN THE SAME STEP
                    PathSegment* path_segment = new PathSegment(head, tail);
                    new_path2 -> add_segment(path_segment);      // ADD CURRENT MOVE TO NEW PATH
                    direction_now = 3;
                    if(direction_before!=direction_now)
                        c_turn2 ++;
                    direction_before = 3;
                    
                }
                else
                    move = false;
                
            }
            if(x-1 >= 0 && x-1 <= width-1 && !move)     // LEFT STEP IS NOT OUT OF RANGE
            {
                if(grid.at(y).at(x-1)-> get_cost() == d && abs(y-sink_y)+abs(x-1-sink_x) == step+1)    // COST OF THIS STEP DECREASE BY 1
                {
                    Point head(x ,y);
                    x--;
                    Point tail(x ,y);
                    move = true;     // AVOID FURTHER MOVE IN THE SAME STEP
                    PathSegment* path_segment = new PathSegment(head, tail);
                    new_path2 -> add_segment(path_segment);      // ADD CURRENT MOVE TO NEW PATH
                    direction_now = 2;
                    if(direction_before!=direction_now)
                        c_turn2 ++;
                    direction_before = 2;
                }
                else if(grid.at(y).at(x-1)-> get_cost() == d-1)
                {
                    Point head(x ,y);
                    x--;
                    Point tail(x ,y);
                    move = true;     // AVOID FURTHER MOVE IN THE SAME STEP
                    PathSegment* path_segment = new PathSegment(head, tail);
                    new_path2 -> add_segment(path_segment);      // ADD CURRENT MOVE TO NEW PATH
                    direction_now = 2;
                    if(direction_before!=direction_now)
                        c_turn2 ++;
                    direction_before = 2;
                }
                else
                    move = false;
                
            }
            if(x+1 >= 0 && x+1 <= width-1)     // RIGHT STEP IS NOT OUT OF RANGE
            {
                if(grid.at(y).at(x+1)-> get_cost() == d && abs(y-sink_y)+abs(x+1-sink_x) == step+1)    // COST OF THIS STEP DECREASE BY 1
                {
                    Point head(x ,y);
                    x++;
                    Point tail(x ,y);
                    move = true;     // AVOID FURTHER MOVE IN THE SAME STEP
                    PathSegment* path_segment = new PathSegment(head, tail);
                    new_path2 -> add_segment(path_segment);      // ADD CURRENT MOVE TO NEW PATH
                    direction_now = 1;
                    if(direction_before!=direction_now)
                        c_turn2 ++;
                    direction_before = 1;
                    
                }
                else if(grid.at(y).at(x+1)-> get_cost() == d-1)
                {
                    Point head(x ,y);
                    x++;
                    Point tail(x ,y);
                    move = true;     // AVOID FURTHER MOVE IN THE SAME STEP
                    PathSegment* path_segment = new PathSegment(head, tail);
                    new_path2 -> add_segment(path_segment);      // ADD CURRENT MOVE TO NEW PATH
                    direction_now = 1;
                    if(direction_before!=direction_now)
                        c_turn2 ++;
                    direction_before = 1;
                }
                else
                    move = false;
                
            }
            
        }
        std::cout << "c_turn1 "<<c_turn1<<std::endl;
        std::cout << "c_turn2 "<<c_turn2<<std::endl;
        if(c_turn1 < c_turn2)
            paths.push_back(new_path1);
        else
            paths.push_back(new_path2);
    }
    print();

}







void Utilities::grid_routing::Rubin_DFS()
{
    initialize_map(0,-1);
    vector<Connection> source_sink_connections = this->problem_object -> get_connections();
    int num_paths = this->problem_object -> get_connections().size();
    
    for(int i = 0; i < num_paths ; i++)
    {
        vector<std::stack<Node*> > detour_stack;
        Path* new_path = new Path();
        int source_x = source_sink_connections.at(i).source.x;
        int source_y = source_sink_connections.at(i).source.y;
        int sink_x = source_sink_connections.at(i).sink.x;
        int sink_y = source_sink_connections.at(i).sink.y;
        int height = problem_object->get_height();
        int width = problem_object->get_width();
        int md = abs(sink_x - source_x) + abs(sink_y - source_y);
        if(source_x >= 0 && source_x < width && source_y >= 0 && source_y < height && sink_x >= 0 && sink_x < width && sink_y >= 0 && sink_y < height)
        {
            grid.at(source_y).at(source_x) -> set_cost(md); //LABEL NODE AS COST=0 WHEN THE NODE IS IN A SOURCE
            grid.at(sink_y).at(sink_x) -> set_cost(-3); //LABEL NODE AS COST=-3 WHEN THE NODE IS IN A SINK
            
        }
        //grid.at(source_y).at(source_x) -> set_cost(0);     // COST AT SOURCE=0
        
        
        int d;     // STEPS FROM SOURCE
        
        // EXPANSION STARTS HERE
        bool flag = true;     // FLAG = FALSE WHEN SINK IS REACHED
        
        int x = source_x;
        int y = source_y;
        
        std::stack<Node*> stack0;
        detour_stack.push_back(stack0);
        while(flag)
        {
            d = grid.at(y).at(x)->get_cost();
            if(x+1>=0 && x+1<=width -1)
            {
                if(grid.at(y).at(x+1)->get_cost()== 0)          /* the grid never be waved */
                {
                    if(sink_x >= source_x && x+1 <= sink_x)
                    {
                        grid.at(y).at(x+1) -> set_cost(d);
                        detour_stack.at((d-md)/2).push(grid.at(y).at(x+1));
                        
                        Node* head = new Node(x,y);
                        Node* tail = new Node(x+1,y);
                        Edge* edge = new Edge(head, tail);
                        grid.at(y).at(x+1) -> add_connection(edge);
                        
                    }
                    else
                    {
                        grid.at(y).at(x+1) -> set_cost(d+2);
                        if(detour_stack.size()< (d-md)/2 + 2)              /* check if the queue store (d+1) is created, if not, create it */
                        {
                            Node* head = new Node(x, y);
                            Node* tail = new Node(x+1,y);
                            Edge* edge = new Edge(head, tail);
                            grid.at(y).at(x+1) -> add_connection(edge);
                        
                            std::stack<Node*> stack;
                            detour_stack.push_back(stack);
                            detour_stack.at((d+2-md)/2).push(grid.at(y).at(x+1));
                        }
                        else
                        {
                            Node* head = new Node(x, y);
                            Node* tail = new Node(x+1,y);
                            Edge* edge = new Edge(head, tail);
                            grid.at(y).at(x+1) -> add_connection(edge);
                            detour_stack.at((d+2-md)/2).push(grid.at(y).at(x+1));
                        }
                    }
                }
                if(grid.at(y).at(x+1) ->get_cost() == -3)         /* meet source */
                {
                    Node* head = new Node(x, y);
                    Node* tail = new Node(x+1,y);
                    Edge* edge = new Edge(head, tail);
                    grid.at(y).at(x+1) -> add_connection(edge);
                    
                    flag = false;
                }
            }
            if(x-1>=0 && x-1<=width -1)
            {
                if(grid.at(y).at(x-1)->get_cost()== 0)          /* the grid never be waved */
                {
                    if(sink_x <= source_x && x-1 >= sink_x)
                    {
                        grid.at(y).at(x-1) -> set_cost(d);
                        detour_stack.at((d-md)/2).push(grid.at(y).at(x-1));
        
                        Node* head = new Node(x, y);
                        Node* tail = new Node(x-1,y);
                        Edge* edge = new Edge(head, tail);
                        grid.at(y).at(x-1) -> add_connection(edge);
                        
                    }
                    else
                    {
                        grid.at(y).at(x-1) -> set_cost(d+2);
                        if(detour_stack.size()< (d-md)/2 + 2)              /* check if the queue store (d+1) is created, if not, create it */
                        {
                            std::stack<Node*> stack;
                            detour_stack.push_back(stack);
                            detour_stack.at((d+2-md)/2).push(grid.at(y).at(x-1));
                            Node* head = new Node(x, y);
                            Node* tail = new Node(x-1,y);
                            Edge* edge = new Edge(head, tail);
                            grid.at(y).at(x-1) -> add_connection(edge);
                            
                        }
                        else
                        {
                            detour_stack.at((d+2-md)/2).push(grid.at(y).at(x-1));
                            Node* head = new Node(x, y);
                            Node* tail = new Node(x-1,y);
                            Edge* edge = new Edge(head, tail);
                            grid.at(y).at(x-1) -> add_connection(edge);
                        }
                        
                        
                    }
                }
                if(grid.at(y).at(x-1)->get_cost() == -3)          /* meet source */
                {
                    Node* head = new Node(x, y);
                    Node* tail = new Node(x-1,y);
                    Edge* edge = new Edge(head, tail);
                    grid.at(y).at(x-1) -> add_connection(edge);
                    flag = false;
                }
            }
            if(y-1>=0 && y-1<=height -1)
            {
                if(grid.at(y-1).at(x)->get_cost()== 0)        /* the grid never be waved */
                {
                    if(sink_y <= source_y && y-1 >= sink_y)
                    {
                        grid.at(y-1).at(x) -> set_cost(d);
                        detour_stack.at((d-md)/2).push(grid.at(y-1).at(x));
                        Node* head = new Node(x, y);
                        Node* tail = new Node(x,y-1);
                        Edge* edge = new Edge(head, tail);
                        grid.at(y-1).at(x) -> add_connection(edge);
                    }
                    else
                    {
                        grid.at(y-1).at(x) -> set_cost(d+2);
                        if(detour_stack.size()< (d-md)/2+2)               /* check if the queue store (d+1) is created, if not, create it */
                        {
                            std::stack<Node*> stack;
                            detour_stack.push_back(stack);
                            detour_stack.at((d+2-md)/2).push(grid.at(y-1).at(x));
                            
                            Node* head = new Node(x, y);
                            Node* tail = new Node(x,y-1);
                            Edge* edge = new Edge(head, tail);
                            grid.at(y-1).at(x) -> add_connection(edge);
                            
                        }
                        else
                        {
                            detour_stack.at((d+2-md)/2).push(grid.at(y-1).at(x));
                            Node* head = new Node(x, y);
                            Node* tail = new Node(x,y-1);
                            Edge* edge = new Edge(head, tail);
                            grid.at(y-1).at(x) -> add_connection(edge);
                            
                        }
                        
                    }
                }
                if(grid.at(y-1).at(x)->get_cost()== -3)            /* meet source */
                {
                    Node* head = new Node(x, y);
                    Node* tail = new Node(x,y-1);
                    Edge* edge = new Edge(head, tail);
                    grid.at(y-1).at(x) -> add_connection(edge);
                    flag = false;
                    
                }
            }
            if(y+1>=0 && y+1<=height -1)
            {
                if(grid.at(y+1).at(x)->get_cost()== 0)           /* the grid never be waved */
                {
                    if(sink_y >= source_y && y+1 <= sink_y)
                    {
                        grid.at(y+1).at(x) -> set_cost(d);
                        detour_stack.at((d-md)/2).push(grid.at(y+1).at(x));
                        Node* head = new Node(x, y);
                        Node* tail = new Node(x,y+1);
                        Edge* edge = new Edge(head, tail);
                        grid.at(y+1).at(x) -> add_connection(edge);
                        
                    }
                    else
                    {
                        grid.at(y+1).at(x) -> set_cost(d+2);
                        if(detour_stack.size()< (d-md)/2+2)              /* check if the queue store (d+1) is created, if not, create it */
                        {
                            std::stack<Node*> stack;
                            detour_stack.push_back(stack);
                            detour_stack.at((d+2-md)/2).push(grid.at(y+1).at(x));
                            Node* head = new Node(x, y);
                            Node* tail = new Node(x,y+1);
                            Edge* edge = new Edge(head, tail);
                            grid.at(y+1).at(x) -> add_connection(edge);
                            
                        }
                        else
                        {
                            detour_stack.at((d+2-md)/2).push(grid.at(y+1).at(x));
                            Node* head = new Node(x, y);
                            Node* tail = new Node(x,y+1);
                            Edge* edge = new Edge(head, tail);
                            grid.at(y+1).at(x) -> add_connection(edge);
                        }
                    }
                }
                if(grid.at(y+1).at(x)->get_cost()== -3)          /* meet source */
                {
                    Node* head = new Node(x, y);
                    Node* tail = new Node(x,y+1);
                    Edge* edge = new Edge(head, tail);
                    grid.at(y+1).at(x) -> add_connection(edge);
                    flag = false;
                }
            }
           if(!detour_stack.at((d-md)/2).empty())  /*if the queue store current detour value isn't empty, means the four nodes around has same detour, we can wave it */
           {
               x = detour_stack.at((d-md)/2).top() -> get_x();
               y = detour_stack.at((d-md)/2).top() -> get_y();
               detour_stack.at((d-md)/2).pop();
           }
           else
           {
               x = detour_stack.at((d+2-md)/2).top() -> get_x();
               y = detour_stack.at((d+2-md)/2).top() -> get_y();
               detour_stack.at((d+2-md)/2).pop();
           }
        }
        // EXPAND OVER
        for(int m = height -1; m>=0; m--)
           for(int n = 0; n<width; n++ )
           {
               if(n == width -1)
                   std::cout << grid.at(m).at(n)->get_cost()<< std::endl;
               else
                   std::cout << grid.at(m).at(n)->get_cost()<< " ";
           }
        // START TO TRACEBACK
        x = sink_x;
        y = sink_y;
                                                       
                                                       
        flag = true;
        while(flag)
        {
            int x1 = grid.at(y).at(x)-> connections_at(0) -> get_head() -> get_x();
            int y1 = grid.at(y).at(x)-> connections_at(0) -> get_head() -> get_y();
            PathSegment* path_segment = new PathSegment(x,y,x1, y1);
            new_path -> add_segment( path_segment);
            x = x1;
            y = y1;
           
            if(x == source_x && y == source_y)
                flag = false;
        }
        paths.push_back(new_path);
    }
                                                       
    print();
    
}








void Utilities::grid_routing::Rubin_BFS()
{
    initialize_map(0,1);
    vector<Connection> source_sink_connections = this->problem_object -> get_connections();
    int num_paths = this->problem_object -> get_connections().size();
    
    for(int i = 0; i < num_paths ; i++)
    {
        vector<std::queue<Node*> > nodes_queue;
        Path* new_path1 = new Path();
        Path* new_path2 = new Path();
        int source_x = source_sink_connections.at(i).source.x;
        int source_y = source_sink_connections.at(i).source.y;
        int sink_x = source_sink_connections.at(i).sink.x;
        int sink_y = source_sink_connections.at(i).sink.y;
        int height = problem_object->get_height();
        int width = problem_object->get_width();
        int md = abs(sink_x - source_x) + abs(sink_y - source_y);
        if(source_x >= 0 && source_x < width && source_y >= 0 && source_y < height && sink_x >= 0 && sink_x < width && sink_y >= 0 && sink_y < height)
        {
            grid.at(source_y).at(source_x) -> set_cost(md); //LABEL NODE AS COST=0 WHEN THE NODE IS IN A SOURCE
            grid.at(sink_y).at(sink_x) -> set_cost(-3); //LABEL NODE AS COST=-3 WHEN THE NODE IS IN A SINK
            
        }
        //grid.at(source_y).at(source_x) -> set_cost(0);     // COST AT SOURCE=0
        
        
        int d;     // STEPS FROM SOURCE
        
        // EXPANSION STARTS HERE
        bool flag = true;     // FLAG = FALSE WHEN SINK IS REACHED
        
        int x = source_x;
        int y = source_y;
        
        std::queue<Node*> queue0;
        nodes_queue.push_back(queue0);
        while(flag)
        {
            d = grid.at(y).at(x)->get_cost();
            if(x+1>=0 && x+1<=width -1)
            {
                if(grid.at(y).at(x+1)->get_cost()== 0)          /* the grid never be waved */
                {
                    if(sink_x >= source_x && x+1 <= sink_x)
                    {
                        grid.at(y).at(x+1) -> set_cost(d);
                        nodes_queue.at((d-md)/2).push(grid.at(y).at(x+1));
                        
                        Node* head = new Node(x,y);
                        Node* tail = new Node(x+1,y);
                        Edge* edge = new Edge(head, tail);
                        grid.at(y).at(x+1) -> add_connection(edge);
                        
                    }
                    else
                    {
                        grid.at(y).at(x+1) -> set_cost(d+2);
                        if(nodes_queue.size()< (d-md)/2 + 2)              /* check if the queue store (d+1) is created, if not, create it */
                        {
                            Node* head = new Node(x, y);
                            Node* tail = new Node(x+1,y);
                            Edge* edge = new Edge(head, tail);
                            grid.at(y).at(x+1) -> add_connection(edge);
                        
                            std::queue<Node*> queue;
                            nodes_queue.push_back(queue);
                            nodes_queue.at((d+2-md)/2).push(grid.at(y).at(x+1));
                        }
                        else
                        {
                            Node* head = new Node(x, y);
                            Node* tail = new Node(x+1,y);
                            Edge* edge = new Edge(head, tail);
                            grid.at(y).at(x+1) -> add_connection(edge);
                            nodes_queue.at((d+2-md)/2).push(grid.at(y).at(x+1));

                        }
                    }
                }
                if(grid.at(y).at(x+1) ->get_cost() == -3)         /* meet source */
                {
                    Node* head = new Node(x, y);
                    Node* tail = new Node(x+1,y);
                    Edge* edge = new Edge(head, tail);
                    grid.at(y).at(x+1) -> add_connection(edge);
                    
                    flag = false;
                }
            }
            if(x-1>=0 && x-1<=width -1)
            {
                if(grid.at(y).at(x-1)->get_cost()== 0)          /* the grid never be waved */
                {
                    if(sink_x <= source_x && x-1 >= sink_x)
                    {
                        grid.at(y).at(x-1) -> set_cost(d);
                        nodes_queue.at((d-md)/2).push(grid.at(y).at(x-1));
                        
                        Node* head = new Node(x, y);
                        Node* tail = new Node(x-1,y);
                        Edge* edge = new Edge(head, tail);
                        grid.at(y).at(x-1) -> add_connection(edge);
                        
                    }
                    else
                    {
                        grid.at(y).at(x-1) -> set_cost(d+2);
                        if(nodes_queue.size()< (d-md)/2 + 2)              /* check if the queue store (d+1) is created, if not, create it */
                        {
                            std::queue<Node*> queue;
                            nodes_queue.push_back(queue);
                            nodes_queue.at((d+2-md)/2).push(grid.at(y).at(x-1));
                            Node* head = new Node(x, y);
                            Node* tail = new Node(x-1,y);
                            Edge* edge = new Edge(head, tail);
                            grid.at(y).at(x-1) -> add_connection(edge);
                            
                        }
                        else
                        {
                            nodes_queue.at((d+2-md)/2).push(grid.at(y).at(x-1));
                            Node* head = new Node(x, y);
                            Node* tail = new Node(x-1,y);
                            Edge* edge = new Edge(head, tail);
                            grid.at(y).at(x-1) -> add_connection(edge);
                        }
                        
                        
                    }
                }
                if(grid.at(y).at(x-1)->get_cost() == -3)          /* meet source */
                {
                    Node* head = new Node(x, y);
                    Node* tail = new Node(x-1,y);
                    Edge* edge = new Edge(head, tail);
                    grid.at(y).at(x-1) -> add_connection(edge);
                    flag = false;
                }
            }
            if(y-1>=0 && y-1<=height -1)
            {
                if(grid.at(y-1).at(x)->get_cost()== 0)        /* the grid never be waved */
                {
                    if(sink_y <= source_y && y-1 >= sink_y)
                    {
                        grid.at(y-1).at(x) -> set_cost(d);
                        nodes_queue.at((d-md)/2).push(grid.at(y-1).at(x));
                        Node* head = new Node(x, y);
                        Node* tail = new Node(x,y-1);
                        Edge* edge = new Edge(head, tail);
                        grid.at(y-1).at(x) -> add_connection(edge);
                    }
                    else
                    {
                        grid.at(y-1).at(x) -> set_cost(d+2);
                        if(nodes_queue.size()< (d-md)/2+2)               /* check if the queue store (d+1) is created, if not, create it */
                        {
                            std::queue<Node*> queue;
                            nodes_queue.push_back(queue);
                            nodes_queue.at((d+2-md)/2).push(grid.at(y-1).at(x));
                            
                            Node* head = new Node(x, y);
                            Node* tail = new Node(x,y-1);
                            Edge* edge = new Edge(head, tail);
                            grid.at(y-1).at(x) -> add_connection(edge);
                            
                        }
                        else
                        {
                            nodes_queue.at((d+2-md)/2).push(grid.at(y-1).at(x));
                            Node* head = new Node(x, y);
                            Node* tail = new Node(x,y-1);
                            Edge* edge = new Edge(head, tail);
                            grid.at(y-1).at(x) -> add_connection(edge);
                            
                        }
                        
                    }
                }
                if(grid.at(y-1).at(x)->get_cost()== -3)            /* meet source */
                {
                    Node* head = new Node(x, y);
                    Node* tail = new Node(x,y-1);
                    Edge* edge = new Edge(head, tail);
                    grid.at(y-1).at(x) -> add_connection(edge);
                    flag = false;
                    
                }
            }
            if(y+1>=0 && y+1<=height -1)
            {
                if(grid.at(y+1).at(x)->get_cost()== 0)           /* the grid never be waved */
                {
                    if(sink_y >= source_y && y+1 <= sink_y)
                    {
                        grid.at(y+1).at(x) -> set_cost(d);
                        nodes_queue.at((d-md)/2).push(grid.at(y+1).at(x));
                        Node* head = new Node(x, y);
                        Node* tail = new Node(x,y+1);
                        Edge* edge = new Edge(head, tail);
                        grid.at(y+1).at(x) -> add_connection(edge);
                        
                    }
                    else
                    {
                        grid.at(y+1).at(x) -> set_cost(d+2);
                        if(nodes_queue.size()< (d-md)/2+2)              /* check if the queue store (d+1) is created, if not, create it */
                        {
                            std::queue<Node*> queue;
                            nodes_queue.push_back(queue);
                            nodes_queue.at((d+2-md)/2).push(grid.at(y+1).at(x));
                            Node* head = new Node(x, y);
                            Node* tail = new Node(x,y+1);
                            Edge* edge = new Edge(head, tail);
                            grid.at(y+1).at(x) -> add_connection(edge);
                            
                        }
                        else
                        {
                            nodes_queue.at((d+2-md)/2).push(grid.at(y+1).at(x));
                            Node* head = new Node(x, y);
                            Node* tail = new Node(x,y+1);
                            Edge* edge = new Edge(head, tail);
                            grid.at(y+1).at(x) -> add_connection(edge);
                        }
                    }
                }
                if(grid.at(y+1).at(x)->get_cost()== -3)          /* meet source */
                {
                    Node* head = new Node(x, y);
                    Node* tail = new Node(x,y+1);
                    Edge* edge = new Edge(head, tail);
                    grid.at(y+1).at(x) -> add_connection(edge);
                    flag = false;
                }
            }
            if(!nodes_queue.at((d-md)/2).empty())  /*if the queue store current detour value isn't empty, means the four nodes around has same detour, we can wave it */
            {
                x = nodes_queue.at((d-md)/2).front() -> get_x();
                y = nodes_queue.at((d-md)/2).front() -> get_y();
                nodes_queue.at((d-md)/2).pop();
            }
            else
            {
                x = nodes_queue.at((d+2-md)/2).front() -> get_x();
                y = nodes_queue.at((d+2-md)/2).front() -> get_y();
                nodes_queue.at((d+2-md)/2).pop();
            }
        }
        for(int m = height -1; m>=0; m--)
            for(int n = 0; n<width; n++ )
            {
                if(n == width -1)
                    std::cout << grid.at(m).at(n)->get_cost()<< std::endl;
                else
                    std::cout << grid.at(m).at(n)->get_cost()<< " ";
            }
        std::cout << d <<std::endl;
        /*
        x = sink_x;
        y = sink_y;
        
        flag = true;
        while(flag)
        {
            int x1 = grid.at(y).at(x)-> connections_at(0) -> get_head() -> get_x();
            int y1 = grid.at(y).at(x)-> connections_at(0) -> get_head() -> get_y();
            PathSegment* path_segment = new PathSegment(x,y,x1, y1);
            new_path -> add_segment( path_segment);
            x = x1;
            y = y1;
            
            if(x == source_x && y == source_y)
                flag = false;
        }
        paths.push_back(new_path);
         */
        x = sink_x;
        y = sink_y;
        
        int direction_before = 0;
        int direction_now = 0;
        int c_turn1 = 0;
        int c_turn2 = 0;
        int distance = d;
        std::cout<< distance <<std::endl;
        
        grid.at(y).at(x)->set_cost(d);
        
        for(int step = 0; step < distance ; step++)     // MOVE TO SOURCE IN THE DECREASING ORDER OF DISTANCE
        {
            bool move = false;    // WHENEVER MAKE A MOVE
            d = grid.at(y).at(x)->get_cost();
            if(x+1 >= 0 && x+1 <= width-1)     // RIGHT STEP IS NOT OUT OF RANGE
            {
                if(grid.at(y).at(x+1)-> get_cost() == d && abs(y-sink_y)+abs(x+1-sink_x) == step+1)    // COST OF THIS STEP DECREASE BY 1
                {
                    Point head(x ,y);
                    x++;
                    Point tail(x ,y);
                    move = true;     // AVOID FURTHER MOVE IN THE SAME STEP
                    PathSegment* path_segment = new PathSegment(head, tail);
                    new_path1 -> add_segment(path_segment);      // ADD CURRENT MOVE TO NEW PATH
                    direction_now = 1;
                    if(direction_before!=direction_now)
                        c_turn1 ++;
                    direction_before = 1;
                    
                }
                else if(grid.at(y).at(x+1)-> get_cost() == d-2)
                {
                    Point head(x ,y);
                    x++;
                    Point tail(x ,y);
                    move = true;     // AVOID FURTHER MOVE IN THE SAME STEP
                    PathSegment* path_segment = new PathSegment(head, tail);
                    new_path1 -> add_segment(path_segment);      // ADD CURRENT MOVE TO NEW PATH
                    direction_now = 1;
                    if(direction_before!=direction_now)
                        c_turn1 ++;
                    direction_before = 1;
                }
                else
                    move = false;
                
            }
            
            if(x-1 >= 0 && x-1 <= width-1 && !move)     // LEFT STEP IS NOT OUT OF RANGE
            {
                if(grid.at(y).at(x-1)-> get_cost() == d && abs(y-sink_y)+abs(x-1-sink_x) == step+1)    // COST OF THIS STEP DECREASE BY 1
                {
                    Point head(x ,y);
                    x--;
                    Point tail(x ,y);
                    move = true;     // AVOID FURTHER MOVE IN THE SAME STEP
                    PathSegment* path_segment = new PathSegment(head, tail);
                    new_path1 -> add_segment(path_segment);      // ADD CURRENT MOVE TO NEW PATH
                    direction_now = 2;
                    if(direction_before!=direction_now)
                        c_turn1 ++;
                    direction_before = 2;
                }
                else if(grid.at(y).at(x-1)-> get_cost() == d-2)
                {
                    Point head(x ,y);
                    x--;
                    Point tail(x ,y);
                    move = true;     // AVOID FURTHER MOVE IN THE SAME STEP
                    PathSegment* path_segment = new PathSegment(head, tail);
                    new_path1 -> add_segment(path_segment);      // ADD CURRENT MOVE TO NEW PATH
                    direction_now = 2;
                    if(direction_before!=direction_now)
                        c_turn1 ++;
                    direction_before = 2;
                }
                else
                    move = false;
                
            }
            
            if(y+1 >= 0 && y+1 <= height-1 && !move)     // UP STEP IS NOT OUT OF RANGE
            {
                if(grid.at(y+1).at(x)-> get_cost() == d && abs(y+1-sink_y)+abs(x-sink_x) == step+1)    // COST OF THIS STEP DECREASE BY 1
                {
                    Point head(x ,y);
                    y++;
                    Point tail(x ,y);
                    move = true;     // AVOID FURTHER MOVE IN THE SAME STEP
                    PathSegment* path_segment = new PathSegment(head, tail);
                    new_path1 -> add_segment(path_segment);      // ADD CURRENT MOVE TO NEW PATH
                    direction_now = 3;
                    if(direction_before!=direction_now)
                        c_turn1 ++;
                    direction_before = 3;
                    
                }
                else if(grid.at(y+1).at(x)-> get_cost() == d-2)
                {
                    Point head(x ,y);
                    y++;
                    Point tail(x ,y);
                    move = true;     // AVOID FURTHER MOVE IN THE SAME STEP
                    PathSegment* path_segment = new PathSegment(head, tail);
                    new_path1 -> add_segment(path_segment);      // ADD CURRENT MOVE TO NEW PATH
                    direction_now = 3;
                    if(direction_before!=direction_now)
                        c_turn1 ++;
                    direction_before = 3;
                    
                }
                else
                    move = false;
                
            }
            
            if(y-1 >= 0 && y-1 <= height-1 && !move)     // DOWN STEP IS NOT OUT OF RANGE
            {
                if(grid.at(y-1).at(x)-> get_cost() == d && abs(y-1-sink_y)+abs(x-sink_x) == step+1)    // COST OF THIS STEP DECREASE BY 1
                {
                    Point head(x ,y);
                    y--;
                    Point tail(x ,y);
                    move = true;     // AVOID FURTHER MOVE IN THE SAME STEP
                    PathSegment* path_segment = new PathSegment(head, tail);
                    new_path1 -> add_segment(path_segment);      // ADD CURRENT MOVE TO NEW PATH
                    direction_now = 4;
                    if(direction_before!=direction_now)
                        c_turn1 ++;
                    direction_before = 4;
                }
                else if(grid.at(y-1).at(x)-> get_cost() == d-2)
                {
                    Point head(x ,y);
                    y--;
                    Point tail(x ,y);
                    move = true;     // AVOID FURTHER MOVE IN THE SAME STEP
                    PathSegment* path_segment = new PathSegment(head, tail);
                    new_path1 -> add_segment(path_segment);      // ADD CURRENT MOVE TO NEW PATH
                    direction_now = 4;
                    if(direction_before!=direction_now)
                        c_turn1 ++;
                    direction_before = 4;
                    
                }
                else
                    move = false;
                
            }
        }
        
        
        x = sink_x;
        y = sink_y;
        for(int step = 0; step < distance ; step++)     // MOVE TO SOURCE IN THE DECREASING ORDER OF DISTANCE
        {
            bool move = false;    // WHENEVER MAKE A MOVE
            d = grid.at(y).at(x)->get_cost();
            if(y-1 >= 0 && y-1 <= height-1 && !move)     // DOWN STEP IS NOT OUT OF RANGE
            {
                if(grid.at(y-1).at(x)-> get_cost() == d && abs(y-1-sink_y)+abs(x-sink_x) == step+1)    // COST OF THIS STEP DECREASE BY 1
                {
                    Point head(x ,y);
                    y--;
                    Point tail(x ,y);
                    move = true;     // AVOID FURTHER MOVE IN THE SAME STEP
                    PathSegment* path_segment = new PathSegment(head, tail);
                    new_path2 -> add_segment(path_segment);      // ADD CURRENT MOVE TO NEW PATH
                    direction_now = 4;
                    if(direction_before!=direction_now)
                        c_turn2 ++;
                    direction_before = 4;
                }
                else if(grid.at(y-1).at(x)-> get_cost() == d-2)
                {
                    Point head(x ,y);
                    y--;
                    Point tail(x ,y);
                    move = true;     // AVOID FURTHER MOVE IN THE SAME STEP
                    PathSegment* path_segment = new PathSegment(head, tail);
                    new_path2 -> add_segment(path_segment);      // ADD CURRENT MOVE TO NEW PATH
                    direction_now = 4;
                    if(direction_before!=direction_now)
                        c_turn2 ++;
                    direction_before = 4;
                    
                }
                else
                    move = false;
                
            }
            if(y+1 >= 0 && y+1 <= height-1 && !move)     // UP STEP IS NOT OUT OF RANGE
            {
                if(grid.at(y+1).at(x)-> get_cost() == d && abs(y+1-sink_y)+abs(x-sink_x) == step+1)    // COST OF THIS STEP DECREASE BY 1
                {
                    Point head(x ,y);
                    y++;
                    Point tail(x ,y);
                    move = true;     // AVOID FURTHER MOVE IN THE SAME STEP
                    PathSegment* path_segment = new PathSegment(head, tail);
                    new_path2 -> add_segment(path_segment);      // ADD CURRENT MOVE TO NEW PATH
                    direction_now = 3;
                    if(direction_before!=direction_now)
                        c_turn2 ++;
                    direction_before = 3;
                    
                }
                else if(grid.at(y+1).at(x)-> get_cost() == d-2)
                {
                    Point head(x ,y);
                    y++;
                    Point tail(x ,y);
                    move = true;     // AVOID FURTHER MOVE IN THE SAME STEP
                    PathSegment* path_segment = new PathSegment(head, tail);
                    new_path2 -> add_segment(path_segment);      // ADD CURRENT MOVE TO NEW PATH
                    direction_now = 3;
                    if(direction_before!=direction_now)
                        c_turn2 ++;
                    direction_before = 3;
                    
                }
                else
                    move = false;
                
            }
            if(x-1 >= 0 && x-1 <= width-1 && !move)     // LEFT STEP IS NOT OUT OF RANGE
            {
                if(grid.at(y).at(x-1)-> get_cost() == d && abs(y-sink_y)+abs(x-1-sink_x) == step+1)    // COST OF THIS STEP DECREASE BY 1
                {
                    Point head(x ,y);
                    x--;
                    Point tail(x ,y);
                    move = true;     // AVOID FURTHER MOVE IN THE SAME STEP
                    PathSegment* path_segment = new PathSegment(head, tail);
                    new_path2 -> add_segment(path_segment);      // ADD CURRENT MOVE TO NEW PATH
                    direction_now = 2;
                    if(direction_before!=direction_now)
                        c_turn2 ++;
                    direction_before = 2;
                }
                else if(grid.at(y).at(x-1)-> get_cost() == d-2)
                {
                    Point head(x ,y);
                    x--;
                    Point tail(x ,y);
                    move = true;     // AVOID FURTHER MOVE IN THE SAME STEP
                    PathSegment* path_segment = new PathSegment(head, tail);
                    new_path2 -> add_segment(path_segment);      // ADD CURRENT MOVE TO NEW PATH
                    direction_now = 2;
                    if(direction_before!=direction_now)
                        c_turn2 ++;
                    direction_before = 2;
                }
                else
                    move = false;
                
            }
            if(x+1 >= 0 && x+1 <= width-1)     // RIGHT STEP IS NOT OUT OF RANGE
            {
                if(grid.at(y).at(x+1)-> get_cost() == d && abs(y-sink_y)+abs(x+1-sink_x) == step+1)    // COST OF THIS STEP DECREASE BY 1
                {
                    Point head(x ,y);
                    x++;
                    Point tail(x ,y);
                    move = true;     // AVOID FURTHER MOVE IN THE SAME STEP
                    PathSegment* path_segment = new PathSegment(head, tail);
                    new_path2 -> add_segment(path_segment);      // ADD CURRENT MOVE TO NEW PATH
                    direction_now = 1;
                    if(direction_before!=direction_now)
                        c_turn2 ++;
                    direction_before = 1;
                    
                }
                else if(grid.at(y).at(x+1)-> get_cost() == d-2)
                {
                    Point head(x ,y);
                    x++;
                    Point tail(x ,y);
                    move = true;     // AVOID FURTHER MOVE IN THE SAME STEP
                    PathSegment* path_segment = new PathSegment(head, tail);
                    new_path2 -> add_segment(path_segment);      // ADD CURRENT MOVE TO NEW PATH
                    direction_now = 1;
                    if(direction_before!=direction_now)
                        c_turn2 ++;
                    direction_before = 1;
                }
                else
                    move = false;
                
            }
            
        }
        std::cout << "c_turn1 "<<c_turn1<<std::endl;
        std::cout << "c_turn2 "<<c_turn2<<std::endl;
        if(c_turn1 < c_turn2)
            paths.push_back(new_path1);
        else
            paths.push_back(new_path2);

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

