/*
 * ManuPCBot.h
 *
 *  Created on: 15 ene. 2018
 *      Author: manupc
 */

#include "Bot.h"
#include <list>
#include <iostream>

#ifndef MANUPCBOT_H_
#define MANUPCBOT_H_

struct Valor_Move{	//Lo que nos debe devolver el algoritmo
	int valor;
	Move move;
};

class nodo{
	private:
		GameState juego;	//informacion de cada jugador y piezas
		Move movimiento; //movimiento desde el padre
		bool PlayerMax;
	public:
		nodo(GameState game, Move m, bool max);

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
		void Crear_Hijos(list<nodo>& hijos);

};


class ElenaBot:Bot {
public:
	ElenaBot();
	~ElenaBot();

	//Heuristica del nodo hoja
	int heuristica(const GameState& game);
	Valor_Move minimax_poda_function(nodo& n, int profundidad, int alpha, int beta);

	void initialize();
	string getName();
	Move nextMove(const vector<Move> &adversary, const GameState &state);
};

#endif /* MANUPCBOT_H_ */
