///  Contains Graph related classes implementation
#include "Graph.h"

//  This function generates a random double between dMin and dMax
double GenerateRandomDouble(double dMin, double dMax)
{
	double d = static_cast<double>(rand()) / RAND_MAX;
	return dMin + d * (dMax - dMin);
}

Vertex::Vertex() : m_vertexNumber(0), m_playerColor(NONE)
{

}

Vertex::Vertex(unsigned int vertexNumber) : m_vertexNumber(vertexNumber), m_playerColor(NONE)
{
}

Vertex::~Vertex()
{
}

bool Vertex::Adjacent(unsigned int v2) const
{
	for (auto it = m_edgeList.begin(); it != m_edgeList.end(); ++it)
		if (it->GetEndVertexNumber() == v2)
			return true;

	return false;
}

double Vertex::GetEdgeValue(unsigned int v2) const
{
	for (auto it = m_edgeList.begin(); it != m_edgeList.end(); ++it)
		if (it->GetEndVertexNumber() == v2)
			return it->GetEdgeWeight();

	return -1;
}

PlayerColor Vertex::GetColor() const
{
	return m_playerColor;
}

const list<Edge> &Vertex::GetNodeEdges() const
{
	return m_edgeList;
}

list<Edge> &Vertex::GetNodeEdges()
{
	return m_edgeList;
}

void Vertex::AddEdge(unsigned int v2, double distance, PlayerColor playerColor)
{
	m_edgeList.push_back(Edge(m_vertexNumber, v2, distance, playerColor));
}

void Vertex::AddEdge(unsigned int v2, PlayerColor playerColor /* = PlayerColor::NONE */)
{
	m_edgeList.push_back(Edge(m_vertexNumber, v2, playerColor));
}

void Vertex::AddEdge(const Edge& edge)
{
	m_edgeList.push_back(edge);
}

void Vertex::DeleteEdge(unsigned int v2)
{
	//  cannot erase path to itself
	if (v2 == m_vertexNumber)
		return;

	if (Adjacent(v2))
	{
		for (auto it = m_edgeList.begin(); it != m_edgeList.end(); ++it)
			if (it->GetEndVertexNumber() == v2)
			{
				m_edgeList.erase(it);
				break;
			}
	}
}

void Vertex::SetEdgeValue(unsigned int v2, double value)
{
	if (Adjacent(v2))
	{
		for (auto it = m_edgeList.begin(); it != m_edgeList.end(); ++it)
			if (it->GetEndVertexNumber() == v2)
			{
				it->SetEdgeWeight(value);
				return;
			}
	}
}

void Vertex::SetVertexNumber(unsigned int number)
{
	m_vertexNumber = number;
}

bool Vertex::SetVertexColor(PlayerColor playerColor)
{
	if (m_playerColor != NONE)
		return false;

	m_playerColor = playerColor;
	return true;
}

Edge::Edge() : m_startVertex(UINT_MAX), m_endVertex(UINT_MAX), m_Weight(DBL_MAX), m_playerColor(NONE)
{
}

Edge::Edge(unsigned int startVertex, unsigned int endVertex, double weight, PlayerColor playerColor) : m_startVertex(startVertex), m_endVertex(endVertex), m_Weight(weight), m_playerColor(playerColor)
{
}

Edge::Edge(unsigned int startVertex, unsigned int endVertex, PlayerColor playerColor) : m_startVertex(startVertex), m_endVertex(endVertex), m_Weight(0.0), m_playerColor(playerColor)
{

}

Edge::~Edge()
{
}

unsigned int Edge::GetStartVertexNumber() const
{
	return m_startVertex;
}

unsigned int Edge::GetEndVertexNumber() const
{
	return m_endVertex;
}

double Edge::GetEdgeWeight() const
{
	return m_Weight;
}

PlayerColor Edge::GetEdgeColor() const
{
	return m_playerColor;
}

void Edge::SetEdgeWeight(double weight)
{
	m_Weight = weight;
}

void Edge::SetEdgeColor(PlayerColor playerColor)
{
	m_playerColor = playerColor;
}

