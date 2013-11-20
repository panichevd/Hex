#include "Hex.h"

Hex::Hex(unsigned int size) : m_Size(size), m_HexBoard(size*size + 4)
{
	//  line 1 
	m_HexBoard.AddEdge(0, 1);
	m_HexBoard.AddEdge(0, size);

	for (unsigned int i = 1; i < size - 1; ++i)
	{
		m_HexBoard.AddEdge(i, i - 1);
		m_HexBoard.AddEdge(i, i + 1);
		m_HexBoard.AddEdge(i, size + i - 1);
		m_HexBoard.AddEdge(i, size + i);
	}

	m_HexBoard.AddEdge(size - 1, size - 2);
	m_HexBoard.AddEdge(size - 1, size + size - 2);
	m_HexBoard.AddEdge(size - 1, size + size - 1);

	//  lines 2 to size - 1
	for (unsigned int i = 1; i < size -1; ++i)
	{
		m_HexBoard.AddEdge(i*size, (i - 1)*size);
		m_HexBoard.AddEdge(i*size, (i - 1)*size + 1);
		m_HexBoard.AddEdge(i*size, i*size + 1);
		m_HexBoard.AddEdge(i*size, (i + 1)*size);

		for (unsigned int j = 1; j < size - 1 ; ++j)
		{
			m_HexBoard.AddEdge(i*size + j, (i - 1)*size + j);
			m_HexBoard.AddEdge(i*size + j, (i - 1)*size + j + 1);
			m_HexBoard.AddEdge(i*size + j, i*size + j - 1);
			m_HexBoard.AddEdge(i*size + j, i*size + j + 1);
			m_HexBoard.AddEdge(i*size + j, (i + 1)*size + j - 1);
			m_HexBoard.AddEdge(i*size + j, (i + 1)*size + j);
		}

		m_HexBoard.AddEdge(i*size + size - 1, (i - 1)*size + size - 1);
		m_HexBoard.AddEdge(i*size + size - 1, i*size + size - 2);
		m_HexBoard.AddEdge(i*size + size - 1, (i + 1)*size + size - 2);
		m_HexBoard.AddEdge(i*size + size - 1, (i + 1)*size + size - 1);
	}

	//  line size
	m_HexBoard.AddEdge((size - 1)*size, (size - 2)*size);
	m_HexBoard.AddEdge((size - 1)*size, (size - 2)*size + 1);
	m_HexBoard.AddEdge((size - 1)*size, (size - 1)*size + 1);

	for (unsigned int i = 1; i < size - 1; ++i)
	{
		m_HexBoard.AddEdge((size - 1)*size + i, (size - 2)*size + i);
		m_HexBoard.AddEdge((size - 1)*size + i, (size - 2)*size + i + 1);
		m_HexBoard.AddEdge((size - 1)*size + i, (size - 1)*size + i - 1);
		m_HexBoard.AddEdge((size - 1)*size + i, (size - 1)*size + i + 1);
	}

	m_HexBoard.AddEdge((size - 1)*size + size - 1, (size - 1)*size + size - 2);
	m_HexBoard.AddEdge((size - 1)*size + size - 1, (size - 2)*size + size - 1);

	//  Virtual vertices
	m_HexBoard.SetVertexColor(size*size, BLUE);
	for (unsigned int i = 0; i < size; ++i)
	{
		m_HexBoard.AddEdge(size*size, i*size, BLUE);
		m_HexBoard.AddEdge(i*size, size*size, BLUE);
	}

	m_HexBoard.SetVertexColor(size*size + 1, BLUE);
	for (unsigned int i = 0; i < size; ++i)
	{
		m_HexBoard.AddEdge(size*size + 1, i*size + size - 1, BLUE);
		m_HexBoard.AddEdge(i*size + size - 1, size*size + 1, BLUE);
	}

	m_HexBoard.SetVertexColor(size*size + 2, RED);
	for (unsigned int i = 0; i < size; ++i)
	{
		m_HexBoard.AddEdge(size*size + 2, i, RED);
		m_HexBoard.AddEdge(i, size*size + 2, RED);
	}

	m_HexBoard.SetVertexColor(size*size + 3, RED);
	for (unsigned int i = 0; i < size; ++i)
	{
		m_HexBoard.AddEdge(size*size + 3, (size - 1)*size + i, RED);
		m_HexBoard.AddEdge((size - 1)*size + i, size*size + 3, RED);
	}
}

