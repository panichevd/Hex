#include "Hex.h"

int main()
{
	Hex hex(3);

	hex.MakeTurn("1A", RED);
	hex.MakeTurn("2A", RED);
	hex.MakeTurn("2B", RED);
	hex.MakeTurn("2C", RED);
	hex.MakeTurn("3C", RED);

	cout << hex;

	auto a = hex.GetWinner();
	return 0;
}