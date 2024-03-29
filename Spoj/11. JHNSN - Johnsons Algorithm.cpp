/*
	Nombre: Junior Miguel Lara Torres
	Carnet: 1710303
	Nick: jmlt_undercode
	Problema: SPOJ 11. JHNSN - Johnsons Algorithm
	
	** Idea General para el problema: **
	
	El problema se solventa aplicando el Algoritmo de Jhonson para determinar caminos de 
	costo minimo para todo par de nodos en el grafo. Dicho grafo pro definicion del
	Algoritmo admite costos/pesos netivos para los arcos. A su vez permite de terminar
	si un grafo posee ciclos de costo negativos.

	Dicho algoritmo usa inicialmente una estrategia de agregar un "nodo fantasma" tal que 
	contecte con todos los nodos grafo, dichos arcos son de costo/peso 0. Luego se aplica
	el algoritmo de Bellman-Ford desde ese nodo fantansma, dentro de este se calcula el 
	vector "H" de Jhonson que contiene la distancia minima desde el nodo fantasma a todos
	los nodos. Si en el algoritmo de Bellman Ford para la iteracion n+1(camino de 
	longitud n+1) logramos relajar un arco entonces existe ciclos de costo negativo, de 
	lo contrario el grafo no lo posee. 
	
	Finalmente, limpiamos el grafo de los arcos fantasmas agregos. Para la implementacion
	actual de la libreria mostrada mas abajo no es necesario limpiar dichos arcos, pues
	todo esta 1 indexado por tanto la posicion 0 del arrecho de listas de adyacencia esta 
	disponible para agregar estos arcos fantasmas sin que estos afecten al grafo original.
	Ya para concluir, es aplicar para todos los nodos del grafo(sin incluir el fantasma)
	el algoritmo de Dijsktra y asi calculos el camino de costo minimo para todo par de 
	nodos.

	** Actualizacion de Libreria/Class: **
	1. Inclusion del Algoritmo/Funcion "Jhonson" para determinar caminos de costo minino 
	para todo par de nodos en el grafo.
	2. Modificacion menor en Algoritmo/Funcion "Bellman-Ford", permite modificar al vector
	"H" usado en algoritmo Jhonson.
	3. Modificacion moderada en Algoritmo/Funcion "Disjktra" para el calculo de caminos
	de costo minimo desde una raiz a todos los demas nodos del grafo usando el vector H
	de Jhonson.
	4. Inclusion de funcion "print_mll" para mostrar matrices con entradas LONG LONG y en caso
	de encontrar INF como entrada se sustituye por "#".

	
*/
#include <bits/stdc++.h>

using namespace std;
/*************************************************************************************************/
#define pb push_back
#define FOR1(i, N) for(int i = 1; i-1 < N; i++)
#define FOR0(i, N) for(int i = 0; i < N; i++)
typedef vector<int> vi;
typedef vector<vector<int> > vvi;
typedef pair<int, int> pii;
typedef vector<pair<int, int>> vpii;
typedef pair<long long int, int> pllii;
typedef vector<pair<long long int, int>> vpllii;
typedef vector<long long> vll;
typedef vector<vector<long long>> vvll;
typedef vector<vector<bool>> vvb;
typedef vector<bool> vb;
typedef long long ll;
typedef long long int lli;
const lli INF = 1000000000000LL;	// 1e12

// Class/Struct NODE para informacion de un nodo en grafos. Implementada por Junior Lara.
class NODE{
public:
	int n;		// Numero del nodo.
	int visited;	// Identificador de visitado.
	lli min_cost;	// Costo del nodo.
	/*
	int ex_grade;	// Numero de arcos que salen desde el nodo.
	int in_grade;	// Numero de arcos que entran al nodo.
	int n_ady;	// Numero de nodos adyacentes a n.
	string color;	// Color del nodo.
	int range;	// Rango para busqueda BFS/DFS.
	int father;	// Nodo padre del nodo n;
	int level;	// Para saber el nivel de un nodo en la busqueda BFS\DFS.
	*/
	// Constructor DEFAULT.
	NODE(){
		n = 0; visited = 0; min_cost = INF;
		//ex_grade = 0; in_grade = 0;
		//n_ady = 0;  color = ""; range = 0; father = 0; level = 0;
	}
	// Constructor basico.
	NODE(int _id, int _vis) : n(_id), visited(_vis) {
		min_cost = INF; 
		//ex_grade = 0; in_grade = 0;
		//n_ady = 0;  color = ""; range = 0; father = 0; level = 0;
	}
	/*// En caso de necesitar Num nodo, isVisitado y color del nodo.
	NODE(int _id, int _vis, string _color) : n(_id), visited(_vis), color(_color) {
		n_ady = 0; min_cost = 0; range = 0; father = 0; level = 0;
	}
	// En caso de necesitar Num nodo, isVisitado y costo del nodo.
	NODE(int _id, int _vis, int _cost) : n(_id), visited(_vis), min_cost(_cost) {
		n_ady = 0; color = ""; range = 0; father = 0; level = 0;
	}*/

