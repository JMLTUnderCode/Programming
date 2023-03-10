/*
	Nombre: Junior Miguel Lara Torres
	Carnet: 1710303
	Nick: JMLTUnderCode
	Problema: Codeforces TypeDB Forces 2023 (Div. 1 + Div. 2, Rated, Prizes!) D. Game on Axis

	** Idea General para el problema: **
		
	Para solventar el problema se debe observar el input como un posible grafo dirigido y
	a su vez como permutaciones con valores entre -N <= i <= N. Se debe saber que indice
	del arreglo/nodo del grafo que lleva a ganar el juego(Se marca como INF).
	Luego realizar una busqueda DFS sobre la raiz del juego/grafo (NODO 1) y luego analizar
	todos los pares combinatorios (caminos en el array/grafo) tal que se culmine el juego,
	es decir cambiar un "arco" y verificar que este realizando un camino tome con un nodo
	que tenga INF(gana). Se tiende como ejemplo:

 	N:      3
	input: -1 -2 -1
	.--------------
	index:  1  2  3

	Inicialmente tenemos el vector WIN_NODES = [ INF, INF, 2 ], con indice/nodo 1 y 2 salimos
	del rango(ganamos) mediante un camino de length 1, notemos que realizar indice 3 + (-1) = 2 
	e indice 2 apunta a nodo INF, por tanto nodo 3 tambien es nodo ganador siguiente un camino 
	de longitud 2 en este caso.

	Cualquier camino/permutacion debemos considerar el caso de obtener un opuesto (-i) tal que 
	genere un ciclo infinito y no poder salir del juego. Adicionalmente tener en cuenta el caso
	de obtener 0(bucle base) en el nodo inicial(nodo 1) pues no importa que otro valor del array
	/grafo se cambie si el nodo inicial esta en bucle base este nunca termina el juego.

	** Actualizacion de Libreria/Class: **

	1. Creacion de dos funciones "DFS_recurr" y "path_legth" para solventar el problema.
	
*/

#include <bits/stdc++.h>

using namespace std;

/*************************************************************************************************/
#define pb push_back
#define FOR1(i, N) for(int i = 1; i-1 < N; i++)
#define FOR0(i, N) for(int i = 0; i < N; i++)
typedef long long ll;
typedef long long int lli;
typedef vector<int> vi;
typedef vector<vi> vvi;
typedef pair<int, int> pii;
typedef vector<pii> vpii;
typedef pair<lli, int> pllii;
typedef vector<pllii> vpllii;
typedef vector<ll> vll;
typedef vector<lli> vlli;
typedef vector<vector<ll>> vvll;
typedef vector<vector<lli>> vvlli;
typedef vector<bool> vb;
typedef vector<vb> vvb;

const lli INF = 1000000000000LL; // 1e12

// Class/Struct NODE para informacion de un nodo en grafos. Implementada por Junior Lara.
class NODE{
public:
	int n;		// Numero del nodo.
	int visited;	// Identificador de visitado.
	int range;	// Rango para busqueda BFS/DFS.
	/*
	int in_grade;	// Numero de arcos que entran al nodo.
	int ex_grade;	// Numero de arcos que salen desde el nodo.
	lli min_cost;	// Costo del nodo.
	int n_ady;	// Numero de nodos adyacentes a n.
	string color;	// Color del nodo.
	int father;	// Nodo padre del nodo n;
	int level;	// Para saber el nivel de un nodo en la busqueda BFS\DFS.
	*/

