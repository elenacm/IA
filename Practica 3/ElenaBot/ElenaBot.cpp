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
Player jugador;
void setPlayer(Player p){ jugador = p; }
Player getPlayer(){ return jugador; }

struct Anodo{	//Lo que nos debe devolver el algoritmo
	int valor;
	Move move;
};

class nodo{
	private:
		GameState juego;	//informacion de cada jugador y piezas
		Move movimiento; //movimiento desde el padre
		bool PlayerMax;
	public:
		nodo(GameState game, Move m, bool max){ //Constructor
			juego = game;
			movimiento = m;
			PlayerMax = max;
		}

		//Devuelve el atributo privado juego
		GameState getGS(){ return juego; }
		//Devuelve el atributo privado movimiento
		Move getM(){ return movimiento; }
		//Devuelve el atributo privado PlayerMax
		bool getPM(){ return PlayerMax; }

		void setPM(bool pm){ PlayerMax = pm; }

		friend ostream& operator<< (ostream& os, nodo gn){
	    os << "\tJ1: Granero=" << (int) gn.juego.getSeedsAt(J1, GRANERO) <<
	       " C1=" << (int) gn.juego.getSeedsAt(J1, P1) <<
	       " C2=" << (int) gn.juego.getSeedsAt(J1, P2) <<
	       " C3=" << (int) gn.juego.getSeedsAt(J1, P3) <<
	       " C4=" << (int) gn.juego.getSeedsAt(J1, P4) <<
	       " C5=" << (int) gn.juego.getSeedsAt(J1, P5) <<
	       " C6=" << (int) gn.juego.getSeedsAt(J1, P6) << endl;

	    os << "\tJ2: Granero=" << (int) gn.juego.getSeedsAt(J2, GRANERO) <<
	       " C1=" << (int) gn.juego.getSeedsAt(J2, P1) <<
	       " C2=" << (int) gn.juego.getSeedsAt(J2, P2) <<
	       " C3=" << (int) gn.juego.getSeedsAt(J2, P3) <<
	       " C4=" << (int) gn.juego.getSeedsAt(J2, P4) <<
	       " C5=" << (int) gn.juego.getSeedsAt(J2, P5) <<
	       " C6=" << (int) gn.juego.getSeedsAt(J2, P6) << endl;
	    return os;
		}

		//Creamos los hijos del nodo actual
		list<nodo> Crear_Hijos(){
			list<nodo> hijos;
			hijos.clear();

			for(int i = 1; i <= 6; i++){
				//simulo cada uno de los posibles movimientos
				GameState hijo = juego.simulateMove((Move) i);
				//cada movimiento lo meto en una lista

				bool mismojugador = hijo.getCurrentPlayer() == getPlayer();
				bool hijoamaximizar;

				if(mismojugador)
					hijoamaximizar = PlayerMax;
				else
					hijoamaximizar = !PlayerMax;

				nodo actual(hijo, (Move) i, hijoamaximizar);

				hijos.push_back(actual);
			}

			return hijos;
		}
};

//Heuristica del nodo hoja
int heuristica(GameState game){
	//restar el granero del jugador 1 menos el del jugador 2
	return game.getScore(J1)-game.getScore(J2);
}

Anodo minimax_function(nodo n, int profundidad){
	cerr << "camino elegido" << endl;
	cerr << n << endl;
	Anodo minodo, v;
	int mejorValor;
	//Creo los movimientos posibles que se pueden hacer
	list<nodo> hijos = n.Crear_Hijos();

	if(profundidad == 0 || n.getGS().isFinalState()){
		minodo.valor = heuristica(n.getGS());
		minodo.move = n.getM();
		cerr << "movimiento" << minodo.move << endl;
		return minodo;
	}

	if(n.getPM()){	//Jugador max
		cerr << "jugador max" << endl;
		mejorValor = INT_MIN;	//inicializo a menos infinito
		//Recorremos los hijos del nodo
		for(auto hijo : hijos){
			v = minimax_function(hijo, profundidad-1);
			if (v.valor > mejorValor){
				mejorValor = v.valor;
				minodo.valor = mejorValor;
				minodo.move = hijo.getM();
			}
		}
		return minodo;
	}
	else{	//Jugador min
		cerr << "Jugador min" << endl;
		mejorValor = INT_MAX;	//inicializo a mas infinito
		//Recorremos los hijos del nodo
		for(auto hijo : hijos){
			v = minimax_function(hijo, profundidad-1);
			if (v.valor < mejorValor){
				mejorValor = v.valor;
				minodo.valor = mejorValor;
				minodo.move = hijo.getM();
			}
		}
		return minodo;
	}
}

Move ElenaBot::nextMove(const vector<Move> &adversary, const GameState &state) {

	int profundidad = 2;
	Player turno = state.getCurrentPlayer();
	bool soyj1 = turno == J1;
	nodo origen(state, M_NONE, soyj1);
	long timeout = this->getTimeOut();
	Anodo minodo;

	setPlayer(turno);
  //FUNCION MINIMAX
	//esta funcion devuelve el valor correspondiente a cada nodo
	minodo = minimax_function(origen, profundidad);

	cerr << "Fin MINIMAX" << endl;

	Move movimiento = minodo.move;

	return movimiento;
}
