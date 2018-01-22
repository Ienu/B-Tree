#include "BTree.h"
#include <cmath>
#include <iostream>

using namespace std;

class feature
{
public:
	float data[2];
	char label[10];
	int layer;

public:
	feature()
	{
		layer = 0;
	}
	float getValue(int index)
	{
		return data[index];
	}

};
bool operator < (feature f1, feature f2)
{
	int k = 0;
	while(k < 2)
	{
		float v1 = f1.getValue(k);
		float v2 = f2.getValue(k);

		if(fabs(v1 - v2) < 0.01)
		{
			k++;
		}
		else
		{
			return v1 < v2;
		}
	}
	return f1.getValue(k) < f2.getValue(k);
}

bool operator > (feature f1, feature f2)
{
	int k = 0;
	while(k < 2)
	{
		float v1 = f1.getValue(k);
		float v2 = f2.getValue(k);

		if(fabs(v1 - v2) < 0.01)
		{
			k++;
		}
		else
		{
			return v1 > v2;
		}
	}
	return f1.getValue(k) > f2.getValue(k);
}

bool operator == (feature f1, feature f2)
{
	for(int i = 0; i < 2; i++)
	{
		if(f1.data[i] != f2.data[i])
		{
			return false;
		}
	}
	return true;
}

ostream& operator << (ostream& ct, feature& f)
{
	return ct << f.data[0] << ", " << f.data[1] << "--> " << f.label; 
}
int main()
{
	//test<int> t;
	//t.show();
	Btree<feature> bt;
	for(int i = 0; i < 100; i++)
	{
		feature f;
		for(int j = 0; j < 2; j++)
		{
			f.data[j] = float(rand()) / 10000.0f;
		}
		itoa(i % 4, f.label, 10);
		bt.insert(f);
	}


	bt.display();
	return 0;
}