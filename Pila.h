/**
  @file Pila.h

  Implementaci�n del TAD Pila utilizando un
  vector din�mico cuyo tama�o va creciendo si
  es necesario.

  Estructura de Datos y Algoritmos
  Facultad de Inform�tica
  Universidad Complutense de Madrid

 (c) Marco Antonio G�mez Mart�n, 2012
*/
#ifndef __PILA_H
#define __PILA_H

#include "Excepciones.h"

/**
 Implementaci�n del TAD Pila utilizando vectores din�micos.

 Las operaciones son:

 - PilaVacia: -> Pila. Generadora implementada en el
   constructor sin par�metros.
 - apila: Pila, Elem -> Pila. Generadora
 - desapila: Pila - -> Pila. Modificadora parcial.
 - cima: Pila - -> Elem. Observadora parcial.
 - esVacia: Pila -> Bool. Observadora.
 - numElems: Pila -> Entero. Observadora.

 @author Marco Antonio G�mez Mart�n
 */
template <class T>
class Pila {
public:

	/** Tama�o inicial del vector din�mico. */
	enum { TAM_INICIAL = 10 };

	/** Constructor; operaci�n PilaVacia */
	Pila() {
		inicia();
	}

	/** Destructor; elimina el vector. */
	~Pila() {
		libera();
	}

	/**
	 Apila un elemento. Operaci�n generadora.

	 @param elem Elemento a apilar.
	*/
	void apila(const T &elem) {
		if (_numElems == _tam)
			amplia();
		_v[_numElems] = elem;
		_numElems++;
	}
	
	/**
	 Desapila un elemento. Operaci�n modificadora parcial,
	 que falla si la pila est� vac�a.

	 desapila(Apila(elem, p)) = p
	 error: desapila(PilaVacia)
	*/
	void desapila() {
		if (esVacia())
			throw EPilaVacia();
		--_numElems;
	}

	/**
	 Devuelve el elemento en la cima de la pila. Operaci�n
	 observadora parcial, que falla si la pila est� vac�a.

	 cima(Apila(elem, p) = elem
	 error: cima(PilaVacia)

	 @return Elemento en la cima de la pila.
	 */
	const T &cima() const {
		if (esVacia())
			throw EPilaVacia();
		return _v[_numElems - 1];
	}

	/**
	 Devuelve true si la pila no tiene ning�n elemento.

	 esVacia(PilaVacia) = true
	 esVacia(Apila(elem, p)) = false

	 @return true si la pila no tiene ning�n elemento.
	 */
	bool esVacia() const {
		return _numElems == 0;
	}

	/**
	 Devuelve el n�mero de elementos que hay en la
	 pila.
	 numElems(PilaVacia) = 0
	 numElems(Apila(elem, p)) = 1 + numElems(p)

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
	Pila(const Pila<T> &other) {
		copia(other);
	}

	/** Operador de asignaci�n */
	Pila<T> &operator=(const Pila<T> &other) {
		if (this != &other) {
			libera();
			copia(other);
		}
		return *this;
	}

	/** Operador de comparaci�n. */
	bool operator==(const Pila<T> &rhs) const {
		if (_numElems != rhs._numElems)
			return false;
		for (unsigned int i = 0; i < _numElems; ++i)
			if (_v[i] != rhs._v[i])
				return false;
		return true;
	}

	bool operator!=(const Pila<T> &rhs) const {
		return !(*this == rhs);
	}

protected:

	void inicia() {
		_v = new T[TAM_INICIAL];
		_tam = TAM_INICIAL;
		_numElems = 0;
	}

	void libera() {
		delete []_v;
		_v = NULL;
	}

	void copia(const Pila &other) {
		_tam = other._numElems + TAM_INICIAL;
		_numElems = other._numElems;
		_v = new T[_tam];
		for (unsigned int i = 0; i < _numElems; ++i)
			_v[i] = other._v[i];
	}

	void amplia() {
		T *viejo = _v;
		_tam *= 2;
		_v = new T[_tam];

		for (unsigned int i = 0; i < _numElems; ++i)
			_v[i] = viejo[i];

		delete []viejo;
	}

private:

	/** Puntero al array que contiene los datos. */
	T *_v;

	/** Tama�o del vector _v. */
	unsigned int _tam;

	/** N�mero de elementos reales guardados. */
	unsigned int _numElems;
};

#endif // __PILA_H
