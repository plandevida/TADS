/*
 * GrafoDirigido.h
 *
 *  Created on: 17/10/2012
 *      Author: Alberto Verdejo
 */

#ifndef GRAFODIRIGIDO_H_
#define GRAFODIRIGIDO_H_

#include <fstream>
#include <limits>

#include "Lista.h"
#include "Cola.h"
//#include "Pila.h"

class VerticeInexistente : public ExcepcionTAD {
public:
	VerticeInexistente() {};
	VerticeInexistente(const std::string &msg) : ExcepcionTAD(msg) {}
};

class IllegalArgumentException : public ExcepcionTAD {
public:
	IllegalArgumentException() {};
	IllegalArgumentException(const std::string &msg) : ExcepcionTAD(msg) {}
};


typedef unsigned int uint;
typedef Lista<uint> Adys;
typedef Lista<uint>::Iterador Iter;
typedef Lista<uint> Path;


class GrafoDirigido {

private:
	uint _V;      // número de vértices
	uint _E;      // número de aristas
	Adys* _adj;   // vector de listas de adyacencia

public:

	/**
	 * Crea un grafo aleatorio con V vértices y E aristas.
	 */
	GrafoDirigido(uint v, uint e=0) : _V(v), _E(e), _adj(new Adys[_V]) {
		for (uint i = 0; i < e; i++) {
			uint v = rand() % _V; uint w = v;
			while(w == v) w = rand() % _V;
			ponArista(v, w);
		}
	}

	/**
	 * Crea un grafo a partir de un fichero de entrada.
	 */
	GrafoDirigido(string file) {
		ifstream ent(file.c_str());
		ent >> _V;
		_E = 0;
		_adj = new Adys[_V];
		uint e;
		ent >> e;
		uint v, w;
		for (uint i = 0; i < e; i++) {
			ent >> v;
			ent >> w;
			ponArista(v, w);
		}
		ent.close();
	}

	~GrafoDirigido(){
		delete[] _adj;
		_adj = NULL;
	}

	/**
	 * Constructor de copia.
	 */
	GrafoDirigido(const GrafoDirigido& G)  : _V(G.V()), _E(G.E()), _adj(new Adys[_V]) {
		copia(G);
	}

	/**
	 * Devuelve el número de vértices del grafo.
	 */
	uint V() const { return _V; }

	/**
	 * Devuelve el número de aristas del grafo.
	 */
	uint E() const { return _E; }

	/**
	 * Añade la arista v-w al grafo.
	 * @throws VerticeInexistente si algún vértice no existe
	 */
	void ponArista(uint v, uint w) {
		if (v >= _V || w >= _V) throw VerticeInexistente();
		_E++;
		_adj[v].Cons(w);
	}

	/**
	 * Devuelve la lista de adyacencia de v.
	 * @throws VerticeInexistente si v no existe
	 */
	const Adys& adj(uint v) const {
		if (v >= _V) throw new VerticeInexistente();
		return _adj[v];
	}

	/**
	 * Return the reverse of the digraph.
	 */
	GrafoDirigido reverse() const {
		GrafoDirigido R(_V);
		for (uint v = 0; v < _V; v++) {
			for (Iter it = _adj[v].principio(); it != _adj[v].final(); it.avanza()) {
				R.ponArista(it.elem(), v);
			}
		}
		return R;
	}

	/**
	 * Muestra el grafo en el stream de salida o
	 */
	void mostrar(ostream& o) const {
		o << _V << " vertices, " << _E << " aristas" << endl;
		for (uint v = 0; v < _V; v++) {
			o << v << ": ";
			for (Iter it = _adj[v].principio(); it != _adj[v].final(); it.avanza()) {
				o << it.elem() << " " << flush;
			}
			o << endl << flush;
		}
	}

private:
	void copia(const GrafoDirigido &other) {
		for (uint v = 0; v < V(); v++) {
			// recorrer adyacentes a v
			const Lista<uint>& lista = other.adj(v);
			for (Iter it = lista.principio(); it != lista.final(); it.avanza()) {
				uint w = it.elem();
				_adj[v].ponDr(w);
			}
		}
	}

};

/**
 * Para mostrar grafos por la salida estándar.
 */
ostream& operator<<(ostream& o,const GrafoDirigido& g){
	g.mostrar(o);
	return o;
}

class DepthFirstDirectedPaths {
private:
	bool* marked;     // marked[v] = is there an s-v path?
	uint* edgeTo;     // edgeTo[v] = last edge on s-v path
	uint s;           // source vertex

	// depth first search from v
	void dfs(const GrafoDirigido& G, uint v) {
		marked[v] = true;
		for (Iter it = G.adj(v).principio(); it != G.adj(v).final(); it.avanza()) {
			uint w = it.elem();
			if (!marked[w]) {
				edgeTo[w] = v;
				dfs(G, w);
			}
		}
	}

public:
	DepthFirstDirectedPaths(const GrafoDirigido& G, uint s) : s(s) {
		edgeTo = new uint[G.V()];
		marked = new bool[G.V()];
		for(uint i = 0; i < G.V(); i++) marked[i] = false;
		dfs(G, s);
	}

