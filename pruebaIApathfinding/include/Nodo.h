#ifndef NODO_H
#define NODO_H
#include <iostream>

class Nodo
{
    public:
        Nodo(int x, int y);
        virtual ~Nodo();

        int peso,g,h,x,y;
        bool estaEnFrontera,estaEnInterior;
        Nodo *padre = NULL;

    protected:

    private:
};

#endif // NODO_H
