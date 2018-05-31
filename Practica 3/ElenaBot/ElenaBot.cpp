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
using namespace std;

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

//FALTA DESCRIPCION
Player p;
void setPlayer(Player player){ p = player; }
Player getPlayer(){ return p; }

//heuristicas miradas
//http://nineil-leissi-cs.blogspot.com/2008/06/proyecto-1-unidad-juego-mancala.html


//Heuristica del nodo hoja
int ElenaBot::heuristica(const GameState& game){
	int nadie = 0;

	if(getPlayer() == J1)
		return game.getScore(J1)-game.getScore(J2);
	else if(getPlayer() == J2)
		return game.getScore(J2)-game.getScore(J1);
	else
		return nadie;

}

nodo::nodo(GameState game, Move m, bool max){ //Constructor
	juego = game;
	movimiento = m;
	PlayerMax = max;
}

//Creamos los hijos del nodo actual
//Pasar por referencia
void nodo::Crear_Hijos(list<nodo>& hijos){

	for(int i = 1; i <= 6; i++){
		//simulo cada uno de los posibles movimientos
		GameState hijo = juego.simulateMove((Move) i);
		//cada movimiento lo meto en una lista

		bool child_maximize = juego.getCurrentPlayer() == hijo.getCurrentPlayer()
		 ? PlayerMax : !PlayerMax;

		/*bool mismojugador = hijo.getCurrentPlayer() == getPlayer();
		bool hijoamaximizar;

		if(mismojugador)
			hijoamaximizar = PlayerMax;
		else
			hijoamaximizar = !PlayerMax;*/

		nodo actual(hijo, (Move) i, child_maximize);

		hijos.push_back(actual);
	}
}

Valor_Move ElenaBot::minimax_poda_function(nodo& n, int profundidad, int alpha, int beta){
	//cerr << "camino elegido" << endl;
	//cerr << "Explorando nodo: " << endl <<  n << endl;
	Valor_Move minodo, v;

	//si la profundidad es 0 o hemos llegado a un nodo hoja
	if(profundidad == 0 || n.getGS().isFinalState()){
		minodo.valor = heuristica(n.getGS());
		minodo.move = n.getM();
		//cerr << "Heuristica ->" << minodo.valor << endl;
		return minodo;
	}

	list<nodo> hijos;
	n.Crear_Hijos(hijos);

	if(n.getPM()){	//Jugador max
		//cerr << "Es un nodo a maximizar" << endl << endl;
		//mejorValor = INT_MIN;	//inicializo a menos infinito
		minodo.valor = INT_MIN;

		//Recorremos los hijos del nodo
		for(auto hijo : hijos){
			v = minimax_poda_function(hijo, profundidad-1, alpha, beta);
			if (v.valor > minodo.valor){
				minodo.valor = v.valor;
				minodo.move = hijo.getM();
			}

			if(alpha < minodo.valor)
				alpha = minodo.valor;

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
		//mejorValor = INT_MAX;	//inicializo a mas infinito
		minodo.valor = INT_MAX;

		//Recorremos los hijos del nodo
		for(auto hijo : hijos){
			v = minimax_poda_function(hijo, profundidad-1, alpha, beta);
			if (v.valor < minodo.valor){
				minodo.valor = v.valor;
				minodo.move = hijo.getM();
			}

			if(beta > minodo.valor)
				beta = minodo.valor;

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

	int profundidad = 15;
	Player turno = state.getCurrentPlayer();
	bool soyj1 = turno == J1;
	nodo origen(state, M_NONE, true);
	long timeout = this->getTimeOut();
	Valor_Move minodo;

	setPlayer(turno);
  //FUNCION MINIMAX
	//esta funcion devuelve el valor correspondiente a cada nodo
	//minodo = minimax_function(origen, profundidad);

	//FUNCION MINIMAX CON PODA
	//esta funcion devuelve el valor correspondiente a cada nodo
	minodo = minimax_poda_function(origen, profundidad, INT_MIN, INT_MAX);
	//cerr << "Valor poda " << minodo.valor << endl;
	//cerr << "Fin MINIMAX" << endl;

	Move movimiento = minodo.move;

	return movimiento;
}
