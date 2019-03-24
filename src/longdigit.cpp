
#include "longdigit.h"
#include "string.h"
#include "stdio.h"

char str1[] = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
char str2[] = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
char str3[500] = "";

char str_p[] = "65";
char str_q[] = "67";
char str_e[] = "1F";


void longdigits::readHexString(CHAR *hexString)
{
	if (hexString)
	{
		CHAR signValue = hexString[0];
		if (signValue == '+' || signValue == '-')
		{
			hexString++;
			if (signValue == '-')
				this->setsign(NEGATIVE);
		}
		
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
	char *str = new CHAR[counter * 8 +2];
	str[0] = NULL;
	linkedList *list = longdigit;
	for (INT32 i = 0; i < counter; i++)
	{
		if (list->get_next())
			list = list->get_next();

	}
	if (getsign() == NEGATIVE)
	{
		str[0] = '-'; str[1] = NULL;
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

linkedList* longdigits::append_digit(UINT32 value)
{
	linkedList *list = longdigit;
	linkedList *prev = NULL;
	while(list)
	{
		prev = list;
		list = list->get_next();
	}
	return append_digit(prev,value);
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
	if (counter > 0)
		counter--;
	if (counter == 0)
		longdigit = NULL;
}

void longdigits::trim()
{
	linkedList *list = this->longdigit;
	linkedList *listp = NULL;
	UINT32 counter = this->counter;

	for (INT32 i = 0; i < counter; i++)
	{
		if (list->get_next())
			list = list->get_next();
	}
	if (counter > 0)
		counter--;
	for (INT32 i = 0; i < counter; i++)
	{
		UINT32 value = list->get_value();
		if (value) break;
		listp = list;
		list = list->get_prev();
		this->delete_digit(listp);
	}
}

longdigits longdigits::add(longdigits const &num2) const
{
	longdigits sum;
	UINT64 v1 = 0;
	UINT32 v2 = 0;
	linkedList *list1 = this->longdigit;
	linkedList *list2 = num2.longdigit;
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

longdigits longdigits::sub(longdigits const &num2) const
{
	longdigits diff;
	UINT64 v1 = 0;
	UINT32 v2 = 0;
	UINT32 c1 = 0;
	linkedList *list1 = this->longdigit;
	linkedList *list2 = num2.longdigit;
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

int longdigits::abscompare(longdigits const &num2) const
{
	int RETVAL_GREATER = 1;
	int RETVAL_LESSER = -1;
	int RETVAL_EQUAL = 0;

	linkedList *list1 = this->longdigit;
	linkedList *list2 = num2.longdigit;

	if (counter > num2.counter) return RETVAL_GREATER;
	else if (counter < num2.counter) return RETVAL_LESSER;

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
			else if (list1->get_value() > list2->get_value()) return RETVAL_GREATER;
			else return RETVAL_LESSER;
		}
	}
	return RETVAL_EQUAL;
}

int longdigits::compare(longdigits const &num2) const
{
	int RETVAL_GREATER = 1;
	int RETVAL_LESSER = -1;
	int RETVAL_EQUAL = 0;

	SIGN num1sign, num2sign;
	longdigits const &num1 = *this;

	num1sign = num1.getsign();
	num2sign = num2.getsign();
	if (num1sign > num2sign)
		return RETVAL_GREATER;
	else if (num1sign < num2sign)
		return RETVAL_LESSER;

	//Sign is same after this
	int compareresult = abscompare(num2);

	if (compareresult != RETVAL_EQUAL && num1sign == NEGATIVE)
		compareresult = compareresult == RETVAL_GREATER ? RETVAL_LESSER : RETVAL_GREATER;

	return compareresult;
}

longdigits longdigits::operator + (longdigits const &num2)
{
	SIGN num1sign, num2sign;
	longdigits sum, &num1 = *this;

	num1sign = num1.getsign();
	num2sign = num2.getsign();
	
	if (num1sign == num2sign)
	{
		sum = num1.add(num2);
		sum.setsign(num1sign);
	}
	else
	{
		int compareresult = num1.abscompare(num2);
		if (compareresult == 0)
		{
			sum.append_digit(0);
		}
		else if (compareresult > 0)
		{
			sum = num1.sub(num2);
			sum.setsign(num1sign);
		}
		else
		{
			sum = num2.sub(num1);
			sum.setsign(num2sign);
		}		
	}
	return sum;
}

longdigits longdigits::operator - (longdigits const &num2)
{
	SIGN num1sign, num2sign;
	longdigits diff, &num1 = *this;

	num1sign = num1.getsign();
	num2sign = num2.getsign();

	if (num1sign != num2sign)
	{
		diff = num1.add(num2);
		diff.setsign(num1sign);
	}
	else
	{
		int compareresult = num1.abscompare(num2);
		if (compareresult == 0)
		{
			diff.append_digit(0);
		}
		else if (compareresult > 0)
		{
			diff = num1.sub(num2);
			diff.setsign(num1sign);
		}
		else
		{
			diff = num2.sub(num1);
			diff.setsign(num1sign == SIGN::POSTIVE? SIGN::NEGATIVE : SIGN::POSTIVE);
		}
	}
	return diff;
}

longdigits longdigits::operator += (longdigits const &num2)
{
	longdigits sum = *this + num2;
	this->clear();
	this->longdigit = sum.longdigit;
	this->counter = sum.getcounter();
	this->setsign(sum.getsign());
	return sum;
}

longdigits longdigits::operator -= (longdigits const &num2)
{
	longdigits diff = *this - num2;
	this->clear();
	this->longdigit = diff.longdigit;
	this->counter = diff.getcounter();
	this->setsign(diff.getsign());
	return diff;
}

bool longdigits::operator > (longdigits const &num2)
{
	if (compare(num2) > 0) return TRUE;
	return FALSE;
}

bool longdigits::operator >= (longdigits const &num2)
{
	if (compare(num2) >= 0) return TRUE;
	return FALSE;
}

bool longdigits::operator < (longdigits const &num2)
{
	if (compare(num2) < 0) return TRUE;
	return FALSE;
}

bool longdigits::operator <= (longdigits const &num2)
{
	if (compare(num2) <= 0) return TRUE;
	return FALSE;
}

bool longdigits::operator == (longdigits const &num2)
{
	if (compare(num2) == 0) return TRUE;
	return FALSE;
}

bool longdigits::operator != (longdigits const &num2)
{
	if (compare(num2) != 0) return TRUE;
	return FALSE;
}

bool longdigits::operator == (UINT32 num)
{
	longdigits &num1 = *this, num2;
	num2.append_digit(num);
	bool ret = num1 == num2;
	num2.clear();
	return ret;
}

bool longdigits::operator != (UINT32 num)
{
	longdigits &num1 = *this, num2;
	num2.append_digit(num);
	bool ret = num1 != num2;
	num2.clear();
	return ret;
}

bool longdigits::operator > (UINT32 num)
{
	longdigits &num1 = *this, num2;
	num2.append_digit(num);
	bool ret = num1 > num2;
	num2.clear();
	return ret;
}

bool longdigits::operator >= (UINT32 num)
{
	longdigits &num1 = *this, num2;
	num2.append_digit(num);
	bool ret = num1 >= num2;
	num2.clear();
	return ret;
}

bool longdigits::operator < (UINT32 num)
{
	longdigits &num1 = *this, num2;
	num2.append_digit(num);
	bool ret = num1 < num2;
	num2.clear();
	return ret;
}

bool longdigits::operator <= (UINT32 num)
{
	longdigits &num1 = *this, num2;
	num2.append_digit(num);
	bool ret = num1 <= num2;
	num2.clear();
	return ret;
}
longdigits longdigits::operator + (UINT32 value)
{
	longdigits &num1 = *this, num2, sum;
	num2.append_digit(value);
	sum = num1 + num2;
	num2.clear();
	return sum;
}

longdigits longdigits::operator - (UINT32 value)
{
	longdigits &num1 = *this, num2, diff;
	num2.append_digit(value);
	diff = num1 - num2;
	num2.clear();
	return diff;
}

longdigits longdigits::operator ++ ()
{
	longdigits &num1 = *this, num2, sum;
	num2.append_digit(1);
	sum = num1 + num2;
	num2.clear();
	return sum;
}
longdigits longdigits::operator -- ()
{
	longdigits &num1 = *this, num2, sum;
	num2.append_digit(1);
	sum = num1 - num2;
	num2.clear();
	return sum;
}

void longdigits::operator >> (UINT32 value)
{
	UINT64 v1 = 0;
	UINT32 v2 = 0;
	linkedList *list = this->longdigit;
	linkedList *listp = NULL;


	while (list)
	{
		v1 = ((UINT64)list->get_value()) << (32 - value);
		v2 = v1 & 0xFFFFFFFF;
		if (v2 && listp)
		{
			v2 += listp->get_value();
			listp->set_value(v2);
		}
		v1 >>= 32;
		list->set_value(v1);
		listp = list;
		list = list->get_next();
	}
	if (!v1 && listp->get_prev() != NULL)
	{
		delete_digit(listp);
	}
}

void longdigits::operator << (UINT32 value)
{
	UINT64 v1 = 0;
	UINT32 v2 = 0;
	linkedList *list = this->longdigit;
	linkedList *listp = NULL;

	while (list)
	{
		listp = list;
		v1 = list->get_value() << value;
		v1 += v2;
		v2 = v1 >> 32;
		list->set_value(v1 & 0xFFFFFFFF);
		list = list->get_next();
	}
	if (v2)
	{
		append_digit(listp, v2);
	}
}

UINT32 longdigits::getbits()
{
	UINT32 bitcount = 0;
	UINT32 v1 = 0;
	linkedList *list = this->longdigit;

	while (list)
	{
		v1 = list->get_value();
		list = list->get_next();
		if (list)
			bitcount += 32;
		else
		{
			while (v1)
			{
				bitcount++;
				v1 >>= 1;
			}
		}
	}
	return bitcount;
}

BOOL longdigits::setbit(UINT32 Idx, BOOL Value)
{
	//Should be inside the total length
	UINT32 bitcount = Idx;
	UINT32 v1 = 0;
	linkedList *list = this->longdigit;

	while (list)
	{
		list = list->get_next();
		if (bitcount < 32)
		{
			v1 = list->get_value();
			v1 &= ~(1 << bitcount);
			if (Value)
				v1 |= (1 << bitcount);
			list->set_value(v1);
			return TRUE;
		}
		else
		{
			bitcount -= 32;
		}
	}
	return FALSE;
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
	num.setsign(this->getsign());
}

bool longdigits::divide(longdigits &denominator,
	longdigits &quotient, longdigits &reminder)
{
	longdigits &numerator = *this;
	longdigits intermediate;
	longdigits subproduct;
	if(denominator == 0)
	{
		//Error Case counter = 0
		return FALSE;
	}

	if (numerator < denominator)
	{
		quotient.append_digit(NULL, 0);
		denominator.copy(reminder);
		return TRUE;
	}

	UINT32 bit_count = numerator.getbits()-1;
	UINT32 first_value = 1 << (bit_count % 32);
	
	intermediate.insertemptyroot(numerator.getcounter()-1);
	intermediate.append_digit(first_value);
	numerator.copy(reminder);
	while(intermediate != 0)
	{
		subproduct = intermediate * denominator;
		if (subproduct <= reminder)
		{
			reminder -= subproduct;
			quotient += intermediate;
		}
		subproduct.clear();
		intermediate >> 1;
	}


	SIGN num1sign, num2sign;
	num1sign = numerator.getsign();
	num2sign = denominator.getsign();
	if (num1sign != num2sign)
	{
		quotient.setsign(NEGATIVE);
	}
	else
	{
		quotient.setsign(POSTIVE);
	}
	reminder.setsign(num1sign);

	return TRUE;
}

longdigits longdigits::operator / (longdigits &denominator)
{
	longdigits quotient;
	longdigits reminder;
	longdigits &numerator = *this;
	numerator.divide(denominator, quotient, reminder);
	reminder.clear();

	return quotient;
}

longdigits longdigits::operator % (longdigits &denominator)
{
	longdigits quotient;
	longdigits reminder;
	longdigits &numerator = *this;
	numerator.divide(denominator, quotient, reminder);
	quotient.clear();

	return reminder;
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
	SIGN num1sign, num2sign;
	num1sign = this->getsign();
	num2sign = num.getsign();
	if (num1sign != num2sign)
	{
		product.setsign(NEGATIVE);
	}
	else
	{
		product.setsign(POSTIVE);
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


longdigits inverse(longdigits a, longdigits n)
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

	num3 = num1 / num2;
	num4 = num1 % num2;
	printall("Test5", num1, num2, num3, num4);
	num3.clear(); num4.clear();


	longdigits p, q, N, r, e, d;
	p.readHexString(str_p);
	q.readHexString(str_q);
	N = p * q;
	r = (p - 1) * (q - 1);
	e.readHexString(str_e);
	d = inverse(e, r);

	printall("RSA1", p, q, N, r);
	printall("RSA2", e, N, d, N);



	printf("%d %d %d\n", num1 >= num2, num1 <= num1, num1 == num2);
	num4 = num1 * num2;
	num4.writeHexString(&strtemp);
	printf("num4: %s\n", strtemp);
	int value = strlen(strtemp);
	value++;

}