	~DepthFirstDirectedPaths() {
		delete[] marked; marked = NULL;
		delete[] edgeTo; edgeTo = NULL;
	}

	// is there a path between s and v?
	bool hasPathTo(uint v) const {
		return marked[v];
	}

	// return a path between s to v; null if no such path
	Path pathTo(uint v) const {
		Path path;
		if (!hasPathTo(v)) return path;
		for (uint x = v; x != s; x = edgeTo[x])
			path.Cons(x);
		path.Cons(s);
		return path;
	}
};


class BreadthFirstDirectedPaths {
private:
	bool* marked;     // marked[v] = is there an s-v path?
	uint* edgeTo;     // edgeTo[v] = last edge on s-v path
	uint* distTo;     // distTo[v] = number of edges shortest s-v path
	//	uint s;           // source vertex

	// BFS from single source
	void bfs(const GrafoDirigido& G, uint s) {
		Cola<uint> q;
		distTo[s] = 0;
		marked[s] = true;
		q.ponDetras(s);

		while (!q.esVacia()) {
			uint v = q.primero(); q.quitaPrim();
			for (Iter it = G.adj(v).principio(); it != G.adj(v).final(); it.avanza()) {
				uint w = it.elem();
				if (!marked[w]) {
					edgeTo[w] = v;
					distTo[w] = distTo[v] + 1;
					marked[w] = true;
					q.ponDetras(w);
				}
			}
		}
	}

	// BFS from multiple sources
	void bfs(const GrafoDirigido& G, Lista<uint> sources) {
		Cola<uint> q;
		for(Iter it = sources.principio(); it != sources.final(); it.avanza()) {
			uint s = it.elem();
			distTo[s] = 0;
			marked[s] = true;
			q.ponDetras(s);
		}

		while (!q.esVacia()) {
			uint v = q.primero(); q.quitaPrim();
			for (Iter it = G.adj(v).principio(); it != G.adj(v).final(); it.avanza()) {
				uint w = it.elem();
				if (!marked[w]) {
					edgeTo[w] = v;
					distTo[w] = distTo[v] + 1;
					marked[w] = true;
					q.ponDetras(w);
				}
			}
		}
	}

public:
	// single source
	BreadthFirstDirectedPaths(const GrafoDirigido& G, uint s) {
		edgeTo = new uint[G.V()];
		marked = new bool[G.V()];
		distTo = new uint[G.V()];
		for(uint i = 0; i < G.V(); i++) marked[i] = false;
		for(uint v = 0; v < G.V(); v++) distTo[v] = numeric_limits<uint>::infinity();
		bfs(G, s);
	}

	// multiple source
	BreadthFirstDirectedPaths(const GrafoDirigido& G, const Lista<uint>& sources) {
		edgeTo = new uint[G.V()];
		marked = new bool[G.V()];
		distTo = new uint[G.V()];
		for(uint i = 0; i < G.V(); i++) marked[i] = false;
		for(uint v = 0; v < G.V(); v++) distTo[v] = numeric_limits<uint>::infinity();
		bfs(G, sources);
	}

	~BreadthFirstDirectedPaths() {
		delete[] marked; marked = NULL;
		delete[] edgeTo; edgeTo = NULL;
		delete[] distTo; distTo = NULL;
	}

	// length of shortest path from s (or sources) to v
	uint distance(uint v) const {
		return distTo[v];
	}

	// is there a path between s and v?
	bool hasPathTo(uint v) const {
		return marked[v];
	}

	// return a path between s and v; empty if no such path
	Path pathTo(uint v) const {
		Path path;
		if (!hasPathTo(v)) return path;
		uint x;
		for (x = v; distTo[x] != 0; x = edgeTo[x])
			path.Cons(x);
		path.Cons(x);
		return path;
	}

};


class DepthFirstOrder {
private:
	bool* marked;           // marked[v] = has v been marked in dfs?
	int* pre;               // pre[v]    = preorder  number of v
	int* post;              // post[v]   = postorder number of v
	Lista<uint> preorder;   // vertices in preorder
	Lista<uint> postorder;  // vertices in postorder
	Lista<uint> revPost;    // vertices in reverse postorder
	uint preCounter;        // counter or preorder numbering
	uint postCounter;       // counter for postorder numbering

