#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Graph.h"

using namespace std;
// -------------------------------------------------------------------------
struct sPoint
{
  float X, Y, Z;
  float distanceTo(const sPoint &b) const
  {
    float x = X - b.X;
    float y = Y - b.Y;
    float z = Z - b.Z;
    return (std::sqrt((x * x) + (y * y) + (z * z)));
  }
  bool operator<(const sPoint &b) const
  {
    return ((this->X < b.X) || (this->Y < b.Y) || (this->Z < b.Z));
  }
  bool operator==(const sPoint &b) const
  {
    return ((this->X == b.X) && (this->Y == b.Y) && (this->Z == b.Z));
  }
  string to_string() const
  {
    return ("Point (" + std::to_string(this->X) + ", " + std::to_string(this->Y) + ", " + std::to_string(this->Z) + ")");
  }
};

// -------------------------------------------------------------------------
typedef double U;
typedef Graph<sPoint, U> TGraph;

// -------------------------------------------------------------------------
int main(int argc, char *argv[])
{
  if (argc < 4)
  {
    std::cerr
        << "Usage: " << argv[0] << " input_mesh start end"
        << std::endl;
    return (1);

  } // fi
  long start_id = std::atoi(argv[2]);
  long end_id = std::atoi(argv[3]);

  TGraph g;

  // Load file in a buffer
  std::ifstream in_mesh_stream(argv[1], std::ifstream::binary);
  if (!in_mesh_stream)
  {
    std::cerr << "Error reading \"" << argv[1] << "\"" << std::endl;
    return (1);

  } // fi
  in_mesh_stream.seekg(0, in_mesh_stream.end);
  unsigned long in_mesh_file_length = in_mesh_stream.tellg();
  in_mesh_stream.seekg(0, in_mesh_stream.beg);
  char *in_mesh_file_buffer = new char[in_mesh_file_length];
  in_mesh_stream.read(in_mesh_file_buffer, in_mesh_file_length);
  in_mesh_stream.close();
  std::istringstream in_mesh(in_mesh_file_buffer);
  //New
  // Read vertices
  long nPoints;
  sPoint pnt;
  vector<sPoint> puntos;
  in_mesh >> nPoints;
  for (long pId = 0; pId < nPoints; pId++)
  {
    in_mesh >> pnt.X >> pnt.Y >> pnt.Z;
    g.agregarVertice(pnt);
    puntos.push_back(pnt);
  } // rof

  for (int i = 0; i < nPoints; ++i)
    cout << i << ": " << puntos[i].to_string() << '.';
  cout << endl
       << endl
       << endl;
  // Read edges
  long nEdges, start, end;
  float cost, tmp;
  in_mesh >> nEdges;
  for (long eId = 0; eId < nEdges; eId++)
  {
    in_mesh >> start >> end;
    cost = (puntos[start]).distanceTo(puntos[end]);
    g.agregarArista(puntos[start], puntos[end], cost);
  } // rof
  cout << g.printGraph() << endl;
  delete[] in_mesh_file_buffer;

  if (
      start_id < 0 || start_id >= nPoints ||
      end_id < 0 || end_id >= nPoints)
  {
    std::cerr << "Invalid path endpoints." << std::endl;
    return (1);
  }
  // fi
  map<sPoint, std::pair<sPoint, U>> path = g.dijkstra(puntos[start_id]);
  map<sPoint, std::pair<sPoint, U>>::iterator dj_iterator = path.begin();
  cout << path.size() << endl;
  string impr, impr2;
  float total_weight = 0;
  for (; dj_iterator != path.end(); dj_iterator++)
  {
    impr = dj_iterator->first.to_string();
    impr2 = dj_iterator->second.first.to_string();
    std::cout << impr << ": "
              << "(" <<impr2<<", " << dj_iterator->second.second << ")"<< endl;
  }

  return (0);
}

// eof -
