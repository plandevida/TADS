/**
  @file ColaPrioPares.h

   Created on: 19/09/2012
       Author: Alberto Verdejo
 */

#ifndef ColaPrioPares_H_
#define ColaPrioPares_H_

#include <iostream>
using namespace std;

#include "Excepciones.h"
#include "ColaPrio.h"

/**
 Excepción generada cuando se intenta insertar un elemento
 que ya esta en un moticulo de pares.
 */
class EElemRepe : public ExcepcionTAD {
public:
	EElemRepe() {};
	EElemRepe(const std::string &msg) : ExcepcionTAD(msg) {}
};



// registro para las parejas < elem, prioridad >
template <class T>
struct Par {
   unsigned int elem;
   T prioridad;
};


template <class T, bool(*antes)(const T &, const T &)>
class ColaPrioPares {
public:
   /** Constructor */
   ColaPrioPares(int t) :
      v(new Par<T>[t+1]), posiciones(new unsigned int[t+1]), tam(t), numElems(0) {
      for(unsigned int i=1; i <= tam; i++)
         posiciones[i] = 0; // el elemento i no esta
   };

   /** Destructor; elimina los vectores */
   ~ColaPrioPares() {
      libera();
   };

   void inserta(unsigned int e, const T& p) {
      if (posiciones[e] != 0) throw EElemRepe();
      else if (numElems == tam) throw EColaPrLlena();
      else {
         numElems++;
         v[numElems].elem = e; v[numElems].prioridad = p;
         posiciones[e] = numElems;
         flotar(numElems);
      }
      return;
   }

   void modifica(unsigned int e, const T& p) {
      int i = posiciones[e];
      if (i == 0) // el elemento e se inserta por primera vez
         inserta(e, p);
      else {
         v[i].prioridad = p;
         if (i != 1 && antes(v[i].prioridad, v[i/2].prioridad))
            flotar(i);
         else // puede hacer falta hundir a e
            hundir(i);
      }
   }

   bool esVacia() const {
      return (numElems == 0);
   }

   const Par<T>& primero() const {
      if (numElems == 0) throw EColaPrVacia("No se puede consultar el primero");
      else return v[1];
   }

   void quitaPrim() {
      if (numElems == 0) throw EColaPrVacia("Imposible eliminar primero");
      else {
         posiciones[v[1].elem] = 0; // para indicar que no esta
         v[1] = v[numElems];
         posiciones[v[1].elem] = 1;
         numElems--;
         hundir(1);
      }
   }

   /** Constructor copia */
   ColaPrioPares(const ColaPrioPares<T,antes> &other) {
      copia(other);
   }

   /** Operador de asignación */
   ColaPrioPares<T,antes> &operator=(const ColaPrioPares<T,antes> &other) {
      if (this != &other) {
         libera();
         copia(other);
      }
      return *this;
   }

   void mostrar(ostream& o) const {
      for(unsigned int i=1; i <= numElems; ++i)
         o << i << ":" << v[i].elem << "," << v[i].prioridad << " | ";
      cout << endl;
      for(unsigned int i=1; i <= tam; ++i)
         o << i << ":" << posiciones[i] << " | ";
      cout << endl;
      cout << flush;
   }


private:
   void inicia(int ta) {
      v = new Par<T>[ta+1];
      posiciones = new unsigned int[ta+1];
      tam = ta;
      numElems = 0;
   }

   void libera() {
      delete[] v;
      v = NULL;
      delete[] posiciones;
      posiciones = NULL;
   }

   void copia(const ColaPrioPares &other) {
      tam = other.tam;
      numElems = other.numElems;
      v = new Par<T>[tam+1];
      posiciones = new unsigned int[tam+1];
      v = new T[tam];
      for (unsigned int i = 1; i <= numElems; ++i)
         v[i] = other.v[i];
      for (unsigned int i = 1; i <= tam; ++i)
         posiciones[i] = other.posiciones[i];
   }

   void flotar(unsigned int n) {
      unsigned int i = n;
      Par<T> parmov = v[i];
      while ((i != 1) && antes(parmov.prioridad, v[i/2].prioridad)) {
         v[i] = v[i/2]; posiciones[v[i].elem] = i;
         i = i/2;
      }
      v[i] = parmov; posiciones[v[i].elem] = i;
   }

   void hundir(unsigned int n) {
      unsigned int i = n;
      Par<T> parmov = v[i];
      unsigned int m = 2*i; // hijo izquierdo de i, si existe
      while (m <= numElems)  {
         // cambiar al hijo derecho de i si existe y va antes que el izquierdo
         if ((m < numElems) && ( antes(v[m + 1].prioridad, v[m].prioridad)))
            m = m + 1;
         // flotar el hijo m si va antes que el elemento hundiendose
         if (antes(v[m].prioridad, parmov.prioridad)) {
            v[i] = v[m]; posiciones[v[i].elem] = i;
            i = m; m = 2*i;
         }
         else break;
      }
      v[i] = parmov; posiciones[v[i].elem] = i;
   }

   /** Puntero al array que contiene los datos (pares < elem, prio >). */
   Par<T>* v;

   /** Puntero al array que contiene las posiciones en v de los elementos. */
   unsigned int* posiciones;

   /** Tama–o del vector v. */
   unsigned int tam;

   /** Numero de elementos reales guardados. */
   unsigned int numElems;
};


template <class T, bool(*antes)(const T &, const T &)>
ostream& operator<<(ostream& o,const ColaPrioPares<T, antes>& a){
   a.mostrar(o);
   return o;
}


#endif /* ColaPrioPares_H_ */