	// Redefinicion de operador MENOR QUE y MAYOR QUE para que clase NODE puede ser usar en
	// priority queue.
	bool operator>(const NODE &otherNode) const {
		return min_cost < otherNode.min_cost;
	}
	bool operator<(const NODE &otherNode) const {
		return min_cost > otherNode.min_cost;
	}
};

class ARC{
public:
	int n, ady;	// Puntas del arco.
	lli weight;	// Peso del arco.
	ARC() : n(-1), ady(-1), weight(-1){}
	ARC(int _n, int _ady, lli _w) : n(_n), ady(_ady), weight(_w){}
};

typedef vector<NODE> vn;
typedef vector<vector<NODE>> vvn;
typedef vector<ARC> va;
typedef vector<vector<ARC>> vva;


// Fin de la Class//Struct NODE.
/*************************************************************************************************/

// Inicializacion de variables y estructuras de datos.

const int MaxN = 105;		// Numero maximo de nodos.(Monumentos)
int T;				// Numero de casos de prueba.(Grafos)
int N;				// Numero de nodos.
int M; 				// Numero de arcos.
int A, B;			// Extremos de un arco.
lli W;				// Peso de un arco.

/*************************************************************************************************/
// Libreria Graph implementada por Junior Lara.

class Graph{
	int n_nodes;		// Numero de nodos en el grafo.
	int type_graph;		// Tipo de grafo: 1 = Lista Adyacencia y 2 = Matriz de Adyacencia.
	vva lists;		// Estructura para la lista de adyacencia.
	vvi Matriz;		// Estructura para la matriz de adyacencia.

public:
	vn Nodes;		// Vector para guardar cada nodo de class NODE.
	/*
	vi parent_tracker;	// Vector de nodos para tener un track de los nodos padres de cada nodo.
	vi child_tracker;	// Vector de nodos o contador de hijos de cada nodo.
	vi low;			// Vector para marcar el low en algoritmo Tarjan.
	vi disc;		// Vector para marcar el discovery en algoritmo Tarjan.
	vi opened;		// Vector para marcar si un nodo esta abierto en llamado BFS/DFS.
	vvi container_CC;	// Contenedor de Componentes Conexas.
	int c_time, cnt_CC;	// Inicializador y contador para el algoritmo Tarjan.
	stack<int> Pila;	// Stack usada por el algoritmo Tarjan.
	vpii bridges; 		// Vector de arcos que clasifican como puentes.
	vb articulation_points;	// Vector de nodos que clasifican como puntos de articulacion.
	set<NODE, less<NODE>> RB_tree;// Estructura set que simula un arbol rojo negro para Dijkstra.
	*/
	priority_queue<pllii, vpllii, greater<pllii>> P_Q;// Priotiry Queue usada en Dijsktra.
	vvll dist;		// Vector de distancias para el algoritmo Floyd-Warshall
	vll h;			// Vector h usado en algoritmo Jonhson.

	// Constructor principal del grafo. Se espeficica el tamaÑo y el tipo de grafo,
	// 1 para el caso de usar listas de adyacencias, 2 para el caso de matrices.
	Graph(int type, int size) : type_graph(type), n_nodes(size) {
		assert(type > 0 && type < 3);
		/*
		vi newFatherT(size+1, 0); parent_tracker = newFatherT;
		vi newChildT(size+1, 0); child_tracker = newChildT;
		vi newLow(size+1, 0); low = newLow;
		vi newDisc(size+1, 0); disc = newDisc;
		vi newOpened(size+1, 0); opened = newOpened;
		vvi newContainer(size+1); container_CC = newContainer;
		c_time = 0; cnt_CC = 0;
		vb newArtPoint(size+1, false); articulation_points = newArtPoint;
		*/
		vvll newDist(size+1, vll(size+1 , INF)); dist = newDist;
		vll newh(size+1, INF); h = newh;

		if (type == 1) { 				// Listas de Adyacencias.
			vva newLists(size+1); lists = newLists;
			vn newNodes(size+1); Nodes = newNodes;

		} else if (type == 2) { 			// Matriz de Adyacencias.
			vvi newMatriz(size+1, vi (size+1));
			Matriz = newMatriz;

		} else {					// Error de tipo.
			printf("%s", "Error: Tipo de grafo especificado (Argument).");
		}
	}

	// Funcion que permite agregar un nodo al grafo, en caso de grafo tipo 1 entonces un pushback
	// de una vector vacio simulando una nueva posicion disponible en la estructura del grafo.
	// Para el caso grafo tipo 2 entonces es recrear una nueva matriz con 1 unidad fila/columna mas
	// y con el contenido de la antigua matriz.
	void add_node() {
		n_nodes++;
		if (type_graph == 1) {
			lists.pb(va ());
		} else {
			vvi aux(n_nodes+1, vi (n_nodes+1));
			for (int i = 1; i < n_nodes; i++ ) {
				for (int j = 1; j < n_nodes; j++ ) aux[i][j] = Matriz[i][j];
			}
			Matriz = aux;
		}
	}

