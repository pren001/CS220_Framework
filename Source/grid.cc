#include "../Headers/grid.h"
#include "../Headers/edge.h"
#include "../Headers/claim.h"
#include "../Headers/problem_object.h"
#include "math.h"

//Takes an x and y coordinate as input and creates a grid of that size filled with default nodes
Utilities::Grid::Grid(ProblemObject* problem_object) {
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
      
      /////////////////////////////////////////////

      vector<Blocker> blockers = problem_object -> get_blockers();
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
      				grid.at(h).at(w) -> set_cost(-1);
      			}
      		}
      }
      
      vector<Connection> source_sink_connections = problem_object -> get_connections();
      num_connections = problem_object -> get_connections().size(); ///////第一题晚上改错了，grid本身有num_connections，不需要额外设
      for(int i = 0; i < num_connections ; i++)
      {
          int source_x = source_sink_connections.at(i).source.x;
          int source_y = source_sink_connections.at(i).source.y;
          int sink_x = source_sink_connections.at(i).sink.x;
          int sink_y = source_sink_connections.at(i).sink.y;
          grid.at(source_y).at(source_x) -> set_cost(-2);
          grid.at(sink_y).at(sink_x) -> set_cost(-3);
      }
      ///////////////////////////////////////////
}

//Destructs the grid by deleting each node individually, the node destructors will delete their own set of edges
Utilities::Grid::~Grid() {
      int width = this->get_width();
      int height = this->get_height();
      for(int x = 0; x < width; x++) {
            for(int y = 0; y < width; y++) {
                  delete grid.at(y).at(x);
            }
      }
}

int Utilities::Grid::get_width() {
	//Assumes a perfect rectangle
	return grid.empty()?0:grid.at(0).size();
}

int Utilities::Grid::get_height() {
	//Assumes a perfect rectangle
	return this->grid.size();
}

int Utilities::Grid::get_num_connections() {
  return this->num_connections;
}

Node* Utilities::Grid::get_node(int x, int y) {
      if(y >= this->grid.size()) {
        claim("Attemping to access a node outside of the grid's range (y-value out of range)", kError);
	    return NULL;
      }
      if(x >= this->grid.at(y).size()) {
        claim("Attemping to access a node outside of the grid's range (x-value out of range)", kError);
	    return NULL;
      }
      return this->grid.at(y).at(x);      //at(...) will throw an out of range exception
}

Node* Utilities::Grid::get_node(Point coord) {
      return this->get_node(coord.x, coord.y);
}

vector<Path*> Utilities::Grid::get_paths() {
      return this->paths;
}

Path* Utilities::Grid::get_path(int i) {
      if(i >= paths.size()) {
            claim("Attempting to access a path outside of the path list's range", kError);
      }
      return this->paths.at(i);           //at(...) will throw an out of range exception
}

/*
Since each node has an x/y coordinate, if you pass a Node* into replace node, it will take the node at the passed in
Nodes location, delete it, and then place the passed in node into the grid at its proper location.
*/
void Utilities::Grid::replace_node(Node* replacement_node) {
      delete this->grid.at(replacement_node->get_y()).at(replacement_node->get_x());
      this->grid.at(replacement_node->get_y()).at(replacement_node->get_x()) = replacement_node;
}

void Utilities::Grid::set_paths(vector<Path*> paths) {
      this->paths = paths;
}

void Utilities::Grid::add_path(Path* path) {
      this->paths.push_back(path);
}

void Utilities::Grid::replace_path(int i, Path* path) {
      if(i >= this->paths.size()) {
            claim("Attemping to replace path outside of the path list's range", kError);
      }
      paths.at(i) = path;
}

void Utilities::Grid::remove_path(int i) {
      if(i >= paths.size()) {
      	claim("Attempting to remove a path outside of the path list's range", kError);
      }
      vector<Path*>::iterator it = this->paths.begin();
      it += i;
      paths.erase(it);
}

