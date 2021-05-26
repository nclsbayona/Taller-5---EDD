#include "Graph2.h"
#include <iostream>
template <class T, class U>
Graph<T, U>::Graph(bool dirigido)
{
    //Default is 0
    this->dirigido = dirigido;
}

template <class T, class U>
std::string Graph<T, U>::printGraph()
{
    std::string printing = "\nPeso 0 implica que no tiene peso, una lista vacía significa que es un nodo isla o un nodo sumidero\n\n";
    typedef std::map<T, std::set<U>> W;
    typename std::map<T, W>::iterator it_vertices = this->vertices_aristas.begin();
    typename W::iterator it_aristas;
    typename std::set<U>::iterator it_pesos;
    printing += "{  ";
    for (; it_vertices != this->vertices_aristas.end(); ++it_vertices)
    {
        printing += ((*it_vertices).first).to_string();
        printing += ":  { ";
        it_aristas = (*it_vertices).second.begin();
        for (; it_aristas != (*it_vertices).second.end(); ++it_aristas++)
        {
            printing += ((*it_aristas).first).to_string();
            printing += ": [";
            for (it_pesos = (*it_aristas).second.begin(); it_pesos != (*it_aristas).second.end(); ++it_pesos)
            {
                printing += " ";
                printing += std::to_string((*it_pesos));
            }
            printing += " ] ";
        }
        printing += "},  ";
    }
    printing += "}\n\n\n";
    return printing;
}

template <class T, class U>
bool Graph<T, U>::getDirigido()
{
    return this->dirigido;
}

//Vertice
template <class T, class U>
bool Graph<T, U>::agregarVertice(T start)
{
    bool done = false;
    if (!this->buscarVertice(start))
    {
        this->vertices_aristas[start];
        done = true;
    }
    return done;
}

template <class T, class U>
bool Graph<T, U>::buscarVertice(T start)
{
    typedef std::map<T, std::set<U>> W;
    typename std::map<T, W>::iterator it_vertices = this->vertices_aristas.find(start);
    return it_vertices != this->vertices_aristas.end();
}

template <class T, class U>
bool Graph<T, U>::eliminarVertice(T start)
{
    bool total = 0;
    try
    {
        if (this->buscarVertice(start))
        {
            this->vertices_aristas.erase(start);
            typedef std::map<T, std::set<U>> W;
            typedef typename std::map<T, W>::iterator it_vertices;
            it_vertices it = this->vertices_aristas.begin();
            typename W::iterator it_aristas;
            for (; it != this->vertices_aristas.end(); ++it)
            {
                it_aristas = it->second.find(start);
                if (it_aristas != it->second.end())
                    it->second.erase(start);
            }
            total = 1;
        }
    }
    catch (...)
    {
        total = 0;
    }
    return total;
}

template <class T, class U>
int Graph<T, U>::cantidadVertices()
{
    typedef std::map<T, std::set<U>> W;
    typename std::map<T, W>::iterator it_vertices = this->vertices_aristas.begin();
    int total = 0;
    for (; it_vertices != this->vertices_aristas.end(); ++it_vertices)
        total++;
    return total;
}

//Aristas
template <class T, class U>
bool Graph<T, U>::agregarArista(T start, T end, U value, int directed)
{
    bool valid = 0;
    //IDK (2 -> Default, 1 -> Dirigido, 0 -> No dirigido)
    if (directed == 2 && this->dirigido)
        directed = 1;
    else if (directed == 2 && !this->dirigido)
        directed = 0;
    //
    try
    {
        if (!this->buscarVertice(start))
            this->agregarVertice(start);
        if (!this->buscarVertice(end))
            this->agregarVertice(end);
        if (!directed)
            this->vertices_aristas[end][start].insert(value);
        this->vertices_aristas[start][end].insert(value);
        valid = 1;
    }
    catch (...)
    {
        valid = 0;
    }
    return valid;
}

template <class T, class U>
bool Graph<T, U>::buscarArista(T start, T end, U value)
{
    bool exists = this->buscarVertice(start);
    try
    {
        if (exists && !this->buscarVertice(end))
            exists = 0;
        else
        {
            typedef std::map<T, std::set<U>> W;
            typename W::iterator it_vertices = this->vertices_aristas[start].find(end);
            if (it_vertices != this->vertices_aristas[start].end())
                exists = (this->vertices_aristas[start][end].count(value));
            else
                exists = 0;
        }
    }
    catch (...)
    {
        exists = 0;
    }
    return exists;
}

