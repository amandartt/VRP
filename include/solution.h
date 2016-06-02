#ifndef _SOLUTION_H
#define _SOLUTION_H

#include "route.h"
#include "Instance.h"

using namespace std;

class Solution {
private:
	vector<Route*> routes;
	double totalTime;
	double delayedArrivalCost;
	Instance* instancia;

public:
	Solution(Instance* i);
	void insertRoute(Route* newRoute);	
	double getTotalTime();
	double getDelayedArrivalCost();
	Route* getRoute(int i);
	void setTotalTime(double t);
	double calculateInitialTime(int i, int j, int r); //calcula tempo (relativo a uma rota) que o serviço começa no nodo j, ou seja: t anterior + t de serviço de i + t de custo de i a j
	void atualizaTime(int i, int j, int r); //atualiza tempos de inicio do nodo, tempo total da rota e as infrações.
	

}; 

#endif

