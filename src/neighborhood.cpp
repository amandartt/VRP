#include "../include/neighborhood.h"

Neighborhood::Neighborhood(Instance* i){
    this->instancia = i;
}

int Neighborhood::rotaComCliente(Solution s, int route){
	for(int i=1; i<this->instancia->getNumNodes(); i++){
		if(s.getRoute(route)->getForward(i) != -1){
			return 1; //pelo menos um cliente está nessa rota.
		}
	}
	return 0;
}

Solution Neighborhood::updateRota1(Solution s, int nodoSelected, int rota1){

	//apaga arestas adjacentes ao nodo da rota 1 e religa.
	int forwardNodoSelected = s.getRoute(rota1)->getForward(nodoSelected); //3
	int backwardNodoSelected = s.getRoute(rota1)->getBackward(nodoSelected); //0
	s.getRoute(rota1)->setForward(nodoSelected, -1);
	s.getRoute(rota1)->setForward(backwardNodoSelected, forwardNodoSelected);	
	s.getRoute(rota1)->setBackward(nodoSelected, -1);
	s.getRoute(rota1)->setBackward(forwardNodoSelected, backwardNodoSelected);
	return s;
}

Solution Neighborhood::updateRota2(Solution s, int arestaI, int rota2, int nodoSelected){

    //apaga arestas selecionadas da rota 2 e insere nodo selecionado entre elas.
	int forwardNodoI = s.getRoute(rota2)->getForward(arestaI); //6
	s.getRoute(rota2)->setForward(arestaI, nodoSelected);
	s.getRoute(rota2)->setForward(nodoSelected, forwardNodoI);	
	s.getRoute(rota2)->setBackward(nodoSelected, arestaI);
	s.getRoute(rota2)->setBackward(forwardNodoI, nodoSelected);
	return s;
}

Solution Neighborhood::interRoutes(Solution s){

	//seleciona duas rotas aleatoriamente, rota 1 com pelo menos um cliente. ** rota 2 != rota 1.
	int rota1 = rand() % this->instancia->getNumVehicles(); //0 a veiculos-1
	while (rotaComCliente(s, rota1) == 0){
		rota1 = rand() % this->instancia->getNumVehicles();	
	}

	int rota2 = rand() % this->instancia->getNumVehicles();
	while (rota2 == rota1){
		rota2 = rand() % this->instancia->getNumVehicles();	
	}
	
	//rota 1 : seleciona 1 nodo aleatoriamente, não pode ser depósito e tem q ser cliente da rota.
	int nodoSelected = s.getRoute(rota1)->getForward(rand() % this->instancia->getNumNodes());
	while (nodoSelected == -1 || nodoSelected == this->instancia->getNumNodes()){
		nodoSelected = s.getRoute(rota1)->getForward(rand() % this->instancia->getNumNodes());
	}

	// rota 2 seleciona uma aresta.
	int arestaI = rand() % this->instancia->getNumNodes();
	int arestaJ = s.getRoute(rota2)->getForward(arestaI);

	while (arestaJ == -1){
        arestaI = rand() % this->instancia->getNumNodes();
	    arestaJ = s.getRoute(rota2)->getForward(arestaI);
	}

	s = updateRota1(s, nodoSelected, rota1);
	s = updateRota2(s, arestaI, rota2, nodoSelected);

	s.recalculateSolutionOnlyRoute(rota2);
	s.recalculateSolutionOnlyRoute(rota1);


	//s.forcaBrutaRecalculaSolution();
	
    return s;
}


int Neighborhood::clientesNaRota(Solution s, int route){
	int hasClient = 0;

	for(int i=0; i<this->instancia->getNumNodes(); i++){
		if(s.getRoute(route)->getForward(i) != -1 && s.getRoute(route)->getForward(i) != this->instancia->getNumNodes()){
			hasClient++;
			if(hasClient >= 3){
				return 1;  // pelo menos dois clientes na rota
			}
		}
	}

	return 0;
}

int Neighborhood::temAdjacencia(Solution s, int aresta2I, int aresta2F, int aresta1I, int aresta1F){
    
    if((aresta1I == 0 && aresta2F == this->instancia->getNumNodes())
      || (aresta2I == 0 &&  aresta1F == this->instancia->getNumNodes())){ //adjacencia entre deposito
        return 1;
    }

    /* verifica adjacência de arcos */
	if(aresta2F != -1 && aresta1F != aresta2I && aresta1I != aresta2I && aresta1I != aresta2F){
	    return 0;
	}
	return 1;
}

