#include "Hex.h"

IPlayer::IPlayer(PlayerColor playerColor) : m_PlayerColor(playerColor)
{
}

IPlayer::~IPlayer()
{
}

PlayerColor IPlayer::GetColor() const
{
	return m_PlayerColor;
}

HumanPlayer::HumanPlayer(PlayerColor playerColor) : IPlayer(playerColor)
{
}

HumanPlayer::~HumanPlayer()
{
}

bool HumanPlayer::GetVertexNumber(const string &turn, coordinates &result) const
{
	int pos;
	string numberPart, alphaPart;

	for (pos = 0; pos < turn.size() && isdigit(turn[pos]); ++pos)
		numberPart += turn[pos];

	if (pos == 0 || pos == turn.size())
		return false;

	alphaPart = turn.substr(pos);

	result.first = stoul(numberPart) - 1;
	result.second = alphaPart[0] - 'A';  //  Consider Z is a max value for alphabetic part

	return true;
}

bool HumanPlayer::TryTurn(Hex& hexBoard)
{
	string turn;
	coordinates coord;

	cout << "Input your turn: ";
	cin >> turn;
	cout << "\n";
	if (!GetVertexNumber(turn, coord) || coord.first > hexBoard.m_Size || coord.second > hexBoard.m_Size)
	{
		cout << "\nNo such hex on the board! Try again.\n\n";
		return false;
	}

	if (hexBoard.GetVertexColor(coord) != NONE)
	{
		cout << "\nThis hex is already occupied. Try again.\n\n";
		return false;
	}

	hexBoard.SetVertexColor(coord, m_PlayerColor);
	hexBoard.SetEdgesColors(coord);
	return true;
}

RandomStrategyPlayer::RandomStrategyPlayer(PlayerColor playerColor) : IPlayer(playerColor)
{
}

RandomStrategyPlayer::~RandomStrategyPlayer()
{
}

bool RandomStrategyPlayer::TryTurn(Hex &hexBoard)
{
	coordinates coord;

	coord.first = rand() % hexBoard.m_Size;
	coord.second = rand() % hexBoard.m_Size;
	if (hexBoard.GetVertexColor(coord) != NONE)
		return false;

	hexBoard.SetVertexColor(coord, m_PlayerColor);
	hexBoard.SetEdgesColors(coord);
	cout << "\nComputer turn is: " << coord.first + 1 << static_cast<char>('A' + coord.second) << "\n\n";
	return true;
}

IMinMaxPlayer::IMinMaxPlayer(PlayerColor playerColor) : IPlayer(playerColor)
{
}

IMinMaxPlayer::~IMinMaxPlayer()
{
}

int IMinMaxPlayer::Evaluate(Hex &hexBoard)
{
	if (hexBoard.GetWinner() == m_PlayerColor)
		return 5;
	else if (hexBoard.GetWinner() == m_PlayerColor)
		return -5;
	else
		return 0;
}

void IMinMaxPlayer::GetPossibleFields(const Hex &hexBoard, vector<Hex> &boards, vector<coordinates> &coords)
{
	coordinates coord;

	for (coord.first = 0; coord.first < hexBoard.m_Size; ++coord.first)
		for (coord.second = 0; coord.second < hexBoard.m_Size; ++coord.second)
		{
			Hex hexBoardCopy = hexBoard;

			if (hexBoardCopy.GetVertexColor(coord) == NONE)
			{
				hexBoardCopy.SetVertexColor(coord, m_PlayerColor);
				hexBoardCopy.SetEdgesColors(coord);
				boards.push_back(move(hexBoardCopy));
				coords.push_back(coord);
			}
		}
}

MinMaxPlayer::MinMaxPlayer(PlayerColor playerColor) : IMinMaxPlayer(playerColor)
{
}

MinMaxPlayer::~MinMaxPlayer()
{
}

turn MinMaxPlayer::Min(Hex &hexBoard, const coordinates& coord, unsigned int level)
{
	//  The minimum initial value for level is 1 (because if the board is full there is no intenion to try to make a move)
	turn res, t;

	if (level == 0)
	{
		res.first = coord;
		res.second = Evaluate(hexBoard);
	}
	else
	{
		vector<Hex> boards;
		vector<coordinates> coords;

		GetPossibleFields(hexBoard, boards, coords);
		res.first = coord;
		res.second = INT_MAX;
		auto coordsIt = coords.begin();
		for (auto boardsIt = boards.begin(); boardsIt != boards.end(); ++boardsIt, ++coordsIt)
		{
			t = Max(*boardsIt, *coordsIt, level - 1);
			if (t.second < res.second)
			{
				res.first = *coordsIt;
				res.second = t.second;
			}
		}
	}

	return res;
}

