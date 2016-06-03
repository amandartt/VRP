#ifndef _SOLUTION_H
#define _SOLUTION_H

#include "route.h"
#include "Instance.h"

using namespace std;

class Solution {
private:
	vector<Route*> routes;
	double totalCost;
	double delayedArrivalCost;
	int overCapacitated;
	Instance* instancia;
	vector<double> initialServiceTime; //vetor de st = tempo de inicio de serviço em cada cliente.

public:
	Solution(Instance* i);
	void insertRoute(Route* newRoute);	
	double getTotalCost();
	void setTotalCost(double t);
	int getOverCapacitated();
	void setOverCapacitated(int c);
	double getDelayedArrivalCost();
    void setDelayedArrivalCost(double t);
	Route* getRoute(int i);
	void calculateTimeServiceAndFaults(int i, int j, int v); //Não acho que faça parte dessa classe, usada apenas para a construtiva.
	void printSolution();
	

}; 

#endif

