/**
  @file ColaPrio.h

   Created on: 19/09/2012
       Author: Alberto Verdejo
 */

#ifndef COLAPRIO_H_
#define COLAPRIO_H_

#include <iostream>
using namespace std;

#include "Excepciones.h"

/**
 Excepcion generada cuando el array esta lleno.
 */
class EColaPrLlena : public ExcepcionTAD {
public:
	EColaPrLlena() {};
	EColaPrLlena(const std::string &msg) : ExcepcionTAD(msg) {}
};

/**
 Excepcion generada cuando se intenta acceder al primero
 de una cola con prioridad vacia.
 */
class EColaPrVacia : public ExcepcionTAD {
public:
	EColaPrVacia() {};
	EColaPrVacia(const std::string &msg) : ExcepcionTAD(msg) {}
};

const int TAM_INICIAL = 100;

template <class T, bool(*antes)(const T &, const T &)>
class ColaPrio {
public:
	/** Constructor; operacion ColaPVacia */
	ColaPrio(int t = TAM_INICIAL) :
		v(new T[t+1]), tam(t), numElems(0) {};

	/** Constructor a partir de un vector de elementos */
	ColaPrio(T v1[], unsigned int t) :
		v(new T[t+1+TAM_INICIAL]), tam(t+1+TAM_INICIAL), numElems(t) {
		for(unsigned int i=0; i < t; i++) v[i+1] = v1[i];
		//monticulizar1();
		monticulizar2();
	};

	/** Destructor; elimina el vector. */
	~ColaPrio() {
		libera();
	};

	void inserta(const T& x) {
		if (numElems == tam) throw EColaPrLlena();
		else {
			numElems++;
			v[numElems] = x;
			flotar(numElems);
		}
		return;
	}

	bool esVacia() const {
		return (numElems == 0);
	}

	const T& primero() const {
		if (numElems == 0) throw EColaPrVacia("No se puede consultar el primero");
		else return v[1];
	}

	void quitaPrim() {
		if (numElems == 0) throw EColaPrVacia("Imposible eliminar primero");
		else {
			v[1] = v[numElems];
			numElems--;
			hundir(1);
		}
	}

	/** Constructor copia */
	ColaPrio(const ColaPrio<T,antes> &other) {
		copia(other);
	}

	/** Operador de asignación */
	ColaPrio<T,antes> &operator=(const ColaPrio<T,antes> &other) {
		if (this != &other) {
			libera();
			copia(other);
		}
		return *this;
	}

	void mostrar(ostream& o) const {
		for(unsigned int i=1; i <= numElems; ++i)
			o << v[i] << " ";
	}


private:
	void inicia(int ta) {
		v = new T[ta];
		tam = ta;
		numElems = 0;
	}

	void libera() {
		delete[] v;
		v = NULL;
	}

	void copia(const ColaPrio &other) {
		tam = other.numElems + TAM_INICIAL;
		numElems = other.numElems;
		v = new T[tam];
		for (unsigned int i = 1; i <= numElems; ++i)
			v[i] = other.v[i];
	}

	void flotar(unsigned int n) {
		unsigned int i = n;
		T elem = v[i];
		while ((i != 1) && antes(elem, v[i /2])) {
			v[i] = v[i/2];
			i = i/2;
		}
		v[i] = elem;
	}

	void hundir(unsigned int n) {
		unsigned int i = n;
		T elem = v[i];
		unsigned int m = 2*i; // hijo izquierdo de i, si existe
		while (m <= numElems)  {
			// cambiar al hijo derecho de i si existe y va antes que el izquierdo
			if ((m < numElems) && ( antes(v[m + 1], v[m])))
				m = m + 1;
			// flotar el hijo m si va antes que el elemento hundiendose
			if (antes(v[m], elem)) {
				v[i] = v[m];
				i = m; m = 2*i;
			}
			else break;
		}
		v[i] = elem;
	}

	void monticulizar1() {
		for(unsigned int j = 2; j <= numElems; ++j) {
			// cout << "undiendo" << j << endl;
			flotar(j);
			// cout << *this << endl;
		}
	}

	void monticulizar2() {
		for(unsigned int j = numElems/2; j >= 1; --j)
			hundir(j);
	}

	/** Puntero al array que contiene los datos. */
	T* v;

	/** Tama–o del vector v. */
	unsigned int tam;

	/** Numero de elementos reales guardados. */
	unsigned int numElems;
};


template <class T, bool(*antes)(const T &, const T &)>
ostream& operator<<(ostream& o,const ColaPrio<T, antes>& a){
	a.mostrar(o);
	return o;
}


#endif /* COLAPRIO_H_ */