turn MinMaxPlayer::Max(Hex &hexBoard, const coordinates& coord, unsigned int level)
{
	//  The minimum initial value for level is 1 (because if the board is full there is no intenion to try to make a move)
	turn res, t;

	if (level == 0)
	{
		res.first = coord;
		res.second = Evaluate(hexBoard);
	}
	else 
	{
		vector<Hex> boards;
		vector<coordinates> coords;

		GetPossibleFields(hexBoard, boards, coords);
		res.first = coord;
		res.second = INT_MIN;
		auto coordsIt = coords.begin();
		for (auto boardsIt = boards.begin(); boardsIt != boards.end(); ++boardsIt, ++coordsIt)
		{
			t = Min(*boardsIt, *coordsIt, level - 1);
			if (t.second > res.second)
			{
				res.first = *coordsIt;
				res.second = t.second;
			}
		}
	}
	return res;
}

bool MinMaxPlayer::TryTurn(Hex& hexBoard)
{
	turn madeTurn = Max(hexBoard, coordinates(0, 0), hexBoard.m_Empty);

	hexBoard.SetVertexColor(madeTurn.first, m_PlayerColor);
	hexBoard.SetEdgesColors(madeTurn.first);
	return true;
}

AlphaBetaPlayer::AlphaBetaPlayer(PlayerColor playerColor) : IMinMaxPlayer(playerColor)
{
}

AlphaBetaPlayer::~AlphaBetaPlayer()
{
}

turn AlphaBetaPlayer::Min(Hex &hexBoard, const coordinates& coord, int alpha, int beta, unsigned int level)
{
	//  The minimum initial value for level is 1 (because if the board is full there is no intenion to try to make a move)
	turn res, t;

	if (level == 0)
	{
		res.first = coord;
		res.second = Evaluate(hexBoard);
	}
	else
	{
		vector<Hex> boards;
		vector<coordinates> coords;

		GetPossibleFields(hexBoard, boards, coords);
		res.first = coord;
		res.second = beta;
		auto coordsIt = coords.begin();
		for (auto boardsIt = boards.begin(); boardsIt != boards.end(); ++boardsIt, ++coordsIt)
		{
			t = Max(*boardsIt, *coordsIt, alpha, res.second, level - 1);
			if (t.second < res.second)
			{
				res.first = *coordsIt;
				res.second = t.second;
			}
			if (res.second <= alpha)
				return res;
		}
	}

	return res;
}

turn AlphaBetaPlayer::Max(Hex &hexBoard, const coordinates& coord, int alpha, int beta, unsigned int level)
{
	//  The minimum initial value for level is 1 (because if the board is full there is no intenion to try to make a move)
	turn res, t;

	if (level == 0)
	{
		res.first = coord;
		res.second = Evaluate(hexBoard);
	}
	else 
	{
		vector<Hex> boards;
		vector<coordinates> coords;

		GetPossibleFields(hexBoard, boards, coords);
		res.first = coord;
		res.second = alpha;
		auto coordsIt = coords.begin();
		for (auto boardsIt = boards.begin(); boardsIt != boards.end(); ++boardsIt, ++coordsIt)
		{
			t = Min(*boardsIt, *coordsIt, res.second, beta, level - 1);
			if (t.second > res.second)
			{
				res.first = *coordsIt;
				res.second = t.second;
			}
			if (res.second >= beta)
				return res;
		}
	}
	return res;
}

bool AlphaBetaPlayer::TryTurn(Hex& hexBoard)
{
	turn madeTurn = Max(hexBoard, coordinates(0, 0), INT_MIN, INT_MAX, hexBoard.m_Empty);

	hexBoard.SetVertexColor(madeTurn.first, m_PlayerColor);
	hexBoard.SetEdgesColors(madeTurn.first);
	return true;
}

