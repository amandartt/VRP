#include "../include/neighborhood.h"

Neighborhood::Neighborhood(Instance* i){
    this->instancia = i;
}

int Neighborhood::rotaComCliente(Solution s, int route){

	for(int i=1; i<this->instancia->getNumNodes(); i++){
		if(s.getRoute(route).getForward(i) != -1){
			return 1; //pelo menos um cliente está nessa rota.
		}
	}
	return 0;

}

Solution Neighborhood::interRoutes(Solution s){
	
	srand (time(NULL));

	//seleciona duas rotas aleatoriamente, rota 1 com pelo menos um cliente. ** rota 2 != rota 1.
	int rota1 = rand() % this->instancia->getNumVehicles(); //0 a veiculos-1
	while (rotaComCLiente(s, rota1) == 0){
		rota1 = rand() % this->instancia->getNumVehicles();	
	}

	int rota2 = rand() % this->instancia->getNumVehicles();
	while (rota2 == rota1){
		rota2 = rand() % this->instancia->getNumVehicles();	
	}
	
	//rota 1 : seleciona 1 nodo aleatoriamente, não pode ser depósito e tem q ser cliente da rota.
	int nodoSelected = this->instancia->getRoute(rota1)->getForward(rand() % this->instancia->getNumNodes());
	while (nodoSelected == -1 || nodoSelected == 0 || nodoSelected == this->instancia->getNumNodes()){
		nodoSelected = this->instancia->getRoute(rota1)->getForward(rand() % this->instancia->getNumNodes());
	}

	// rota 2 seleciona uma aresta.

	//rota 1:
	//apaga arestas adjacentes ao nodo da rota 1
	//religa com uma aresta

	//rota 2:
	//apaga aresta
	//insere nodo da rota 1

	//recalcula tudo ~~~~tensoo

	//retorna a solução

}