Graph::Graph(unsigned int size) : m_Vertices(size), m_EdgesAmount(0)
{
	//  it - m_Vertices.begin() gives number of vertices between first vertex and vertex pointed by iterator(number of vertex)
	for (auto it = m_Vertices.begin(); it != m_Vertices.end(); ++it)
		it->SetVertexNumber(it - m_Vertices.begin());
}

//  This construct uses this approach to generate graph with the given density: We generate
//  a random double between 0 and 1. If it is less than the density then create an edge.
//  It means the edge in generated in density cases of 1 (or in density % cases).
//  It equals that graph has the given density
Graph::Graph(unsigned int size, double density, double distance_min, double distance_max) : m_Vertices(size), m_EdgesAmount(0)
{
	double random_propability, random_distance;
	random_propability = random_distance = 0.0;

	//  it - m_Vertices.begin() gives number of vertices between first vertex and vertex pointed by iterator(number of vertex)
	for (auto it = m_Vertices.begin(); it != m_Vertices.end(); ++it)
		it->SetVertexNumber(it - m_Vertices.begin());

	for (size_t i = 0; i < m_Vertices.size(); ++i)
	{
		m_Vertices[i].AddEdge(i, 0.0);  //  a path to itself always exists
		m_EdgesAmount++;
		for (size_t j = i + 1; j < m_Vertices.size(); ++j)
		{
			random_propability = GenerateRandomDouble(0.0, 1.0);
			if (random_propability < density)
			{
				random_distance = GenerateRandomDouble(distance_min, distance_max);
				//  graph is undirected so we can go from i to j and from j to i
				m_Vertices[i].AddEdge(j, random_distance);
				m_Vertices[j].AddEdge(i, random_distance);
				m_EdgesAmount += 2;
			}
		}
	}
}


Graph::Graph(const string &filename) : m_EdgesAmount(0)
{
	ifstream fin(filename, ios_base::in);

	if (fin.good())
	{
		int size, v1, v2, len;
		fin >> size;
		if (fin.good())
		{
			m_Vertices = vector<Vertex>(size);
			//  it - m_Vertices.begin() gives number of vertices between first vertex and vertex pointed by iterator(number of vertex)
			for (auto it = m_Vertices.begin(); it != m_Vertices.end(); ++it)
				it->SetVertexNumber(it - m_Vertices.begin());

			while (!fin.eof())
			{
				fin >> v1 >> v2 >> len;
				if (fin.eof())
					break;
				m_Vertices[v1].AddEdge(v2, len);
				m_EdgesAmount++;
			}
		}
	}
	fin.close();
}

Graph::~Graph()
{
}

unsigned int Graph::GetVerticesAmount() const
{
	return m_Vertices.size();
}

unsigned int Graph::GetEdgesAmount() const
{
	return m_EdgesAmount;
}

PlayerColor Graph::GetVertexColor(unsigned int v1) const
{
	return m_Vertices[v1].GetColor();
}

double Graph::GetEdgeValue(unsigned int v1, unsigned int v2) const
{
	if (Adjacent(v1, v2))
		return m_Vertices[v1].GetEdgeValue(v2);

	//  if there is no edge return negative value
	return -1;
}

bool Graph::Adjacent(unsigned int v1, unsigned int v2) const
{
	//  + 1 because of array indexes start with 0 (otherwise v == 50 would pass this check if list size is 50, but the last valid index is 49)
	if (v1 > GetVerticesAmount() + 1 || v2 > GetVerticesAmount() + 1)
		return false;

	//  If v2 is in the list of neighboors of v1 then they are adjacent nodes
	return m_Vertices[v1].Adjacent(v2);
}

//  Get the list of edges that start in the given node
const list<Edge> &Graph::GetNodeEdges(unsigned int v) const
{
	return m_Vertices[v].GetNodeEdges();
}

list<Edge> &Graph::GetNodeEdges(unsigned int v)
{
	return m_Vertices[v].GetNodeEdges();
}

void Graph::AddEdge(unsigned int v1, unsigned int v2, double distance, PlayerColor playerColor)
{
	//  don't need to check whether v1 or v2 are more of the edges amount or not because it's done in the beginning of the Adjacent method
	if (!Adjacent(v1, v2))
	{
		m_Vertices[v1].AddEdge(v2, distance, playerColor);
		m_EdgesAmount++;
	}
}

