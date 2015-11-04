#define DEBUG(x) {cout << x << endl;}
// #define DEBUG(x)
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <limits>

template <typename V, typename E>
struct Edge {
	using edge_type = E;
	V u, v;
	E w;
	template <typename Adj_iter>
	Edge(V a, Adj_iter& b) : u{a}, v{*b}, w(b.weight()) {}
	V source() const   {return u;}
	V dest() const     {return v;}
	E weight() const   {return w;}
};

// helper edges for constructing graphs
template <typename V, typename E = int>
struct WEdge {
	V source;
	V dest;
	E weight;
	E get_weight() const {return weight;}
	// can't maintain as POD because V might not be POD
	WEdge(V u, V v, E w) : source{u}, dest{v}, weight{w} {} 
};
template <typename V>
struct UEdge {
	V source;
	V dest;
	size_t get_weight() const {return 1;}
	UEdge(V u, V v) : source{u}, dest{v} {} 
};

// iterate over neighbours of a vertex 
// for adjacency lists just a wrapper around map's iterator
template <typename Iter>
struct Adjacent_iterator {
	using CR = const Adjacent_iterator<Iter>&;
	using V = typename Iter::value_type::first_type;
	using E = typename Iter::value_type::second_type;
	Iter cur;

	void operator++() {++cur;}
	void operator--() {--cur;}
	Adjacent_iterator operator++(int) {return {cur++};}
	Adjacent_iterator operator--(int) {return {cur--};}
	V& operator*() {return cur->first;}
	Iter& operator->() {return cur;}
	bool operator==(CR other) {return other.cur == cur;}
	bool operator!=(CR other) {return !(*this == other);}
	V& dest() {return cur->first;}
	E& weight() {return cur->second;}
	friend std::ostream& operator<<(std::ostream& os, CR itr) {os << itr.cur->first << ' '; return os;}
};
template <typename Iter>
struct Adjacent_const_iterator {
	using CR = const Adjacent_const_iterator<Iter>&;
	using V = typename Iter::value_type::first_type;
	using E = typename Iter::value_type::second_type;
	Iter cur;

	void operator++() {++cur;}
	void operator--() {--cur;}
	Adjacent_const_iterator operator++(int) {return {cur++};}
	Adjacent_const_iterator operator--(int) {return {cur--};}
	V operator*() const {return cur->first;}
	const Iter& operator->() const {return cur;}
	bool operator==(CR other) {return other.cur == cur;}
	bool operator!=(CR other) {return !(*this == other);}
	V dest() {return cur->first;}
	E weight() {return cur->second;}
};

// iterate over vertices of a graph
// for adjacency lists just a wrapper around map's iterator
template <typename Iter>
struct Vertex_iterator {
	using CR = const Vertex_iterator<Iter>&;
	using V = typename Iter::value_type::first_type;
	using E = typename Iter::value_type::second_type::value_type;
	using adjacent_iterator = Adjacent_iterator<typename Iter::value_type::second_type::iterator>;
	Iter cur;

	void operator++() {++cur;}
	void operator--() {--cur;}
	Vertex_iterator operator++(int) {return {cur++};}
	Vertex_iterator operator--(int) {return {cur--};}
	CR operator+(int scalar) const {return {cur + scalar};}
	CR operator-(int scalar) const {return {cur - scalar};}
	V& operator*() {return cur->first;}
	Iter& operator->() {return cur;}
	bool operator==(CR other) {return other.cur == cur;}
	bool operator!=(CR other) {return !(*this == other);}
	E& edge(V adj) {return *cur->second.find(adj);}
	std::pair<adjacent_iterator, adjacent_iterator> adjacent() {
		return {{cur->second.begin()}, {cur->second.end()}};
	}
	adjacent_iterator begin() 	{return {cur->second.begin()};}
	adjacent_iterator end() 	{return {cur->second.end()};}
	friend std::ostream& operator<<(std::ostream& os, CR itr) {os << itr.cur->first << ' '; return os;}
};
template <typename Iter>
struct Vertex_const_iterator {
	using CR = const Vertex_const_iterator<Iter>&;
	using V = typename Iter::value_type::first_type;
	using E = typename Iter::value_type::second_type::value_type;
	using adjacent_const_iterator = Adjacent_const_iterator<typename Iter::value_type::second_type::const_iterator>;
	Iter cur;

