#include <stdio.h>

int
printScript(int max)
{
	printf("#include <stdio.h>\n");
	printf("\n");
	printf("int\n");
	printf("Double(int n)\n");
	printf("{\n");
	printf("    if (n == 0) {\n");
	printf("        return 0;\n");
	for (int arg = 1; arg <= max; arg++) {
		printf("    } else if (n == %d) {\n", arg);
		printf("        return %d;\n", 2*arg);
	}
	printf("    } else {\n");
	printf("        // to big, just guess a number\n");
	printf("        return 12;\n");
	printf("    }\n");
	printf("}\n");
	printf("\n");
	printf("int\n");
	printf("main(void)\n");
	printf("{\n");
	printf("    return 0;\n");
	printf("}\n");
	return 0;
}

int
main(void)
{
	printScript(10000);
}
