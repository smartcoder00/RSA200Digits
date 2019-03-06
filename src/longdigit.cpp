
#include "longdigit.h"
#include "string.h"
#include "stdio.h"

char str1[] = "5555555555555555555555555";
char str2[] = "1000000";
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
		for (INT32 i = len-1; i >= 0; i -= 8)
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
			if (i == 0) {
				sprintf(&str[idx], "%x", value);
			}
			else {
				sprintf(&str[idx], "%08x", value);
			}
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

void longdigits::delete_digit(linkedList* list)
{
	delete list;
	counter--;
	if (counter == 0)
		longdigit = NULL;
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

longdigits longdigits::operator += (longdigits const &num)
{
	UINT64 v1 = 0;
	UINT32 v2 = 0;
	linkedList *list1 = this->longdigit;
	linkedList *listp = NULL;
	linkedList *listn = NULL;
	linkedList *list2 = num.longdigit;

	while (list1 || list2)
	{
		if (list1)
		{
			v1 += list1->get_value();
			listp = list1;
			list1 = list1->get_next();
		}
		if (list2)
		{
			v1 += list2->get_value();
			list2 = list2->get_next();
		}
		v2 = v1 & 0xFFFFFFFF;
		v1 >>= 32;
		if (listp)
		{
			listp->set_value(v2);
			listn = listp;
			listp = NULL;
		}
		else
		{
			listn = this->append_digit(listn, v2);
		}
	}
	if (v1)
	{
		this->append_digit(listp, v1);
	}
	return *this;
}

longdigits longdigits::operator - (longdigits const &num)
{
	longdigits diff;
	UINT64 v1 = 0;
	UINT32 v2 = 0;
	UINT32 c1 = 0;
	linkedList *list1 = this->longdigit;
	linkedList *list2 = num.longdigit;
	linkedList *diff_list = diff.longdigit;

	while (list1 || list2)
	{
		if (list1)
		{
			v1 = list1->get_value();
			list1 = list1->get_next();
		}
		if (list2)
		{
			v2 = list2->get_value();
			list2 = list2->get_next();
		}
		if (v1 < v2 + c1)
		{
			v1 += 0x100000000;
			v1 -= c1; c1 = 1;

		}
		else
		{
			v1 -= c1; c1 = 0;
		}
		v1 -= v2;
		if (list1 || list2 || v1)
		{
			diff_list = diff.append_digit(diff_list, v1);
		}
		v2 = v1 = 0;
	}
	return diff;
}

bool longdigits::operator > (longdigits const &num)
{
	UINT64 v1 = 0;
	UINT32 v2 = 0;
	UINT32 c1 = 0;
	linkedList *list1 = this->longdigit;
	linkedList *list2 = num.longdigit;

	if (counter > num.counter) return true;
	else if (counter < num.counter) return false;

	for (INT32 i = 0; i < counter; i++)
	{
		if (list1->get_next())
			list1 = list1->get_next();
		if (list2->get_next())
			list2 = list2->get_next();

	}
	for (INT32 i = 0; i < counter; i++)
	{
		if (list1 || list2)
		{
			if (list1->get_value() == list2->get_value())
			{
				list1 = list1->get_prev();
				list2 = list2->get_prev();

			}
			else if (list1->get_value() > list2->get_value()) return true;
			else return false;
		}
	}
	return false;
}

bool longdigits::operator >= (longdigits const &num)
{
	UINT64 v1 = 0;
	UINT32 v2 = 0;
	UINT32 c1 = 0;
	linkedList *list1 = this->longdigit;
	linkedList *list2 = num.longdigit;

	if (counter > num.counter) return true;
	else if (counter < num.counter) return false;

	for (INT32 i = 0; i < counter; i++)
	{
		if (list1->get_next())
			list1 = list1->get_next();
		if (list2->get_next())
			list2 = list2->get_next();

	}
	for (INT32 i = 0; i < counter; i++)
	{
		if (list1 || list2)
		{
			if (list1->get_value() == list2->get_value())
			{
				list1 = list1->get_prev();
				list2 = list2->get_prev();

			}
			else if (list1->get_value() > list2->get_value()) return true;
			else return false;
		}
	}
	return true;
}

bool longdigits::operator <= (longdigits const &num)
{
	if (*this > num)
		return false;
	return true;
}

bool longdigits::operator < (longdigits const &num)
{
	if (*this > num || *this == num)
		return false;
	return true;
}

bool longdigits::operator == (longdigits const &num)
{
	UINT64 v1 = 0;
	UINT32 v2 = 0;
	UINT32 c1 = 0;
	linkedList *list1 = this->longdigit;
	linkedList *list2 = num.longdigit;

	if (counter != num.counter) return false;

	for (INT32 i = 0; i < counter; i++)
	{
		if (list1->get_next())
			list1 = list1->get_next();
		if (list2->get_next())
			list2 = list2->get_next();

	}
	for (INT32 i = 0; i < counter; i++)
	{
		if (list1 || list2)
		{
			if (list1->get_value() == list2->get_value())
			{
				list1 = list1->get_prev();
				list2 = list2->get_prev();

			}
			else return false;
		}
	}
	return true;
}

bool longdigits::operator == (UINT32 num)
{
	if (counter != 1 )
		return false;
	linkedList *list = this->longdigit;
	if (list->get_value() == num)
		return true;
	return false;
}

bool longdigits::operator > (UINT32 num)
{
	if (counter > 1)
		return true;
	linkedList *list = this->longdigit;
	if(counter == 1)
		if (list->get_value() > num) return true;
	return false;
}

bool longdigits::operator >= (UINT32 num)
{
	return (*this > num) | (*this == num);
}

bool longdigits::operator < (UINT32 num)
{
	if (counter > 1)
		return false;
	linkedList *list = this->longdigit;
	if (counter == 1)
		if (list->get_value() < num) return true;
	return false;
}

bool longdigits::operator <= (UINT32 num)
{
	return (*this < num) | (*this == num);
}


longdigits longdigits::operator + (UINT32 value)
{
	UINT64 v1 = value;
	UINT32 v2 = 0;
	linkedList *list = this->longdigit;

	while (list)
	{
		if (list)
		{
			v1 += list->get_value();
			v2 = v1 & 0xFFFFFFFF;
			list->set_value(v2);
			v1 >>= 32;
			list = list->get_next();
		}
	}
	if (v1)
	{
		this->append_digit(list, v1);
	}
	return *this;
}

longdigits longdigits::operator ++ ()
{
	return *this + 1;
}

longdigits longdigits::operator -- ()
{
	return *this - 1;
}

longdigits longdigits::operator - (UINT32 value)
{
	UINT64 v1 = value;
	UINT32 v2 = 0;
	linkedList *list = this->longdigit;

	while (list && value)
	{
		if (list && value)
		{
			v1 = list->get_value();
			if (v1 < value) {
				v1 = v1 + 0x100000000 - value;
				value = 1;
			}
			else
			{
				v1 -= value;
				value = 0;
			}
			list->set_value(v1);
			if (!list->get_next() && !v1)
			{
				this->delete_digit(list);
				list = NULL;
			}
			else
			{
				list = list->get_next();
			}
		}
	}
	return *this;
}

longdigits longdigits::operator ~ ()
{
	longdigits sum;
	UINT32 v1 = 0;
	linkedList *list = this->longdigit;

	while (list)
	{
		if (list)
		{
			v1 = ~(list->get_value());
			list->set_value(v1);
			list = list->get_next();
		}
	}
	return *this;
}

void longdigits::copy(longdigits &num)
{
	linkedList *list1 = this->longdigit;
	linkedList *list2 = NULL;
	while (list1)
	{
		if (list1)
		{
			list2 = num.append_digit(list2,list1->get_value());
			list1 = list1->get_next();
		}
	}
}

longdigits longdigits::operator * (longdigits &num)
{
	longdigits num1;
	longdigits num2;
	longdigits product;
	num.copy(num2);
	do
	{
		num1 += *this;

	} while (--num2 > 0);
	
	return num1;
}


void main()
{
	CHAR *strtemp;
	longdigits num1, num2, num3, num4;
	num1.readHexString(str1);
	num1.writeHexString(&strtemp);
	printf("num1: %s\n", strtemp);
	num2.readHexString(str2);;
	num2.writeHexString(&strtemp);
	printf("num2: %s\n", strtemp);

	num3 = num1 + num2;
	num3.writeHexString(&strtemp);
	printf("num3(sum): %s\n", strtemp);
	num3 = num1 - num2;
	num3.writeHexString(&strtemp);
	printf("num3(diff): %s\n", strtemp);
	//~num2;
	num2.writeHexString(&strtemp);
	printf("~num2: %s\n", strtemp);
	//++num2;
	num2.writeHexString(&strtemp);
	printf("++num2: %s\n", strtemp);
	//--num2;
	num2.writeHexString(&strtemp);
	printf("--num2: %s\n", strtemp);
	printf("%d %d %d\n", num1 >= num2, num1 <= num1, num1 == num2);
	num4 = num1 * num2;
	num4.writeHexString(&strtemp);
	printf("num4: %s\n", strtemp);
	int value = strlen(strtemp);
	value++;

}