	void operator++() {++cur;}
	void operator--() {--cur;}
	Vertex_const_iterator operator++(int) {return {cur++};}
	Vertex_const_iterator operator--(int) {return {cur--};}
	CR operator+(int scalar) const {return {cur + scalar};}
	CR operator-(int scalar) const {return {cur - scalar};}
	V operator*() const {return cur->first;}
	const Iter& operator->() const {return cur;}
	bool operator==(CR other) {return other.cur == cur;}
	bool operator!=(CR other) {return !(*this == other);}
	E edge(V adj) {return *cur->second.find(adj);}
	std::pair<adjacent_const_iterator, adjacent_const_iterator> adjacent() const {
		return {{cur->second.begin()}, {cur->second.end()}};
	}
	adjacent_const_iterator begin() const 	{return {cur->second.begin()};}
	adjacent_const_iterator end() const 	{return {cur->second.end()};}
};


// undirected weighted graph implementation
template <typename V = int, typename E = int, typename Edges = std::map<V, E>>
class Adjacency_list {
protected:
	// destination and weight
	// internally vertices can be integers/indices since they
	// can be labeled vertex 0, vertex 1, vertex 2, ...
	using Adj = std::map<V, Edges>;
	Adj adj;
public:
	using vertex_type = V;
	using edge_type = E;
	using adjacent_iterator = Adjacent_iterator<typename Edges::iterator>;
	using adjacent_const_iterator = Adjacent_const_iterator<typename Edges::const_iterator>;
	using iterator = Vertex_iterator<typename Adj::iterator>;
	using const_iterator = Vertex_const_iterator<typename Adj::const_iterator>;
	using reverse_iterator = Vertex_iterator<typename Adj::reverse_iterator>;
	using const_reverse_iterator = Vertex_iterator<typename Adj::const_reverse_iterator>;
	// constructors
	Adjacency_list() = default;
	Adjacency_list(const std::initializer_list<UEdge<V>>& l) {
		for (const auto& edge : l) {
			adj[edge.source][edge.dest] = (E)1;
			adj[edge.dest][edge.source] = (E)1;
		}		
	}	
	Adjacency_list(const std::initializer_list<WEdge<V,E>>& l) {
		for (const auto& edge : l) {
			adj[edge.source][edge.dest] = edge.weight;
			adj[edge.dest][edge.source] = edge.weight;
		}		
	}
	template <typename Iter_edgelist>
	Adjacency_list(Iter_edgelist begin, const Iter_edgelist end) {
		while (begin != end) {
			adj[begin->source][begin->dest] = begin->get_weight();
			adj[begin->dest][begin->source] = begin->get_weight();
			++begin;
		}
	}
	Adjacency_list(Adjacency_list&& g) : adj{std::move(g.adj)} {}
	~Adjacency_list() = default;

	// cardinality of vertex set and edge set
	size_t num_vertex() const {return adj.size();}
	virtual size_t num_edge() const {
		size_t edges {};
		for (auto v = adj.begin(); v != adj.end(); ++v)
			edges += v->second.size();
		return edges >> 1;	// divide by 2 for unweighted
	} 

	// check for existence of vertex and edge
	bool is_vertex(V v) const {return (adj.find(v) != adj.end());}
	bool is_edge(V u, V v) const {
		auto u_itr = adj.find(u);
		if (u_itr == adj.end()) return false;
		auto v_itr = u_itr->second.find(v);
		return !(v_itr == u_itr->second.end());
	}

	// weight of edge, 1/0 for unweighted edge
	E weight(V u, V v) const {
		auto u_itr = adj.find(u);
		if (u_itr == adj.end()) return 0;
		auto v_itr = u_itr->second.find(v);
		return (v_itr == u_itr->second.end())? 0 : v_itr->second;		
	}
	size_t degree(V v) const {
		auto v_itr = adj.find(v); 
		return (v_itr == adj.end())? 0 : v_itr->second.size();
	}

