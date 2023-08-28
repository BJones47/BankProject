/* 
File: hashMap
Name: Braxton Jones
date: 4/26/23
description: hashMap class header file
*/



#include <vector>
#include <string>
#include <cmath>
#include <cstdlib>

template <class t1, class t2>
class hashMap
{
public:
	hashMap(std::size_t = 5);
	t2& operator[](t1);
private:
	struct keyVal
	{
		keyVal()
		{
			key = t1();
			value = t2();
		}

		t1 key;
		t2 value;
	};

	void resize(std::size_t);

	std::size_t hash1(std::string);
	std::size_t hash2(std::string);

	std::vector<keyVal> table1;
	std::vector<keyVal> table2;

	std::size_t items1;
	std::size_t items2;
};

template <class t1, class t2>
hashMap<t1, t2>::hashMap(std::size_t size)
{

	// resize tables
	table1.resize(size);
	table2.resize(size);
	// set items = 0
	items1 = 0;
	items2 = 0;


}

template <class t1, class t2>
t2& hashMap<t1, t2>::operator[](t1 key)
{
	// if load factor is above 20 percent or equal resize
	// 1.
	
	if (static_cast<double>(items1)/table1.size() >= .20 ||
		static_cast<double>(items2)/table2.size() >= .20)
	{
		this->resize(table1.size());
	}

	// 2.
	std::size_t index1 = hash1(key) % table1.size();
	std::size_t index2 = hash2(key) % table2.size();

	// 3.
	// initialize collision counters i1 and i2 with 0
	int i1 = 0;
	int i2 = 0;

	// 4. check table[index 1], if blank create a new entry and increment 
	// items by 1.

	while ( true)
	{
		if(table1[index1].key == "")
		{
			table1[index1].key = key;
			items1++;
			return table1[index1].value;
		}
		else if(table1[index1].key == key)
		{
			return table1[index1].value;
		}

		// 5.
		// same steps for table 2
		else if(table2[index2].key == "")
		{
			table2[index2].key = key;
			items2++;
			return 	table2[index2].value;
		}
		else if(table2[index2].key == key)
		{
			return table2[index2].value;
		}

		// 6. inc collision counters then find new index 
		else
		{
			i1++;
			i2++;
			if (i1 % 2 == 0)
			{
				index1 = ( index1 + i1 * hash2 ( key )) % table1 . size (); // on even iterations
				index2 = ( index2 + i2 * hash1 ( key )) % table2 . size (); // on even iterations	
			}
			if (i1 % 2 == 1)
			{
				index1 = ( index1 + i1 * hash1 ( key )) % table1 . size (); // on odd iterations
				index2 = ( index2 + i2 * hash2 ( key )) % table2 . size (); // on odd iterations
			}
		}
	}


}

template <class t1, class t2>
void hashMap<t1, t2>::resize(std::size_t amount)
{
	// create 2 temporary vectors
	std::vector<keyVal> temp1;
	std::vector<keyVal> temp2;

	// copy table 1 and 2
	temp1 = table1;
	temp2 = table2;

	// clear out both tables
	table1.clear();
	table2.clear();

	// resize both tables
	table1.resize(amount*2);
	table2.resize(amount*2);

	//copy temporary back to tables
	for (auto i : temp1)
	{
		if (i.key != "")
		{
			(*this)[i.key] = i.value;
		}
	}

	for (auto i : temp2)
	{
		if (i.key != "")
		{
			(*this)[i.key] = i.value;
		}
	}
}

template <class t1, class t2>
std::size_t hashMap<t1, t2>::hash1(std::string key)
{
	std::size_t sum = 0;
	for (int i = 0; i < key.length(); i++)
	{
		sum += (key[i] * pow(10, i));
	}	
	return sum;
}

template <class t1, class t2>
std::size_t hashMap<t1, t2>::hash2(std::string key)
{
	std::size_t sum = 0;
	for (int i = 0; i < key.length(); i++)
	{
		sum += (key[key.length()-1-i] * pow(10, i));
	}	
	return sum;
}
