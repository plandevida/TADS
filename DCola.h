/**
  @file DCola.h

  Implementaci�n del TAD doble cola, utilizando una
  lista doblemente enlazada con nodo fantasma o
  cabecera.

  Estructura de Datos y Algoritmos
  Facultad de Inform�tica
  Universidad Complutense de Madrid

 (c) Marco Antonio G�mez Mart�n, 2012
*/
#ifndef __DCOLA_H
#define __DCOLA_H

#include "Excepciones.h"
#include <cassert>
/**
 Implementaci�n del TAD Doble Cola utilizando una lista doblemente
 enlazada circular y con nodo fantasma.

 Las operaciones son:

 - DColaVacia: -> DCola. Generadora implementada en el
   constructor sin par�metros.
 - PonDetras: DCola, Elem -> DCola. Generadora
 - ponDelante: DCola, Elem -> DCola. Modificadora.
 - quitaPrim: DCola - -> DCola. Modificadora parcial
 - primero: DCola - -> Elem. Observadora parcial
 - quitaUlt: DCola - -> DCola. Modificadora parcial
 - ultimo: DCola - -> Elem. Observadora parcial
 - esVacia: DCola -> Bool. Observadora

 @author Marco Antonio G�mez Mart�n
 */
template <class T>
class DCola {
public:

	/** Constructor; operaci�n DColaVacia. */
	DCola() {
		_fantasma = new Nodo();
		_fantasma->_sig = _fantasma;
		_fantasma->_ant = _fantasma;
		_numElems = 0;
	}

	/** Destructor; elimina la lista doblemente enlazada. */
	~DCola() {
		libera();
	}

	/**
	 A�ade un elemento por la parte de atr�s de la cola.
	 Es una operaci�n generadora.
	*/
	void ponDetras(const T &e) {
		insertaElem(e, _fantasma->_ant, _fantasma);
		_numElems++;
	}

	/**
	 Devuelve el primer elemento de la cola; es una operaci�n
	 observadora parcial, pues es un error preguntar por
	 el primer elemento de una doble cola vac�a.

	 primero(PonDetras(elem, DColaVacia)) = elem
	 primero(PonDetras(elem, xs)) = primero(xs) si !esVacia(xs)
	 error: primero(DColaVacia)
	 */
	const T &primero() const {
		if (esVacia())
			throw EDColaVacia();
		return _fantasma->_sig->_elem;
	}


	/**
	 Elimina el primer elemento de la doble cola.
	 Operaci�n modificadora parcial, que falla si 
	 est� vac�a.

	 quitaPrim(PonDetras(elem, DColaVacia)) = DColaVacia
	 quitaPrim(PonDetras(elem, xs)) = PonDetras(elem, quitaPrim(xs)) si !esVacia(xs)
	 error: quitaPrim(DColaVacia)
	*/
	void quitaPrim() {
		if (esVacia())
			throw EDColaVacia();

		borraElem(_fantasma->_sig);
		--_numElems;
	}

	/**
	 A�ade un elemento a la parte delantera de una doble cola.
	 Operaci�n modificadora.

	 ponDelante(elem, DColaVacia) = ponDetras(elem, DColaVacia)
	 ponDelante(elem, ponDetras(x, xs)) = 
		ponDetras(x, ponDelante(elem, xs))

	 @param e Elemento que se a�ade
	 */
	void ponDelante(const T &e) {
		insertaElem(e, _fantasma, _fantasma->_sig);
		++_numElems;
	}

	/**
	 Devuelve el �ltimo elemento de la doble cola. Es
	 un error preguntar por el �ltimo de una doble cola vac�a.

	 ultimo(PonDetras(x, xs)) = x
	 error: ultimo(DColaVacia)

	 @return �ltimo elemento de la cola.
	 */
	const T &ultimo() const {
		if (esVacia())
			throw EDColaVacia();
		return _fantasma->_ant->_elem;
	}

	/**
	 Elimina el �ltimo elemento de la doble cola. Es
	 un error quitar el �ltimo de una doble cola vac�a.

	 quitaUlt(PonDetras(x, xs)) = xs
	 error: quitaUlt(DColaVacia)
	 */
	void quitaUlt() {
		if (esVacia())
			throw EDColaVacia();

		borraElem(_fantasma->_ant);
		--_numElems;
	}

	/**
	 Operaci�n observadora para saber si una doble cola
	 tiene o no elementos.

	 esVacia(DColaVacia) = true
	 esVacia(ponDetras(x, xs)) = false

	 @return true si la doble cola no tiene elementos.
	 */
	bool esVacia() const {
		return _fantasma->_sig == _fantasma;
		/* return _numElems == 0; */
	}