	// Funcion que permite agregar un arco con peso entre dos nodos existentes en el grafo. 
	// En caso de grafo tipo 1 entonces realizamos el pushback del elemento a su respectivo
	// vector de adyacencia. En caso de grafo tipo 2 entonces se marca en la posicion dada en 1.
	// Adicionalmente, se procede agregar el nodo con formado NODE.
	void add_arc(int n1, int n2, lli w) {
		if ( type_graph == 1 ) {
			lists[n1].pb(ARC(n1, n2, w));
			if(Nodes[n1].n == 0 && n1 != 0) Nodes[n1] = NODE (n1, 0);
			//Nodes[n1].ex_grade++;
			if(Nodes[n2].n == 0 && n2 != 0) Nodes[n2] = NODE (n2, 0);
			//Nodes[n2].in_grade++;
		} else {
			Matriz[n1][n2] = 1;
		}
	}

	// Funcion que permite extraer para un nodo dado x como argumento su vector de elementos
	// adyacentes, en caso de listas se expresa por tipo de grafo 1, en caso de matrices entonces
	// por tipo de grafo 2 extraemos los elementos marcado con 1 en la fila respectiva de la matriz.
	vi get_ady(int x) {
		if (type_graph == 1) {
			vi ady_v;
			for(auto it : lists[x]) ady_v.pb(it.ady);
			return ady_v;
		} else {
			vi aux;
			for (int k = 1; k-1 < n_nodes; k++) if (Matriz[x][k] == 1) aux.pb(k);
			return aux;
		}
	}

	// Funcion que permite revertir los arcos del grafo.
	void arc_reverse(){
		vva rev_list(n_nodes+1);
		for(int k = 1; k-1 < n_nodes; k++){
			for(auto it : lists[k]) rev_list[it.ady].pb(ARC(it.ady, it.n, it.weight));
		}
		lists = rev_list;
	}

	// Funcion que retorna el arbol DFS para un elemento root. Este "arbol dfs" consta
	// de un vector de enteros cuyo orden expresa el orden en que fueron procesador en el 
	// algoritmo DFS.
	/*vn DFS_tree(NODE root) {
		// Inicializacion, push, marcado de variables y nodos.
		//int visited[n_nodes+1];
		stack<NODE> S;

		root.visited = 1;
		S.push(root);

		int cnt = 0;
		vn aux(n_nodes+1);

		// Calculo del arbol DFS mediante la estructura Pila(Stack).
		while ( !S.empty() ) {
			NODE v = S.top();
			S.pop();
			aux[cnt++] = v;

			if(type_graph == 1) {
				for ( auto it : lists[v.n]) {
					if ( Nodes[it].visited != 1 ) {
						Nodes[it].visited = 1; S.push(Nodes[it]);
					}
				}
			} else {
				//Por implementar
			}
		}

		// Se procede a extrar el arbol DFS respectivo del vector aux, esto pautado
		// por la variable cnt.
		vn tree(cnt);
		for(int k = 0; k < cnt; k++) tree[k] = aux[k];

		return tree;
	}*/

	// Funcion que retorna el arbol BFS para un elemento root. Este "arbol bfs" consta
	// de un vector de enteros cuyo orden expresa el orden en que fueron procesador en el 
	// algoritmo BFS.
	/*vn BFS_tree(NODE root) {
		// Inicializacion, push, marcado de variables y nodos.
		//int visited[n_nodes+1];
		queue<NODE> Q;

		root.visited = 1;
		Q.push(root);

		int cnt = 0;
		vn aux(n_nodes+1);

		// Calculo del arbol BFS mediante la estructura Cola(Queue).
		while ( !Q.empty() ) {
			NODE v = Q.front();
			Q.pop();
			aux[cnt++] = v;

			if(type_graph == 1) {
				for ( auto it : lists[v.n]) {
					if ( Nodes[it].visited != 1 ) {
						Nodes[it].visited = 1; Q.push(Nodes[it]);
					}
				}
			} else {
				//Por implementar
			}
		}

		// Se procede a extrar el arbol BFS respectivo del vector aux, esto pautado
		// por la variable cnt.
		vn tree(cnt);
		for(int k = 0; k < cnt; k++) tree[k] = aux[k];

		return tree;
	}*/

