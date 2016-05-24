set N;
set C;
set V;

param a{i in N}, integer; #Início da janela do cliente i.
param b{i in N}, integer; #Fim da janela do cliente i.
param c{i in N, j in N}, integer; #Custo da viajem do cliente i ao cliente j.
param t{i in N, j in N}, integer; #Soma do custo de i a j e o tempo de atendimento do cliente i.
param d{i in C}, integer; #demanda do cliente i.
param q, integer; #capacidade dos veículos.
param m, integer; #número máximo da veículos.
param n := card(C); #número de clientes, n=|C|.
param G, integer;

var x{i in N, j in N, k in V: i!=j and i!=n+1 and j!=0}, binary; #Binária
var s{i in N, k in V}, integer; 

minimize obj: sum{k in V, i in N, j in N: i!=j and i!=n+1 and j!=0} c[i,j] * x[i,j,k];

#Cada cliente deve ser atendido apenas uma vez.
s.t. c1{i in C}: 
    sum{k in V, j in N: j!=0 and i!=j} x[i,j,k] = 1;
    
# O veículo não deve exceder a sua capacidade.    
s.t. c2{k in V}: 
    sum{i in C, j in N: i!=j and j!=0} d[i] * x[i,j,k] <= q;

#Todos os veículos partem do depósito.
s.t. c3{k in V}: 
    sum{j in N: j!=0} x[0,j,k] = 1;
    
# O veículo chega em um cliente e parte dele.    
s.t. c4{h in C, k in V}: 
    sum{i in N: i!=n+1 and  h!=i} x[i,h,k] - sum{j in N: j!=0 and  h!=j} x[h,j,k] = 0;
    
#Todos os veículos chegam no depósito.
s.t. c5{k in V}: 
    sum{i in N: i!=n+1} x[i,n+1,k] = 1;

#Para não viajar no tempo.  
s.t. c6{i in N, j in N, k in V: i!=j and i!=n+1 and j!=0}: 
    s[i,k] + t[i,j] - G * (1-x[i,j,k]) <= s[j,k];
    
#Atender a janela de tempo. ok
s.t. c7{i in N, k in V}: 
    a[i] <= s[i,k] <= b[i]; 

end;

