#include "path.h"
#include <queue>
#include <stack>
using namespace std;


// vector <pair<int,int>> pathPts;

// pair<int,int> start;
// pair<int,int> end;
// PNG image;


// initializes variables and calls BFS to initialize path.
path::path(const PNG & im, pair<int,int> s, pair<int,int> e)
   :start(s),end(e),image(im){
    BFS();
}

// used to initialize member variable pathPts.
// called by constructor to create path if it 
// exists. 
//
// requires: neighbors, good, assemble helpers
//
// See the interfaces for good and assemble
// to get a hint on two auxiliary structures
// you will want to build: predecessor table,
// and visited table.
void path::BFS(){
	// initialize working vectors
	vector<vector<bool>> V(image.height(), vector<bool> (image.width(),false));
	vector<vector<pair<int,int>>> P(image.height(), vector<pair<int,int>> (image.width(),end));

  queue<pair<int,int>> myqueue;

  myqueue.push(start);

  //V[start.second][start.first] = true;

    for (int y = 0; y < (int) image.height(); ++y)
    {
      for (int x = 0; x < (int) image.width(); ++x)
      {
        //pair<int,int> coordinate = make_pair(x, y);
        P[y][x] = end;
        V[y][x] = false;
      }
    }

  while (!myqueue.empty())
  {
    pair<int,int> temp = myqueue.front();
    myqueue.pop();
    vector<pair<int,int>> tempNeighbors;
    tempNeighbors = neighbors(temp);

    //std::cout<< "Before for loop" <<endl;

    for (int i = 0; i < (int) tempNeighbors.size() ; ++i)
    {
      //std::cout<< "Before if statement" <<endl;
      if (good(V, temp, tempNeighbors[i]))
      {
        // std::cout<< "image width:" << image.width()<<endl;
        // std::cout<< "image height:" << image.height()<<endl;
        // std::cout<< "y value:" << tempNeighbors[i].second <<endl;
        // std::cout<< "x value:" << tempNeighbors[i].first  <<endl;
        //std::cout<< "Before V" <<endl;
        V[tempNeighbors[i].second][tempNeighbors[i].first] = true;
        //std::cout<< "After V" <<endl;
        P[tempNeighbors[i].second][tempNeighbors[i].first] = temp;
        //std::cout<< "After P" <<endl;
        myqueue.push(tempNeighbors[i]);
        //std::cout<< "After push" <<endl;
      }
    }
    //std::cout<< "Before V temp" <<endl;
    V[temp.second][temp.first] = true;
    //std::cout<< "After V temp" <<endl;
  }
  //std::cout<< "Before assemble" <<endl;
	pathPts = assemble(P,start,end);
  //std::cout<< "After assemble" <<endl;
}

//draws path points in red on a copy of the image and returns it
PNG path::render(){
  PNG copy = PNG(image);
  //std::cout<< length()<<endl;
  for (int i = 0; i < length(); ++i)
  {
    pair<int,int> temp = pathPts[i];
    RGBAPixel *pixel = copy.getPixel(temp.first, temp.second);
    pixel->r = 255;
    pixel->g = 0;
    pixel->b = 0;
  }
  return copy;
    


}

//returns path of points
vector<pair<int,int>> path::getPath() { return pathPts;}

//returns length of shortest path of points
int path::length() { return pathPts.size();}


// tests a neighbor (adjacent vertex) to see if it is 
// 1. within the image, 2. unvisited, and 3. close in color to curr.
// An entry in table V is true if a cell has previously been 
// visited.
bool path::good(vector<vector<bool>> & v, pair<int,int> curr, pair<int,int> next){
 
  //std::cout<< next.first <<endl;
  //std::cout<< next.second <<endl;
  if (next.first < (int) image.width() && next.first >= 0 && next.second >= 0 && next.second < (int) image.height() &&
    v[next.second][next.first] == false && closeEnough(*image.getPixel(curr.first, curr.second), *image.getPixel(next.first, next.second) ))
  {
    //v[next.first][next.second] = true;
    return true;
  }
  else
  {
    return false;
  }
}

// builds a vector containing the locations of the 
// four vertices adjacent to curr:
// above, left, right, below (no particular order). 
// does not pay any attention to whether or not the neighbors are 
// valid (in the image, previously visited, or the right color).
vector<pair<int,int>> path::neighbors(pair<int,int> curr) {

	vector<pair<int,int>> n;
  n.push_back(make_pair(curr.first - 1, curr.second     ));
  n.push_back(make_pair(curr.first    , curr.second + 1 ));
  n.push_back(make_pair(curr.first + 1 , curr.second    ));
  n.push_back(make_pair(curr.first    , curr.second - 1 ));
  //vector<pair<int,int>> nnn = goodNeighbors(n, curr);
  return n;

    /* your code here */

}

// vector<pair<int,int>> goodNeighbors(vector<pair<int,int>> n, pair<int,int> curr)
// {
//   vector<pair<int,int>> nn;

//   for (int i = 0; i < n.size(); ++i)
//   {
//     /* code */
//   }

// }

// Assumes the predecessor table, built in the BFS as follows: For each 
// location in the image reachable from the start vertex, "loc", the 
// table contains the location "pred" from which "loc" was first seen.
// ("pred", "loc") is thus an edge in the shortest path from s to 
// "loc".
//
// returns the set of points on the shortest path from s to e, if 
// it exists. Call this vector P.
//
// if there is a shortest path: position 0 should contain s, 
// and for all 0 < i < size, P[0] to P[i] is the set of points
// on the shortest path from s to point P[i]. P[size-1] == e.

// if no path from s to e exists, then just return a single element
// vector P with P[0] == s.
vector<pair<int,int>> path::assemble(vector<vector<pair<int,int>>> & p,pair<int,int> s, pair<int,int> e) {



  //hint, gold code contains the following line:
	stack<pair<int,int>> S;
 
  S.push(e);
  while(s != e)
  {
    S.push(p[e.second][e.first]);
    e = p[e.second][e.first];

  }
  vector<pair<int,int>> shortestPath;
  while(!S.empty())
  {
    shortestPath.push_back(S.top());
    S.pop();
  }
  return shortestPath;

}

// tests whether p1 and p2 are near in color. returns 
// true if the sum of squared difference over color channels
// is less than or equal to 80. 

bool path::closeEnough(RGBAPixel p1, RGBAPixel p2){
   int dist = (p1.r-p2.r)*(p1.r-p2.r) + (p1.g-p2.g)*(p1.g-p2.g) +
               (p1.b-p2.b)*(p1.b-p2.b);

   return (dist <= 80);
}

// vector <pair<int,int>> pathPts;

// pair<int,int> start;
// pair<int,int> end;
// PNG image;
