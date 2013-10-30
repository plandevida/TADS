
#ifndef AVL_H_
#define AVL_H_

int max(int a,int b){if(a>=b)return a;else return b;}

#include "Excepciones.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include "Lista.h"

using namespace std;


template <class Clave, class Valor>
class AVL {
private:
    /**
     Clase nodo que almacena internamente la pareja (clave, valor),
     los punteros al hijo izquierdo y al hijo derecho, y la altura.
     */
    class Nodo {
    public:
        Clave clave;
        Valor valor;
        Nodo *iz;
        Nodo *dr;
        int altura;
        int tam_i;
        Nodo(const Clave& c, const Valor& v, Nodo* i=NULL, Nodo* d=NULL, int alt=1, int tam_izquierdos=1)
        : clave(c), valor(v), iz(i), dr(d), altura(alt), tam_i(tam_izquierdos) {}
    };
    
    
public:
    /** constructor */
    AVL() : raiz(NULL) {};
    
    /** destructor */
    ~AVL(){
        libera();
        raiz = NULL;
    };
    
    void inserta(const Clave &c, const Valor &v) {
        bool sobreescritura = false;
        
        inserta(c, v, raiz, sobreescritura);
        
        assert( esAVLcorrecto(raiz) );
    }
    
    bool esVacio() const {
        return esVAcio(raiz);
    }
    
    bool esta(const Clave& c) const {
        return busca(c, raiz) != NULL;
    }
    
    const Valor& consulta(const Clave &clave) {
        Nodo* p = busca(clave, raiz);
        if (p == NULL)
            throw EClaveErronea("La clave no se puede consultar");
        return p->valor;
    }
    
    void mostrar(ostream& o, int indent) const {
        mostrar(o,indent,raiz, false);
        cout << endl;
    }
    
    void mostrar(ostream& o, int indent, bool debug) {
        mostrar(o, indent, raiz, debug);
        cout << endl;
    }
    
    
    /** Constructor copia */
    AVL(const AVL<Clave, Valor>& other) : raiz(NULL) {
        copia(other);
    }
    
    
    /** Operador de asignacion */
    AVL<Clave, Valor>& operator=(const AVL<Clave, Valor> &other) {
        if (this != &other) {
            libera();
            copia(other);
        }
        return *this;
    }
    
    /**
     *
     * Operación que devuelve si un árvol es equilibrado,
     * es decir, si la diferencia de las alturas de sus
     * hijos no supera 1.
     */
    bool equilibrado() {
        
        int altura;
        
        return equilibrado(raiz, altura);
    }
    
    /**
     *
     * Operación que determina si el árbol es quilibrado
     * y si el atrubuto altura de cada uno de los nodos
     * está correctamente calculado.
     */
    bool esAVLcorrecto() {
        int altura;
        
        return AVLcorrecto(raiz, altura);
    }
    
    const Clave kesimoelementominimo(const int kesimo) {
        
        return (kesimoelementominimo(raiz, kesimo))->clave;
    }
    
    /**
     * Método que devuelve una lista con las claves ordenadas que
     * pertenezcan al rango dado.
     */
    Lista<Clave> rango(const Clave& k1, const Clave& k2) const {
        
        Lista<Clave> clavesRango;
        Clave inicio = k1;
        Clave fin = k2;
        
        if ( k1 > k2 ) {
            Clave aux = inicio;
            inicio = fin;
            fin = aux;
        }
        
        assert( k1 < k2 );
        
        rango(raiz, k1, k2, clavesRango);
        
        return clavesRango;
    }
    
	/**
	 Operación que borra un elemento del árbol
	 */
	void borra(const Clave& clave) {
		raiz = borraAux(raiz, clave, 0);
	}
    
    Nodo* borraAux(Nodo* p, const Clave &clave, int nivel) {
        
		if (p == NULL)
			return NULL;
        
		if (clave == p->clave) {
            p = borraRaiz(p);
            
//            if ( nivel == 0 ) {
//                reequilibraIzq(p->dr);
//            }
//            else {
//                reequilibraDer(p);
//            }
            
		} else if (clave < p->clave) {
			p->iz = borraAux(p->iz, clave, nivel+1);
            p->tam_i--;
            
            reequilibraIzq(p);
		} else { // clave > p->_clave
			p->dr = borraAux(p->dr, clave, nivel+1);
            p->tam_i--;
            
            reequilibraDer(p);
		}
        
        return p;
	}
    
    Nodo* borraRaiz(Nodo *p) {
        
		Nodo* aux;
        
		// Si no hay hijo izquierdo, la raíz pasa a ser
		// el hijo derecho
		if (p->iz == NULL) {
			aux = p->dr;
			delete p;
		} else
            // Si no hay hijo derecho, la raíz pasa a ser
            // el hijo izquierdo
            if (p->dr == NULL) {
                aux = p->iz;
                delete p;
            } else {
                // Convertimos el elemento más pequeño del hijo derecho
                // en la raíz.
                aux = mueveMinYBorra(p);
            }
        
        return aux;
	}
    