	// Funcion que permite calcular el camino entre dos nodos dados mediante el uso de
	// metodo/algoritmo DFS, esto por implementacion iterativa y tracker de padres.
	// Se retorna true en caso de encontrar el camino y antes de esto se muestra por
	// pantalla, de lo contrario se retorna false.
	/*bool path_between(int i, int f) {
		if( i == f ) {
			print_vi({i}); 
			return true;
		}

		stack<NODE> S;
		Nodes[i].visited = 1;
		//Nodes[i].level = 1;
		S.push(Nodes[i]);

		bool finded = false;
		while( !S.empty() ){
			NODE v = S.top();
			S.pop();

			for( auto it : get_ady(v.n)){
				if( Nodes[it].visited != 1 ){
					//Nodes[it].level = v.level + 1;
					//Nodes[it].father = v.n;
					if( it == f ){
						finded = true; 
						break;
					}
					Nodes[it].visited = 1;
				}
			}
			if(finded) break;
		}
		return false;

		if(finded){
			int sz = Nodes[f].level, curr = f;
			vi path(sz);
			for(int cnt = sz-1; cnt > -1; cnt--){
				path[cnt] = f;
				curr = Nodes[curr].father;
			}
			print_vi(path); return true;
		} else {
			return false;
		}
	}*/

	// Funcion que realiza un BFS para una seria de raices "LIMITADO A" un cierto rango de amplitud 
	// de busqueda, es decir se tiene un nodo ROOT con rango DIST por tanto todo nodo a distancia
	// <= DIST del ROOT sera procesado en el BFS. Retorna true en caso de recorrer todos los nodos y
	// que en cada nodo fue visitado al menos 1 vez, en caso de haberse topado con un nodo visitado
	// se llega un conteo de nodos visitados mas de una vez (crashes).
	/*bool BFS_lim_to(int root, int dist) {
		queue<NODE> C;
		Nodes[root].visited = 1;
		Nodes[root].range = dist;
		C.push(Nodes[root]);

		int cnt_vis = 1;
		int crashes = 0;

		while(!C.empty()){
			NODE v = C.front();
			C.pop();

			if( v.range > 0 ){
				for(auto it : lists[v.n]){
					if ( Nodes[it].visited != 1) {
						cnt_vis++;
						Nodes[it].visited = 1;
						Nodes[it].range = v.range - 1;
						C.push(Nodes[it]);
					} else {
						crashes++;
					}
				}
			}
		}
		
		// Sino se visitaron todos los nodos, entonces no cumple la condicion.
		if(cnt_vis < n_nodes) return false;

		return true;
	}*/

	// Funcion para determinar si un grafo es bipartito o no. Esto se realiza mediante la
	// coloracion del grafo con 2 colores con recorrido BFS. Si es posible colorear el grafo 
	// tal que dos nodos adyacentes tengan dos colores distllos, en caso afirmativo es bipartito. 
	// Modificacion particular del problema: Se realizan cambios para determinar los arcos
	// posibles que pueden agregarse talque sea un grafo bipartito.
	/*int isBipartito(int n){
		// Inicializacion de variables, datos y push's.
		queue<NODE> C;
		Nodes[n].visited = 1;
		Nodes[n].color = "Blanco";
		C.push(Nodes[n]);
		int cnt_n = 0;

		// Inicio del recorrido BFS.
		while( !C.empty() ){
			NODE v = C.front();
			C.pop();

			for(auto it : lists[v.n]){
				if( Nodes[it].visited != 1 ) {
					Nodes[it].visited = 1;
					Nodes[it].color = (v.color == "Blanco") ? "Negro" : "Blanco";
					if (Nodes[it].color == "Negro") cnt_n++;
					C.push(Nodes[it]);
				}

			}

		}
		
		// Conteo de arcos posibles para satisfacer la condificion de Biparticion.
		int adds_arcs = 0;
		for(int cnt = 1; cnt-1 < N; cnt++){
			if( Nodes[cnt].color == "Blanco") {
				adds_arcs += (int)(cnt_n - Nodes[cnt].n_ady);
			}
		}

		return adds_arcs;
	}*/

