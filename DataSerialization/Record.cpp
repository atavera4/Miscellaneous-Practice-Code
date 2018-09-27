#include "stdafx.h"
using namespace std;

void Record::SerializeInteger(int& var, vector<uint8_t>& bytes)
{
	//int* v = new int(var);
	uint8_t* v_binary = reinterpret_cast<uint8_t*>(&var);
	// iterate through the bytes of the integer:
	for (int i = 0; i < sizeof(int); i++)
	{
		bytes.push_back(*v_binary);
		//cout << "func: " << *v_binary << endl;
		v_binary++;
	}
	return;
}

int Record::UnSerializeInteger(vector<char*>& bytes)
{

	uint32_t integer(0x0000);// | (bytes[3] << 24) | (bytes[2] << 16) | (bytes[1] << 8) | bytes[0];
	for (int i = 3; i >= 0; i--)
	{
		if (*reinterpret_cast<uint8_t*>(bytes[i]) == 0xfd)
			continue;
		uint32_t tmp = 0x0000 | *reinterpret_cast<uint8_t*>(bytes[i]);
		tmp = tmp << i * 8;
		integer |= tmp;
	}
	return *reinterpret_cast<int*>(&integer);
}

void Record::SerializeDouble(double& var, vector<uint8_t>& bytes)
{
	//double* d = new double(var);
	uint8_t* d_binary = reinterpret_cast<uint8_t*>(&var);
	// iterate through bytes of double:
	for (int i = 0; i < sizeof(double); i++)
	{
		bytes.push_back(*d_binary);
		d_binary++;
	}
	return;
}

double Record::UnSerializeDouble(vector<char*>& bytes)
{
	// doubles are 64 bytes:
	uint64_t d_64(0x00000000);
	for (int i = 7; i >= 0; i--)
	{
		//uint8_t t(*bytes[i]);
		uint64_t tmp = 0x00000000 | *reinterpret_cast<uint8_t*>(bytes[i]);
		tmp = tmp << i * 8;
		d_64 |= tmp;
	}
	return *reinterpret_cast<double*>(&d_64);	
}

void Record::SerializeChar(char& var, vector<uint8_t>& bytes)
{
	bytes.push_back(var);
	return;
}

char Record::UnSerializeChar(vector<char*>& bytes)
{
	return *bytes[0];
}

void Record::SerializeString(string& str, vector<uint8_t>& bytes)
{
	int size = str.size();
	for (int i = 0; i < size; i++)
	{
		bytes.push_back(str[i]);
	}
	return;
}

string Record::UnSerializeString(vector<char*>& bytes)
{
	string str;
	for (int i = 0; i < bytes.size(); i++)
	{
		str += *bytes[i];
	}
	return str;
}

