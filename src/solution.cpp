#include "../include/solution.h"
#include <cstdlib>

Solution::Solution(Instance* i){
    this->instancia = i;
    this->delayedArrivalCost = 0;
    this->totalCost = 0;
	this->initialServiceTime.resize(this->instancia->getNumNodes()+1, 0);
	this->overCapacitated = 0;
}

int Solution::getOverCapacitated(){
	return this->overCapacitated;
}

void Solution::setOverCapacitated(int c){
	this->overCapacitated = c;
}

double Solution::getDelayedArrivalCost(){
    return this->delayedArrivalCost;
}

void Solution::setDelayedArrivalCost(double t){
    this->delayedArrivalCost = t;
}

void Solution::insertRoute(Route* newRoute){
	routes.push_back(newRoute);
}

double Solution::getTotalCost(){
    return this->totalCost;
}

Route* Solution::getRoute(int i){
    return this->routes[i];
}

void Solution::setTotalCost(double t){
    this->totalCost = t;
}

//atualizar tempo de atendimento do nodo, tempo total da rota e infactibilidades (cap. e janela)
void Solution::calculateTimeServiceAndFaults(int i, int j, int v){

    this->setTotalCost(this->getTotalCost() + this->instancia->getCost(i,j));
	//tempo que o nodo j começa a ser atendido.
	this->initialServiceTime[j] = this->initialServiceTime[i] + this->instancia->getDurationTimeNode(i) + this->instancia->getCost(i,j);

	//se o veiculo chega antes do tempo, o tempo inicial é o mesmo de janela, se chega depois do fim da janela, acrescenta penalizações.
	if(this->initialServiceTime[j] <= this->instancia->getNode(j)->getInitialTime()){
		this->initialServiceTime[j] = this->instancia->getNode(j)->getInitialTime();
	}else if(this->initialServiceTime[j] > this->instancia->getNode(j)->getEndTime()){
		double diffService = this->initialServiceTime[j] - this->instancia->getNode(j)->getEndTime();
		this->routes[v]->setLateCost( routes[v]->getLateCost() + diffService); //add penalizaçao na rota
		this->delayedArrivalCost += diffService; //add penalização de atraso na solução, soma das penalizações de todas as rotas.
	}
	this->routes[v]->setTotalTime(this->initialServiceTime[j]); //atualiza tempo total da rota

	this->routes[v]->setTotalCapacitated( this->routes[v]->getTotalCapacitated() +  this->instancia->getNode(j)->getDemand()); //atualiza capacidade da rota
	if(this->routes[v]->getTotalCapacitated() > this->instancia->getCapacitated()){ //se ultrapassou a capacidade permitida
		int diffCapacitated = this->routes[v]->getTotalCapacitated() - this->instancia->getCapacitated();
		this->routes[v]->setOverCapacitated(diffCapacitated );//add penalização  de capacidade na rota
		if(diffCapacitated > this->instancia->getNode(j)->getDemand()){
		    this->overCapacitated += this->instancia->getNode(j)->getDemand(); //add penalização de excesso de carga na solução,soma das penalizações de todas as rotas.
		}else{
		    this->overCapacitated += diffCapacitated;
		}
	}
}

void Solution::printSolution(){

	for(int r=0; r<this->instancia->getNumVehicles(); r++){
		cout << "Total Time da rota: " << this->routes[r]->getTotalTime() << endl;
		cout << "Atrasos da rota: " << this->routes[r]->getLateCost() << endl;
		cout << "excesso de capacidade da rota: " << this->routes[r]->getOverCapacitated() << endl;
		for(int c=0; c<this->instancia->getNumNodes()+1; c++){
			cout << this->routes[r]->getForward(c) << " " ;
		}
		cout << endl;
		for(int c=0; c<this->instancia->getNumNodes()+1; c++){
			cout << this->routes[r]->getBackward(c) << " " ;
		}
		cout << endl;
		cout << endl;

	}
	cout << "*******************************************************" << endl << endl;
	cout << "Custo da solução: " << this->totalCost << endl;
	cout << "Penalidade atraso: " << this->delayedArrivalCost << endl;
	cout << "Penalidade de excesso de carga: " << this->overCapacitated << endl;

}