Solution Neighborhood::intraRoutes(Solution s){

	/* 
	* seleciona aleatoriamente uma rota com ao menos dois clientes 
	* para haver possibilidade de trocar dois arcos não adjacentes 
	*/

    //e se nenhuma rota possui 3 clientes? TODO: TENTAR GARANTIR ISSO ANTES DE CHAMAR ESSE METODO   
    //funcao: alguma rota tem dois cliente, se não aborta.
	int rota = rand() % this->instancia->getNumVehicles();
	if(clientesNaRota(s, rota) == 0){
	    s.recalculateSolutionOnlyRoute(rota);
        return s;
	}
	
	/*while(clientesNaRota(s, rota) == 0){
		rota = rand() % this->instancia->getNumVehicles();
	}*/

	/*
	* as arestas nao podem ser adjacentes,   I - inicio aresta  F - fim aresta
	ADD :  NAO PODE TER ADJACENCIA DE DEPOSITO TB
	       COM APENAS DOIS CLIENTES, DEPENDENDO DO CASO, SE SELECIONAR UMA ARESTA, NUNCA MAIS CONSEGUIRA SELECIONAR A SEGUNDA SEM ADJACENCIA << LOOP INFINITO! POR ISSO TEM QUE REESCALAR A PRIMEIRA ARESTA, SEMPRE.
	*/

	// primeira aresta
	int aresta1I = rand() % this->instancia->getNumNodes();
	int aresta1F = s.getRoute(rota)->getForward(aresta1I);
    /* arco valido */
	while (aresta1F == -1){
		aresta1I = rand() % this->instancia->getNumNodes();
	   	aresta1F = s.getRoute(rota)->getForward(aresta1I);
	}

	// segunda aresta
	int aresta2I = rand() % this->instancia->getNumNodes();
	int aresta2F = s.getRoute(rota)->getForward(aresta2I);
	/* arco valido */
	while (aresta2F == -1){
		aresta2I = rand() % this->instancia->getNumNodes();
	   	aresta2F = s.getRoute(rota)->getForward(aresta2I);
	}
	
	/* garante que não exista adjacências. */
	while(temAdjacencia(s, aresta2I, aresta2F, aresta1I, aresta1F) == 1){
	    
	    aresta1I = rand() % this->instancia->getNumNodes();
	    aresta1F = s.getRoute(rota)->getForward(aresta1I);
        /* arco válido */
	    while (aresta1F == -1){
		    aresta1I = rand() % this->instancia->getNumNodes();
	       	aresta1F = s.getRoute(rota)->getForward(aresta1I);
	    }

	    // segunda aresta
	    int aresta2I = rand() % this->instancia->getNumNodes();
	    int aresta2F = s.getRoute(rota)->getForward(aresta2I);
	    /* arco válido */
	    while (aresta2F == -1){
		    aresta2I = rand() % this->instancia->getNumNodes();
	       	aresta2F = s.getRoute(rota)->getForward(aresta2I);
	    }
	}

    /* Garantir que inversão de rota não envolva inversão de depósitos */
    
    int currentNodo = 0;
    while(s.getRoute(rota)->getForward(currentNodo) != this->instancia->getNumNodes()){
        
        if(currentNodo == aresta1I){ //primeiro que encontrou é a primeira aresta, ordenação ok.
            break;
        }
        if(currentNodo== aresta2I){ //primeiro que encontrou é a segunda aresta, inverter ordenação.
            int auxAresta1I = aresta1I;
            int auxAresta1F = aresta1F;
            aresta1I = aresta2I;
            aresta1F = aresta2F;
            aresta2I = auxAresta1I;
            aresta2F = auxAresta1F; 
            break;
        }
        currentNodo = s.getRoute(rota)->getForward(currentNodo);    
    }
	//cout << "sem adjacencia rota " << rota << endl;
	//cout << "1I - 1F " << aresta1I << " " << aresta1F << endl;	
    //cout << "2I - 2F " << aresta2I << " " << aresta2F << endl << endl<<endl<<endl;	
	/** atualiza rota  forward**/

	s.getRoute(rota)->setForward(aresta1I, aresta2I); 
	int enteringNode = aresta1I;
	int flippedNode = aresta2I;
	int nextNode = aresta2I;

	while(s.getRoute(rota)->getBackward( nextNode ) != aresta1I){
		// nodo anterior passa a ser o nodo para qual se aponta
		s.getRoute(rota)->setForward(nextNode, s.getRoute(rota)->getBackward(nextNode));
		// guarda o nodo para atualizar na proxima iteração
		flippedNode = s.getRoute(rota)->getBackward(nextNode);
		
		s.getRoute(rota)->setBackward(nextNode, enteringNode);
		enteringNode = nextNode;
		nextNode = flippedNode;
	
	}
	s.getRoute(rota)->setForward(nextNode, aresta2F); 
	s.getRoute(rota)->setBackward(aresta2F, nextNode);
	s.getRoute(rota)->setBackward(nextNode, enteringNode);
	
	s.recalculateSolutionOnlyRoute(rota);
	return s;

}