template <class T, class U>
int Graph<T, U>::cantidadAristas()
{
    int total = 0;
    typedef std::map<T, std::set<U>> W;
    typename std::map<T, W>::iterator it_vertices = this->vertices_aristas.begin();
    typename W::iterator it_aristas;
    for (; it_vertices != this->vertices_aristas.end(); ++it_vertices)
    {
        it_aristas = (it_vertices)->second.begin();
        if ((it_vertices)->second.size() > 0)
            for (; it_aristas != (it_vertices)->second.end(); ++it_aristas)
                total++;
    }
    return total;
}

template <class T, class U>
bool Graph<T, U>::eliminarArista(T start, T end, U value)
{
    bool total = 0;
    if (this->buscarVertice(start) && this->buscarVertice(end) && this->buscarArista(start, end, value))
    {
        typedef std::map<T, std::set<U>> W;
        typename std::set<U>::iterator it_set;
        it_set = this->vertices_aristas[start][end].find(value);
        this->vertices_aristas[start][end].erase(it_set);
        if (this->vertices_aristas[start][end].size() == 0)
            this->vertices_aristas[start].erase(end);
        total = 1;
    }
    return total;
}

//Recorridos
template <class T, class U>
std::queue<T> Graph<T, U>::DFS(T start)
{
    typedef typename std::map<T, std::set<U>>::reverse_iterator it;
    it it_aristas;
    std::queue<T> queueDFS;
    std::stack<T> stackVisited;
    int w = 0, i;
    T top;
    bool visited;
    T arrayVisited[this->vertices_aristas.size()];
    for (int i = 0; i < this->vertices_aristas.size(); ++i)
        arrayVisited[i] = -100;
    if (this->buscarVertice(start))
    {
        stackVisited.push(start);
        while (!stackVisited.empty())
        {
            top = stackVisited.top();
            stackVisited.pop();
            visited = false;
            for (i = 0; i < w && !visited; ++i)
                if (arrayVisited[i] == top)
                    visited = true;
            if (!visited)
            {
                queueDFS.push(top);
                arrayVisited[w++] = top;
                it_aristas = this->vertices_aristas[top].rbegin();
                for (; it_aristas != this->vertices_aristas[top].rend(); it_aristas++)
                {
                    visited = false;
                    for (i = 0; i < w && !visited; ++i)
                        if (arrayVisited[i] == (it_aristas->first))
                            visited = true;
                    if (!visited)
                        stackVisited.push(it_aristas->first);
                }
            }
        }
    }
    return queueDFS;
}

//BFS
template <class T, class U>
std::queue<T> Graph<T, U>::BFS(T start)
{
    typedef typename std::map<T, std::set<U>>::iterator it;
    it it_aristas;
    std::queue<T> queueDFS;
    std::queue<T> queueVisited;
    int w = 0, i;
    T top, last;
    bool visited;
    T arrayVisited[this->vertices_aristas.size()];
    for (int i = 0; i < this->vertices_aristas.size(); ++i)
        arrayVisited[i] = -100;

    if (this->buscarVertice(start))
    {
        queueVisited.push(start);
        while (!queueVisited.empty())
        {
            top = queueVisited.front();
            queueVisited.pop();
            visited = false;
            for (i = 0; i < w && !visited; ++i)
                if (arrayVisited[i] == top)
                    visited = true;
            if (!visited)
            {
                queueDFS.push(top);
                arrayVisited[w++] = top;
                it_aristas = this->vertices_aristas[top].begin();
                for (; it_aristas != this->vertices_aristas[top].end(); it_aristas++)
                {
                    visited = false;
                    for (i = 0; i < w && !visited; ++i)
                        if (arrayVisited[i] == (it_aristas->first))
                            visited = true;
                    last = it_aristas->first;
                    if (!visited)
                        queueVisited.push(it_aristas->first);
                }
            }
        }
    }
    return queueDFS;
}

template <class T, class U>
std::string Graph<T, U>::printDFS(T start)
{
    std::string printing = "";
    std::queue<T> queueDFS = this->DFS(start);
    while (!queueDFS.empty())
    {
        printing += " ";
        printing += std::to_string(queueDFS.front());
        printing += " ";
        queueDFS.pop();
    }
    printing += "\n\n";
    return printing;
}

