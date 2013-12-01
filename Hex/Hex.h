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

class IMinMaxPlayer : public IPlayer
{
protected:
	virtual int Evaluate(Hex &hexBoard);
	void GetPossibleFields(const Hex &hexBoard, vector<Hex> &boards, vector<coordinates> &coords);
public:
	IMinMaxPlayer(PlayerColor playerColor);
	virtual ~IMinMaxPlayer();

	virtual bool TryTurn(Hex& hexBoard) = 0;
};

class MinMaxPlayer : public IMinMaxPlayer
{
private:
	turn Min(Hex &hexBoard, const coordinates& coord, unsigned int level);
	turn Max(Hex &hexBoard, const coordinates& coord, unsigned int level);
public:
	MinMaxPlayer(PlayerColor playerColor);
	virtual ~MinMaxPlayer();

	bool TryTurn(Hex& hexBoard);
};

class AlphaBetaPlayer : public IMinMaxPlayer
{
private:
	turn Min(Hex &hexBoard, const coordinates& coord, int alpha, int beta, unsigned int level);
	turn Max(Hex &hexBoard, const coordinates& coord, int alpha, int beta, unsigned int level);
public:
	AlphaBetaPlayer(PlayerColor playerColor);
	virtual ~AlphaBetaPlayer();

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

	//  Returns the color of the winner if there is one. Otherwise returns NONE (form PlayerColor enum)
	PlayerColor GetWinner();

	//  Make a turn in the hex game
	PlayerColor MakeTurn();
public:
	//  Construct a hex board of given size
	explicit Hex(unsigned int size);
	Hex(const Hex& hex);
	Hex(Hex &&hex);
	~Hex();

	PlayerColor Play();

	//  Outputs the hex board
	friend ostream &operator<< (ostream &os, const Hex &hexGame);

	friend HumanPlayer;
	friend RandomStrategyPlayer;
	friend IMinMaxPlayer;
	friend MinMaxPlayer;
	friend AlphaBetaPlayer;
};

#endif