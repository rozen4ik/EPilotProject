#include <iostream>
#include "pilot_nt.h"

int main()
{
	setlocale(LC_ALL, "");

	int command;
	bool loop = true;

	while (loop)
	{
		std::cout << "1 - Закрыть смену" << std::endl;
		std::cout << "2 - Произвести оплату" << std::endl;
		std::cout << "3 - Возврат" << std::endl;
		std::cout << "4 - Ожидание карты" << std::endl;
		std::cout << "5 - Аварийная отмена платежа" << std::endl;

		std::cin >> command;
		int error;

		switch (command)
		{
			case 1:
				auth_answer argument1;
				memset(&argument1, 0, sizeof(argument1));

				error = close_day(&argument1);;

				std::cout << argument1.RCode << std::endl;
				for (char c : argument1.RCode)
					std::cout << std::hex << std::uppercase << int(c);
				std::cout << std::endl;

				//if (argument1.Check)
				//	::GlobalFree(argument1.Check);
				break;
			case 2:
				auth_answer14 argument2;
				memset(&argument2, 0, sizeof(argument2));
				argument2.ans.Amount = 10000;
				argument2.ans.TType = OP_PURCHASE;

				error = card_authorize15(NULL, &argument2, NULL, 0, 0);

				//if (argument2.ans.Check)
				//	::GlobalFree(argument2.ans.Check);
				break;
			case 3:
				auth_answer14 argument3;
				memset(&argument3, 0, sizeof(argument3));
				argument3.ans.Amount = 10000;
				argument3.ans.TType = OP_RETURN;

				error = card_authorize15(NULL, &argument3, NULL, 0, 0);

				//if (argument3.ans.Check)
				//	::GlobalFree(argument3.ans.Check);
				break;
			case 4:
				error = ReadCardContext(0);
				break;
			case 5:
				error = RollbackTrx(10000, NULL);
				break;
			default:
				loop = false;
				break;
		}
	}	


	std::system("pause");
	return 0;
}