	// begin and end
	std::pair<adjacent_iterator, adjacent_iterator> adjacent(V v) {
		auto v_itr = adj.find(v);
		if (v_itr != adj.end()) 
			return {{v_itr->second.begin()}, {v_itr->second.end()}};
		else return {{},{}};
	}	
	std::pair<adjacent_const_iterator, adjacent_const_iterator> adjacent(V v) const {
		auto v_itr = adj.find(v);
		if (v_itr != adj.end()) 
			return {{v_itr->second.begin()}, {v_itr->second.end()}};
		else return {{},{}};
	}

	// retrieving vertex (find by default gives vertex)
	iterator vertex(V v) 		 		{return {adj.find(v)};}
	iterator find(V v) 		 			{return {adj.find(v)};}
	const_iterator vertex(V v) const 	{return {adj.find(v)};}	
	const_iterator find(V v) const 		{return {adj.find(v)};}


	// vertex iteration
	iterator begin() 					{return {adj.begin()};}
	iterator end() 						{return {adj.end()};}
	const_iterator begin() const 		{return {adj.begin()};}
	const_iterator end() const   		{return {adj.end()};}
	reverse_iterator rbegin() 			 	{return {adj.rbegin()};}
	reverse_iterator rend() 				{return {adj.rend()};}
	const_reverse_iterator rbegin() const 	{return {adj.rbegin()};}
	const_reverse_iterator rend() const  	{return {adj.rend()};}


	V min_vertex() const {
		if (adj.empty()) return {};	// default
		return adj.begin()->first;
	}
	V max_vertex() const {
		if (adj.empty()) return {};
		return adj.rbegin()->first;
	}

	// modifier interface
	void add_vertex(V v) {adj[v];}
	// undirected
	virtual void add_edge(V u, V v, E weight = 1) {
		adj[u][v] = weight;
		adj[v][u] = weight;
	}

	// printing
	friend std::ostream& operator<<(std::ostream& os, const Adjacency_list& g) {
		for (const auto& vertex : g.adj) {
			os << vertex.first << '(';
			if (!vertex.second.empty()) {
				for (const auto& neighbour : vertex.second)
					os << neighbour.first << ':' << neighbour.second << ',';
				// backspace to remove extra comma
				os << '\b';
			}
			os << ") ";
		}
		return os;
	}
};

// directed adjacency list
template <typename V = int, typename E = int, typename Edges = std::map<V, E>>
class Adjacency_list_directed : public Adjacency_list<V, E, Edges> {
	using Adjacency_list<V, E, Edges>::adj;
public:
	using vertex_type = V;
	using edge_type = E;
	using adjacent_iterator = typename Adjacency_list<V, E, Edges>::adjacent_iterator;
	using adjacent_const_iterator = typename Adjacency_list<V, E, Edges>::adjacent_const_iterator;
	using iterator = typename Adjacency_list<V, E, Edges>::iterator;
	using const_iterator = typename Adjacency_list<V, E, Edges>::const_iterator;
	// constructors
	Adjacency_list_directed() = default;
	Adjacency_list_directed(const std::initializer_list<WEdge<V,E>>& l) {
		for (auto& edge : l) {
			adj[edge.source][edge.dest] = edge.weight;
			adj[edge.dest];
		}		
	}	
	Adjacency_list_directed(const std::initializer_list<UEdge<V>>& l) {
		for (auto& edge : l) {
			adj[edge.source][edge.dest] = (E)1;
			adj[edge.dest];
		}		
	}
	template <typename Iter_edgelist>
	Adjacency_list_directed(Iter_edgelist begin, const Iter_edgelist end) {
		while (begin != end) {
			adj[begin->source][begin->dest] = begin->get_weight();
			adj[begin->dest];	// default construct destination vertex
			++begin;
		}
	}
	Adjacency_list_directed(Adjacency_list_directed&& g) {adj = std::move(g.adj);}
	~Adjacency_list_directed() = default;

