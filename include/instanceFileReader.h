#ifndef _INSTANCEFILEREADER_H
#define _INSTANCEFILEREADER_H

#include <vector>
#include <string>
#include <cstdio>
#include <algorithm> 
#include <iostream>  
#include "nodes.h"
using namespace std;

class InstanceFileReader {

private:
	int numNodes; //Número de nós = deposito (id = 0) + clientes (n) = n.
    int numVehicles; //Número de veículos.
    int capacitated; //Capacidade dos veículos.
	vector< vector<double> > cost; //Custo de viagem do nodo i a j. Matriz = (numNodes+1 x numNodes+1), acrescenta depósito virtual. 
	vector< vector<double> > t; //Variável t = custo de i a j + tempo de atendimento i. (i!=NUmNodes+1)
	vector<Nodes> nodes; //Vetor de nós.
	string filename;
	FILE* fileRead;

public:
	InstanceFileReader(const char* filename);
	int getCapacitated();
	void setCapacitated(int n);
	double getDurationTimeNode(int n);
	double getInitialTimeNode(int n);
	double getEndTimeNode(int n);
	int getNumNodes();
	void setNumNodes(int n);
	void readFile();
	void calculateCost();
    void calculateT();
	void createGLPKDataFile();
	
};

#endif