	// Funcion que calcula el camino "optimo" entre un par de nodos, se calcula el camino si existe
	// un camino entre ellos. Mediante el uso de BFS se permite llevar un tracker de padres de cada
	// nodo procesado pro el BFS, esto permite determinar el camino mas corto entre un root y un
	// destino.
	// Modificacion particular del problema: Mediante el uso de un grafo auxiliar dado por argumento
	// se procede con el recorrido BFS para hallar dicho camino pero con la condicion de ignorar
	// los nodos del grafo auxilizar.
	// Se requiere la funcion "setUp_arcs".
	/*bool optimal_path(int root, int dest, Graph G_a){
		// Caso base rot = dest.
		if( root == dest ) {
			cout << 1 << endl; print_vi({root});
			return true;
		}
		// Inicializacion de variables, datos y push's.
		bool finded = false;

		queue<NODE> C;
		Nodes[root].visited = 1;
		Nodes[root].level = 1;
		C.push(Nodes[root]);

		// Inicio del recorrido BFS.
		while( !C.empty() ){
			NODE v = C.front();
			C.pop();

			// Lista de elementos adyacentes del Spiders Graph.
			for( auto it : lists[v.n] ){ 
				// Si el nodo en el grafo de primos ya fue visitado, es porque
				// se agregaron todos sus arañas adyacentes y no es necesario 
				// procesarlo de nuevo.
				if( G_a.Nodes[it].visited == 1 ) continue;
				G_a.Nodes[it].visited = 1;
				
				// Lista de elementos adyacentes del Primes Graph.
				for( auto it_a : G_a.get_ady(it) ){
					if( Nodes[it_a].visited != 1 && spider_legs[it_a] > 1) {
						Nodes[it_a].visited = 1;
						Nodes[it_a].level = v.level + 1;
						Nodes[it_a].father = v.n;

						if( it_a == dest ) {
							finded = true; break;
						}

						C.push(Nodes[it_a]);
					}
				}
				if(finded) break;
			}
			if(finded) break;
		}
		// Mostramos por pantalla el camino.
		if(finded){
			int sz = Nodes[dest].level, curr = dest;
			vi path(sz);
			for(int cnt = sz-1; cnt > -1; cnt--){
				path[cnt] = curr;
				curr = Nodes[curr].father;
			}
			cout << sz << endl;
			print_vi(path); return true;
		} else {
			return false;
		}
	}*/

	// Funcion que arma los arcos para las arañas, se usa un grafo auxiliar con nodos
	// iguales a los numeros primos encontramos durante el proceso Sieve of Erastothenes.
	// Estacion es requerida para el funcionamiento de "optimal_path".
	/*Graph setUp_arcs() {
		// Aplicamos Sieve of Eratosthenes para determinar los numeros primos.
		for(int div = 2; div+1 < MaxN; div++) {
			// Si un elemento divisor es = 1 es porque no fue marcando anteriormente
			// como multiplo de un primo, por tanto este elemento es primo.
			if(legs_divisors[div] == 1){
				legs_divisors[div] = div;
				for(int legs = div*div; legs+1 < MaxN;  legs+=div){
					if(legs_divisors[legs] == 1) legs_divisors[legs] = div;
				}
				
			}
		}
		// Se arman los arcos entre las arañas que cumplen con tener un divisor comun mayor a 1.
		Graph G_a(1, MaxN);		// Grafo auxiliar de primos.
		int div, legs, last_div;
		
		for(int spider = 1; spider-1 < N; spider++){
			last_div = 1; div = 0;
			
			// Como en Sieve of Eratosthenes se setearon en cada posicion del vector
			// legs_divisors un numero primo que divide al indice del vector y que a su
			// vez este indice representa el numero de patas. Entonces, al dividir el
			// primo por el indice obtenemos otra posicion del vector que contiene otro
			// numero primo, por tanto seteamos los arcos para cada spider con su primo
			// divisor. 
			legs = spider_legs[spider];
			while( legs > 1 ) {
				div = legs_divisors[legs];
				if( div != last_div ) {
					add_arc(spider, div);
					G_a.add_arc(div, spider);
				}
				legs /= div;
				last_div = div;
			}
		}
		return G_a;
	}*/

	// Funcion que simula el primer DFS que usa el algoritmo de Kosaraju.
	// Se encarga de obtener la stack de nodos.
	/*void DFS_V1(stack<NODE> &S, NODE root){
		for(auto it : lists[root.n]){
			if(!Nodes[it].visited){
				Nodes[it].visited = 1;
				DFS_V1(S, Nodes[it]);
			}
		}
		S.push(root);
	}*/

	// Funcion que simula el segundo DFS que usa el algoritmo de Kosaraju.
	// Se encarga de detectar las C.F.C.
	/*void DFS_V2(vi &vis, NODE root){
		for(auto it : lists[root.n]){
			if(!vis[it]){
				vis[it] = 1;
				DFS_V2(vis, Nodes[it]);
			}
		}
	}*/

	// Funcion que permite calcular componentes fuertemente conexas para un grafo dirigido.
	// Tiene como implementacion base el algoritmo de Kosaraju.
	/*void kosaraju(){
		// Primer DFS para llenar la stack con el recorrido del arbol DFS respectivo.
		stack<NODE> S;
		for(int k = 1; k-1 < n_nodes; k++){
			if(!Nodes[k].visited){
				Nodes[k].visited = 1; DFS_V1(S, Nodes[k]);
			}
		}

		// Segundo DFS para detectar las componentes fuertemente conexas.
		vi visited_k(n_nodes+1, 0);
		arc_reverse();
		while(!S.empty()) {
			NODE v = S.top();
			S.pop();
			if(!visited_k[v.n]) {
				SCC_cnt++;
				visited_k[v.n] = 1;
				DFS_V2(visited_k, v);
			}
		}
		arc_reverse();
	}*/