Hex::Hex(unsigned int size) : m_Size(size), m_Empty(m_Size*m_Size), m_HexBoard(size*size + 4),
	m_Left(size*size), m_Right(size*size + 1), m_Top(size*size + 2), m_Bottom(size*size + 3)
{
	srand(time(NULL));

	m_Player1 = new HumanPlayer(RED);
	m_Player2 = new AlphaBetaPlayer(BLUE);
	m_NextPlayer = 1;
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
	m_HexBoard.SetVertexColor(m_Left, BLUE);
	for (unsigned int i = 0; i < size; ++i)
	{
		m_HexBoard.AddEdge(m_Left, i*size, NONE);
		m_HexBoard.AddEdge(i*size, m_Left, NONE);
	}

	m_HexBoard.SetVertexColor(m_Right, BLUE);
	for (unsigned int i = 0; i < size; ++i)
	{
		m_HexBoard.AddEdge(m_Right, i*size + size - 1, NONE);
		m_HexBoard.AddEdge(i*size + size - 1, m_Right, NONE);
	}

	m_HexBoard.SetVertexColor(m_Top, RED);
	for (unsigned int i = 0; i < size; ++i)
	{
		m_HexBoard.AddEdge(m_Top, i, NONE);
		m_HexBoard.AddEdge(i, m_Top, NONE);
	}

	m_HexBoard.SetVertexColor(m_Bottom, RED);
	for (unsigned int i = 0; i < size; ++i)
	{
		m_HexBoard.AddEdge(m_Bottom, (size - 1)*size + i, NONE);
		m_HexBoard.AddEdge((size - 1)*size + i, m_Bottom, NONE);
	}
}

Hex::Hex(const Hex &hex) : m_Size(hex.m_Size), m_Empty(hex.m_Empty), m_HexBoard(hex.m_HexBoard),
	m_Left(hex.m_Left), m_Right(hex.m_Right), m_Top(hex.m_Top), m_Bottom(hex.m_Bottom), m_NextPlayer(hex.m_NextPlayer)
{
	m_Player1 = new HumanPlayer(hex.m_Player1->GetColor());
	m_Player2 = new AlphaBetaPlayer(hex.m_Player2->GetColor());
}

Hex::Hex(Hex &&hex) : m_Size(hex.m_Size), m_Empty(hex.m_Empty), m_HexBoard(move(hex.m_HexBoard)),
	m_Left(hex.m_Left), m_Right(hex.m_Right), m_Top(hex.m_Top), m_Bottom(hex.m_Bottom), m_NextPlayer(hex.m_NextPlayer)
{
	m_Player1 = hex.m_Player1;
	hex.m_Player1 = nullptr;
	m_Player2 = hex.m_Player2;
	hex.m_Player2 = nullptr;
}

Hex::~Hex()
{
	delete m_Player1;
	delete m_Player2;
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

void Hex::SetEdgesColors(const coordinates &coord)
{
	unsigned int vertexIndex = coord.first * m_Size + coord.second;

	const list<Edge> &edges = m_HexBoard.GetNodeEdges(vertexIndex);
	for (auto it = edges.begin(); it != edges.end(); ++it)
	{
		unsigned int endHexNumber = it->GetEndVertexNumber();
		if (m_HexBoard.GetVertexColor(endHexNumber) == m_HexBoard.GetVertexColor(vertexIndex))
		{
			m_HexBoard.SetEdgeColor(vertexIndex, endHexNumber, m_HexBoard.GetVertexColor(vertexIndex));
			m_HexBoard.SetEdgeColor(endHexNumber, vertexIndex, m_HexBoard.GetVertexColor(vertexIndex));
		}
	}
}

bool Hex::SetVertexColor(const coordinates &coord, PlayerColor playerColor)
{
	unsigned int vertexIndex = coord.first * m_Size + coord.second;
	m_Empty--;
	return m_HexBoard.SetVertexColor(vertexIndex, playerColor);
}

PlayerColor Hex::GetVertexColor(const coordinates &coord) const
{
	unsigned int vertexIndex = coord.first * m_Size + coord.second;
	return m_HexBoard.GetVertexColor(vertexIndex);
}

PlayerColor Hex::GetWinner()
{
	//  BLUE wins if two his virtual vertices are connected (left and right)
	auto blueVertices = m_HexBoard.GetConnections(m_Left);
	if (find(blueVertices.begin(), blueVertices.end(), m_Right) != blueVertices.end())
		return BLUE;

	//  RED wins if two his virtual vertices are connected (upper and lower)
	auto redVertices = m_HexBoard.GetConnections(m_Top);
	if (find(redVertices.begin(), redVertices.end(), m_Bottom) != redVertices.end())
		return RED;

	return NONE;
}

PlayerColor Hex::MakeTurn()
{
	IPlayer *currentPlayer;

	if (m_NextPlayer == 1)
		currentPlayer = m_Player1;
	else
		currentPlayer = m_Player2;

	while (!currentPlayer->TryTurn(*this))
		cout << *this;

	m_NextPlayer = !m_NextPlayer;  //  easy way to change player

	return GetWinner();
}

PlayerColor Hex::Play()
{
	PlayerColor winner;
	do
	{
		cout << *this;
		winner = MakeTurn();
	} while (winner == NONE);

	cout << *this;

	return winner;
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
	os << endl << endl;

	return os;
}