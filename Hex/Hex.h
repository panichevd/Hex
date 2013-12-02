#ifndef HEX_H__
#define HEX_H__

#include <iostream>
#include <algorithm>
#include "Graph.h"

using std::ostream;
using std::cin;
using std::cout;
using std::endl;
using std::stoul;

typedef pair<unsigned int, unsigned int> coordinates;
typedef pair<coordinates, int> turn;
class Hex;

//  I for Interface
class IPlayer
{
protected:
	PlayerColor m_PlayerColor;
public:
	IPlayer(PlayerColor playerColor);
	virtual ~IPlayer();

	PlayerColor GetColor() const;

	virtual bool TryTurn(Hex& hexBoard) = 0;
};

class HumanPlayer : public IPlayer
{
private:
	bool GetVertexNumber(const string &turn, coordinates &result) const;
public:
	HumanPlayer(PlayerColor playerColor);
	virtual ~HumanPlayer();

	bool TryTurn(Hex& hexBoard);
};

class RandomStrategyPlayer : public IPlayer
{
public:
	RandomStrategyPlayer(PlayerColor playerColor);
	virtual ~RandomStrategyPlayer();
	
	bool TryTurn(Hex &hexBoard);
};

class IPredictingPlayer : public IPlayer
{
protected:
	PlayerColor InverseColor();
	virtual int Evaluate(Hex &hexBoard);
	void GetPossibleFields(const Hex &hexBoard, vector<Hex> &boards, vector<coordinates> &coords);
public:
	IPredictingPlayer(PlayerColor playerColor);
	virtual ~IPredictingPlayer();

	virtual bool TryTurn(Hex& hexBoard) = 0;
};

class MinMaxPlayer : public IPredictingPlayer
{
private:
	turn Min(Hex &hexBoard, const coordinates& coord, unsigned int level);
	turn Max(Hex &hexBoard, const coordinates& coord, unsigned int level);
public:
	MinMaxPlayer(PlayerColor playerColor);
	virtual ~MinMaxPlayer();

	bool TryTurn(Hex& hexBoard);
};

class AlphaBetaPlayer : public IPredictingPlayer
{
private:
	turn Min(Hex &hexBoard, const coordinates& coord, int alpha, int beta, unsigned int level);
	turn Max(Hex &hexBoard, const coordinates& coord, int alpha, int beta, unsigned int level);
public:
	AlphaBetaPlayer(PlayerColor playerColor);
	virtual ~AlphaBetaPlayer();

	bool TryTurn(Hex& hexBoard);
};

class MonteCarloAlphaBetaPlayer : public IPredictingPlayer
{
private:
	unsigned int m_InitialLevel;
	unsigned int m_Simulations;

	int Evaluate(Hex &hexBoard);
	turn Min(Hex &hexBoard, const coordinates& coord, int alpha, int beta, unsigned int level);
	turn Max(Hex &hexBoard, const coordinates& coord, int alpha, int beta, unsigned int level);
public:
	MonteCarloAlphaBetaPlayer(PlayerColor playerColor);
	virtual ~MonteCarloAlphaBetaPlayer();

	bool TryTurn(Hex& hexBoard);
};

class MonteCarloPlayer : public IPredictingPlayer
{
private:
	unsigned int m_Simulations;

	int Evaluate(Hex &hexBoard);
public:
	MonteCarloPlayer(PlayerColor playerColor);
	virtual ~MonteCarloPlayer();

	bool TryTurn(Hex& hexBoard);
};

//  Implements the game of hex. The board is represented as a graph that has size*size vertices(hexagons)
//  and 4 virtual vertices used to determine a winner.
//  Each player has two virtual vertices connected to all the vertices on opposite sides of the board
//  Each virtual vertex represents a side. 
//  A player wins if he connects his 2 virtual vertices (i.e. opposite sides of the board).
class Hex
{
private:
	//  Virtual vertex indexes
	const int m_Left, m_Right, m_Top, m_Bottom;
	Graph m_HexBoard;
	unsigned int m_Size;
	unsigned int m_Empty;

	IPlayer *m_Player1, *m_Player2;
	int m_NextPlayer;

	char GetXCoord(unsigned int xCoord) const;
	//  Gets vertex number in the Graph from its position on the board
	bool GetVertexNumber(const string &turn, unsigned int &result) const;
	//  Colors all the edges coming outta vertex that don't have a color yet
	void SetEdgesColors(const coordinates &coord);

	PlayerColor GetVertexColor(const coordinates &coord) const;
	bool SetVertexColor(const coordinates &coord, PlayerColor playerColor);

	PlayerColor GetNextPlayerColor() const;

	//  Returns the color of the winner if there is one. Otherwise returns NONE (form PlayerColor enum)
	PlayerColor GetWinner();

	//  Make a turn in the hex game
	PlayerColor MakeTurn();

	PlayerColor MakeTurnInSimulation();
public:
	//  Construct a hex board of given size
	explicit Hex(unsigned int size);
	Hex(const Hex& hex);
	Hex(Hex &&hex);
	~Hex();

	PlayerColor Play();
	PlayerColor RandomSimulation();

	//  Outputs the hex board
	friend ostream &operator<< (ostream &os, const Hex &hexGame);

	friend HumanPlayer;
	friend RandomStrategyPlayer;
	friend IPredictingPlayer;
	friend MinMaxPlayer;
	friend AlphaBetaPlayer;
	friend MonteCarloAlphaBetaPlayer;
	friend MonteCarloPlayer;
};

#endif