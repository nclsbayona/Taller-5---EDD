#ifndef Graph_H
#define Graph_H
#include <map>
#include <set>
#include <unordered_set>
#include <vector>
#include <queue>
#include <cmath>
#include <stack>
#include <utility>
#include <algorithm>

template <class T, class U=int>
class Graph{
    private:
        bool dirigido;
        std::map<T,std::map<T, std::set<U>>> vertices_aristas;
    public:
        Graph(bool dirigido=false);
        std::string printGraph();
        bool getDirigido();
        bool agregarArista(T start, T end, U value=0, int directed=2);
        bool buscarArista(T start, T end, U value=0);
        bool eliminarArista(T start, T end, U value=0);
        int cantidadAristas();
        bool agregarVertice(T start);
        bool buscarVertice(T start);
        bool eliminarVertice(T start);
        int cantidadVertices();
        //Recorridos
        std::queue<T> DFS(T start);
        std::queue<T> BFS(T start);
        std::string printDFS(T start); 
        std::string printBFS(T start);
        //Descendientes / Ascendientes
        std::set<T> descendientes(T vertice);
        void descendientesRecursivo(T vertice, std::set<T>& queue);
        std::set<T> ascendientes(T vertice);
        //Campos conectados
        bool grafoConectado();
        int cantidadCamposConectados();
        bool aristaPuente(T start, T end, U valor=0);
        int gradoSalida(T vertice);
        int gradoEntrada(T vertice);
        int grado(T vertice);
        //Recorridos
        std::vector<T> prim(T start);
        std::vector<T> kruskal();
        std::map<T, std::pair<T, U>> dijkstra2(T start);
        std::pair<U, std::set<T>> dijkstra(T start, T end);
        //Falta KRUSKAL; EULER; PREGUNTAR BIPARTITO
};
#include "Graph.hxx"
#endif