#include "Nodo.h"

Nodo::Nodo(int ejX, int ejY)
{
    peso=0;
    g=0;
    h=0;
    estaEnFrontera = false;
    estaEnInterior = false;
    x = ejX;
    y = ejY;
    padre = NULL;
}


Nodo::~Nodo()
{
    //dtor
}