Hex::~Hex()
{
}

bool Hex::GetVertexNumber(const string &turn, unsigned int &result) const
{
	int pos;
	string numberPart, alphaPart;

	for (pos = 0; pos < turn.size() && isdigit(turn[pos]); ++pos)
		numberPart += turn[pos];

	if (pos == 0 || pos == turn.size())
		return false;

	alphaPart = turn.substr(pos);

	result = (stoul(numberPart) - 1) * m_Size;
	result += alphaPart[0] - 'A';  //  Consider Z is a max value for alphabetic part

	return true;
}

void Hex::SetEdgesColors(unsigned int hexNumber)
{
	const list<Edge> &edges = m_HexBoard.GetNodeEdges(hexNumber);
	for (auto it = edges.begin(); it != edges.end(); ++it)
	{
		unsigned int endHexNumber = it->GetEndVertexNumber();
		if (m_HexBoard.GetVertexColor(endHexNumber) == m_HexBoard.GetVertexColor(hexNumber))
		{
			m_HexBoard.SetEdgeColor(hexNumber, endHexNumber, m_HexBoard.GetVertexColor(hexNumber));
			m_HexBoard.SetEdgeColor(endHexNumber, hexNumber, m_HexBoard.GetVertexColor(hexNumber));
		}
	}
}

bool Hex::MakeTurn(const string &turn, PlayerColor playerColor)
{
	unsigned int vertexNum;
	if (!GetVertexNumber(turn, vertexNum))
	{
		cout << "No such hex on the board! Try again.\n";
		return false;
	}

	if (m_HexBoard.GetVertexColor(vertexNum) != NONE)
	{
		cout << "This hex is already occupied. Try again.\n";
		return false;
	}

	m_HexBoard.SetVertexColor(vertexNum, playerColor);
	SetEdgesColors(vertexNum);
	return true;
}

PlayerColor Hex::GetWinner()
{
	//  BLUE wins if two his virtual vertices are connected (left and right)
	auto blueVertices = m_HexBoard.GetConnections(m_Size*m_Size);
	if (find(blueVertices.begin(), blueVertices.end(), m_Size*m_Size + 1) != blueVertices.end())
		return BLUE;

	//  RED wins if two his virtual vertices are connected (upper and lower)
	auto redVertices = m_HexBoard.GetConnections(m_Size*m_Size + 2);
	if (find(redVertices.begin(), redVertices.end(), m_Size*m_Size + 3) != redVertices.end())
		return RED;

	return NONE;
}

//  doesn't support field size > alphabet letters amount
char Hex::GetXCoord(unsigned int xCoord) const
{
	return 'A' + xCoord;
}

ostream &operator<<(ostream &os, const Hex &hexGame)
{
	os << " ";
	for (unsigned int i = 0; i < hexGame.m_Size; ++i)
		os << " " << hexGame.GetXCoord(i);
	os << endl;

	for (unsigned int i = 0; i < hexGame.m_Size; ++i)
	{
		unsigned j;
		for (j = 0; j < i - (i + 1)/10; ++j)
			os << " ";
		os << i + 1 << " ";

		for (unsigned int j = 0; j < hexGame.m_Size; ++j)
		{
			switch (hexGame.m_HexBoard.GetVertexColor(i*hexGame.m_Size + j))
			{
			case NONE:
				os << ".";
				break;
			case RED:
				os << "x";
				break;
			case  BLUE:
				os << "o";
				break;
			}
			os << " ";
		}
		os << i + 1 << endl;
	}

	for (unsigned int i = 0; i < hexGame.m_Size; ++i)
		os << " ";
	for (unsigned int i = 0; i < hexGame.m_Size; ++i)
		os << " " << hexGame.GetXCoord(i);
	os << endl;

	return os;
}