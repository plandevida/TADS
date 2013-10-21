
#ifndef AVL_H_
#define AVL_H_

int max(int a,int b){if(a>=b)return a;else return b;}

#include "Excepciones.h"
#include <iostream>
#include <fstream>
#include <cassert>

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
        inserta(c, v, raiz);
        
        mostrar(cout, 0);
        cout << endl;
        
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
        mostrar(o,indent,raiz);
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
        
        int altura = 0;
        bool eq = true;
        
        equilibrado(raiz, altura, eq);
        
        return eq;
    }
    
    /**
     *
     * Operación que determina si el árbol es quilibrado
     * y si el atrubuto altura de cada uno de los nodos
     * está correctamente calculado.
     */
    bool esAVLcorrecto() {
        
        bool equi = false;
        int altura = 1;
        
        AVLcorrecto(raiz, altura, equi);
        
        return equi;
    }
    
    bool esAVLcorrecto(Nodo* nodo) {
        
        bool equi = false;
        int altura = 1;
        
        AVLcorrecto(nodo, altura, equi);
        
        return equi;
    }
    
    void borrar(const Clave& c) {
        
        if ( esta(c) ) {
            
            borraAux(raiz, c);
        }
    }
    
    bool borraAux(Nodo* n, Clave c) {
        
        bool equi = false;
        
        if (n == NULL) {
            equi = false;
        } else if (c == n->clave) {
            
            if ( esVAcio(n->iz) && esVAcio(n->dr) ) {
                delete n;
            }
            else if ( !esVAcio(n->iz) && esVAcio(n->dr) ) {
                
                
            }
            else if ( esVAcio(n->iz) && !esVAcio(n->dr) ) {
                
                
            }
            
        } else if (c < n->clave) {
            borraAux2(n->iz, c);
        } else { // c > a->clave
            borraAux2(n->dr, c);
        }
        
        return equi;
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
    
    static void inserta(const Clave& c, const Valor& v, Nodo*& a){
        if (a == NULL) {
            a = new Nodo(c,v);
        } else if (c == a->clave) {
            a->valor = v;
        } else if (c < a->clave) {
            inserta(c, v, a->iz);
            a->tam_i++;
            reequilibraDer(a);
        } else { // c > a->clave
            inserta(c, v, a->dr);
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
    
    void mostrar(ostream& o,int indent, Nodo* r) const {
        
        if(r!=NULL) {
            
            mostrar(o, indent+2, r->dr);
            
            for(int i=0; i<indent; i++) o << " ";
            
            o << "(" << r->clave << "," << r-> valor << ")" << " --> tam_i = " << r->tam_i << endl;
            
            mostrar(o,indent+2,r->iz);
        }
    }
    
    
    bool equilibrado(Nodo* n, int& altura, bool& equi) {
        
        int alturaIz, alturaDr;
        alturaIz = alturaDr = altura;
        
        if ( n == NULL ) {
            
            altura = 0;
            return false;
        }
        else {
            
            bool resI = equilibrado(n->iz, alturaIz, equi);
            
            bool resD = equilibrado(n->dr, alturaDr, equi);
            
            if ( resI ) {
                alturaIz++;
            }
            if ( resD ) {
                alturaDr++;
            }
            
            if ( altura < alturaIz ) altura = alturaIz;
            else if ( altura < alturaDr ) altura = alturaDr;
            
            if ( abs(alturaIz - alturaDr)  > 1 ) equi = false;
            else equi = true;
        }
        
        return true;
    }
    
    
    bool AVLcorrecto(Nodo* n, int& altura, bool& equi) {
        
        int alturaIz, alturaDr, alturacopy;
        alturaIz = alturaDr = alturacopy = altura;
        
        if ( n == NULL ) {
            
            //altura = 0;
            return false;
        }
        else {
            
            bool resI = AVLcorrecto(n->iz, alturaIz, equi);
            
            bool resD = AVLcorrecto(n->dr, alturaDr, equi);
            
            if ( resI ) {
                alturaIz++;
            }
            if ( resD ) {
                alturaDr++;
            }
            
            if ( alturacopy < alturaIz ) altura = alturacopy = alturaIz;
            if ( alturacopy < alturaDr ) altura = alturacopy = alturaDr;
            
            // Si la altura está mal calculada o la diferencia entre sus hijos es
            // mayor que 1, la condición será falsa.
            if ( n->altura != alturacopy || ( abs(alturaIz - alturaDr)  > 1 ) ) {
                equi = false;
            }
            else {
                equi = true;
            }
        }
        
        return true;
    }
    
};

template <class Clave, class Valor>
ostream& operator<<(ostream& o,const AVL<Clave, Valor>& a){
    a.mostrar(o, 0);
    return o;
}

#endif /* AVL_H_ */