void Graph::AddEdge(unsigned int v1, unsigned int v2, PlayerColor playerColor /* = PlayerColor::NONE */)
{
	if (!Adjacent(v1, v2))
	{
		m_Vertices[v1].AddEdge(v2, playerColor);
		m_EdgesAmount++;
	}
}

void Graph::AddEdge(const Edge &edge)
{
	if (!Adjacent(edge.GetStartVertexNumber(), edge.GetEndVertexNumber()))
	{
		AddEdge(edge.GetStartVertexNumber(), edge.GetEndVertexNumber(), edge.GetEdgeWeight(), edge.GetEdgeColor());
		m_EdgesAmount++;
	}
}

void Graph::DeleteEdge(unsigned int v1, unsigned int v2)
{
	//  cannot erase path to itself
	if (v1 == v2)
		return;

	//  don't need to check whether v1 or v2 are more of the edges amount or not because it's done in the beginning of the Adjacent method
	if (Adjacent(v1, v2))
	{
		m_Vertices[v1].DeleteEdge(v2);
		m_Vertices[v2].DeleteEdge(v1);
		m_EdgesAmount--;
	}
}

void Graph::SetNodeValue(unsigned int v1, double value)
{
	//  just a stub for now. We don't need any implementation for now cause node number == its value
}

bool Graph::SetVertexColor(unsigned int v, PlayerColor playerColor)
{
	return m_Vertices[v].SetVertexColor(playerColor);
}

bool Graph::SetEdgeColor(unsigned int v1, unsigned int v2, PlayerColor playerColor)
{
	list<Edge> &edges = m_Vertices[v1].GetNodeEdges();

	for (auto it = edges.begin(); it != edges.end(); ++it)
	{
		if (it->GetEndVertexNumber() == v2)
		{
			it->SetEdgeColor(playerColor);
			return true;
		}
	}

	return false;
}

void Graph::SetEdgeValue(unsigned int v1, unsigned int v2, double value)
{
	m_Vertices[v1].SetEdgeValue(v2, value);
	m_Vertices[v2].SetEdgeValue(v1, value);
}

Graph Graph::PrimMST(double &length)
{
	Graph G(GetVerticesAmount());
	list<unsigned int> closeSet;
	PriorityQueue<Edge, double> PQ;
	//  we should check if the Graph is disconnected and return the tree of 0 size in that case
	bool bDisconnected = true;

	length = 0;
	if (GetVerticesAmount() == 0)
		return G;
	
	//  Start with a single vertex
	closeSet.push_back(0);
	for (auto it = m_Vertices[0].GetNodeEdges().begin(); it != m_Vertices[0].GetNodeEdges().end(); ++it)
		PQ.Insert(*it, it->GetEdgeWeight());

	//  Algorithm should work while the resulting tree doesn't contain all nodes 
	//  and Graph is not disconnected (second condition)
	while (closeSet.size() != G.GetVerticesAmount() && PQ.Size() > 0)
	{
		bDisconnected = true;
		Edge e;
		//  Get an unvisited vertex with the highest priority
		while (PQ.Size() > 0)
		{
			e =  PQ.Top();
			PQ.Pop();

			if (find(closeSet.begin(), closeSet.end(), e.GetEndVertexNumber()) == closeSet.end())
			{
				bDisconnected = false;
				break;
			}
		}

		if (bDisconnected)
			break;

		//  Add the vertex to the tree and its edges to the queue
		G.AddEdge(e);
		length += e.GetEdgeWeight();
		closeSet.push_back(e.GetEndVertexNumber());
		for (auto it = m_Vertices[e.GetEndVertexNumber()].GetNodeEdges().begin(); it != m_Vertices[e.GetEndVertexNumber()].GetNodeEdges().end(); ++it)
			PQ.Insert(*it, it->GetEdgeWeight());
	}

	if (!bDisconnected)
		return G;
	else
	{
		length = DBL_MAX;
		return Graph(0);
	}
}