	/**
	 Devuelve el n�mero de elementos que hay en la
	 doble cola.
	 numElems(DColaVacia) = 0
	 numElems(PonDetras(elem, p)) = 1 + numElems(p)

	 @return N�mero de elementos.
	 */
	int numElems() const {
		return _numElems;
	}

	// //
	// M�TODOS DE "FONTANER�A" DE C++ QUE HACEN VERS�TIL
	// A LA CLASE
	// //

	/** Constructor copia */
	DCola(const DCola<T> &other) : _fantasma(NULL) {
		copia(other);
	}

	/** Operador de asignaci�n */
	DCola<T> &operator=(const DCola<T> &other) {
		if (this != &other) {
			libera();
			copia(other);
		}
		return *this;
	}

	/** Operador de comparaci�n. */
	bool operator==(const DCola<T> &rhs) const {
		if (_numElems != rhs._numElems)
			return false;
		Nodo *p1 = _fantasma->_sig;
		Nodo *p2 = rhs._fantasma->_sig;
		while ((p1 != _fantasma) && (p2 != rhs._fantasma)) {
			if (p1->_elem != p2->_elem)
				return false;
			p1 = p1->_sig;
			p2 = p2->_sig;
		}

		return (p1 == _fantasma) && (p2 == rhs._fantasma);
	}

	bool operator!=(const DCola<T> &rhs) const {
		return !(*this == rhs);
	}

protected:

	void libera() {
		// Usamos libera(nodo), pero antes quitamos
		// la circularidad para evitar bucle
		// infinito...
		_fantasma->_ant->_sig = NULL;
		_fantasma->_ant = NULL;
		libera(_fantasma);
		_fantasma = NULL;
	}

	void copia(const DCola<T> &other) {
		// En vez de trabajar con punteros en la inserci�n,
		// usamos ponDetras.
		_fantasma = new Nodo();
		_fantasma->_sig = _fantasma;
		_fantasma->_ant = _fantasma;
		_numElems = 0;

		Nodo *act = other._fantasma->_sig;
		while (act != other._fantasma) {
			ponDetras(act->_elem);
			act = act->_sig;
		}
	}

private:

	/**
	 Clase nodo que almacena internamente el elemento (de tipo T),
	 y dos punteros, uno al nodo anterior y otro al nodo siguiente.
	 Ambos punteros podr�an ser NULL si el nodo es el primero
	 y/o �ltimo de la lista enlazada.
	 */
	class Nodo {
	public:
		Nodo() : _sig(NULL) {}
		Nodo(const T &elem) : _elem(elem), _sig(NULL), _ant(NULL) {}
		Nodo(Nodo *ant, const T &elem, Nodo *sig) : 
		    _elem(elem), _sig(sig), _ant(ant) {}

		T _elem;
		Nodo *_sig;
		Nodo *_ant;
	};

	/**
	 Inserta un elemento entre el nodo1 y el nodo2.
	 Devuelve el puntero al nodo creado.
	 Caso general: los dos nodos existen.
	    nodo1->_sig == nodo2
	    nodo2->_ant == nodo1
	 Casos especiales: alguno de los nodos no existe
	    nodo1 == NULL y/o nodo2 == NULL
	*/
	static Nodo *insertaElem(const T &e, Nodo *nodo1, Nodo *nodo2) {
		Nodo *nuevo = new Nodo(nodo1, e, nodo2);
		if (nodo1 != NULL)
			nodo1->_sig = nuevo;
		if (nodo2 != NULL)
			nodo2->_ant = nuevo;
		return nuevo;
	}

	/**
	 Elimina el nodo n. Si el nodo tiene nodos antes
	 o despu�s, actualiza sus punteros anterior y siguiente.
	 Caso general: hay nodos anterior y siguiente.
	 Casos especiales: algunos de los nodos (anterior o siguiente
	 a n) no existen.
	*/
	static void borraElem(Nodo *n) {
		if (n == NULL)
			return;

		Nodo *ant = n->_ant;
		Nodo *sig = n->_sig;
		if (ant != NULL)
			ant->_sig = sig;
		if (sig != NULL)
			sig->_ant = ant;
		delete n;
	}

	/**
	 Elimina todos los nodos de la lista enlazada cuyo
	 primer nodo se pasa como par�metro.
	 Se admite que el nodo sea NULL (no habr� nada que
	 liberar). En caso de pasarse un nodo v�lido,
	 su puntero al nodo anterior debe ser NULL (si no,
	 no ser�a el primero de la lista!).
	 */
	static void libera(Nodo *prim) {
		if (prim == NULL)
			return;

		assert(!prim || !prim->_ant);

		while (prim != NULL) {
			Nodo *aux = prim;
			prim = prim->_sig;
			delete aux;
		}
	}

	// Puntero al nodo fantasma
	Nodo *_fantasma;

	// N�mero de elementos
	unsigned int _numElems;
};

#endif // __DCOLA_H
