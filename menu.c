#include <stdio.h>
#include "menu.h"

int main(int argc, char** argv)
{
	char choice;

	printf("\033[32m############This program can create and overlay sounds################\n");

	do
	{
		static char i = 0;
		printf("\033[31m######################################################################\033[32m\n");
		printf("\t1. Create simple sound.\n");
		printf("\t2. Overlay sounds.\n");
		printf("\t3. Exit\n");
		printf("Your choice is: ");
		if (i != 0)
			getchar();
		while ((choice = getchar()) != '1' && choice != '2' && choice != '3')
		{
			while (getchar() != '\n')
				continue;
			printf("\033[31mSorry, buy you have entered incorrect data!\n");
			printf("\033[32mYour choice is: ");
		}
		getchar();
		if (choice == '1')
			creating_sound();
		else if (choice == '2')
			Overlay();
		else
			return 0;
	} while (1);
	return 0;
}