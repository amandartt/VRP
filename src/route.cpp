#include "../include/route.h"

#include <stdio.h>

Route::Route(int n) {
	forward.resize(n, NODE_NOT_IN_ROUTE); //Inicializa com nodos -1, não com 0 pq o depósito é 0.
	backward.resize(n, NODE_NOT_IN_ROUTE);
	this->lateCost = 0;
	this->totalTime = 0;	
}

int Route::getForward(int indice){
    return this->forward[indice];
}

int Route::getBackward(int indice){
    return this->backward[indice];
}

void Route::setForward(int indice, int valor){ //sainte
    this->forward[indice] = valor;
}
void Route::setBackward(int indice, int valor){ //entrante
    this->backward[indice] = valor;
}

double Route::getTotalTime(){
	return this->totalTime;
}

void Route::setTotalTime(double time){
	this->totalTime = time;
}	