    Nodo* mueveMinYBorra(Nodo *p) {
    
		// Vamos bajando hasta que encontramos el elemento
		// más pequeÒo (aquel que no tiene hijo izquierdo).
		// Vamos guardando también el padre (que será null
		// si el hijo derecho es directamente el elemento
		// más pequeño).
		Nodo* padre = NULL;
		Nodo* aux = p->dr;
		while (aux->iz != NULL) {
			padre = aux;
			aux = aux->iz;
		}
        
		// aux apunta al elemento más pequeño.
		// padre apunta a su padre (si el nodo es hijo izquierdo)
        
		// Dos casos dependiendo de si el padre del nodo con
		// el mínimo es o no la raÌz a eliminar
		// (=> padre != NULL)
		if (padre != NULL) {
            
			padre->iz = aux->dr;
            
            // Actualiza el número de hijos izquierdos del padre
            // del nodo que sube y su altura.
            padre->tam_i = aux->tam_i;
            padre->altura = max(altura(padre->iz),altura(padre->dr))+1;
            
			aux->iz = p->iz;
            
			aux->dr = p->dr;
            
            // Actualiza el número de hijos izquierdos del nodo
            // que sube y su altura.
            aux->tam_i = p->tam_i;
            aux->altura = max(altura(aux->iz),altura(aux->dr))+1;
            
		} else {
            
			aux->iz = p->iz;
            
            // Actualiza el número de hijos izquierdos del nodo
            // que sube y su altura.
            aux->tam_i = p->tam_i;
            aux->altura = max(altura(aux->iz),altura(aux->dr))+1;
		}
    
		delete p;
		return aux;
	}
    
protected:
    
    void libera(){
        libera(raiz);
    }
    
    void copia(const AVL<Clave, Valor>& a){
        copia(raiz,a.raiz);
    }
    
private:
    
    /**
     Puntero a la raiz de la estructura jerarquica de nodos.
     */
    Nodo* raiz;
    
    
    static void libera(Nodo* a){
        if (a != NULL){
            libera(a->iz);
            libera(a->dr);
            delete a;
        }
    }
    
    static void copia(Nodo*& a,Nodo* b){
        if(b==NULL) a = NULL;
        else{
            Nodo* iz, *dr;
            copia(iz,b->iz);
            copia(dr,b->dr);
            a = new Nodo(b->clave,b->valor,iz,dr,max(altura(iz),altura(dr))+1);
        }
    }
    
    static bool esVAcio(const Nodo* n) {
        return n == NULL;
    }
    
    static Nodo* busca(const Clave& c, Nodo* a) {
        if (a == NULL)
            return NULL;
        else if (a->clave == c)
            return a;
        else if (c < a->clave)
            return busca(c, a->iz);
        else // c > a->clave
            return busca(c, a->dr);
    }
    
    static int altura(Nodo* a){
        if (a==NULL) return 0;
        else return a->altura;
    }
    
    static void inserta(const Clave& c, const Valor& v, Nodo*& a, bool& sobreescritura){
        if (a == NULL) {
            a = new Nodo(c,v);
        } else if (c == a->clave) {
            a->valor = v;
            sobreescritura = true;
        } else if (c < a->clave) {
            inserta(c, v, a->iz, sobreescritura);
            if ( !sobreescritura ) {
                a->tam_i++;
            }
            reequilibraDer(a);
        } else { // c > a->clave
            inserta(c, v, a->dr, sobreescritura);
            reequilibraIzq(a);
        }
    }
    
    
    static void rotaDer(Nodo*& k2){
        Nodo* k1 = k2->iz;
        k2->iz = k1->dr;
        
        // Deja de tener todos los nodos izquierdos el número de tam_i de k1
        // y pasa a tener como nodos izquierdos los derechos de k1.
        k2->tam_i -= k1->tam_i;
        
        k1->dr = k2;
        k2->altura = max(altura(k2->iz),altura(k2->dr))+1;
        k1->altura = max(altura(k1->iz),altura(k1->dr))+1;
        k2 = k1;
    }
    
    static void rotaIzq(Nodo*& k1){
        Nodo* k2 = k1->dr;
        k1->dr = k2->iz;
        k2->iz = k1;
        
        // k1 pasa a ser hijo izquierdo de k2
        k2->tam_i += k1->tam_i;
        
        k1->altura = max(altura(k1->iz),altura(k1->dr))+1;
        k2->altura = max(altura(k2->iz),altura(k2->dr))+1;
        k1=k2;
    }
    
    static void rotaIzqDer(Nodo*& k3){
        rotaIzq(k3->iz);
        rotaDer(k3);
    }
    
    static void rotaDerIzq(Nodo*& k1){
        rotaDer(k1->dr);
        rotaIzq(k1);
    }
    
