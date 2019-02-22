
#include "TableFormatter.h"


int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("Incorrect arguments count.\n\r");
		return 0;
	}

	TableFormatter tableFormatter;

	tableFormatter.Parse(argv[1]);

	return 0;
}
