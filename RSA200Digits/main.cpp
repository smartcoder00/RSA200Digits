
#include <string>
#include <fstream>
#include "longdigit.h"
#include "string.h"
#include "stdio.h"

static longdigits inverse(longdigits a, longdigits n)
{
	longdigits inv, newinv;
	longdigits r, newr, quotient, temp;
	longdigits noinv;

	inv.append_digit(0);
	newinv.append_digit(1);
	n.copy(r);
	a.copy(newr);

	while (newr != 0)
	{
		quotient = r / newr;
		temp.clear();
		temp = inv - quotient * newinv;
		inv.clear(); newinv.copy(inv);
		newinv.clear(); temp.copy(newinv);

		temp.clear();
		temp = r - quotient * newr;
		r.clear(); newr.copy(r);
		newr.clear(); temp.copy(newr);
	}
	if (r > 1)
	{
		return noinv; //Not invertible
	}

	if (inv < 0)
	{
		inv += n;
	}
	return inv;
}
bool GetStrings(char **StringList, char *path)
{
	int i = 0, MAX_STRING = 3;
	std::ifstream file(path);
	if (file.is_open()) {
		std::string line;
		while (std::getline(file, line) && i < MAX_STRING) {
			// using printf() in all tests for consistency
			int len = strlen(line.c_str());
			len++;
			StringList[i] = new char[len];
			strcpy(StringList[i], line.c_str());
			i++;
		}
		file.close();
	}
	return TRUE;
}
void main(int argc, char *argv[])
{
	CHAR *StringList[3];
	if (argc >= 2)
	{
		printf("Input File %s\n",argv[1]);
	}
	else
	{
		printf("Need Input File as argument");
		return;
	}
	GetStrings(StringList, argv[1]);
	printf("============== Input ==============\n");
	printf("p: %s\n", StringList[0]);
	printf("q: %s\n", StringList[1]);
	printf("e: %s\n", StringList[2]);
	longdigits p, q, N, PhyN, e, d;
	p.readHexString(StringList[0]);
	q.readHexString(StringList[1]);
	N = p * q;
	PhyN = (p - 1) * (q - 1);
	e.readHexString(StringList[2]);
	d = inverse(e, PhyN);
	CHAR *str;
	printf("============== Private Key Pair ===\n");
	e.writeHexString(&str);
	printf("e: %s\n", str);
	N.writeHexString(&str);
	printf("N: %s\n", str);
	printf("============== Public Key Pair ====\n");
	d.writeHexString(&str);
	printf("d: %s\n", str);
	N.writeHexString(&str);
	printf("N: %s\n", str);
	printf("============== END ================\n");
}