	// most operations the same as undirected, degree is outdegree
	size_t num_edge() const override {
		size_t edges {};
		for (auto v = adj.begin(); v != adj.end(); ++v)
			edges += v->second.size();
		return edges;
	} 
	// add edge only to source vertex
	void add_edge(V u, V v, E weight = 1) override {
		adj[u][v] = weight;
		adj[v]; // default construct destination vertex
	}
};
// divide by 2 to prevent overflow
#define POS_INF(T) (std::numeric_limits<T>::max() >> 1)
#define PRINTLINE(line) (std::cout << line << std::endl)
#define PRINT2D(table,row,col) {for (size_t r = 0; r < (row); ++r) {for (size_t c = 0; c < (col); ++c) std::cout << table[r][c] << ' '; std::cout << '\n';}
#define SENTINEL(T) (T{})

template <typename T>
using Iter_value = typename std::iterator_traits<T>::value_type;
template <typename T>
using Iter_diff = typename std::iterator_traits<T>::difference_type;

using namespace std;

#define IS_WHITE(x) (property[x].start == POS_INF(decltype(property[x].start)))
#define IS_GREY(x) (property[x].finish == 0)

template <typename V>
struct DFS_vertex {
	using edge_type = size_t;
	V parent;
	// time stamps
	// discovery and finish time, from 1 to |V|
	size_t start;
	size_t finish;
	DFS_vertex() = default;
	DFS_vertex(const V& v) : parent{v}, start{POS_INF(edge_type)}, finish{0} {}
	size_t edge() const {return 1;}
};

template <typename V>
using DFS_property_map = std::unordered_map<V, DFS_vertex<V>>;
template <typename Graph>
using DPM = DFS_property_map<typename Graph::vertex_type>;

// assumes unweighted graph, and assumes V is simple type (name of vertex)
// works with directed and undirected
// O(V + E) time
// creates a vertex property map relative to source (breadth first tree)


// common initialization shared by most single-source algorithms (BFS, Bellman-Ford, Djikstra, shortest DAG) 
template <typename Property_map, typename Graph>
void initialize_single_source(Property_map& property, const Graph& g, typename Graph::vertex_type s) {
	for (auto v = g.rbegin(); v != g.rend(); ++v) 
		property[*v] = {*v};	// parent is itself
	property[s].distance = 0;
}	

struct DFS_visitor {
	template <typename Property_map, typename Graph>
	std::vector<typename Graph::vertex_type> initialize_vertex(Property_map& property, const Graph& g) {
		std::vector<typename Graph::vertex_type> exploring;
		for (auto v = g.rbegin(); v != g.rend(); ++v) {
			// mark unexplored
			property[*v] = {*v};
			// expore every vertex
			exploring.push_back(*v);
		}
		return std::move(exploring);
	}
	template <typename Graph>
	void start_vertex(typename Graph::vertex_type, const Graph&) {}
	template <typename Graph>
	void discover_vertex(typename Graph::vertex_type, const Graph&) {}
	template <typename Graph>
	void finish_vertex(typename Graph::vertex_type, const Graph&) {}
	template <typename Graph>
	void back_edge(typename Graph::vertex_type, const Graph&) {}
	template <typename Graph>
	void forward_or_cross_edge(typename Graph::vertex_type, const Graph&) {}
};

// for DFS on only 1 source vertex
template <typename Graph>
struct Graph_single_visitor : public DFS_visitor {
	using V = typename Graph::vertex_type;
	V source;
	Graph_single_visitor(V s) : source{s} {}
	template <typename Property_map>
	std::vector<V> initialize_vertex(Property_map& property, const Graph& g) {
		for (auto v = g.rbegin(); v != g.rend(); ++v)
			property[*v] = {*v};
		return {source};
	}
};

