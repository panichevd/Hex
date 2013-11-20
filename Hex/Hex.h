#ifndef HEX_H__
#define HEX_H__

#include <iostream>
#include <algorithm>
#include "Graph.h"

using std::ostream;
using std::cout;
using std::endl;
using std::stoul;

//  Implements the game of hex. The board is represented as a graph that has size*size vertices(hexagons)
//  and 4 virtual vertices used to determine a winner.
//  Each player has two virtual vertices connected to all the vertices on opposite sides of the board
//  Each virtual vertex represents a side. 
//  A player wins if he connects his 2 virtual vertices (i.e. opposite sides of the board).
class Hex
{
private:
	Graph m_HexBoard;
	unsigned int m_Size;

	char GetXCoord(unsigned int xCoord) const;
	//  Gets vertex number in the Graph from its position on the board
	bool GetVertexNumber(const string &turn, unsigned int &result) const;
	//  Colors all the edges coming outta vertex that don't have a color yet
	void SetEdgesColors(unsigned int hexNumber);
public:
	//  Construct a hex board of given size
	explicit Hex(unsigned int size);
	~Hex();

	//  Make a turn in the hex game (input is the number of a hex and the color of the player)
	bool MakeTurn(const string &turn, PlayerColor playerColor);
	//  Returns the color of the winner if there is one. Otherwise returns NONE (form PlayerColor enum)
	PlayerColor GetWinner();

	//  Outputs the hex board
	friend ostream &operator<< (ostream &os, const Hex &hexGame);
};

#endif