	// Funcion que aplica Algoritmo de Kahn para orden topologicos. Su uso se basa el tener un
	// grafo con 1 unico nodo raiz cuyo grado interior sea 0, esto es seteando un "nodo fantasma"
	// que conecte con todos los demas nodo que originalmente tengan grado interior en 0.
	// Esta mejora permite realizar una unica llamada al DFS/BFS.
	/*vi kahn(){
		vi order_top;
		int del_arcs = 0;

		// Se usa un Min Heap como cola de prioridad, dado que el problema solicita 
		// hacer la tarea/trabajo con menor numero lo antes posible.
		priority_queue<int, vector<int>, greater<int>> Q;
		Q.push(0);

		while ( !Q.empty() ) {
			int v = Q.top();
			Q.pop();

			order_top.pb(v);
			Nodes[v].visited = 1;

			for ( auto it : lists[v]) {
				if ( !Nodes[it].visited ) {
					Nodes[it].in_grade--;
					del_arcs++;
					if(!Nodes[it].in_grade) Q.push(it);
				}
			}
		}

		// Si al final del BFS tenemos que la cantidad de arcos eliminados es distinto
		// al numero de arcos total M(incluyendo los arcos fantasmas) entonce hay circuitos
		// en el grafo.
		if (del_arcs != M) {
			vi empty_v; return empty_v;
		}

		return order_top;
	}*/

	// Funcion que aplica el algoritmo Tarjan para el calculo de C.F.C de un grafo. Es tambien
	// utilizado para determinar puentes y puntos de articulacion mediante una leve mejora al
	// mismo. 
	/*void Tarjan (int root) {
		opened[root] = 1;		// Marcamos como "abierto" 

		low[root] = c_time++;		// Actualizamos y marcado del Discovery y Low de cada nodo.
		disc[root] = low[root];

		Nodes[root].visited = 1;	// Marcado de nodos en el arreglo de visitado en cada llamada al Tarjan.

		Pila.push(root);		// Agregar a la pila el nodo raiz.

		// Nos aseguramos de que hayan nodos adyacentes (de salida/Delta+) a procesar.
		if (!lists[root].empty()) {
			for(auto it : lists[root]) {
				if(it == parent_tracker[root]) continue;// No se visita al padre.

				if( !Nodes[it].visited ) { 		// Visitamos al hijo de root.
					parent_tracker[it] = root;	// Trackeamos el padre de cada nodo.
					child_tracker[root]++;		// Aumentamos contador de hijos.

					Tarjan(it);

					// Verificando si es un puente.
					if( disc[root] < low[it] ){	
						if(root < it){
							bridges.pb(make_pair(root, it));
						} else {
							bridges.pb(make_pair(it, root));
						}
					}

					// Verificamos si es un punto de articulacion.
					if( disc[root] <= low[it] ) 	
						articulation_points[root] = true;

					low[root] = min(low[it], low[root]);	// Actualizacion del Low.

				} else if ( opened[it] ) {
				//} else {
					low[root] = min(low[root], disc[it]);	// Actualizacion del Low.
				}
			}
		}

		// Cuando tengamos mismo Low a Discovery entonces hemos encontrado una C.F.C.
		if( disc[root] == low[root] ) {	
			int nodo;
			cnt_CC++;
			if (!Pila.empty()) {
				while( root != Pila.top() ) {
					nodo = Pila.top();
					Pila.pop();
					opened[nodo] = 0;		// Cerramos la llamada recursiva al nodo pop.
					container_CC[cnt_CC].pb(nodo);	// Agregamos el nodo a la Componente.
				} 
			}

			// Popeamos la root de la stack. Pues esta forma parte de la componente y por tanto
			// esta debe agregarse.
			Pila.pop();		
			container_CC[cnt_CC].pb(root);
			
			// Cerramos la llamada recursiva al nodo root.
			opened[root] = 0;	
		}
	}*/

	// Funcion que aplica el algoritmo de Dijsktra para calcular caminos de costo minimo desde
	// una fuente(root) a todos los nodos del grafo, siempre y cuando haya un camino. Esta
	// funcion usa la estructura de dato Priority Queue.
	void DijkstraPQ(int root){	
		Nodes[root].min_cost = 0;
		int n, ady;
		lli cost;
		vector <pair<int, long long> > g_aux[n_nodes + 1];
		vi vis(n_nodes+1, 0);

		FOR1(k, n_nodes){
			n = k;
			for (auto it : lists[n]) {
				ady = it.ady;
				cost = it.weight;
				g_aux[n].pb(make_pair(ady, h[n]-h[ady] + cost));
			}
		}

		dist[root][root] = 0;

		P_Q.push(make_pair(0, root));
		
		while( !P_Q.empty() ){
			auto v = P_Q.top();
			P_Q.pop();
			int nod = v.second;

			// Verifiquemos que no este visitado.
			//if(Nodes[v.second].visited) continue; 	// Dijsktra
			if(vis[nod]) continue;				// Jhonson
			vis[nod] = 1;

			// Verificamos que el costo actual del nodo sacado del set sea el mismo
			// ya calculado.
			//int curr_c = Nodes[v.second].min_cost;	// Dijsktra
			int curr_c = dist[root][nod];			// Jhonson

			// Relajamos los arcos para todos los adyacentes del nodo actual.
			//for(auto it : lists[v.second]){		// Dijsktra
			for(auto it : g_aux[v.second]){			// Jhonson
				//int new_c = curr_c + it.weight;	// Dijsktra
				lli new_c = curr_c + it.second;		// Jhonson

				//if( Nodes[it.ady].min_cost <= new_c ) continue;// Dijsktra
				if( dist[root][it.first] <= new_c ) continue;	 // Jhonson

				//Nodes[it.ady].min_cost = new_c;	// Dijsktra
				dist[root][it.first] = new_c;		// Jhonson

				//P_Q.push(make_pair(new_c, it.ady));	// Dijsktra
				P_Q.push(make_pair(new_c, it.first));	// Jhonson
			}
			//Nodes[v.second].visited = 1;	// Dijsktra
		}
		// Jhonson
		FOR1(i, n_nodes) {if(dist[root][i] < INF) dist[root][i] -= (h[root] - h[i]);}
	}

