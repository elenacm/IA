/*
 * ElenaBot.cpp
 *
 *  Created on: 15 ene. 2018
 *      Author: manupc
 */

#include "ElenaBot.h"
#include <string>
#include <cstdlib>
#include <iostream>
#include <list>
#include <stack>
#include <cmath>
#include <limits.h>
#include <chrono>
using namespace std;
using namespace chrono;

ElenaBot::ElenaBot() {
	// Inicializar las variables necesarias para ejecutar la partida
	srand(time(0));
}

ElenaBot::~ElenaBot() {
	// Liberar los recursos reservados (memoria, ficheros, etc.)
}

void ElenaBot::initialize() {
	// Inicializar el bot antes de jugar una partida
}

string ElenaBot::getName() {
	return "ElenaBot"; // Sustituir por el nombre del bot
}

//Función global para conocer el jugador que somos
Player p;
void setPlayer(Player player){ p = player; }
Player getPlayer(){ return p; }

//Heuristica del nodo hoja
int ElenaBot::heuristica(const GameState& game){
	int nadie = 0;

	if(getPlayer() == J1)	//Si soy el jugador 1
		return game.getScore(J1)-game.getScore(J2);
	else if(getPlayer() == J2)	//Si soy el jugador 2
		return game.getScore(J2)-game.getScore(J1);
	else	//Si no soy nadie
		return nadie;
}

nodo::nodo(GameState game, Move m, bool max){ //Constructor
	juego = game;
	movimiento = m;
	PlayerMax = max;
}

//Creamos los hijos del nodo actual
void nodo::Crear_Hijos(list<nodo>& hijos){

	for(int i = 1; i <= 6; i++){
		//simulo cada uno de los posibles movimientos
		GameState hijo = juego.simulateMove((Move) i);
		//Compruebo si es mi turno o no, para saber cuando maximizar
		bool child_maximize = juego.getCurrentPlayer() == hijo.getCurrentPlayer()
		 ? PlayerMax : !PlayerMax;

		//Creo un nodo con el GameState simulado,
		//El movimiento que se ha realizado y
		//si es jugador max
		nodo actual(hijo, (Move) i, child_maximize);

		//cada movimiento lo meto en una lista
		hijos.push_back(actual);
	}
}

high_resolution_clock::time_point tantes;
Valor_Move ElenaBot::minimax_poda_function(nodo& n, int profundidad, int alpha, int beta){
	//cerr << "camino elegido" << endl;
	//cerr << "Explorando nodo: " << endl <<  n << endl;
	//int mejorValor;
	high_resolution_clock::time_point tdespues = high_resolution_clock::now();
	Valor_Move minodo, v;

	duration<double> transcurrido = duration_cast<duration<double>>(tdespues - tantes);

	if(transcurrido.count() > 1.90){	//Comprobamos que el tiempo es menos de 2 seg
		return {1000, n.getM()};
	}

	//si la profundidad es 0 o hemos llegado a un nodo hoja
	if(profundidad == 0 || n.getGS().isFinalState()){
		minodo.valor = heuristica(n.getGS());
		minodo.move = n.getM();	//Movimiento usado
		//cerr << "Heuristica ->" << minodo.valor << endl;
		return minodo;
	}

	list<nodo> hijos;
	n.Crear_Hijos(hijos);	//Generamos los hijos

	if(n.getPM()){	//Jugador max
		//cerr << "Es un nodo a maximizar" << endl << endl;
		//mejorValor = INT_MIN;
		minodo.valor = INT_MIN;	//inicializo a menos infinito

		//Recorremos los hijos del nodo
		for(auto hijo : hijos){
			//Llamamos recursivamente al método con profundidad-1
			v = minimax_poda_function(hijo, profundidad-1, alpha, beta);

			//Parte del iterative deepening
			if(v.valor == 1000){ return {1000, n.getM()}; }

			//Si el valor devuelto es mayor que el que se iba a devolver
			if (v.valor > minodo.valor){
				minodo.valor = v.valor;
				minodo.move = hijo.getM();
			}

			//Si alpha es más pequeño que el valor a devolver
			if(alpha < minodo.valor)
				alpha = minodo.valor;

			//Condición de poda: Si beta es menor o igual a alpha
			if(beta <= alpha)
				break;

			/*
			CODIGO PARA EL ALGORITMO MINIMAX ANTES IMPLEMENTADO
			v = minimax_function(hijo, profundidad-1);
			if (v.valor > mejorValor){
				mejorValor = v.valor;
				minodo.valor = mejorValor;
				minodo.move = hijo.getM();
			}*/
		}
	}
	else{	//Jugador min
		//cerr << "Es un nodo a minimizar" << endl << endl;
		//mejorValor = INT_MAX;
		minodo.valor = INT_MAX;	//inicializo a mas infinito

		//Recorremos los hijos del nodo
		for(auto hijo : hijos){
			//Llamamos recursivamente al método con profundidad-1
			v = minimax_poda_function(hijo, profundidad-1, alpha, beta);

			//Parte del iterative deepening
			if(v.valor == 1000){ return {1000, n.getM()}; }

			//Si el valor devuelto es menor que el que se iba a devolver
			if (v.valor < minodo.valor){
				minodo.valor = v.valor;
				minodo.move = hijo.getM();
			}

			//Si beta es mayor que el valor a devolver
			if(beta > minodo.valor)
				beta = minodo.valor;

			//Condición de poda: Si beta es menor o igual a alpha
			if(beta <= alpha)
				break;

			/*
			CODIGO PARA EL ALGORITMO MINIMAX ANTES IMPLEMENTADO
			v = minimax_function(hijo, profundidad-1);
			if (v.valor < mejorValor){
				mejorValor = v.valor;
				minodo.valor = mejorValor;
				minodo.move = hijo.getM();
			}*/
		}
	}

	return minodo;
}

Move ElenaBot::nextMove(const vector<Move> &adversary, const GameState &state) {

	tantes = high_resolution_clock::now();
	int profundidad = 20;
	Player turno = state.getCurrentPlayer();
	bool soyj1 = turno == J1;
	nodo origen(state, M_NONE, true);
	long timeout = this->getTimeOut();
	Valor_Move minodo, aux;
	aux = {0, (Move) 0};

	setPlayer(turno);
  //FUNCION MINIMAX
	//esta funcion devuelve el valor correspondiente a cada nodo
	//minodo = minimax_function(origen, profundidad);

	//FUNCION MINIMAX CON PODA
	//esta funcion devuelve el valor correspondiente a cada nodo
	//minodo = minimax_poda_function(origen, profundidad, INT_MIN, INT_MAX);
	//cerr << "Valor poda " << minodo.valor << endl;
	//cerr << "Fin MINIMAX" << endl;

	//Iterative deepening
	for(int depth = 1; depth <= profundidad && aux.valor != 1000; depth++){
		minodo = aux;
		//cerr << "Profundidad -> " << depth << endl;
		//cerr << "Valor -> " << aux.valor << endl;
		//cerr << "Accion -> " << aux.move << endl;
		aux = minimax_poda_function(origen, depth, INT_MIN, INT_MAX);
	}

	Move movimiento = minodo.move;

	return movimiento;
}
