
#include "longdigit.h"
#include "string.h"
#include "stdio.h"

char str1[] = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF";
char str2[] = "01";
char str3[500] = "";

void longdigits::readHexString(CHAR *hexString)
{
	if (hexString)
	{
		UINT32 len  = strlen(hexString);
		CHAR *hexStringtemp =  new CHAR[len + 1];
		linkedList *list = longdigit;
		CHAR *end_ptr;
		INT32 start_index;
		UINT32 value;
		strcpy(hexStringtemp, hexString);
		for (INT32 i = len-1; i > 0; i -= 8)
		{
			end_ptr = &hexStringtemp[i];
			hexStringtemp[i + 1] = 0;
			start_index = max(i - 7, 0);
			value = strtoul(&hexStringtemp[start_index], &end_ptr, 16);
			list = append_digit(list, value);
		}

	}
}

void longdigits::writeHexString(char **hexString)
{
	INT32 count = counter;
	char *str = new CHAR[counter * 8 + 1];
	str[0] = NULL;
	linkedList *list = longdigit;
	for (INT32 i = 0; i < counter; i++)
	{
		if (list->get_next())
			list = list->get_next();

	}
	for (INT32 i = 0; i < counter; i++)
	{
		if (list)
		{
			INT32 idx = strlen(str);
			UINT32 value = list->get_value();
			sprintf(&str[idx], "%08x", value);
			list = list->get_prev();
		}
	}
	*hexString = str;
}

linkedList* longdigits::append_digit(linkedList* prev, UINT32 value)
{
	linkedList *list = new linkedList(value, prev);
	if (counter == 0)
		longdigit = list;
	counter++;
	return list;
}

longdigits longdigits::operator + (longdigits const &num)
{
	longdigits sum;
	UINT64 v1 = 0;
	UINT32 v2 = 0;
	linkedList *list1 = this->longdigit;
	linkedList *list2 = num.longdigit;
	linkedList *sum_list = sum.longdigit;

	while (list1 || list2)
	{
		if (list1)
		{
			v1 += list1->get_value();
			list1 = list1->get_next();
		}
		if (list2)
		{
			v1 += list2->get_value();
			list2 = list2->get_next();
		}
		v2 = v1 & 0xFFFFFFFF;
		v1 >>= 32;
		sum_list = sum.append_digit(sum_list, v2);
	}
	if (v1)
	{
		sum_list = sum.append_digit(sum_list, v1);
	}
	return sum;
}

void main()
{
	CHAR *strtemp;
	longdigits num1, num2, num3;
	num1.readHexString(str1);
	num2.readHexString(str2);
	num3 = num1 + num2;
	num3.writeHexString(&strtemp);
	int value = strlen(strtemp);
	value++;

}