	// Funcion que aplica el algoritmo de Dijsktra para calcular caminos de costo minimo desde
	// una fuente(root) a todos los nodos del grafo, siempre y cuando haya un camino. Esta
	// funcion usa la estructura de dato SET como una simulador de Arbol Rojo Negro.
	/*void DijkstraSET(int root){
		// Agregamos todos los elementos/nodos al set.
		FOR1(k, n_nodes){
			if(k == root) {
				Nodes[root].min_cost = 0;
			} else {
				Nodes[k].min_cost += k;
			}
			RB_tree.insert(Nodes[k]);
		}
	
		int curr_c;
		while( !RB_tree.empty() ){
			auto v = RB_tree.begin();
			RB_tree.erase(v);
			
			// Verificamos que el costo actual del nodo sacado del set sea el mismo
			// ya calculado.
			curr_c = Nodes[(*v).n].min_cost;
			if( (*v).min_cost != curr_c) continue;

			// Relajamos los arcos para todos los adyacentes del nodo actual.
			for(const auto &it : lists[(*v).n]){

				if( Nodes[it.ady].min_cost <= curr_c + it.weight ) continue;

				RB_tree.erase(Nodes[it.ady]);
				Nodes[it.ady].min_cost = curr_c + it.weight;
				RB_tree.insert(Nodes[it.ady]);
			}
		}
	}*/

	// Funcion que implementa el Algoritmo de Floyd-Warshall que determina los costos minimos 
	// para todo par de nodos en un grafo.
	/*void Floyd_Warshall() {
		int n, ady;
		lli cost;
		FOR1(root, n_nodes){
			// Seteamos inicialmente en 0 la distancia del propio nodo, para cuando se revise
			// el paso de relajacion al obtener del grafo que su valor -INF se cambia entonces.
			dist[root][root] = 0;

			// Armando un camino de tamaño i.
			FOR1(i, n_nodes) {
				// iteramos por todos los arcos del grafo.
				FOR1(k, n_nodes){
					n = k;
					for (auto it : lists[k]) {
						ady = it.ady;
						cost = it.weight;

						// Si la distancia de un arco esta marcada como INF es porque no existe
						// dicha distancia, es decir el arco no existe.
						if (dist[root][n] == INF || dist[root][ady] <= dist[root][n] + cost) continue;

						// Actualizamos la ruta root - ady.
						dist[root][ady] = dist[root][n] + cost;
					}
				}
			}
		}

		// Verificacion de ciclos negativos para el Algoritmo Floyd-Warshall.
		// Se itera por todos los arcos revisando las distancias de los nodos
		// adyacentes usando como intermediario al NODO K. 
		// Si se cumple que el nodo adyacente posee numeros negativos,
		// el camino usando el NODO K para conectar con el adyacente existe(< INF) y
		// que el camino usando el NODO ADY para conectar con el nodo actual existe (< INF).
		// Entonces el arco entre nodo puente K y n(actual) pertenece a un ciclo negativo.
		// Ej. K ----> ady -----> n Si ady < 0 y los demas arcos existen entonces, k-n es
		// parte de un ciclo negativo.
		FOR1(k, n_nodes){
			FOR1(t, n_nodes){
				n = t;
				for (auto it : lists[t]) {
					ady = it.ady;
					if(dist[k][ady] < INF && dist[ady][ady] < 0 && dist[ady][n] < INF ){
						dist[k][n] = -INF;
					}
				}
			}
		}
	}*/

