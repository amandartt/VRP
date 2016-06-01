#include "../include/instanceFileReader.h"
#include "../include/nodes.h"
#include "../include/route.h"
#include <iostream>
#include <vector>
using namespace std;

InstanceFileReader *data = NULL;

//Calcula o custo entre dois nodos.
double calculateCostNeighbor(int i, int j){

	float p1=0.2, p2=0.2, p3=0.4; //pesos, temos q ver valores ainda! talvez testes ad-hoc

	double distance = data.getCost(i,j);
	double t = data.getInitialServiceTimeNode(j) - (data.getInitialServiceTimeNode(i) + data.getDurationTimeNode(i));
	double v = data.getEndTimeNode(j) - (data.getInitialServiceTimeNode(i) + data.getDurationTimeNode(i) + t);
	double cost = (p1*distance) + (p2*t) + (p3*v);

	return cost;
}

//Retorna o vizinho mais próximo (menor custo) ainda não atendido.
int nearestNeighbor(int v, vector<int>& atendidos){
		
	double lowCost = 9999999999;
	int lowNodo = 0;
	for(int j=0; j<data.getNumNodes; j++){
		if(calculateCostNeighbor(v, j) < lowCost && atendidos[j] == 0){
			lowCost = calculateCostNeighbor(v, j);
			lowNodo = j;
		}
	}
	return lowNodo;
}

void construtor(){

	vector<int> atendidos(data.getNumNodes, 0); //vetor que indica de cliente foi visitado ou não.
	int veiculos = data.getNumVehicles();
	int naoAtendido = 1;

	for(int k=0; k<veiculos; k++){ //abrir rota para todos os veículos 

		route = new Route(data.getNumNodes+1);
		
		//enquanto existir clientes nao atendidos:
			naoAtendido = 1;
			for(int i=1; i<atendidos.size(); i++){ //não pode ir até o depósito
				if (visitados[i] == 0){
					naoAtendido = 0;
					break;
				}
			}
			//enquanto a rota nao fecha
				if(naoAtendido == 0 && k != veiculos-1)//se existem cliente nao atendidos e este não é o ultimo veiculo, entao:
					//busca cliente nao atendido mais proximo, factivel = esta no tempo de janela, capacidade n excede e deve poder voltar ao depósito
					if(route.getForward(0) == -1){ //rota recem aberta
				
					}else{
						//quando for cogitar inserir nodo na rota lembrar de setar tempo inicial de serviço!***************** uso no calculo do custo ele ja indicado
						//setar intenção
						nodoParainserir = nearestNeighbor(int v, atendidos);
						//verifica se é factivel, se for insere, se nao encerra rota e retorna.
					}
			

				}else if(naoAtendido == 0 && k == veiculos -1){ //se nao: atende todos esses clientes mesmo q seja infactivel:

				}
			//
		//

}


int main(int argc, char** argv){
    data = new InstanceFileReader(argv[1]); //Parâmetro 1: nome do arquivo de instância para ser lido.
    data->readFile();
	//data->createGLPKDataFile();

	return 0;
}

