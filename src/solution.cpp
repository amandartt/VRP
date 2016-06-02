#include "../include/solution.h"
#include <cstdlib>

Solution::Solution(Instance* i){
    this->instancia = i;
    this->delayedArrivalCost = 0;
    this->totalTime = 0;
}

void Solution::insertRoute(Route* newRoute){
	routes.push_back(newRoute);
	this->totalTime += newRoute->getTotalTime();
}

double Solution::getTotalTime(){
    return this->totalTime;
}

double Solution::getDelayedArrivalCost(){
    return this->delayedArrivalCost;
}

Route* Solution::getRoute(int i){
    return this->routes[i];
}

void Solution::setTotalTime(double t){
    this->totalTime = t;
}

double Solution::calculateInitialTime(int i, int j, int r){ //calcula tempo q começa o serviço em j.
    return this->getRoute(r)->getTotalTime() + this->instancia->getInitialServiceTimeNode(j) + this->instancia->getCost(i,j); 
}

void Solution::atualizaTime(int i, int j, int r){
    double timeServiceNode = calculateInitialTime(i,j,r);
    this->instancia->getNode(j).setInitialServiceTime(timeServiceNode); //atualiza tempo inicial de serviço no nodo j.
    //atualizar tempo total da rota.
    //atualizar numero de infrações (capacidade e tempo de janela)
    //terminar serviço antes do tempo de janela
    //calcular quando chega antes ds tempo de janela
}


