/*longdigits.h*/
#include <Windows.h>

class linkedList
{
public:
	linkedList() : next4Byte(NULL), prev4Byte(NULL), value(NULL), isEmpty(1) {}
	linkedList(UINT32 value) : next4Byte(NULL), prev4Byte(NULL), value(value), isEmpty(0) {}
	linkedList(UINT32 value, linkedList  *prev, BOOL on_tail = TRUE) : next4Byte(NULL), prev4Byte(prev), value(value), isEmpty(0) 
	{
		if (NULL != prev)
		{
			if (on_tail)
			{
				prev->update_next(this);
			}
			else
			{
				next4Byte = prev;
				prev->update_prev(this);
				prev4Byte = NULL;
			}
		}
	}
	linkedList(UINT32 value, linkedList  *prev, linkedList  *next) : next4Byte(next), prev4Byte(prev), value(value), isEmpty(0)
	{
		if (NULL != prev)
		{
			next->update_next(this);
		}
		if (NULL != next)
		{
			next->update_prev(this);
		}
	}
	~linkedList()
	{
		if (NULL != prev4Byte)
		{
			prev4Byte->update_next(next4Byte);
		}
		if (NULL != next4Byte)
		{
			next4Byte->update_prev(prev4Byte);
		}
	}
private:
	linkedList  *next4Byte;
	linkedList  *prev4Byte;
	UINT32       value;
	BOOL         isEmpty;
protected:

public:
	void update_next(linkedList  *next)
	{
		next4Byte = next;
	}
	void update_prev(linkedList  *prev)
	{
		prev4Byte = prev;
	}
	UINT32 get_value()
	{
		return value;
	}
	void set_value(UINT32 val)
	{
		value = val;
	}
	linkedList* get_prev()
	{
		return  prev4Byte;
	}
	linkedList* get_next()
	{
		return  next4Byte;
	}
};

class longdigits
{
public:

	longdigits() : longdigit(NULL), counter(0) {}
	~longdigits()
	{
		//clear();
	}
	UINT32 carry;
	UINT32 intermediate_results;

private:
	linkedList *longdigit;
	UINT32 counter;

private:
	bool longdigits::divide(longdigits &denominator,
		longdigits &quotient, longdigits &reminder);
	UINT32 getbits();
	BOOL setbit(UINT32 Idx, BOOL Value);
	void longdigits::trim();

public:
	void readHexString(char *hexString);
	void writeHexString(char **hexString);
	UINT32 getcounter() { return counter; }
	linkedList* append_digit(UINT32);
	linkedList* append_digit(linkedList*, UINT32);
	void delete_digit(linkedList*);
	void copy(longdigits &num);

	longdigits operator + (longdigits const &num);
	longdigits operator += (longdigits const &num);
	longdigits operator + (UINT32);
	longdigits operator - (longdigits const &num);
	longdigits operator -= (longdigits const &num);
	longdigits operator - (UINT32);
	longdigits operator * (longdigits &num);
	longdigits operator / (longdigits &num);
	longdigits operator % (longdigits &num);
	bool operator > (longdigits const &num);
	bool operator >= (longdigits const &num);
	bool operator > (UINT32);
	void operator >> (UINT32); // Max Shift is 32
	void operator << (UINT32); // Max Shift is 32
	bool operator >= (UINT32);
	bool operator < (UINT32);
	bool operator <= (UINT32);
	bool operator == (longdigits const &num);
	bool operator == (UINT32);
	bool operator != (UINT32);
	bool operator < (longdigits const &num);
	bool operator <= (longdigits const &num);
	longdigits operator ++ ();
	longdigits operator -- ();
	longdigits operator ~ ();
	void scalarmultiply(UINT32);
	void insertroot(UINT32);
	void insertemptyroot(UINT32);
	void clear()
	{
		linkedList *list = this->longdigit;
		while (list) {

			if (list->get_next()) delete(list->get_next());
			else list = NULL;
		}
		if (list) delete(list);
		counter = 0;
		this->longdigit = 0;
	}
};