list<unsigned int> Graph::GetConnections(unsigned int v)
{
	list<unsigned int> connections;
	list<Edge> edges;
	PlayerColor playerColor = GetVertexColor(v);

	connections.push_back(v);
	if (GetVerticesAmount() == 0)
		return connections;
	
	for (auto it = m_Vertices[v].GetNodeEdges().begin(); it != m_Vertices[v].GetNodeEdges().end(); ++it)
		if (it->GetEdgeColor() == playerColor)
			edges.push_back(*it);

	while (connections.size() != GetVerticesAmount() && edges.size() > 0)
	{
			Edge e = *edges.begin();
			edges.pop_front();  //  That's why we use list

			if (find(connections.begin(), connections.end(), e.GetEndVertexNumber()) == connections.end())
			{
				unsigned int endVertex = e.GetEndVertexNumber();
				if (m_Vertices[endVertex].GetColor() == playerColor)
					connections.push_back(endVertex);
				for (auto it = m_Vertices[endVertex].GetNodeEdges().begin(); it != m_Vertices[endVertex].GetNodeEdges().end(); ++it)
					if (it->GetEdgeColor() == playerColor)
						edges.push_back(*it);
			}
	}

	return connections;
}

Path::Path(unsigned int start) : m_Weight(0.0)
{
	m_Path.push_back(start);
}

Path::Path(const Path &path) : m_Path(path.m_Path), m_Weight(path.m_Weight)
{
}

//  Continue the path
Path::Path(const Path &path, const Edge &edge) : m_Path(path.m_Path)
{
	m_Path.push_back(edge.GetEndVertexNumber());
	m_Weight = path.m_Weight + edge.GetEdgeWeight();
}

double Path::GetWeight() const
{
	return m_Weight;
}

unsigned int Path::GetFinalVertex() const
{
	return m_Path.back();
}

const list<unsigned int> &Path::GetPath() const
{
	return m_Path;
}

//  Two paths are equal if their final vertices. This means we can use this function to understand 
//  if there already is a path to this vertex in the close set. It helps for the algo to work faster.
bool Path::operator==(const Path& path) const
{
	if (m_Path.size() != path.m_Path.size())
		return false;

	return GetFinalVertex() == path.GetFinalVertex();
}

void Path::AddVertex(const Edge &edge)
{
	m_Path.push_back(edge.GetEndVertexNumber());
	m_Weight += edge.GetEdgeWeight();
}

ShortestPathAlgorithm::ShortestPathAlgorithm() : m_CloseSet(), m_OpenSet()
{
}

ShortestPathAlgorithm::~ShortestPathAlgorithm()
{
}

//  Check if the open set contains the vertex
bool ShortestPathAlgorithm::OpenSetContains(unsigned int vertex) const
{
	for (auto it = m_OpenSet.begin(); it != m_OpenSet.end(); ++it)
		if (*it == vertex)
			return true;
	return false;
}

//  Get Shortest Path LENGTH from u to v
double ShortestPathAlgorithm::GetShortestPathLength(const Graph& G, unsigned int u, unsigned int v)
{
	m_CloseSet = PriorityQueue<unsigned int, double>();
	m_OpenSet.resize(0);

	const list<Edge> &firstVNeighbors = G.GetNodeEdges(u);
	//  Add u to the open set
	m_OpenSet.push_back(u);
	//  Add all the neighboors of u to the close set
	for (auto it = firstVNeighbors.begin(); it != firstVNeighbors.end(); ++it)
		m_CloseSet.Insert(it->GetEndVertexNumber(), it->GetEdgeWeight());

	//  While we can find a path from u to v
	while (!m_CloseSet.Empty())
	{
		//  Get the vertex with the best weight
		unsigned int vertex = m_CloseSet.Top();
		double priority = m_CloseSet.GetTopPriority();
		m_CloseSet.Pop();

		//  If it is v we're over (Dijkstra algoritm guarantees this path's the shortest)
		if (vertex == v)
			return priority;

		//  If this vertex is not in the open set (it equals there is no shorter path to this vertex)
		if (!OpenSetContains(vertex))
		{
			m_OpenSet.push_back(vertex);

			const list<Edge> & neighbors = G.GetNodeEdges(vertex);
			for (auto it = neighbors.begin(); it != neighbors.end(); ++it)
			{
				if (!OpenSetContains(it->GetEndVertexNumber()))
				{
					//  Inserting the vertex if it is not in the queue already or
					//  if its weight is less than a weight of another path to this vertex
					//  (i.e. this path is better than any prior paths to this vertex found)
					m_CloseSet.InsertIfPriorityLess(it->GetEndVertexNumber(), priority + it->GetEdgeWeight());
				}
			}
		}
	}

	return -1;
}

