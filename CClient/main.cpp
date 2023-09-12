#include <iostream>
#include "pilot_nt.h"

int main()
{
	setlocale(LC_ALL, "");

	int command;
	bool loop = true;

	while (loop)
	{
		std::cout << std::endl;
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
				argument1.TType = 7;
				std::cout << "SizeOf ammessage: " << sizeof(argument1.AMessage) << std::endl;


				error = _close_day(&argument1);;

				std::cout << argument1.RCode << std::endl;

				for (int i = 0; i < 32; i++)
					std::cout << argument1.Check[i];

				std::cout << std::endl;

				//if (argument1.Check)
				//	::GlobalFree(argument1.Check);
				break;
			case 2:
				auth_answer14 argument2;
				memset(&argument2, 0, sizeof(argument2));
				argument2.ans.Amount = 10000;
				argument2.ans.TType = OP_PURCHASE;

				error = _card_authorize15(NULL, &argument2, NULL, 0, 0);

				//if (argument2.ans.Check)
				//	::GlobalFree(argument2.ans.Check);
				break;
			case 3:
				auth_answer14 argument3;
				memset(&argument3, 0, sizeof(argument3));
				argument3.ans.Amount = 10000;
				argument3.ans.TType = OP_RETURN;

				error = _card_authorize15(NULL, &argument3, NULL, 0, 0);

				//if (argument3.ans.Check)
				//	::GlobalFree(argument3.ans.Check);
				break;
			case 4:
			{
				CONTEXT_PTR ctx = _ctxAlloc();
				error = _ReadCardContext(ctx);
				char buff[1024];
				_ctxGetString(ctx, PAR_HASH, buff, 1024);
				for (int i = 0; i < 400; i++)
					std::cout << buff[i];
				std::cout << std::endl;
				_ctxFree(ctx);
				break;
			}
			case 5:
				error = _RollbackTrx(10000, NULL);
				break;
			default:
				loop = false;
				break;
		}
	}	


	std::system("pause");
	return 0;
}