    static void reequilibraIzq(Nodo*& a){
        if (altura(a->dr)-altura(a->iz) > 1) {
            if (altura(a->dr->iz) > altura(a->dr->dr))
                rotaDerIzq(a);
            else rotaIzq(a);
        }
        else a->altura = max(altura(a->iz),altura(a->dr))+1;
    }
    
    static void reequilibraDer(Nodo*& a){
        if (altura(a->iz)-altura(a->dr) > 1) {
            if (altura(a->iz->dr) > altura(a->iz->iz))
                rotaIzqDer(a);
            else rotaDer(a);
        }
        else a->altura = max(altura(a->iz),altura(a->dr))+1;
    }
    
    void mostrar(ostream& o,int indent, Nodo* r, bool debug) const {
        
        if(r!=NULL) {
            
            mostrar(o, indent+2, r->dr, debug);
            
            for(int i=0; i<indent; i++) o << " ";
            
            o << "(" << r->clave << "," << r-> valor << ")";
            
            if (debug) {
                o << " tam_i = " << r->tam_i << " altura = " << r->altura;
            }
            
            o << endl;
            
            mostrar(o,indent+2,r->iz, debug);
        }
    }
    
    static bool equilibrado(Nodo* n, int& altura) {
        
        bool equi;
        
        if ( n == NULL ) {
            altura = 0;
            equi = true;
        }
        else {
            
            int alturaIz, alturaDr;
            
            bool equiI = equilibrado(n->iz, alturaIz);
            bool equiD = equilibrado(n->dr, alturaDr);
            
            altura = max(alturaIz, alturaDr);
            
            equi = equiI && equiD && (abs(alturaIz - alturaDr) <= 1);
        }
        
        return equi;
    }
    
    /**
     *
     * Operación que determina si el árbol es quilibrado
     * y si el atrubuto altura de cada uno de los nodos
     * está correctamente calculado.
     */
    static bool esAVLcorrecto(Nodo* nodo) {
        int altura;
        
        return AVLcorrecto(nodo, altura);
    }
    
    static bool AVLcorrecto(Nodo* n, int& altura) {
        
        bool equi;
        
        if ( n == NULL ) {
            altura = 0;
            equi = true;
        }
        else {
            
            int alturaIz, alturaDr;
            
            bool equiI = AVLcorrecto(n->iz, alturaIz);
            bool equiD = AVLcorrecto(n->dr, alturaDr);
            
            altura = max(alturaIz, alturaDr);
            
            equi = equiI && equiD && (abs(alturaIz - alturaDr) <= 1) && ( n->altura == altura );
        }
        
        return equi;
    }
    
    
//    static bool AVLcorrecto(Nodo* n, int& altura, bool& equi) {
//        
//        int alturaIz, alturaDr, alturacopy;
//        alturaIz = alturaDr = alturacopy = altura;
//        
//        if ( n == NULL ) {
//            
//            return false;
//        }
//        else {
//            
//            bool resI = AVLcorrecto(n->iz, alturaIz, equi);
//            
//            bool resD = AVLcorrecto(n->dr, alturaDr, equi);
//            
//            if ( resI ) {
//                alturaIz++;
//            }
//            if ( resD ) {
//                alturaDr++;
//            }
//            
//            if ( alturacopy < alturaIz ) altura = alturacopy = alturaIz;
//            if ( alturacopy < alturaDr ) altura = alturacopy = alturaDr;
//            
//            // Si la altura está mal calculada o la diferencia entre sus hijos es
//            // mayor que 1, la condición será falsa.
//            if ( n->altura != alturacopy || ( abs(alturaIz - alturaDr)  > 1 ) ) {
//                equi = false;
//            }
//            else {
//                equi = true;
//            }
//        }
//        
//        return true;
//    }
    
    static Nodo* kesimoelementominimo(Nodo* n, const int kesimo) {
        
        Nodo* vn;
        
        if ( n == NULL ) {
            vn = NULL;
        }
        else {
            
            if ( kesimo == n->tam_i ) {
                vn = n;
            }
            else {
                if ( n->tam_i > kesimo ) {
                    vn = kesimoelementominimo(n->iz, kesimo);
                }
                else {// n->tam_i < kesimo
                    vn = kesimoelementominimo(n->dr, kesimo-n->tam_i);
                }
            }
        }
        
        return vn;
    }
    
    static void rango(Nodo* n, const Clave& k1, const Clave& k2, Lista<Clave>& claves) {
        
        if ( n != NULL ) {
            
            if ( n->clave > k1) {
                rango(n->iz, k1, k2, claves);
            }
            
            if ( n->clave >= k1 && n->clave <= k2) {
                claves.ponDr(n->clave);
            }
        
            if ( n->clave < k2 ) {
                rango(n->dr, k1, k2, claves);
            }
        }
    }
};

template <class Clave, class Valor>
ostream& operator<<(ostream& o,const AVL<Clave, Valor>& a){
    a.mostrar(o, 0);
    return o;
}

#endif /* AVL_H_ */
