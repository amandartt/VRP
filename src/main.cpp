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

/* Mecanismo auto-adaptativo para determinar temperatura inicial
	beta = taxa de aumento da temperatura (1.1)
	alfa = taxa mínima de aceitação de soluções vizinhas (0.9)
	s = solução
	maxIterTemp = max iteração (no artigo usam o mesmo que o numero de clientes)
	t0 = temperatura de partida (2)
*/
double initialTemperature(double beta, double alfa, Solution s, int maxIterTemp, double t0){
	double T = t0; //temperatura corrente.
	Neighborhood* nb = new Neighborhood(instance);
	bool continua = true; //
	int aceitos = 0; //número de vizinhos aceitos na temperatura T
	Solution auxSolution(instance);
	Solution standartSolution(instance);
	standartSolution = s;
	double delta = 0;
	double x;

	while (continua){
		aceitos = 0;
		for(int i=0; i<maxIterTemp; i++){
		    standartSolution = s;
			auxSolution  = nb->interRoutes(s); //gera vizinho inter-rota aleatoriamente
			auxSolution = nb->intraRoutes(auxSolution);
			delta = auxSolution.getTotalCost() - standartSolution.getTotalCost();
			if(delta < 0){
				aceitos++;
			}else{ //verifica a probabilidade de aceitar a piora
				x = (rand() % 100)/100; //0 a 0,99
				if(x < exp(-delta/T)){
					aceitos++;
				}else{
				    s = standartSolution; //retorna a solução anterior;  
				}
			}
		}

		if(aceitos > alfa * maxIterTemp){
			continua = false;
		}else{
			T = beta * T;
		}	
	}
	t0 = T;
	return t0;
}


/*Simulated annealing, parametros:
	alfa = taxa de decresciment da temperatura, [0,1] 
	s = melhor solucao encontrada
	t0 = temperatura inicial
	maxIterTemp = numero maximo de iterações com a mesma temperatura*/
Solution simulatedAnnealing(double alfa, Solution s, double t0, int maxIterTemp){

	Solution bestSolution(instance);
	Solution standartSolution(instance);
	standartSolution = s;
	bestSolution = s;
	Solution currentSolution(instance);
	currentSolution = s;
	Neighborhood* nb = new Neighborhood(instance);
	double iterTemp = 0;
	double T = t0;
	double x;
	double delta;

	while(T > 0.00001){
		while(iterTemp < maxIterTemp){
			iterTemp += 1;
			standartSolution = s;
			currentSolution  = nb->interRoutes(s); //gera vizinho inter-rota aleatoriamente	
			currentSolution = nb->intraRoutes(currentSolution); 				
			delta = currentSolution.getTotalCost() - standartSolution.getTotalCost();
			
			if(delta < 0){ //se a solução gerada é melhor que a antiga
				s = currentSolution;
				if(currentSolution.getTotalCost() < bestSolution.getTotalCost()){ //se a solução gerada é melhor que a melhor já encontrada
					bestSolution = currentSolution;
					cout << "Melhor solução: " << bestSolution.getTotalCost() << endl;
				}
			}else{ //verifica a probabilidade de aceitar a piora
				x = (rand() % 100)/100; //0 a 0,99
				if(x < exp(-delta/T)){ //constante k?
					s = currentSolution;
				}else{
				    s = standartSolution; //retorna a solução anterior;  
				}
			}
		}
		T = T*alfa;
		iterTemp = 0;
	}

	return bestSolution;
}

