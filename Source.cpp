#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//Храню по 9 знаков.
typedef struct
{ // 1024бита->128байт.Инт хранит 10 знаков, но только до двух млрд, поэтому буду хранить по 9.
// 35*9=315,в 308 попадаем
// 35*4=140 байтю, всего лишь на 9 процентов больше<20.
	int array[35];
	int size;
	int UndefinedBehavior;
} uint1024_t;

uint1024_t from_uint(unsigned long long x)
{
	uint1024_t generate;
	for (int i = 0; i < 34; i++)
		generate.array[i] = 0;
	if (x < 1000000000)
		generate.array[0] = x % 1000000000;
	else
	{
		generate.array[0] = x % 1000000000;
		generate.array[1] = x / 1000000000;
	}
	return (generate);
}

void scanf_value(uint1024_t* x)
{
	x->size = 0;
	x->UndefinedBehavior = 0;
	for (int i = 0; i < 35; i++)
		x->array[i] = 0;
	char* string = (char*)malloc(309 * sizeof(char));
	scanf("%s", string);

	if (strlen(string) > 308)
	{
		x->UndefinedBehavior = 1;
		printf("Your number is bigger than 308!");
	}
	else
	{
		if (strlen(string) % 9 == 0)
			x->size = strlen(string) / 9;
		else
			x->size = (strlen(string) / 9) + 1;

		for (int i = 0; i < x->size; i++)
			x->array[i] = 0;

		int cell = 0;
		for (int i = strlen(string); i > 0; i = i - 9) // передаю указатели на строку и обрубаю ее присваивая терминирующий 0
		{
			string[i] = '\0';
			if (i >= 9)
				x->array[cell] = atoi(string + i - 9);
			else
				x->array[cell] = atoi(string);
			cell++;
		}
	}
	free(string);
}

void printf_value(uint1024_t x)
{
	if (x.UndefinedBehavior == 0)
		for (int i = x.size - 1; i >= 0; i--)
			if (i == x.size - 1)
				printf("%d", x.array[i]);
			else
				printf("%09d", x.array[i]);
	else
		printf("UndefinedBehavior");
}

uint1024_t add_op(uint1024_t x, uint1024_t y)
{
	uint1024_t sum;
	sum.size = 0;
	sum.UndefinedBehavior = 0;
	int maxCell = 0;
	if (x.size >= y.size)
		maxCell = x.size;
	else
		maxCell = y.size;

	for (int i = 0; i < 35; i++)
		sum.array[i] = 0;

	int sumshift = 0;

	for (int i = 0; i < maxCell; ++i)
	{
		sum.array[i] = (x.array[i] + y.array[i] + sumshift) % 1000000000;
		sumshift = (x.array[i] + y.array[i]) / 1000000000;
	}
	if (sumshift != 0)
	{
		sum.array[maxCell] = sumshift;
		maxCell++;
	}
	sum.size = maxCell;
	return sum;
}

uint1024_t substr_op(uint1024_t x, uint1024_t y)
{
	uint1024_t dif;
	dif.size = 0;
	dif.UndefinedBehavior = 0;
	int maxCell = 0;
	if (x.size >= y.size)
		maxCell = x.size;
	else
		maxCell = y.size;
	dif.size = maxCell;
	int difshift = 0;

	for (int i = 0; i < dif.size; i++)
	{
		if (x.array[i] >= y.array[i])
		{
			dif.array[i] = x.array[i] - y.array[i] - difshift;
			difshift = 0;
		}
		else
		{
			dif.array[i] = 1000000000 + x.array[i] - y.array[i] - difshift;
			difshift = 1;
		}
	}
	if (difshift != 0)
		dif.UndefinedBehavior = 1;

	while (dif.array[dif.size - 1] == 0 && dif.size > 1)
		dif.size--;
	return dif;
}

uint1024_t mult_op(uint1024_t x, uint1024_t y)
{
	uint1024_t comp;
	comp.size = 0;
	comp.UndefinedBehavior = 0;
	comp.size = 35;

	for (int i = 0; i < 35; i++)
		comp.array[i] = 0;

	int compshift = 0;

	for (int i = 0; i < x.size; i++)
		for (int j = 0; j < y.size; j++)
		{
			comp.array[i + j] = ((comp.array[i + j] + (unsigned long long)x.array[i] * y.array[j] + compshift) % 1000000000);
			compshift = comp.array[i + j] / 1000000000;
		}
	while (comp.array[comp.size - 1] == 0 && comp.size > 1)
		comp.size--;
	return comp;
}

int main()
{
	uint1024_t n1, n2, sum, dif, comp;
	printf("Enter your first number:");
	scanf_value(&n1);
	printf("Your number is:");
	printf_value(n1);
	printf("\nEnter your second number:");
	scanf_value(&n2);
	printf("Your number is:");
	printf_value(n2);
	sum = add_op(n1, n2);
	dif = substr_op(n1, n2);
	comp = mult_op(n1, n2);
	printf("\nThe sum is:");
	printf_value(sum);
	printf("\nThe difference is:");
	printf_value(dif);
	printf("\nThe composition is:");
	printf_value(comp);

	return 0;
}