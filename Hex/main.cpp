#include "Hex.h"

int main()
{
	Hex hex(5);


	PlayerColor winner = hex.Play();


	if (winner == RED)
		cout << "RED won\n";
	else
		cout << "BLUE won\n";

	cin.get();
	cin.get();
	return 0;
}