//  Get the AVERAGE of shortest paths from u to other vertices
//  This method computes a shortest path from u to any vertex in the Graph
//  Then it computes the average
double ShortestPathAlgorithm::AverageShortestPath(const Graph &G, unsigned int u)
{
	m_CloseSet = PriorityQueue<unsigned int, double>();
	m_OpenSet.resize(0);

	double sum = 0.0;
	const list<Edge> &firstVNeighbors = G.GetNodeEdges(u);
	//  Add u to the open set
	m_OpenSet.push_back(u);
	//  Add all the neighboors of u to the close set
	for (auto it = firstVNeighbors.begin(); it != firstVNeighbors.end(); ++it)
		m_CloseSet.Insert(it->GetEndVertexNumber(), it->GetEdgeWeight());

	//  While we can find a path from u to v
	while (!m_CloseSet.Empty())
	{
		//  Get the path with the best priority
		unsigned int vertex = m_CloseSet.Top();
		double priority = m_CloseSet.GetTopPriority();
		m_CloseSet.Pop();

		//  If this vertex is not in the open set (it equals there is no shorter path to this vertex)
		if (!OpenSetContains(vertex))
		{
			m_OpenSet.push_back(vertex);
			//  Add this weight to the sum
			sum += priority;

			const list<Edge> & neighbors = G.GetNodeEdges(vertex);
			for (auto it = neighbors.begin(); it != neighbors.end(); ++it)
			{
				if (!OpenSetContains(it->GetEndVertexNumber()))  //  1. find 2. second check
				{
					//  Inserting the vertex if it is not in the queue already or
					//  if its weight is less than a weight of another path to this vertex
					//  (i.e. this path is better than any prior paths to this vertex found)
					m_CloseSet.InsertIfPriorityLess(it->GetEndVertexNumber(), priority + it->GetEdgeWeight());
				}
			}
		}
	}

	if (m_OpenSet.size() > 1)
		return sum / (m_OpenSet.size() - 1);
	else
		return -1.0;
}

//  Get Shortest PATH from u to v
Path ShortestPathAlgorithm::GetShortestPath(const Graph &G, unsigned int u, unsigned int v)
{
	m_PathCloseSet = PriorityQueue<Path, double>();
	m_OpenSet.resize(0);

	Path currentPath(u);
	const list<Edge> &firstVNeighbors = G.GetNodeEdges(u);
	//  Add u to the open set
	m_OpenSet.push_back(u);
	//  Add all the neighboors of u to the close set
	for (auto it = firstVNeighbors.begin(); it != firstVNeighbors.end(); ++it)
	{
		Path nextPath(currentPath, *it);
		m_PathCloseSet.Insert(nextPath, nextPath.GetWeight());
	}

	//  While we can find a path from u to v
	while (!m_PathCloseSet.Empty())
	{
		//  Get the path with the best priority
		currentPath = m_PathCloseSet.Top();
		double priority = m_PathCloseSet.GetTopPriority();
		m_PathCloseSet.Pop();

		//  If it ends in v we're over (Dijkstra algoritm guarantees this path's the shortest)
		if (currentPath.GetFinalVertex() == v)
			return currentPath;

		//  If this vertex is not in the open set (it equals there is no shorter path to this vertex)
		if (!OpenSetContains(currentPath.GetFinalVertex()))
		{
			//  Add vertex to the open set
			m_OpenSet.push_back(currentPath.GetFinalVertex());

			const list<Edge> & neighbors = G.GetNodeEdges(currentPath.GetFinalVertex());
			for (auto it = neighbors.begin(); it != neighbors.end(); ++it)
			{
				if (!OpenSetContains(it->GetEndVertexNumber()))
				{
					//  Inserting the path if it is not in the queue already or
					//  if its weight is less than a weight of another path to this vertex
					//  (i.e. this path is better than any prior paths to this vertex found)
					Path nextPath(currentPath, *it);
					m_PathCloseSet.InsertIfPriorityLess(nextPath, nextPath.GetWeight());
				}
			}
		}
	}

	return Path(u);
}