	// Funcion que implementa el Algoritmo de Bellman-Ford para determinar el camino de menor
	// costo desde una raiz a cualquier otro nodo del grafo siempre que existe camino.
	bool Bellman_Ford(int root){
		h[root] = 0;	 // Jhonson
		bool flag;

		int n, ady;
		lli cost;
		
		// Seteamos inicialmente en 0 la distancia del propio nodo, para cuando se revise
		// el paso de relajacion al obtener del grafo que su valor -INF se cambia entonces.
		dist[root][root] = 0;	// Bellman-Ford

		// Armando un camino de tamaño i.
		//FOR1(i, n_nodes) {	// Bellman-Ford
		for (int i = 0; i <= n_nodes; i++) {	// Jhonson
			flag = true; 			// Jhonson
			// iteramos por todos los arcos del grafo.
			//FOR1(k, n_nodes){	// Bellman-Ford
			for (int k = 0; k <= n_nodes; k++){	// Jhonson
				n = k;
				for (auto it : lists[n]) {
					ady = it.ady;
					cost = it.weight;

					// Actualizando vector H. Para codigo de Jhonson.
					if (h[ady] > h[n] + cost){
						h[ady] = h[n] + cost;
						flag = false;
					}
					/* // Bellman-Ford
					// Si la distancia de un arco esta marcada como INF es porque no existe
					// dicha distancia, es decir el arco no existe.
					if (dist[root][n] == INF || dist[root][ady] <= dist[root][n] + cost) continue;

					// Si al momento de actualizar, el nodo actual o el costo son
					// -INF entonces asumimos un ciclo de costo negativo.
					if(dist[root][n] == -INF || cost == -INF) {
						dist[root][ady] = -INF; return false;
					}

					// Actualizamos la ruta root - ady.
					dist[root][ady] = dist[root][n] + cost;

					// Si se relajo en el paso anterior y estamos en i igual
					// numero de nodos entonces hemos construido un camino 
					// de tamaño N cuando el maximo posible es N-1, por tanto
					// dicho arco forma parte un ciclo negativo.
					if (i != n_nodes) continue;
					dist[root][ady] = -INF; return false;
					*/ // Bellman-Ford
				}
			}
			if(flag) return true;	// Jhonson
		}
		//return true; 	// Bellman-Ford
		return false;	// Jhonson
	}

	// Funcion que implementa el algoritmo de Jhonson, este permite encontrar el camino 
	// más corto entre todos los pares de nodos de un grafo dirigido disperso.
	bool Jonhson(){
		// Agregamos el nodo fantasma con dichos arcos fantasmas de peso 0.
		FOR1(n, n_nodes) add_arc(0, n, 0);

		// Verificamos que no hayan ciclos de costo negativo.
		if(!Bellman_Ford(0)) return false;
	
		cout << "yes\n\n";

		// Mostramos el vector H.
		FOR1(k, n_nodes){
			if(k > 1) cout << ' ';
			cout << h[k];
		}
		cout << ' ' << h[0] << "\n\n";
		
		// Aplicamos Dijkstra con Cola de prioridad para buscar los caminos.
	 	FOR1(k, n_nodes) DijkstraPQ(k);
	 		
		return true;
	}


	// Funcion que permite mostrar por pantalla cualquier vector de class NODE dado.
	void print_vn(vn a){
		FOR0(k, a.size()) {cout << a[k].n << " ";}
		cout << endl;
	}

	// Funcion que permite mostrar por pantalla cualquier vector de enteros dado.
	void print_vi(vi a){
		FOR0(k, a.size()) {cout << a[k] << " ";}
		cout << endl;
	}

	// Funcion que permite mostrar por pantalla cualquier vector de long long dado.
	void print_vll(vll a){
		FOR0(k, a.size()) {cout << a[k] << " ";}
		cout << endl;
	}

	// Funcion que permite extraer los nodos tales que cumplan con TRUE en el arreglo.
	void print_vb(vb a){
		vi r;
		FOR0(k, a.size()) { if(a[k]) r.pb(k); }
		print_vi(r);
	}

	// Funcion que permite mostrar por pantalla una matriz de enteros.
	void print_mll(vvll m){
		FOR1(i, n_nodes){
			FOR1(j, n_nodes){
				if(m[i][j] == INF) {cout << "# "; continue;}
				cout << m[i][j] << " ";
			}
			cout << endl;
		}
		cout << endl;
	}
};

// Escribir "init_code();" justo despues al comienzo del "int main() {" para lectura de datos.
void init_code(){
	#ifndef ONLINE_JUDGE
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
	#endif
}

// Fin de la libreria.
/*************************************************************************************************/

int main() {
	// Mejora de speed para cin/cout.
	ios_base :: sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);

	// Lectura de datos.
	init_code();

	cin >> T;
	FOR1(t, T){
		cin >> N;
		cin >> M;
		Graph G(1, N);
		FOR1(m, M){
			cin >> A >> B >> W;
			G.add_arc(A, B, W);
		}

		cout << "graph " << t << " ";
		if (G.Jonhson()) {
			G.print_mll(G.dist);
			continue;
		}

		cout << "no\n\n";
	}

	return EXIT_SUCCESS;
}