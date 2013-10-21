/*
 * LLRedBlack.h
 *
 *  Created on: 14/09/2012
 *      Author: alberto
 */

#ifndef LLREDBLACK_H_
#define LLREDBLACK_H_

#include <iostream>
#include <fstream>
using namespace std;

#include "Excepciones.h"
#include <cassert>

const bool RED = true;
const bool BLACK = false;

template <class Clave, class Valor>
class LLRedBlackBST {
private:
	/**
	 Clase nodo que almacena internamente la pareja (clave, valor),
	 los punteros al hijo izquierdo y al hijo derecho,
	 y el color de la arista entrante.
	 */
	class Nodo {
	public:
		Clave clave;
		Valor valor;
		Nodo *iz;
		Nodo *dr;
		bool color;
		Nodo(const Clave& c, const Valor& v, Nodo* i=NULL, Nodo* d=NULL, bool col=RED)
		: clave(c), valor(v), iz(i), dr(d), color(col) {}
	};

public:
	/** constructor */
	LLRedBlackBST() : raiz(NULL) {};

	/** destructor */
	~LLRedBlackBST(){
		libera();
		raiz = NULL;
	};

	// return number of key-value pairs in this symbol table
	int size() { return size(raiz); }

	// is this symbol table empty?
	bool esVacio() const {
		return raiz == NULL;
	}

	bool esta(const Clave& c) const {
		return busca(c, raiz) != NULL;
	}

	const Valor& consulta(const Clave &clave) {
		Nodo *p = busca(clave,raiz);
		if (p == NULL)
			throw EClaveErronea();
		return p->valor;
	}

	void mostrar(ostream& o, int indent) const {
		mostrar(o,indent,raiz);
	}

	void inserta(const Clave &c, const Valor &v) {
		inserta(c, v, raiz);
		raiz->color = BLACK;
	}

protected:

	void libera(){
		libera(raiz);
	}

	void copia(const LLRedBlackBST<Clave, Valor>& a){
		copia(raiz,a.raiz);
	}

private:

	/**
	 Puntero a la raiz de la estructura jerarquica de nodos.
	 */
	Nodo* raiz;

	void libera(Nodo* a){
		if (a != NULL){
			libera(a->iz);
			libera(a->dr);
			delete a;
		}
	}

	void copia(Nodo*& a,Nodo* b){
		if(b==NULL) a = NULL;
		else{
			Nodo* iz, *dr;
			copia(iz,b->iz);
			copia(dr,b->dr);
			a = new Nodo(b->clave,b->valor,iz,dr,b->color);
		}
	}

	/*************************************************************************
	 *  Node helper methods
	 *************************************************************************/
	// is node x red; false if x is null ?
	bool isRed(Nodo* x) {
		if (x == NULL) return false;
		return (x->color == RED);
	}

	static Nodo* busca(const Clave &clave, Nodo* a) {
		if (a == NULL)
			return NULL;
		else if (a->clave == clave)
			return a;
		else if (clave < a->clave)
			return busca(clave, a->iz);
		else // clave > a-> clave
			return busca(clave, a->dr);
	}

	void inserta(const Clave& c, const Valor& v, Nodo*& a){
		if (a==NULL) {
			a = new Nodo(c, v);
		} else {
			// split 4-notes in the way down
			if (isRed(a->iz) &&  isRed(a->dr))     flipColors(a);

			if (c == a->clave) {
				a->valor = v;
			} else if (c < a->clave) {
				inserta(c, v, a->iz);
			} else { // c > a->clave
				inserta(c, v, a->dr);
			}

			// fix right-leaning reds on the way up
			if (isRed(a->dr) && !isRed(a->iz))     rotateLeft(a);
			// fix two reds in a row on the way up
			if (isRed(a->iz) &&  isRed(a->iz->iz)) rotateRight(a);

		}
	}

	Nodo* min(Nodo* x) {
		//assert( x != NULL);
		if (x->iz == NULL) return x;
		else                return min(x->iz);
	}


	/*************************************************************************
	 *  red-black tree helper functions
	 *************************************************************************/

	// make a left-leaning link lean to the right
	void rotateRight(Nodo*& h) {
		Nodo* x = h->iz;
		h->iz = x->dr;
		x->dr = h;
		x->color = h->color;
		h->color = RED;
		h = x;
	}

	// make a right-leaning link lean to the left
	void rotateLeft(Nodo*& h) {
		Nodo* x = h->dr;
		h->dr = x->iz;
		x->iz = h;
		x->color = h->color;
		h->color = RED;
		h = x;
	}

	// flip the colors of a node and its two children
	void flipColors(Nodo* h) {
		// h must have opposite color of its two children
		h->color = !h->color;
		h->iz->color = !h->iz->color;
		h->dr->color = !h->dr->color;
	}

	void mostrar(ostream& o,int indent, Nodo* r) const {
		if(r!=NULL){
			mostrar(o,indent+2,r->dr);
			for(int i=0;i<indent;i++) o << " ";
//			o << "(" << r->clave << "," << r-> valor << ")" << endl;
			o << r->clave  << (r->color ? " R" : " N")  << endl;
			mostrar(o,indent+2,r->iz);
		}
	}

};


template <class Clave, class Valor>
ostream& operator<<(ostream& o,const LLRedBlackBST<Clave, Valor>& a){
	a.mostrar(o, 0);
	return o;
}


#endif /* LLREDBLACK_H_ */