template <class T, class U>
std::string Graph<T, U>::printBFS(T start)
{
    std::string printing = "";
    std::queue<T> queueBFS = this->BFS(start);
    while (!queueBFS.empty())
    {
        printing += " ";
        printing += std::to_string(queueBFS.front());
        printing += " ";
        queueBFS.pop();
    }
    printing += "\n\n";
    return printing;
}

template <class T, class U>
std::set<T> Graph<T, U>::descendientes(T vertice)
{
    bool visitedNode = false;
    std::set<T> set;
    if (this->buscarVertice(vertice))
    {
        set.insert(vertice);
        for (typename std::map<T, std::set<U>>::iterator it_aristas = this->vertices_aristas[vertice].begin(); it_aristas != this->vertices_aristas[vertice].end(); ++it_aristas)
        {
            visitedNode = false;
            for (typename std::set<T>::iterator it = set.begin(); !visitedNode && it != set.end(); ++it)
                if ((*it) == it_aristas->first)
                    visitedNode = true;
            if (!visitedNode)
            {
                this->descendientesRecursivo(it_aristas->first, set);
                set.insert(it_aristas->first);
            }
        }
    }
    return set;
}

template <class T, class U>
void Graph<T, U>::descendientesRecursivo(T vertice, std::set<T> &set)
{
    /* bool visitedNode = false;
    for (typename std::set<T>::iterator it = set.begin(); !visitedNode && it != set.end(); ++it)
        if ((*it) == vertice)
            visitedNode = true; */
    if (set.count(vertice) == 0)
    {
        set.insert(vertice);
        for (typename std::map<T, std::set<U>>::iterator it = this->vertices_aristas[vertice].begin(); it != this->vertices_aristas[vertice].end(); ++it)
        {
            /* for (typename std::set<T>::iterator it2 = set.begin(); !visitedNode && it2 != set.end(); ++it2)
                if ((*it2) == it->first)
                    visitedNode = true; */

            if (set.count(it->first) == 0)
            {
                this->descendientesRecursivo(it->first, set);
                set.insert(it->first);
            }
        }
    }
}

template <class T, class U>
std::set<T> Graph<T, U>::ascendientes(T vertice)
{
    bool visitedNode = false;
    std::set<T> set;
    if (this->buscarVertice(vertice))
    {
        set.insert(vertice);
        for (typename std::map<T, std::map<T, std::set<U>>>::iterator it_vertices = this->vertices_aristas.begin(); it_vertices != this->vertices_aristas.end(); ++it_vertices)
        {
            if (set.count(it_vertices->first) == 0)
            {
                if (this->descendientes(it_vertices->first).count(vertice) > 0)
                    set.insert(it_vertices->first);
            }
        }
    }
    return set;
}

template <class T, class U>
bool Graph<T, U>::grafoConectado()
{
    bool connected = true;
    typename std::set<T> ascendientes;
    typename std::set<T> descendientes;
    for (typename std::map<T, std::map<T, std::set<U>>>::iterator it_vertices = this->vertices_aristas.begin(); connected && it_vertices != this->vertices_aristas.end(); ++it_vertices)
    {
        typename std::map<T, std::map<T, std::set<U>>>::iterator it_vertices2 = this->vertices_aristas.begin();
        ascendientes = this->ascendientes(it_vertices->first);
        descendientes = this->descendientes(it_vertices->first);
        for (; connected && it_vertices2 != this->vertices_aristas.end(); ++it_vertices2)
        {
            if (ascendientes.count(it_vertices2->first) != 1 || descendientes.count(it_vertices2->first) != 1)
                connected = false;
        }
    }
    return connected;
}

template <class T, class U>
int Graph<T, U>::cantidadCamposConectados()
{
    int connected = 0;
    if (this->grafoConectado())
        connected += 1;
    else
    {

        typedef typename std::set<T> mySet;
        typename std::map<T, std::map<T, std::set<U>>>::iterator it_vertices;
        mySet ascendientes, missing, descendientes;
        typedef typename std::vector<T> vector;
        vector campo;
        typename vector::iterator campo_iterator;
        T node;
        for (it_vertices = this->vertices_aristas.begin(); it_vertices != this->vertices_aristas.end(); ++it_vertices)
            missing.insert(it_vertices->first);
        while (!missing.empty())
        {
            campo.clear();
            node = *(missing.begin());
            ascendientes = this->ascendientes(node);
            descendientes = this->descendientes(node);
            std::set_intersection(ascendientes.begin(), ascendientes.end(), descendientes.begin(), descendientes.end(), std::back_inserter(campo));
            for (campo_iterator = campo.begin(); campo_iterator != campo.end(); campo_iterator++)
                missing.erase(*campo_iterator);
            connected += 1;
        }
    }
    return connected;
}