	// Constructor DEFAULT.
	NODE(){
		n = 0; visited = 0; range = 0;
		//in_grade = 0; ex_grade = 0; min_cost = 0;
		//n_ady = 0;  color = ""; father = 0; level = 0;
	}
	// Constructor basico.
	NODE(int _id, int _vis) : n(_id), visited(_vis) {
		range = 0;
		//in_grade = 0; ex_grade = 0; min_cost = 0; 
		//n_ady = 0;  color = ""; father = 0; level = 0;
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
	/*bool operator>(const NODE &otherNode) const {
		return min_cost < otherNode.min_cost;
	}
	bool operator<(const NODE &otherNode) const {
		return min_cost > otherNode.min_cost;
	}*/
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

const int MaxN = 2*1e5+5;	// Numero maximo de nodos.
int T;				// Numero de casos de prueba.
lli N;				// Numero de nodos.
lli res;			// Numero de pares formados para finalizar el juego.
lli a_i;			// Valor asociado al indice i, para realizar i + a_i.
vlli win_nodes(MaxN, 0);	// Vector de posibles nodos ganadores(Los que terminan el juego)

/*************************************************************************************************/
// Libreria Graph implementada por Junior Lara.

class Graph{
	int n_nodes;		// Numero de nodos en el grafo.
	int type_graph;		// Tipo de grafo: 1 = Lista Adyacencia y 2 = Matriz de Adyacencia.
	vva lists_a;		// Estructura para la lista de adyacencia de tipo arcos.
	vvi lists_i;		// Estructura para la lista de adyacencia de tipo enteros.
	vvi Matriz;		// Estructura para la matriz de adyacencia.

public:
	vn Nodes;		// Vector para guardar cada nodo de class NODE.
	/*
	vpii bridges; 		// Vector de arcos que clasifican como puentes.
	vb articulation_points;	// Vector de nodos que clasifican como puntos de articulacion.
	set<NODE, less<NODE>> RB_tree;// Estructura set que simula un arbol rojo negro para Dijkstra.
	priority_queue<pllii, vpllii, greater<pllii>> P_Q;// Priotiry Queue usada en Dijsktra.
	vll h;			// Vector h usado en algoritmo Jonhson.
	vvll dist;		// Vector de distancias para el algoritmo Floyd-Warshall
	vi vis1, vis2;		// Vectores de visitados.
	vi parent_tracker;	// Vector de nodos para tener un track de los nodos padres de cada nodo.
	vi child_tracker;	// Vector de nodos o contador de hijos de cada nodo.
	vi low;			// Vector para marcar el low en algoritmo Tarjan.
	vi disc;		// Vector para marcar el discovery en algoritmo Tarjan.
	vi opened;		// Vector para marcar si un nodo esta abierto en llamado BFS/DFS.
	vvi container_CC;	// Contenedor de Componentes Conexas.
	int c_time, cnt_CC;	// Inicializador y contador para el algoritmo Tarjan.
	stack<int> Pila;	// Stack usada por el algoritmo Tarjan.
	*/

	// Constructor principal del grafo. Se espeficica el tama??o y el tipo de grafo,
	// 1 para el caso de usar listas de adyacencias, 2 para el caso de matrices.
	Graph(int type, int size) : type_graph(type), n_nodes(size) {
		assert(type > 0 && type < 3);
		/*
		vb newArtPoint(size+1, false); articulation_points = newArtPoint;
		vll newh(size+1, INF); h = newh;
		vvll newDist(size+1, vll(size+1 , INF)); dist = newDist;
		vis1 = vi(size+1, 0); vis2 = vi(size+1, 0);
		vi newFatherT(size+1, 0); parent_tracker = newFatherT;
		vi newChildT(size+1, 0); child_tracker = newChildT;
		vi newLow(size+1, 0); low = newLow;
		vi newDisc(size+1, 0); disc = newDisc;
		vi newOpened(size+1, 0); opened = newOpened;
		vvi newContainer(size+1); container_CC = newContainer;
		c_time = 0; cnt_CC = 0;
		*/

		if (type == 1) { 				// Listas de Adyacencias.
			vva newListsa(size+1); lists_a = newListsa;
			vvi newListsi(size+1); lists_i = newListsi;
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
	void add_node_v1() {
		n_nodes++;
		if (type_graph == 1) {
			lists_i.pb(vi ());
		} else {
			vvi aux(n_nodes+1, vi (n_nodes+1));
			for (int i = 1; i < n_nodes; i++ ) {
				for (int j = 1; j < n_nodes; j++ ) aux[i][j] = Matriz[i][j];
			}
			Matriz = aux;
		}
	}
	void add_node_v2() {
		n_nodes++;
		if (type_graph == 1) {
			lists_a.pb(va ());
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
	void add_arc_v1(int n1, int n2) {
		if ( type_graph == 1 ) {
			lists_i[n1].pb(n2);
			if(Nodes[n1].n == 0 && n1 != 0) Nodes[n1] = NODE (n1, 0);
			//Nodes[n1].ex_grade++;
			if(Nodes[n2].n == 0 && n2 != 0) Nodes[n2] = NODE (n2, 0);
			//Nodes[n2].in_grade++;
		} else {
			Matriz[n1][n2] = 1;
		}
	}
	void add_arc_v2(int n1, int n2, lli w) {
		if ( type_graph == 1 ) {
			lists_a[n1].pb(ARC(n1, n2, w));
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
	vi get_ady_v1(int x) {
		if (type_graph == 1) {
			return lists_i[x];
		} else {
			vi aux;
			for (int k = 1; k-1 < n_nodes; k++) if (Matriz[x][k] == 1) aux.pb(k);
			return aux;
		}
	}
	vi get_ady_v2(int x) {
		if (type_graph == 1) {
			vi ady_v;
			for(auto it : lists_a[x]) ady_v.pb(it.ady);
			return ady_v;
		} else {
			vi aux;
			for (int k = 1; k-1 < n_nodes; k++) if (Matriz[x][k] == 1) aux.pb(k);
			return aux;
		}
	}

	// Funcion que permite revertir los arcos del grafo.
	void arc_reverse_v1(){
		vvi rev_list(n_nodes+1);
		for(int k = 1; k-1 < n_nodes; k++){
			for(auto it : lists_i[k]) rev_list[it].pb(k);
		}
		lists_i = rev_list;
	}
	void arc_reverse_v2(){
		vva rev_list(n_nodes+1);
		for(int k = 1; k-1 < n_nodes; k++){
			for(auto it : lists_a[k]) rev_list[it.ady].pb(ARC(it.ady, it.n, it.weight));
		}
		lists_a = rev_list;
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
				// se agregaron todos sus ara??as adyacentes y no es necesario 
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

	// Funcion que arma los arcos para las ara??as, se usa un grafo auxiliar con nodos
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
		// Se arman los arcos entre las ara??as que cumplen con tener un divisor comun mayor a 1.
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
		if (!lists_i[root].empty()) {
			for(auto it : lists_i[root]) {
				//if(it == parent_tracker[root]) continue;// No se visita al padre.

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
					low[root] = min(low[root], disc[it]);	// Actualizacion del Low.
				}

				// Marcamos el terminal.
				terminal_n[root] = terminal_n[it];	
				// Si es 0 entonces el propio nodo es su terminal.
				if(!terminal_n[root]) terminal_n[root] = root;
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
	/*void DijkstraPQ(int root){	
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
	}*/

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
	// para todo par de nodos en un grafo mediante el uso de vector de arcos con pesos.
	/*void Floyd_Warshall() {
		int n, ady;
		lli cost;
		FOR1(root, n_nodes){
			// Seteamos inicialmente en 0 la distancia del propio nodo, para cuando se revise
			// el paso de relajacion al obtener del grafo que su valor -INF se cambia entonces.
			dist[root][root] = 0;

			// Armando un camino de tama??o i.
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

	// Algoritmo/Funcion Floyd-Warshall que determina los costos minimos para todo par de nodos
	// dado una matriz de adyacencias y checando caminos de costo k conforme se eliminan nodos 
	// del grafo.
	/*void Floyd_Warshall_m() {
		FOR1(k, n_nodes){ 	// "Paso k" en el greg elimina un nodo, lo que a su vez permite buscar 
					// el nodo agregar en los caminos.
			int n_j = node_del[n_nodes-k+1];	// Vemos el nodo eliminado en el paso N-k+1. Cuando k=1, estamos en el ultimo 
								// nodo N-K+1 = N, lo cual es camino de tama??o 1 basicamente.

			Nodes[n_j].visited = 1;			// Marcamos como visitado el nodo a eliminar.

			// Los fors anidados permiten iniciar el calculo de los costos minimo del camino de tama??o k.
			FOR1(x, n_nodes) {
				FOR1(y, n_nodes) {

					// Actualizar la matriz de distancias en funcion del minimo.
					dist[x][y] = min(dist[x][n_j] + dist[n_j][y], dist[x][y] );
					
					// Cuando se hallen dos nodos visitados entonces fueron "eliminados" lo que general 
					// el final del camino por tanto realizamos la actualizacion de la distancia minima
					// calculada hasta el momento.
					if ( Nodes[x].visited && Nodes[y].visited ) {
						Nodes[N-k+1].min_cost += dist[x][y];
					}
				}
			}
		}
	}*/

	// Funcion que implementa el Algoritmo de Bellman-Ford para determinar el camino de menor
	// costo desde una raiz a cualquier otro nodo del grafo siempre que existe camino.
	/*bool Bellman_Ford(int root){
		h[root] = 0;	 // Jhonson
		bool flag;

		int n, ady;
		lli cost;
		
		// Seteamos inicialmente en 0 la distancia del propio nodo, para cuando se revise
		// el paso de relajacion al obtener del grafo que su valor -INF se cambia entonces.
		dist[root][root] = 0;	// Bellman-Ford

		// Armando un camino de tama??o i.
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
					// Bellman-Ford
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
					// de tama??o N cuando el maximo posible es N-1, por tanto
					// dicho arco forma parte un ciclo negativo.
					if (i != n_nodes) continue;
					dist[root][ady] = -INF; return false;
					// Bellman-Ford
				}
			}
			if(flag) return true;	// Jhonson
		}
		//return true; 	// Bellman-Ford
		return false;	// Jhonson
	}*/

	// Funcion que implementa el algoritmo de Jhonson, este permite encontrar el camino 
	// m??s corto entre todos los pares de nodos de un grafo dirigido disperso.
	/*bool Jonhson(){
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
	}*/

	// Funcion que busca el camino mas directo entre dos grafos abstractos en formato strings
	// compuesto por "-"(zona segura) y "x"(no segura) poder hacer el recorrido DFS/BFS.
	/*bool path_between_recur(int i, bool wall, int water) {
		// Si alguna llamada sobrepasa el valor N, hemos salido del ca??on.
		if(i > n_nodes ) return true; 
		
		// Verificar pared izquierda.
		if(!wall && (w_l[i-1] != '-' || water >= i || vis1[i]) ) return false;
		else if (!wall) vis1[i] = 1;
		

		// Verificar pared derecha.
		if( wall && (w_r[i-1] != '-' || water >= i || vis2[i]) ) return false;
		else if (wall) vis2[i] = 1;
		
		// Aumento del agua en el ca??on.
		water++;

		// Salto de left -> right.
		if(path_between_recur(i + K, !wall, water)) return true;

		// Subir una zona en la pared actual.
		if(path_between_recur(i + 1,  wall, water)) return true;

		// Bajar una zona en la pared actual.
		if(path_between_recur(i - 1,  wall, water)) return true;
		
		return false;
	}*/

	// Funcion DFS recursivo que permite marcos los elementos visitados para el sub arbol DFS
	// formado.
	void DFS_recur(int root){
		Nodes[root].visited = 1;
		lli ady = win_nodes[root];
		
		if (ady == INF) {DFS_recur(0); return;}

		if (ady != 0 && !Nodes[ady].visited) DFS_recur(ady);
	}

	// Funcion DFS que permite realizar una busqueda en profundidad y determinar el rango
	// de un nodo (longitud del camino hasta la raiz).
	int path_lenth(int root){
		Nodes[root].range = 1;

		for(auto it : lists_i[root]){
			Nodes[root].range += path_lenth(it);
		}

		return Nodes[root].range;
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
	// Mejora de velocidad para el cin y cout.
	ios_base::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);

	// Lectura de datos.
	init_code();

	cin >> T;
	while(T--){
		cin >> N;
		Graph G(1, N);

		// Reset de nodos ganadores para el espacio necesario del test.
		FOR1(k, N) win_nodes[k] = 0;
		
		FOR1(i, N){
			cin >> a_i;

			// Si al realizar i + a_i es menor que 1 o mayor que N entonces se acabo el juego.
			if(i + a_i < 1 || i + a_i > N) {
				win_nodes[i] = INF;	// Se marca nodo ganador.
				G.add_arc_v1(0, i);	// Guardamos dicho nodo/indice.
				continue;
			}

			// Si no paso del rango 1 <= i <= N entonces guardamos el next en posibles nodos ganadores.
			win_nodes[i] = i + a_i;

			// Se guarda en el grafo de donde propiene el indice/nodo i.
			G.add_arc_v1(i + a_i, i);
		}

		// Realizamos una busqueda en profundidad desde el nodo partida 1, se requiere verificar
		// que el nodo 0 del grafo (aquellos donde estan los nodos ganadores) fue visitado.
		// Esto permite verificar que si un nodo fue visitado es parte de un camino.
		G.DFS_recur(1);

		// Se realiza un DFS para cada nodo que permite salir del rango directamente, 
		// se cuenta el total de longitudes de todos los caminos indice/nodo a meta.
		G.path_lenth(0);

		// Verificamos casos: 
		// 1. Si el nodo meta en algun punto fue visitado entonces no existe un ciclo infinito
		// tal que no sea posible terminar el juego. 
		res = 0;
		if (G.Nodes[0].visited) {
			// Se comienza con el maximo de pares, por combinatoria es
			// N negativos + N positivos + valor 0 = 2*N+1, esto por N casillas.
			res = N * (2*N + 1);

			// Partiendo desde 1 se van restando pares.
			lli index = 1;
			while (index != INF) {
				//  Depende del length del camino hasta el nodo meta.
				// Cualquiera de estos nodos en el camino su elemento contrario -N
				// forma un bucle infinito.
				res -= G.Nodes[index].range;

				// Restamos los pares que pueden forman un ciclo infinito con el nodo
				// actual. Para las N+1 formas de salir directamente del rango se debe
				// restar la cantidad de nodos de los subcaminos formados por los nodos
				// que salen directamente.
				res -= (N + 1 - G.Nodes[0].range);

				// Nos vamos al siguiente en el camino.
				index = win_nodes[index];
			}
		// 2. Si no fue visitado el nodo meta entonces existe un ciclo de juego infinito desde
		// el punto inicial(1) por tanto para cada nodo del ciclo se debe sumar N pares
		// (numeros positivos y negativos que directamente salen del rango al realizar i + a_i
		// tanto por < 0 o por > N).
		} else {
			FOR1(i, N) {
				if (!G.Nodes[i].visited) continue;
				res += N + G.Nodes[0].range;
			}
		}
		cout << res << endl;
	}
	return EXIT_SUCCESS;
}