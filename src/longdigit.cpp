
#include "longdigit.h"
#include "string.h"
#include "stdio.h"

char str1[] = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF";
char str2[] = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF";
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
		diff_list = diff.append_digit(diff_list, v1);
		v2 = v1 = 0;
	}
	return diff;
}

longdigits longdigits::operator -= (longdigits const &num)
{
	longdigits diff;
	UINT64 v1 = 0;
	UINT32 v2 = 0;
	UINT32 c1 = 0;
	linkedList *listp = NULL;
	linkedList *listn = NULL;
	linkedList *list1 = this->longdigit;
	linkedList *list2 = num.longdigit;
	linkedList *diff_list = diff.longdigit;

	while (list1 || list2)
	{
		if (list1)
		{
			v1 = list1->get_value();
			listp = list1;
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
		if (listp)
		{
			listp->set_value(v1);
			listn = listp;
			if (!list1 && v1 == 0 && listp->get_prev())
				this->delete_digit(listp);
			listp = NULL;				
		}
		v2 = v1 = 0;
	}
	return *this;
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
	longdigits sum;
	linkedList *list = this->longdigit;
	linkedList *sumlist = NULL;
	
	while (list)
	{
			v1 += list->get_value();
			v2 = v1 & 0xFFFFFFFF;
			v1 >>= 32;
			sumlist = sum.append_digit(sumlist, v2);
			list = list->get_next();
	}
	if (v1)
	{
		sumlist = sum.append_digit(sumlist, v1);
	}
	return sum;
}

void longdigits::scalarmultiply (UINT32 value)
{
	UINT64 v1 = 0;
	UINT32 v2 = 0;
	linkedList *list = this->longdigit;
	linkedList *listp = NULL;

	while (list)
	{
		v1 += ((UINT64)value * list->get_value());
		v2 = v1 & 0xFFFFFFFF;
		list->set_value(v2);
		v1 >>= 32;
		listp = list;
		list = list->get_next();
	}
	if (v1 && listp)
	{
		this->append_digit(listp, v1);
	}
}

longdigits longdigits::operator ++ ()
{
	UINT64 v1 = 1;
	UINT32 v2 = 0;
	linkedList *list = this->longdigit;
	linkedList *listp = NULL;

	while (list)
	{
		v1 += list->get_value();
		v2 = v1 & 0xFFFFFFFF;
		v1 >>= 32;
		list->set_value(v2);
		listp = list;
		list = list->get_next();
	}
	if (v1)
	{
		this->append_digit(listp, v1);
	}
	return *this;
}

longdigits longdigits::operator -- ()
{
	UINT64 v1 = 1;
	UINT32 v2 = 0;
	linkedList *list = this->longdigit;
	linkedList *listp = NULL;

	while (list)
	{
		v1 = list->get_value();
		if (v1 > 0)
		{
			list->set_value(v1 - 1);
			listp = list;
			list = NULL;
		}
		else
		{
			list->set_value(0xFFFFFFFF);
			list = list->get_next();
		}
	}
	if (!listp->get_value() && listp->get_prev() && !listp->get_next())
	{
		this->delete_digit(listp);
	}
	return *this;
}

longdigits longdigits::operator - (UINT32 value)
{
	UINT64 v1 = value;
	UINT32 v2 = 0;
	longdigits diff;
	linkedList *list = this->longdigit;
	linkedList *difflist = NULL;

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
			difflist = diff.append_digit(difflist, v1);
			if (!list->get_next() && !v1 && difflist->get_prev())
			{
				diff.delete_digit(difflist);
				list = NULL;
			}
			else
			{
				list = list->get_next();
			}
		}
	}
	return diff;
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

longdigits longdigits::operator / (longdigits &num)
{
	longdigits quotient;
	longdigits den;
	longdigits product;
	if (num == 0)
		return quotient;
	quotient.append_digit(quotient.longdigit, 0);
	this->copy(den);
	while (den >= num )
	{
		++quotient;
		den -= num;
	}

	return quotient;
}

void longdigits::insertroot(UINT32 value)
{
	linkedList *list = new linkedList(value, longdigit, FALSE);
	longdigit = list;
	counter++;
}

void longdigits::insertemptyroot(UINT32 count)
{
	while (count--)
	{
		linkedList *list = new linkedList(0, longdigit, FALSE);
		longdigit = list;
		counter++;
	}
}

longdigits longdigits::operator * (longdigits &num)
{
	longdigits product;
	longdigits intermediate;
	UINT32     loopcount = 0;

	linkedList *list = this->longdigit;
	product.append_digit(NULL, 0);
	while (list)
	{
		num.copy(intermediate);
		intermediate.scalarmultiply(list->get_value());
		intermediate.insertemptyroot(loopcount++);
		list = list->get_next();
		product += intermediate;
		intermediate.clear();
	}
	return product;
}

void printall(char *tag, longdigits &num1, longdigits &num2, longdigits &num3, longdigits &num4)
{
	CHAR *str;
	printf("== %-15s ========================================================\n", tag);
	num1.writeHexString(&str);
	printf("num1: %s\n", str);
	delete str;
	num2.writeHexString(&str);
	printf("num2: %s\n", str);
	delete str;
	num3.writeHexString(&str);
	printf("num3: %s\n", str);
	delete str;
	num4.writeHexString(&str);
	printf("num4: %s\n", str);
	delete str;
	printf("===========================================================================\n");
}

void main()
{
	CHAR *strtemp;
	longdigits num1, num2, num3, num4;
	num1.readHexString(str1);
	num2.readHexString(str2);;
	printall("Read Data",num1, num2, num3, num4);

	num3 = num1 + num2;
	num4 = num1 + 2;
	printall("Test1", num1, num2, num3, num4);
	num3.clear(); num4.clear();

	num3 = num1 - num2; num1.copy(num4); ~num4;
	printall("Test2", num1, num2, num3, num4);
	num3.clear(); num4.clear();

	num1.copy(num3); num1.copy(num4);
	--num3;++num4;
	printall("Test3", num1, num2, num3, num4);
	num3.clear(); num4.clear();

	num3 = num1 * num2;
	printall("Test4", num1, num2, num3, num4);
	num3.clear(); num4.clear();

	printf("%d %d %d\n", num1 >= num2, num1 <= num1, num1 == num2);
	num4 = num1 * num2;
	num4.writeHexString(&strtemp);
	printf("num4: %s\n", strtemp);
	int value = strlen(strtemp);
	value++;

}