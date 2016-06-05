#include "../include/Instance.h"
#include "../include/nodes.h"
#include "../include/route.h"
#include "../include/solution.h"
#include "../include/neighborhood.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <time.h>   
#include <math.h> 
using namespace std;

/* Garantimos em uma solução que todos os clientes são atendidos e que não se pode "voltar no tempo". As restrições de janela de tempo e capacidade podem ser desrespeitadas e são contabilizadas na função objetivo com peso 1000. */

Instance *instance = NULL;

/*Simulated annealing, parametros:
	alfa = taxa de decresciment da temperatura, [0,1] 
	s = melhor solucao encontrada
	t0 = temperatura inicial
	maxIterTemp = numero maximo de iterações com a mesma temperatura*/
Solution simulatedAnnealing(double alfa, Solution s, double t0, int maxIterTemp){

	Solution bestSolution(instance);
	bestSolution = s;
	Solution auxSolution(instance);
	double iterTemp = 0;
	int T = t0;
	double x;
	double delta;

	while(T > 0.0001){
		while(iterTemp < maxIterTemp){
			iterTemp += 1;
			//auxSolution = gera vizinho aleatoriamente(s)
			delta = auxSolution.getTotalCost() - s.getTotalCost();
			if(delta < 0){ //se a solução gerada é melhor que a antiga
				s = auxSolution;
				if(s.getTotalCost() < bestSolution.getTotalCost()){ //se a solução gerada é melhor que a melhor já encontrada
					bestSolution = s;
				}
			}else{ //verifica a probabilidade de aceitar a piora
				x = (rand() % 100)/100; //0 a 0,99
				if(x < exp(-delta/T)){ //constante k?
					s = auxSolution;
				}
			}
		}
		T = T*alfa;
		iterTemp = 0;
	}
	return bestSolution;
}

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
	    while(initialSolution.getRoute(v)->getForward(indice) != -1){ //Busca último elemento adicionado, então força sair do depósito.
	        indice = initialSolution.getRoute(v)->getForward(indice);
	    }  
	
	    initialSolution.getRoute(v)->setForward(indice, c); 
	    initialSolution.getRoute(v)->setBackward(c, indice); 
	    initialSolution.calculateTimeServiceAndFaults(indice, c, v);//atualizar tempo de atendimento do nodo, solução, tempo total da rota, infactibilidades e custo (cap. e janela) >> faz coisa demais!
	}
	
	//força voltar para o depósito.
	for(int r=0; r<instance->getNumVehicles(); r++){
	    int indice =0;
	    while(initialSolution.getRoute(r)->getForward(indice) != -1){ //Busca último elemento adicionado
	        indice = initialSolution.getRoute(r)->getForward(indice);
	    }  
		initialSolution.getRoute(r)->setForward(indice, instance->getNumNodes()); 
	    initialSolution.getRoute(r)->setBackward(instance->getNumNodes(), indice); 
	    initialSolution.calculateTimeServiceAndFaults(indice, instance->getNumNodes(), r);
	}
	//Adcionar a função objetivo os custo das infactibilidades de capacidade janela de tempo.
	initialSolution.setTotalCost(initialSolution.getTotalCost() + 1000*initialSolution.getDelayedArrivalCost() + 1000*initialSolution.getOverCapacitated());
	initialSolution.printSolution();   
	return initialSolution;    	
}


int main(int argc, char** argv){

    time_t seconds;
	time(&seconds);
    srand((unsigned int) seconds);
    instance = new Instance(argv[1]); //Parâmetro 1: nome do arquivo de instância para ser lido.
    Neighborhood* nb = new Neighborhood(instance);

    instance->readFile();
	//instance->createGLPKinstanceFile();
	Solution s = construction();
	s = nb->interRoutes(s);
	s.forcaBrutaRecalculaSolution();
	s.printSolution();

	return 0;
}

