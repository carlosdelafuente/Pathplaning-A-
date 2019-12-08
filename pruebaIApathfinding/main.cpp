#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "Nodo.h"

using namespace sf;

bool primeraVezGrid = false;
sf::Sprite yo;
Nodo ***todosNodos;

std::vector<Nodo*> listaInterior;
std::vector<Nodo*> listaFrontera;
std::vector<Nodo*> listaNodosPadres;
std::vector<sf::RectangleShape> rectangulos;
sf::RectangleShape nodoSolucion(sf::Vector2f(32.f, 32.f));
std::vector<sf::RectangleShape> soluciones;
bool soyElEnemigo = false;
bool meVoyaDormir = false;
bool salir = false;
int enemigoPos=0;
int inicioX;
int inicioY;
int finalX;
int finalY;
float siguientePosX;
float siguientePosY;
bool primeraVez= true;


int mapa2[7][11] = {
    {0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
    {0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
    {0 , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 0 , 0 , 0 },
    {0 , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 0 , 0 , 0 },
    {0 , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 0 , 0 , 0 },
    {0 , 0 , 0 , 0 , 1 , 1 , 0 , 0 , 0 , 0 , 0 },
    {0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }
};


void calcularCamino(float eneX, float eneY, float jugX, float jugY)
{

        listaInterior.clear();
        listaFrontera.clear();
        listaNodosPadres.clear();
        soluciones.clear();
        soyElEnemigo = false;
        meVoyaDormir = false;
        salir = false;


    if(primeraVezGrid == false){
        todosNodos = new Nodo**[7];
        for(int f=0; f<7; ++f){
          todosNodos[f] = new Nodo*[11];
          for(int c=0; c<11; ++c){
           todosNodos[f][c] = new Nodo(f,c);
          }
        }
        primeraVezGrid = true;
    }



    int posDisJugX = trunc(jugX/32.0);
    int posDisJugY = trunc(jugY/32.0);
    int posDisEnemigX = trunc(eneX/32.0);
    int posDisEnemigY = trunc(eneY/32.0);
    int disEnemigJug=pow((pow((posDisJugX-posDisEnemigX),2)+pow(posDisJugY-posDisEnemigY,2)),0.5);

    //METEMOS EL NODO INICIL COMO PRIMER NODO DE LISTA FRONTERA
    Nodo *nodoEnemigo = todosNodos[posDisEnemigY][posDisEnemigX];
    //DOS NODOS NECESARIOS: EL ACTUAL PARA IR RECRRIENDO Y EL DEL JUEGADOR
    Nodo *nodoActual = todosNodos[posDisEnemigY][posDisEnemigX];

    int posMenor = 0;
    salir = false;
    int posMenorX = 0;
    int posMenorY = 0;


    listaFrontera.clear();
    listaInterior.clear();
    listaFrontera.push_back(todosNodos[posDisEnemigY][posDisEnemigX]);

    while (!listaFrontera.empty() && salir == false){
        nodoActual = listaFrontera.at(0);
        int posMenorX = nodoActual->x;
        int posMenorY = nodoActual->y;
        posMenor = 0;
        for(int i=0; i < listaFrontera.size(); i++){
            if(nodoActual->peso >listaFrontera.at(i)->peso){
                posMenor = i;
                nodoActual = listaFrontera.at(i);
                posMenorX = nodoActual->x;
                posMenorY = nodoActual->y;
            }
        }
        if(nodoActual->x == posDisJugY && nodoActual->y == posDisJugX){ //nos salimos que hemos llegado al final
            salir = true;
        }else{
            nodoActual->estaEnInterior = true;
            listaInterior.push_back(listaFrontera.at(posMenor));
            listaFrontera.erase(listaFrontera.begin()+posMenor);

            //NOS MOVEMOS HACIA ARRIBA
            if(posMenorY-1 >= 0)
            {
                if(mapa2[posMenorX][posMenorY-1]==0) //NORTE o ARRIBA
                {
                   // std::cout << "LA POSICION X ES:   " <<posMenorX<<"  LA POSICION Y ES: "<<posMenorY<<std::endl;
                    Nodo *nodoN = todosNodos[posMenorX][posMenorY-1];
                    if(nodoN->estaEnInterior == false){
                        if(nodoN->estaEnFrontera == false){
                            nodoN->padre = nodoActual;

                            nodoN->g = nodoActual->g+10;
                            nodoN->h = pow((pow((posDisJugX-posMenorX),2)+pow((posDisJugY-(posMenorY-1)),2)),0.5);
                            nodoN->peso = nodoActual->g+10 + nodoN->h;

                            nodoN->estaEnFrontera = true;
                            listaFrontera.push_back(nodoN);
                        }
                        else
                        {
                            if(nodoN->g > nodoActual->g+10)
                            {
                                nodoN->g = nodoActual->g+10;
                                nodoN->peso = nodoN->g + nodoN->h;
                                nodoN->padre = nodoActual;
                            }
                        }
                    }
                }
            }
            //NOS MOVEMO HACIA LA DERECHA
            if(posMenorX+1 < 7)
            {
                if(mapa2[posMenorX+1][posMenorY]==0)
                {

                    Nodo *nodoE = todosNodos[posMenorX+1][posMenorY];
                    if(nodoE->estaEnInterior == false){
                        if(nodoE->estaEnFrontera == false){
                            nodoE->padre = nodoActual;

                            nodoE->g = nodoActual->g+10;
                            nodoE->h = pow((pow((posDisJugX-(posMenorX+1)),2)+pow((posDisJugY-posMenorY),2)),0.5);
                            nodoE->peso = nodoActual->g+10 + nodoE->h;

                            nodoE->estaEnFrontera = true;
                            listaFrontera.push_back(nodoE);

                        }
                        else
                        {
                            if(nodoE->g > nodoActual->g+10)
                            {
                                nodoE->g = nodoActual->g+10;
                                nodoE->peso = nodoActual->g+10 + nodoE->h;
                                nodoE->padre = nodoActual;
                            }

                        }
                    }
                }
            }
            //NOS MOVEMOS HACIA ABAJO
            if(posMenorY+1 < 11)
            {
                if(mapa2[posMenorX][posMenorY+1]==0) // SUR o IZQUIERDA
                {
                    Nodo *nodoS = todosNodos[posMenorX][posMenorY+1];
                    if(nodoS->estaEnInterior == false){
                        if(nodoS->estaEnFrontera == false){
                            nodoS->padre = nodoActual;

                            nodoS->g = nodoActual->g+10;
                            nodoS->h = pow((pow(posDisJugX-posMenorX,2)+pow((posDisJugY-(posMenorY+1)),2)),0.5);
                            nodoS->peso = nodoActual->g+10 + nodoS->h;

                            nodoS->estaEnFrontera = true;
                            listaFrontera.push_back(nodoS);
                        }
                        else
                        {
                            if(nodoS->g > nodoActual->g+10)
                            {
                                nodoS->g = nodoActual->g+10;
                                nodoS->peso = nodoActual->g+10 + nodoS->h;
                                nodoS->padre = nodoActual;
                            }
                        }
                    }
                }
            }
            //NOS MOVEMOS A LA IZQUIERDA
            if(posMenorX-1 >= 0)
            {
                if(mapa2[posMenorX-1][posMenorY]==0) // OESTE o ABAJO
                {
                    Nodo *nodoO = todosNodos[posMenorX-1][posMenorY];
                    if(nodoO->estaEnInterior == false){
                        if(nodoO->estaEnFrontera == false){
                            nodoO->padre = nodoActual;

                            nodoO->g = nodoActual->g+10;
                            nodoO->h = pow((pow((posDisJugX-(posMenorX-1)),2)+pow((posDisJugY-posMenorY),2)),0.5);
                            nodoO->peso = nodoActual->g+10 + nodoO->h;

                            nodoO->estaEnFrontera = true;
                            listaFrontera.push_back(nodoO);
                            //std::cout<<"AÑADO 3"<<std::endl;
                        }
                        else
                        {
                            if(nodoO->g > nodoActual->g+10)
                            {
                                nodoO->g = nodoActual->g+10;
                                nodoO->peso = nodoActual->g+10 + nodoO->h;
                                nodoO->padre = nodoActual;
                            }
                        }
                    }
                }
            }
            if(posMenorX-1 >= 0 && posMenorY-1 >= 0)
            {
                if(mapa2[posMenorX-1][posMenorY-1]==0) // OESTE o ABAJO
                {
                    if(mapa2[posMenorX-1][posMenorY]==0 && mapa2[posMenorX][posMenorY+1]==0) // OESTE o ABAJO
                    {
                        Nodo *nodoNO = todosNodos[posMenorX-1][posMenorY-1];
                        if(nodoNO->estaEnInterior == false){
                            if(nodoNO->estaEnFrontera == false){
                                nodoNO->padre = nodoActual;

                                nodoNO->g = nodoActual->g+14;
                                nodoNO->h = pow((pow((posDisJugX-(posMenorX-1)),2)+pow((posDisJugY-(posMenorY-1)),2)),0.5);
                                nodoNO->peso = nodoActual->g+14 + nodoNO->h;

                                nodoNO->estaEnFrontera = true;
                                listaFrontera.push_back(nodoNO);
                            }
                            else
                            {
                                if(nodoNO->g > nodoActual->g+14)
                                {
                                    nodoNO->g = nodoActual->g+14;
                                    nodoNO->peso = nodoActual->g+14 + nodoNO->h;
                                    nodoNO->padre = nodoActual;
                                }
                            }
                        }
                    }
                }
            }
            if(posMenorX+1 < 7 && posMenorY+1 < 11)
            {
                if(mapa2[posMenorX+1][posMenorY+1]==0) // OESTE o ABAJO
                {
                    if(mapa2[posMenorX+1][posMenorY]==0 && mapa2[posMenorX][posMenorY+1]==0)
                    {
                        Nodo *nodoSE = todosNodos[posMenorX+1][posMenorY+1];
                        if(nodoSE->estaEnInterior == false){
                            if(nodoSE->estaEnFrontera == false){
                                nodoSE->padre = nodoActual;

                                nodoSE->g = nodoActual->g+14;
                                nodoSE->h = pow((pow((posDisJugX-(posMenorX+1)),2)+pow((posDisJugY-(posMenorY+1)),2)),0.5);
                                nodoSE->peso = nodoActual->g+14 + nodoSE->h;

                                nodoSE->estaEnFrontera = true;
                                listaFrontera.push_back(nodoSE);
                            }
                            else
                            {
                                if(nodoSE->g > nodoActual->g+14)
                                {
                                    nodoSE->g = nodoActual->g+14;
                                    nodoSE->peso = nodoActual->g+14 + nodoSE->h;
                                    nodoSE->padre = nodoActual;
                                }
                            }
                        }
                    }
                }
            }
            if(posMenorX-1 >= 0 && posMenorY+1 < 11)
            {
                if(mapa2[posMenorX-1][posMenorY+1]==0) // OESTE o ABAJO
                {
                    if(mapa2[posMenorX-1][posMenorY]==0 && mapa2[posMenorX][posMenorY+1]==0) // OESTE o ABAJO
                    {
                        Nodo *nodoSO = todosNodos[posMenorX-1][posMenorY+1];
                        if(nodoSO->estaEnInterior == false){
                            if(nodoSO->estaEnFrontera == false){
                                nodoSO->padre = nodoActual;

                                nodoSO->g = nodoActual->g+14;
                                nodoSO->h = pow((pow((posDisJugX-(posMenorX-1)),2)+pow((posDisJugY-(posMenorY+1)),2)),0.5);
                                nodoSO->peso = nodoActual->g+14 + nodoSO->h;

                                nodoSO->estaEnFrontera = true;
                                listaFrontera.push_back(nodoSO);
                            }
                            else
                            {
                                if(nodoSO->g > nodoActual->g+14)
                                {
                                    nodoSO->g = nodoActual->g+14;
                                    nodoSO->peso = nodoActual->g+14 + nodoSO->h;
                                    nodoSO->padre = nodoActual;
                                }
                            }
                        }
                    }
                }
            }
            if(posMenorX+1 < 7 && posMenorY-1 >= 0)
            {
                if(mapa2[posMenorX+1][posMenorY-1]==0) // OESTE o ABAJO
                {
                    if(mapa2[posMenorX+1][posMenorY]==0 && mapa2[posMenorX][posMenorY-1]==0) // OESTE o ABAJO
                    {
                        Nodo *nodoNE = todosNodos[posMenorX+1][posMenorY-1];
                        if(nodoNE->estaEnInterior == false){
                            if(nodoNE->estaEnFrontera == false){
                                nodoNE->padre = nodoActual;

                                nodoNE->g = nodoActual->g+14;
                                nodoNE->h = pow((pow((posDisJugX-(posMenorX+1)),2)+pow((posDisJugY-(posMenorY-1)),2)),0.5);
                                nodoNE->peso = nodoActual->g+14 + nodoNE->h;

                                nodoNE->estaEnFrontera = true;
                                listaFrontera.push_back(nodoNE);
                            }
                            else
                            {
                                if(nodoNE->g > nodoActual->g+14)
                                {
                                    nodoNE->g = nodoActual->g+14;
                                    nodoNE->peso = nodoActual->g+14 + nodoNE->h;
                                    nodoNE->padre = nodoActual;
                                }
                            }
                        }
                    }
                }
            }



        }//end else
    }//end while
    listaNodosPadres.clear();
    std::cout<< "TAMAÑO LISTA NODOS PADRE AL PRINCIPIO:  "<< listaNodosPadres.size() <<  std::endl;

    //std::cout<< "posX del jugador:  " << posDisJugX <<" posY del jugador: " <<  posDisJugY<< std::endl;
    Nodo *EmpiezaEnNodoJugador =  todosNodos[posDisJugY][posDisJugX];
    soyElEnemigo = false;




    if(EmpiezaEnNodoJugador != NULL){
        //std::cout<< "SI ENTRA:  " <<  std::endl;
        //soyElEnemigo = true;
        //bucle hasta llegar al padre de los padre, el enemigo
        if((EmpiezaEnNodoJugador->x == nodoEnemigo->x && EmpiezaEnNodoJugador->y == nodoEnemigo->y) || (!EmpiezaEnNodoJugador->padre))
        {
            //std::cout << "NO ENTRO NUNCAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA" << std::endl;
            //meVoyaDormir = true;
        }
        else{
            while( (EmpiezaEnNodoJugador->x != nodoEnemigo->x && EmpiezaEnNodoJugador->y != nodoEnemigo->y) || meVoyaDormir == false)
            {
                if(EmpiezaEnNodoJugador->padre != NULL){
                    listaNodosPadres.push_back(EmpiezaEnNodoJugador);
                    EmpiezaEnNodoJugador = EmpiezaEnNodoJugador->padre;
                    //std::cout<< "TOTAL DE NODOS HASTA EL JUGADOR" <<listaNodosPadres.size() << std::endl;
                }
                else
                {
                    meVoyaDormir = true;
                }
            }
        }
    }


    std::cout<< "TAMAÑO LISTA NODOS PADRE AL FINAL:  "<< listaNodosPadres.size() <<  std::endl;

    for(int i = 0; i < listaNodosPadres.size(); i++)
    {
         //std::cout<< "TOTAL:  " << listaNodosPadres.size() <<  std::endl;
         //std::cout<< "POS X DE UN NODO:  " <<listaNodosPadres.at(i)->x << "POS Y DE UN NODO: " << listaNodosPadres.at(i)->y <<  std::endl;
         nodoSolucion.setFillColor(sf::Color::Magenta);
         nodoSolucion.setPosition(listaNodosPadres.at(i)->y*32.0, listaNodosPadres.at(i)->x*32.0);
         soluciones.push_back(nodoSolucion);
    }

    if(listaNodosPadres.size() > 0)
    {

        inicioX = posDisEnemigY;
        inicioY = posDisEnemigX;

        finalX = listaNodosPadres.at(listaNodosPadres.size()-1)->x;
        finalY = listaNodosPadres.at(listaNodosPadres.size()-1)->y;

        //listaNodosPadres.at(listaNodosPadres.size()-2)->padre = NULL;

        std::cout<< "INICIO:  "<< inicioX<< ","<< inicioY<< "  FINAL:" << finalX<< ","<< finalY<<  std::endl;
        if(finalX > inicioX && finalY == inicioY)
        {
            enemigoPos=1;
            siguientePosY=eneY+32;//abajo
        }
        else if(finalX > inicioX && finalY > inicioY)
        {
            enemigoPos=2;//diagonal derecha bajo
            siguientePosX=eneX+32;//abajo
            siguientePosY=eneY+32;//abajo
        }
        else if(finalX == inicioX && finalY > inicioY)
        {
            enemigoPos=3;//derecha
            siguientePosX=eneX+32;//abajo
        }
        else if(finalX < inicioX && finalY > inicioY)
        {
            enemigoPos=4;//diagonal derecha superior
            siguientePosX=eneX+32;//abajo
            siguientePosY=eneY-32;//abajo
        }
        else if(finalX < inicioX && finalY == inicioY)
        {
            enemigoPos=5;//arriba
            siguientePosY=eneY-32;//abajo
        }
        else if(finalX < inicioX && finalY < inicioY)
        {
            enemigoPos=6;//diagonal izquierda superior
            siguientePosX=eneX-32;//abajo
            siguientePosY=eneY-32;//abajo        }
        else if(finalX == inicioX && finalY < inicioY)
        {
            enemigoPos=7;//izquierda
            siguientePosX=eneX-32;//abajo
        }
        else if(finalX > inicioX && finalY < inicioY)
        {
            enemigoPos=8;//diagonal izquierda bajo
            siguientePosX=eneX-32;//abajo
            siguientePosY=eneY+32;//abajo
        }
        else
            enemigoPos=0; //nada

    }
    else{
        enemigoPos=0;

    }

    std::cout<< "POSICION ENEMIGO SIGUIENTE:  " << enemigoPos <<  std::endl;
    std::cout<< "TAMAÑO ARRAY: :  " << listaNodosPadres.size() <<  std::endl;

    //std::cout<< "INICIO X:  " << inicioX << " INICIO Y: " << inicioY <<  std::endl;
    //std::cout<< "FINAL X:  " << finalX << " FINAL Y: " << finalY <<  std::endl;

    for(int f=0;f<7;f++){
        for(int c=0;c<11;c++){
            todosNodos[f][c]->g = 0;
            todosNodos[f][c]->h = 0;
            todosNodos[f][c]->peso = 0;
            todosNodos[f][c]->padre = NULL;
            todosNodos[f][c]->estaEnInterior = false;
            todosNodos[f][c]->estaEnFrontera = false;
        }
    }

}

void movimientoEnemigo(float eneX, float eneY, float jugX, float jugY)
{
    int movimiento;
    std::cout<< eneY << "-" << siguientePosY << " " << eneX << "-" << siguientePosX << std::endl;
    if(int(eneX) == int(siguientePosX) && int(eneY) == int(siguientePosY))
    {
        std::cout<< "SIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII" << std::endl;
        calcularCamino(eneX, eneY, jugX, jugY);
    }

    //moverse
    switch(enemigoPos)
    {
        case 5: yo.move(sf::Vector2f(0, -4));
            break;
        case 4: yo.move(sf::Vector2f(4, -4));
            break;
        case 3: yo.move(sf::Vector2f(4, 0));
            break;
        case 2: yo.move(sf::Vector2f(4, 4));
            break;
        case 1: yo.move(sf::Vector2f(0, 4));
            break;
        case 8: yo.move(sf::Vector2f(-4, 4));
            break;
        case 7: yo.move(sf::Vector2f(-4, 0));
            break;
        case 6: yo.move(sf::Vector2f(-4, -4));
            break;
    }
}


int main()
{
    // Create the main window
    sf::RenderWindow app(sf::VideoMode(352, 224), "SFML window");
    app.setFramerateLimit(30);

    // Load a sprite to display
    sf::Texture texture;
    if (!texture.loadFromFile("verde.png"))
        return EXIT_FAILURE;

    yo.setTexture(texture);
    yo.setPosition(96.0,96.0);

    sf::Texture texture2;
    if (!texture2.loadFromFile("rojo.png"))
        return EXIT_FAILURE;
    sf::Sprite tu(texture2);
    tu.setPosition(224.0,96.0);//352

    siguientePosX = yo.getPosition().x;
    siguientePosY = yo.getPosition().y;
/*
    calcularCamino(yo.getPosition().x, yo.getPosition().y, tu.getPosition().x, tu.getPosition().y);
*/


    for(int c=0; c<7; ++c){
      for(int f=0; f<11; ++f){
       if(mapa2[c][f]==1){
         sf::RectangleShape rectangulo(sf::Vector2f(32.f, 32.f));
         rectangulo.setFillColor(sf::Color::Cyan);
         rectangulo.setPosition(f*32.0, c*32.0);
         rectangulos.push_back(rectangulo);
       }
      }
    }


	// Start the game loop
    while (app.isOpen())
    {
        // Process events
        sf::Event event;
        while (app.pollEvent(event))
        {
            // Close window : exit
            if (event.type == sf::Event::Closed)
                app.close();
        }
        if(Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            tu.setPosition(tu.getPosition().x+4, tu.getPosition().y);
        }
        if(Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            tu.setPosition(tu.getPosition().x-4, tu.getPosition().y);
        }
        if(Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            tu.setPosition(tu.getPosition().x, tu.getPosition().y-4);
        }
        if(Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            tu.setPosition(tu.getPosition().x, tu.getPosition().y+4);
        }

        movimientoEnemigo(yo.getPosition().x, yo.getPosition().y, tu.getPosition().x, tu.getPosition().y);

        // Clear screen
        app.clear();

        // Draw the sprite
        for(int i=0; i<rectangulos.size(); i++)
            app.draw(rectangulos.at(i));
        for(int i=0; i<soluciones.size(); i++)
            app.draw(soluciones.at(i));
        app.draw(tu);
        app.draw(yo);
        // Update the window
        app.display();
    }

    return EXIT_SUCCESS;
}