//Note, we create random paths just as an example of how to create paths, netlists are created similarly
vector<Path*> Utilities::Grid::test_algorithm() {
    vector<Path*> paths;
    srand(time(NULL));
    int number_paths = this->get_num_connections();
    for (int i = 0;i < number_paths;i++) {
      Path* new_path = new Path();
      int x = rand() % this->get_width();
      int y = rand() % this->get_height();
      int path_length = 1+rand()%10;
      for (unsigned j = 0;j < path_length;j++) {
        bool direction = rand()%2;
        Point head(x,y);
        direction?x+=1:y+=1;
        Point tail(x,y);
        PathSegment* new_segment = new PathSegment(head,tail);
        new_path->add_segment(new_segment);
      }
      paths.push_back(new_path);
    }
    return paths;
}
/////////////////////////////////////
vector<Path*> Utilities::Grid::Lee_algorithm(ProblemObject* problem_object)
{
	vector<Path*> paths;
	int num_paths = this -> get_num_connections();
	vector<Connection> source_sink_connections = problem_object -> get_connections();
	
	for(int i = 0; i < num_paths ; i++)
	{
		Path* new_path = new Path();
		int source_x = source_sink_connections.at(i).source.x;
        	int source_y = source_sink_connections.at(i).source.y;
          	int sink_x = source_sink_connections.at(i).sink.x;
          	int sink_y = source_sink_connections.at(i).sink.y;
          	
          	int height = this -> get_height();
          	int width = this -> get_width();
          	//grid.at(source_y).at(source_x) -> set_cost(0);
          	int d = 0;
          	bool flag = true;
          	while(flag)
          	{
          		d = d + 1;
          		for(int j = -d ; j< d; j++)
          		{
          			int l = d - abs(j);
          			if(source_y + l>=0 && source_y + l<= height-1 && source_x + j >=0 && source_x + j <= width -1)
          			{
          				if(grid.at(source_y + l).at(source_x + j)-> get_cost() = 0)
          					grid.at(source_y + l).at(source_x + j) -> set_cost(d);
          				if(grid.at(source_y + l).at(source_x + j)-> get_cost() = -3)
          					flag = false;
          			}
          			if(source_y - l>=0 && source_y - l<= height-1 && source_x - j >=0 && source_x - j <= width -1)
          			{
          				if(grid.at(source_y - l).at(source_x - j)-> get_cost() = 0)
          					grid.at(source_y - l).at(source_x - j) -> set_cost(d);
          				if(grid.at(source_y - l).at(source_x - j)-> get_cost() = -3)
          					flag = false;
          			}
          		}
          	}
          	int x = sink_x;
          	int y = sink_y;
          	for(int j = 0; j<d ; j++)
          	{
          		if(x+1 >= 0 && x+1 <= width-1)
          		{
          			if(grid.at(y).at(x+1)-> get_cost() == d-1)
          			{
          				Point tail(x ,y);
          				x++;
          				Point head(x ,y);
          				PathSegment* path_segment = new PathSegment(head, tail);
          				new_path -> add_segment(path_segment);
          			}
          				
          		}
          		else if(x-1 >= 0 && x-1 <= width-1)
          		{
          			if(grid.at(y).at(x-1)-> get_cost() == d-1)
          			{
          				Point tail(x ,y);
          				x--;
          				Point head(x ,y);
          				PathSegment* path_segment = new PathSegment(head, tail);
          				new_path -> add_segment(path_segment);
          			}
          		}
          		else if(y+1 >= 0 && y+1 <= height-1)
          		{
          			if(grid.at(y+1).at(x)-> get_cost() == d-1)
          			{
          				Point tail(x ,y);
          				y++;
          				Point head(x ,y);
          				PathSegment* path_segment = new PathSegment(head, tail);
          				new_path -> add_segment(path_segment);
          			}
          		}
          		else if(y-1 >= 0 && y-1 <= height-1)
          		{
          			if(grid.at(y-1).at(x)-> get_cost() == d-1)
          			{
          				Point tail(x ,y);
          				y--;
          				Point head(x ,y);
          				PathSegment* path_segment = new PathSegment(head, tail);
          				new_path -> add_segment(path_segment);
          			}
          		}
          		
          	}
          	paths.push_back(new_path);
	}
	return paths;
}