	// run DFS in digraph G from vertex v and compute preorder/postorder
	void dfs(const GrafoDirigido& G, uint v) {
		marked[v] = true;
		pre[v] = preCounter++;
		preorder.ponDr(v);
		for (Iter it = G.adj(v).principio(); it != G.adj(v).final(); it.avanza()) {
			uint w = it.elem();
			if (!marked[w]) {
				dfs(G, w);
			}
		}
		postorder.ponDr(v);
		post[v] = postCounter++;
		revPost.Cons(v);
	}

public:
	// depth-first search preorder and postorder in a digraph
	DepthFirstOrder(const GrafoDirigido& G) {
		pre    = new int[G.V()]; preCounter = 0;
		post   = new int[G.V()]; postCounter = 0;
		marked    = new bool[G.V()];
		for (uint v = 0; v < G.V(); v++) marked[v] = false;
		for (uint v = 0; v < G.V(); v++)
			if (!marked[v]) dfs(G, v);
	}

	int prenum(int v) const {
		return pre[v];
	}

	int postnum(int v) const {
		return post[v];
	}

	// return vertices in postorder as an Iterable
	const Lista<uint>& postOrder() const {
		return postorder;
	}

	// return vertices in postorder as an Iterable
	const Lista<uint>& preOrder() const {
		return preorder;
	}

	// return vertices in reverse postorder as an Iterable
	const Lista<uint>& reversePost() const {
		return revPost;
	}

};


class DirectedCycle {
private:
	bool* marked;        // marked[v] = has vertex v been marked?
	int* edgeTo;         // edgeTo[v] = previous vertex on path to v
	bool* onStack;       // onStack[v] = is vertex on the stack?
	Lista<uint> ciclo;   // directed cycle (or null if no such cycle)
	bool hayciclo;

	// check that algorithm computes either the topological order or finds a directed cycle
	void dfs(const GrafoDirigido& G, int v) {
		onStack[v] = true;
		marked[v] = true;
		for (Iter it = G.adj(v).principio(); it != G.adj(v).final(); it.avanza()) {
			uint w = it.elem();
			// short circuit if directed cycle found
			if (hayciclo) return;
			//found new vertex, so recur
			else if (!marked[w]) {
				edgeTo[w] = v;
				dfs(G, w);
			}
			// trace back directed cycle
			else if (onStack[w]) {
				hayciclo = true;
				for (uint x = v; x != w; x = edgeTo[x]) {
					ciclo.Cons(x);
				}
				ciclo.Cons(w);
				ciclo.Cons(v);
			}
		}
		onStack[v] = false;
	}

public:
	DirectedCycle(const GrafoDirigido& G) {
		hayciclo = false;
		marked  = new bool[G.V()];
		for (uint v = 0; v < G.V(); v++) marked[v] = false;
		onStack = new bool[G.V()];
		for (uint v = 0; v < G.V(); v++) onStack[v] = false;
		edgeTo  = new int[G.V()];
		for (uint v = 0; v < G.V(); v++)
			if (!marked[v]) dfs(G, v);
	}

	bool hasCycle() const { return hayciclo; }
	const Lista<uint>& cycle() const { return ciclo; }
};


class Topological {
private:
	Lista<uint> _order;    // topological order
	bool hayOrden;
public:
	// topological sort in a digraph
	Topological(const GrafoDirigido& G) {
		DirectedCycle finder(G);
		if (!finder.hasCycle()) {
			hayOrden = true;
			DepthFirstOrder dfs(G);
			_order = dfs.reversePost();
		} else hayOrden = false;
	}

	// return topological order if a DAG; null otherwise
	const Lista<uint>& order() const {
		return _order;
	}

	// does digraph have a topological order?
	bool hasOrder() const {
		return hayOrden;
	}
};


class KosarajuSharirSCC {
private:
	bool* marked;     // marked[v] = has vertex v been visited?
	int* _id;         // id[v] = id of strong component containing v
	int _count;       // number of strongly-connected components

	// DFS on graph G
	void dfs(const GrafoDirigido& G, uint v) {
		marked[v] = true;
		_id[v] = _count;
		for (Iter it = G.adj(v).principio(); it != G.adj(v).final(); it.avanza()) {
			uint w = it.elem();
			if (!marked[w]) dfs(G, w);
		}
	}

public:
	KosarajuSharirSCC(const GrafoDirigido& G) {

		// compute reverse postorder of reverse graph
		DepthFirstOrder dfsrev(G.reverse());

		// run DFS on G, using reverse postorder to guide calculation
		marked = new bool[G.V()];
		for (uint v = 0; v < G.V(); v++) marked[v] = false;
		_id = new int[G.V()];
		_count = 0;

		for (Iter it = dfsrev.reversePost().principio(); it != dfsrev.reversePost().final(); it.avanza()) {
			uint v = it.elem();
			if (!marked[v]) {
				dfs(G, v);
				_count++;
			}
		}
	}

	// return the number of strongly connected components
	int count() const { return _count; }

	// are v and w strongly connected?
	bool stronglyConnected(int v, int w) const {
		return _id[v] == _id[w];
	}

	// id of strong component containing v
	int id(int v) const {
		return _id[v];
	}
};

#endif /* GRAFODIRIGIDO_H_ */
