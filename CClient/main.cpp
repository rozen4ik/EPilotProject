#include <iostream>
#include "pilot_nt.h"

int main()
{
	setlocale(LC_ALL, "");

	auth_answer argument;
	memset(&argument, 0, sizeof(argument));
	argument.TType = OP_PIL_OT_TOTALS;
	int error = close_day(&argument);

	std::system("pause");
	return 0;
}