template <class T, class U>
bool Graph<T, U>::aristaPuente(T start, T end, U valor)
{
    bool connected = false;
    bool dirc = this->buscarArista(end, start, valor);
    int camposC = this->cantidadCamposConectados();
    this->eliminarArista(start, end, valor);
    if (dirc)
        this->eliminarArista(end, start, valor);
    if (this->cantidadCamposConectados() > camposC)
        connected = true;
    if (!dirc)
        this->agregarArista(start, end, valor, 1);
    else
        this->agregarArista(start, end, valor, 0);
    return connected;
}

template <class T, class U>
int Graph<T, U>::gradoSalida(T vertice)
{
    int grado = 0;
    typename std::map<T, std::set<U>>::iterator it = this->vertices_aristas[vertice].begin();
    for (; it != this->vertices_aristas[vertice].end(); ++it)
        grado += (it)->second.size();
    return grado;
}

template <class T, class U>
int Graph<T, U>::gradoEntrada(T vertice)
{
    int grado = 0;
    typename std::map<T, std::map<T, std::set<U>>>::iterator it = this->vertices_aristas.begin();
    typename std::map<T, std::set<U>>::iterator it2;
    for (; it != this->vertices_aristas.end(); ++it)
    {
        it2 = it->second.find(vertice);
        if (it2 != it->second.end())
            grado += it->second[vertice].size();
    }
    return grado;
}

template <class T, class U>
int Graph<T, U>::grado(T vertice)
{
    int grado = this->gradoEntrada(vertice) + this->gradoSalida(vertice);
    if (!this->dirigido)
        grado /= 2;
    return grado;
}

template <class T, class U>
std::vector<T> Graph<T, U>::prim(T start)
{
    //Las aristas (pesos) deben estar ordenados
    std::set<T> vnew;
    std::vector<T> enew;
    typename std::map<T, std::map<T, std::set<U>>>::iterator it_vertices;
    typename std::map<T, std::set<U>>::iterator it_aristas;
    std::pair<T, U> weight;
    U actual_weight;
    vnew.insert(start);
    enew.push_back(start);
    while (this->vertices_aristas.size() != vnew.size())
    {
        //Hallar el menor (u,v) que tenga u visitado y v no
        //Buscar U:
        //ABSOLUTE ZERO -273.15K
        weight.second = -273;
        it_vertices = this->vertices_aristas.begin();
        for (; it_vertices != this->vertices_aristas.end(); ++it_vertices)
        {
            //U en vnew
            if (vnew.count(it_vertices->first) != 0)
            {
                it_aristas = this->vertices_aristas[it_vertices->first].begin();
                for (; it_aristas != this->vertices_aristas[it_vertices->first].end(); ++it_aristas)
                {
                    //V no en vnew
                    if (vnew.count(it_aristas->first) == 0)
                    {
                        actual_weight = *(it_aristas->second.begin());
                        if (((actual_weight < weight.second || weight.second == -273)) || (((weight.second != -273) && (actual_weight == weight.second)) && (weight.first > it_aristas->first)))
                        {
                            //La última condición es para orden ascendente
                            weight.first = it_aristas->first;
                            weight.second = actual_weight;
                        }
                    }
                }
            }
        }
        enew.push_back(weight.first);
        vnew.insert(weight.first);
    }
    return enew;
}

template <class T, class U>
std::vector<T> Graph<T, U>::kruskal()
{
    typedef typename std::pair<U, std::pair<T, T>> arista;
    typedef typename std::map<T, std::set<U>> _aristas;
    typedef typename std::vector<T> mst;

    typename std::vector<arista> aristas;
    typename std::vector<mst>::iterator trees_it;
    typename std::vector<mst>::iterator trees_it2;
    typename mst::iterator kruskal_it;
    mst *generic_tree;
    std::vector<T> *kruskal;
    std::vector<mst> trees;
    arista *arista_sola;

    for (typename std::map<T, _aristas>::iterator it_vertices = this->vertices_aristas.begin(); it_vertices != this->vertices_aristas.end(); ++it_vertices)
    {
        generic_tree = new mst;
        generic_tree->push_back(it_vertices->first);
        trees.push_back(*(generic_tree));
        for (typename _aristas::iterator it_aristas = this->vertices_aristas[it_vertices->first].begin(); it_aristas != this->vertices_aristas[it_vertices->first].end(); ++it_aristas)
        {
            for (typename std::set<U>::iterator it_pesos = this->vertices_aristas[it_vertices->first][it_aristas->first].begin(); it_pesos != this->vertices_aristas[it_vertices->first][it_aristas->first].end(); it_pesos++)
            {
                arista_sola = new arista;
                arista_sola->first = (*it_pesos);
                arista_sola->second.first = (it_vertices->first);
                arista_sola->second.second = (it_aristas->first);
                aristas.push_back(*(arista_sola));
            }
        }
    }
    sort(aristas.begin(), aristas.end());
    int a;
    std::cin >> a;
    arista_sola = &(*aristas.begin());
    while (!aristas.empty())
    {
        aristas.pop();
    }
    return *kruskal;
}

