
#include "Core.h"

bool outputMessage(string message)
{
	ofstream out;
	out.open("Log/log.txt", ios::app);

	if (out.is_open())
	{
		out << getCurrentTimeMG();
		out << message << "\n";

		out.close();
		return true;
	}
	return false;
}