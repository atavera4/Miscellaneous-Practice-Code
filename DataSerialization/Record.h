#pragma once
#include "stdafx.h"
using namespace std;

class Record {
public:
	Record() {};

	virtual void Serialize(vector<uint8_t>& serial_data) = 0; 

	void SerializeInteger(int& var, vector<uint8_t>& bytes);
	int UnSerializeInteger(vector<char*>& bytes);
	void SerializeDouble(double& var, vector<uint8_t>& bytes);
	double UnSerializeDouble(vector<char*>& bytes);
	void SerializeChar(char& var, vector<uint8_t>& bytes);
	char UnSerializeChar(vector<char*>& bytes);
	void SerializeString(string& str, vector<uint8_t>& bytes);
	string UnSerializeString(vector<char*>& bytes);
};


class Quote : public Record {
public:
	string time;
	string symbol;
	double bid;
	double ask;
	int bsize;
	int asize;

	// Copy constructor - Load From CSV:
	Quote(string time, string symbol, double bid, double ask, int bsize, int asize)
		: time(time), symbol(symbol), bid(bid), ask(ask),
		bsize(bsize), asize(asize) {}

	// Copy constructor - Load From Binary:
	Quote(vector<char*>& time_bytes, vector<char*>& symbol_bytes,
		vector<char*>& bid_bytes, vector<char*>& ask_bytes, vector<char*>& bsize_bytes, vector<char*>& asize_bytes)
	{
		time = UnSerializeString(time_bytes);
		symbol = UnSerializeString(symbol_bytes);
		bid = UnSerializeDouble(bid_bytes);
		ask = UnSerializeDouble(ask_bytes);
		bsize = UnSerializeInteger(bsize_bytes);
		asize = UnSerializeInteger(asize_bytes);
		return;
	}


	virtual void Serialize(vector<uint8_t>& serial_data)
	{
		string rec_id = "##quote##";
		SerializeString(rec_id, serial_data);
		SerializeString(time, serial_data);
		SerializeString(symbol, serial_data);
		SerializeDouble(bid, serial_data);
		SerializeDouble(ask, serial_data);
		SerializeInteger(bsize, serial_data);
		SerializeInteger(asize, serial_data);

		return;
	}

};

class Trade : public Record {
public:
	string time;
	string symbol;
	double price;
	// if in csv condition is empty just insert a '$' as a placeholder
	// to make it easy to read/write binary:
	char condition;

	// Copy constructor - Load from CSV:
	Trade(string time, string symbol, double price, string condition = "$") : time(time), symbol(symbol),
		price(price), condition(condition.c_str()[0]) {}

	Trade(vector<char*>& time_bytes, vector<char*>& symbol_bytes,
		vector<char*>& price_bytes, vector<char*>& condition_byte)
	{
		time = UnSerializeString(time_bytes);
		symbol = UnSerializeString(symbol_bytes);
		price = UnSerializeDouble(price_bytes);
		condition = UnSerializeChar(condition_byte);
		if (condition == '$')
			condition = ' ';
		return;
	}


	virtual void Serialize(vector<uint8_t>& serial_data)
	{
		string rec_id = "##trade##";
		SerializeString(rec_id, serial_data);
		SerializeString(time, serial_data);
		SerializeString(symbol, serial_data);
		SerializeDouble(price, serial_data);
		SerializeChar(condition, serial_data);
		return;
	}
};

class Signal : public Record {
public:
	string time;
	string symbol;
	double price;
	int code;

	// Copy constructor - Load from CSV:
	Signal(string time, string symbol, double price, int code) :
		time(time), symbol(symbol), price(price), code(code) {}

	// Copy constructor - Load from Binary:
	Signal(vector<char*>& time_bytes, vector<char*>& symbol_bytes,
		vector<char*>& price_bytes, vector<char*>& condition_byte)
	{
		time = UnSerializeString(time_bytes);
		symbol = UnSerializeString(symbol_bytes);
		price = UnSerializeDouble(price_bytes);
		code = UnSerializeChar(condition_byte);
		
		return;
	}

	virtual void Serialize(vector<uint8_t>& serial_data)
	{
		string rec_id = "##signal#";
		SerializeString(rec_id, serial_data);
		SerializeString(time, serial_data);
		SerializeString(symbol, serial_data);
		SerializeDouble(price, serial_data);
		SerializeInteger(code, serial_data);
		return;
	}
};