//construtivo guloso
Solution constructionGuloso(){

    Solution initialSolution(instance);
	int veiculos = instance->getNumVehicles();
	int endNode = 0;
	int forwardNodoI = 0;
	int bestRoute = 0;
	unsigned int costBestRoute = instance->getNumNodes()*10000000;
	int forwardNodoSelected = 0;
	int backwardNodoSelected = 0;
	
	//Inicializa todas as rotas.
	for(int v=0; v<veiculos; v++){
	    initialSolution.getRoute(v)->setForward(0, instance->getNumNodes());
	    initialSolution.getRoute(v)->setBackward(instance->getNumNodes(), 0);
	}
    initialSolution.forcaBrutaRecalculaSolution();
	
	//Para cada cliente, é escolhido a rota onde ele causa menos custo.
	for(int c=1;  c<instance->getNumNodes(); c++){
	    costBestRoute = instance->getNumNodes()*10000000;
	    //Avalia todas as rotas inserindo o cliente, escolhe a que causa menor impacto.
	    for(int v=0; v<veiculos; v++){
	    
	        //Último nodo inserido
	        endNode = initialSolution.getRoute(v)->getBackward(instance->getNumNodes());
	        
	        //insere cliente na rota.    
	        forwardNodoI = initialSolution.getRoute(v)->getForward(endNode);
	        initialSolution.getRoute(v)->setForward(endNode, c);
	        initialSolution.getRoute(v)->setForward(c, forwardNodoI);	
	        initialSolution.getRoute(v)->setBackward(c, endNode);
	        initialSolution.getRoute(v)->setBackward(forwardNodoI, c);
	        
	        //calcula custo.
	        initialSolution.recalculateSolutionOnlyRoute(v);

	        if(initialSolution.getTotalCost() < costBestRoute){
	            bestRoute = v;
	            costBestRoute = initialSolution.getTotalCost();
	        }   
	        
	        //retira nodo da rota
	        forwardNodoSelected = initialSolution.getRoute(v)->getForward(c);
	        backwardNodoSelected = initialSolution.getRoute(v)->getBackward(c);
	        initialSolution.getRoute(v)->setForward(c, -1);
	        initialSolution.getRoute(v)->setForward(backwardNodoSelected, forwardNodoSelected);	
	        initialSolution.getRoute(v)->setBackward(c, -1);
	        initialSolution.getRoute(v)->setBackward(forwardNodoSelected, backwardNodoSelected);
	        initialSolution.recalculateSolutionOnlyRoute(v);
	    }
	    
	    //insere cliente na melhor rota
	    endNode = initialSolution.getRoute(bestRoute)->getBackward(instance->getNumNodes());
        forwardNodoI = initialSolution.getRoute(bestRoute)->getForward(endNode);
        initialSolution.getRoute(bestRoute)->setForward(endNode, c);
        initialSolution.getRoute(bestRoute)->setForward(c, forwardNodoI);	
        initialSolution.getRoute(bestRoute)->setBackward(c, endNode);
        initialSolution.getRoute(bestRoute)->setBackward(forwardNodoI, c);
        
        //calcula custo.
        initialSolution.recalculateSolutionOnlyRoute(bestRoute);
	    
	}
    cout << endl << "----------------- SOLUCAO INICIAL ---------------------------" << endl << endl;
	initialSolution.printSolution();  
	exit(0);
	return initialSolution; 
}

//Construtivo Aleatório
Solution constructionAleatorio(){
    
    Solution initialSolution(instance);
	int veiculos = instance->getNumVehicles();
	
	for(int c=1; c<instance->getNumNodes(); c++){
	    int v = rand() % veiculos;
	    int indice =0;
	    while(initialSolution.getRoute(v)->getForward(indice) != -1){ //Busca último elemento adicionado, então força sair do depósito.
	        indice = initialSolution.getRoute(v)->getForward(indice);
	    }  
	
	    initialSolution.getRoute(v)->setForward(indice, c); 
	    initialSolution.getRoute(v)->setBackward(c, indice); 
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

	initialSolution.forcaBrutaRecalculaSolution();
	//Adcionar a função objetivo os custo das infactibilidades de capacidade janela de tempo.
    cout << endl << "----------------- SOLUCAO INICIAL ---------------------------" << endl << endl;
	initialSolution.printSolution();   
	return initialSolution;    	
}

int main(int argc, char** argv){

    time_t seconds;
	time(&seconds);
    srand((unsigned int) seconds);
    instance = new Instance(argv[1]); //Parâmetro 1: nome do arquivo de instância para ser lido.

    instance->readFile();
	//instance->createGLPKinstanceFile();
	Solution s = constructionGuloso();

	double t0 = initialTemperature(1.2, 0.95, s, instance->getNumNodes(), 2);
	cout << "temperatura inicial: " << t0 << endl;

	s = simulatedAnnealing(0.998, s, t0, instance->getNumNodes()); //double alfa, Solution s, double t0, int maxIterTemp
    s.forcaBrutaRecalculaSolution();
	cout << endl << "----------------- SOLUCAO FINAL ---------------------------" << endl << endl;

	s.printSolution();

	return 0;
}

