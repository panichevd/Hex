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
class Hex;

//  I for Interface
class IPlayer
{
protected:
	PlayerColor m_PlayerColor;
public:
	IPlayer(PlayerColor playerColor);
	virtual ~IPlayer();

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

class RandomStratedyPlayer : public IPlayer
{
public:
	RandomStratedyPlayer(PlayerColor playerColor);
	virtual ~RandomStratedyPlayer();
	
	bool TryTurn(Hex &hexBoard);
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
	~Hex();

	PlayerColor Play();

	//  Outputs the hex board
	friend ostream &operator<< (ostream &os, const Hex &hexGame);
	friend HumanPlayer;
	friend RandomStratedyPlayer;
};

#endif