// depth first search, used usually in other algorithms
// explores all vertices of a graph, produces a depth-first forest
template <typename Graph, typename Visitor = DFS_visitor>
DPM<Graph> dfs(const Graph& g, Visitor&& visitor = DFS_visitor{}) {
	using V = typename Graph::vertex_type;
	DPM<Graph> property;
	// use visitor to initialize stack (order of DFS)
	std::vector<V> exploring {visitor.initialize_vertex(property, g)};
	
	size_t explore_time {0};

	while (!exploring.empty()) {
		V cur {exploring.back()};
		// tree edge, first exploration on (u,v)
		if (IS_WHITE(cur)) {
			property[cur].start = ++explore_time;
			visitor.start_vertex(cur, g);
		}
		
		bool fully_explored {true};
		auto edges = g.adjacent(cur);
		for (auto adj = edges.first; adj != edges.second; ++adj) {
			// check if any neighbours haven't been explored
			if (IS_WHITE(*adj)) {
				property[*adj].start = ++explore_time;
				property[*adj].parent = cur;
				exploring.push_back(*adj);
				fully_explored = false;	// still have unexplored neighbours
				break;		// only push 1 neighbour to achieve depth first
			}
			else if (IS_GREY(*adj)) visitor.back_edge(*adj, g);
			else visitor.forward_or_cross_edge(*adj, g);
		}
		if (fully_explored) {
			exploring.pop_back();
			if (property[cur].finish == 0) {// default value
				property[cur].finish = ++explore_time;
				visitor.finish_vertex(cur, g);
			}
		}
	}

	return property;
}
// overload for specifying a source, can't use other visitors
template <typename Graph>
DPM<Graph> dfs(const Graph& g, typename Graph::vertex_type s, int) {	// dummy argument for overloading
	return dfs(g, Graph_single_visitor<Graph>{s});
}

// for directed graphs, strongly connected components
// any vertex in a component can get to any other
// simple algorithm by guessing a vertex in a sink component, 
// then traversing full cycle to get entire component
// no easy way to get sink vertex, so use source vertices of the transpose
template <typename Output_iter>
struct Inorder_finish_visitor : public DFS_visitor {
	Output_iter out;
	Inorder_finish_visitor(Output_iter o) : out{o} {}

	template <typename Graph>
	void finish_vertex(typename Graph::vertex_type u, const Graph&) {*out++ = u;}

};

// transpose of directed graph is reversing all edges (u,v) to (v,u)
template <typename Graph>
Graph transpose(const Graph& g) {
	Graph g_t;
	for (auto u = g.begin(); u != g.end(); ++u) {
		for (auto v = u.begin(); v != u.end(); ++v) {
			g_t.add_edge(*v, *u);
		}
		g_t.add_vertex(*u);
	}
	return g_t;
} 

template <typename Graph>
using Connected_set =  std::vector<std::unordered_set<typename Graph::vertex_type>>;

template <typename Graph>
struct Connected_visitor : public DFS_visitor {
	using V = typename Graph::vertex_type;
	Connected_set<Graph> component_set;
	std::vector<V> finish_order;
	Connected_visitor(std::vector<V>&& vec) : finish_order{vec} {}

	// visit in order of descending finish time 
	template <typename Property_map>
	std::vector<V> initialize_vertex(Property_map& property, const Graph& g) {
		for (auto v : finish_order) 
			property[v] = {v};

		return std::move(finish_order);
	}

	// start a new set
	void start_vertex(typename Graph::vertex_type u, const Graph&) {
		component_set.push_back({u});
	}
	void finish_vertex(typename Graph::vertex_type u, const Graph&) {
		component_set.back().insert(u);
	}
};


template <typename Graph>
Connected_set<Graph> strongly_connected(const Graph& g) {
	using V = typename Graph::vertex_type;
	std::vector<V> finish_order;
	finish_order.reserve(g.num_vertex());
	// fill finish_order with vertices in descending order of finish time
	dfs(g, Inorder_finish_visitor<decltype(std::back_inserter(finish_order))>
		{std::back_inserter(finish_order)});

	Graph gt {transpose(g)};
	Connected_visitor<Graph> visitor {std::move(finish_order)};
	dfs(gt, visitor);

	return std::move(visitor.component_set);
}

template <typename V, typename E = int>
using digraph = Adjacency_list_directed<V,E>;

int main() {
	int ppl, com;
	cin >> ppl >> com;

	digraph<string> network;
	while (true) {
		string u,v;
		cin >> u;
		if (u == "END") break;
		cin >> v;
		network.add_edge(v,u);
	}

	auto communities = strongly_connected(network);
	vector<int> com_sizes;
	for (const auto& community : communities) {
		com_sizes.push_back(community.size());
		// for (const string& name : community) cout << name << ' ';
		// cout << '\t';
	}
	sort(begin(com_sizes),end(com_sizes), greater<int>{});
	for (int i = 0; i < com; ++i) {
		if (i < com_sizes.size()) cout << com_sizes[i] << endl;
		else cout << "Does not apply!\n";
	}
}