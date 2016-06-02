#include "../include/Instance.h"
#include "../include/nodes.h"
#include "../include/route.h"
#include "../include/solution.h"
#include <iostream>
#include <vector>
#include <cstdlib>
using namespace std;

Instance *instance = NULL;

//Construtivo Aleatório
Solution construction(){
    
    Solution initialSolution(instance);
	int veiculos = instance->getNumVehicles();

	for(int k=0; k<veiculos; k++){ //abrir rota para todos os veículos 
		Route* route = new Route(instance->getNumNodes()+1);	
		initialSolution.insertRoute(route);
	}
	
	for(int c=1; c<instance->getNumNodes(); c++){
	    int v = rand() % veiculos;
	    int indice =0;
	    while(initialSolution.getRoute(v)->getForward(indice) != -1){ //Busca último elemento adicionado
	        indice = initialSolution.getRoute(v)->getForward(indice);
	    }  
	    initialSolution.getRoute(v)->setForward(indice, c); 
	    initialSolution.getRoute(v)->setBackward(c, indice); 
	    
	    //calculateInitialTime(indice, c, v);  //atualizar tempo de atendimento e atrasos do nodo e da rota.
	}
	    
	return initialSolution;    	
}


int main(int argc, char** argv){
    instance = new Instance(argv[1]); //Parâmetro 1: nome do arquivo de instância para ser lido.
    instance->readFile();
	//instance->createGLPKinstanceFile();
	Solution s = construction();

	return 0;
}