template <class T, class U>
std::map<T, std::pair<T, U>> Graph<T, U>::dijkstra2(T start)
{
    std::map<T, std::pair<T, U>> dist;
    std::set<T> visited;
    std::pair<T, U> pair;
    U actual_weight;
    const int MIN = 999999, MAX = 20;
    typename std::map<T, std::set<U>>::iterator it_arista;
    for (typename std::map<T, std::map<T, std::set<U>>>::iterator it_vertices = this->vertices_aristas.begin(); it_vertices != this->vertices_aristas.end(); ++it_vertices)
    {
        pair.first = it_vertices->first;
        pair.second = MIN;
        if (it_vertices->first == start)
        {
            pair.first=start;
            std::cout << "Here " << it_vertices->first.to_string()<<" y "<<start.to_string()<<'\n';
            pair.second = 0;
            dist[start]=(pair);
            std::cout<<"Acabo de insertar "<<start.to_string()<<'\n';
        }
        else
            dist[it_vertices->first] = (pair);
        std::cout<<"Esta "<<start.to_string()<<": "<<(dist.find(start)!=dist.end())<<std::endl;
    }
    std::cout<<"Esta "<<start.to_string()<<": "<<(dist.find(start)!=dist.end())<<std::endl;
    for (auto linea : dist)
        std::cout << linea.first.to_string() << "( " << linea.second.first.to_string() << ", " << linea.second.second << " )" << '\n';

    std::cin >> actual_weight;
    U min_weight;
    T min_obj, fstart = start;
    
    typename std::map<T, std::pair<T, U>>::iterator it_pesos_acumulados;
    while (dist.size() != visited.size() && !(min_obj == fstart) && dist.find(start)!=dist.end())
    {
        if (dist.find(start) != dist.end())
        {
            visited.insert(start);
            it_arista = this->vertices_aristas[start].begin();
            for (; it_arista != this->vertices_aristas[start].end(); ++it_arista)
            {
                actual_weight = dist[start].second + *(this->vertices_aristas[start][it_arista->first].begin());
                if (actual_weight == 0)
                    actual_weight = dist[start].second + start.distanceTo(it_arista->first);
                if (actual_weight < dist[it_arista->first].second)
                {
                    dist[it_arista->first].first = start;
                    dist[it_arista->first].second = actual_weight;
                }
            }
        }
        fstart = start;
        min_weight = MIN;
        min_obj = start;
        for (it_pesos_acumulados = dist.begin(); it_pesos_acumulados != dist.end(); it_pesos_acumulados++)
        {
            actual_weight = it_pesos_acumulados->second.second;
            if (visited.count(it_pesos_acumulados->first) == 0)
            {
                if (actual_weight < min_weight)
                {
                    min_obj = (*it_pesos_acumulados).first;
                    min_weight = actual_weight;
                }
            }
        }
        start = min_obj;
    }
    return dist;
}

template <class T, class U>
std::pair<U, std::set<T>> Graph<T, U>::dijkstra(T start, T end)
{
    typename std::map<T, std::pair<T, U>> dist = this->dijkstra2(start);
    typename std::pair<U, std::set<T>> route;
    T new_end;
    if (dist.find(end)!=dist.end()&&dist[end].second != 999999)
    {
        route.first = dist[end].second;
        route.second.insert(start);
        new_end = dist[end].first;
        while (!(route.second.count(dist[end].first)))
        {
            while (!(route.second.count(dist[new_end].first)))
                new_end = dist[new_end].first;
            route.second.insert(new_end);
        }
        route.second.insert(end);
    }
    return route;
}