// ConsoleApplication2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <math.h>
#include <map>
using namespace std;



int exp_pow(int base, int exp, int mod)
{
	if (exp == 0)
		return 1;

	int result = base;
	for (int i = 1; i < exp; i++)
	{
		result = (result * base) % mod;
	}
	return result % mod;
}

long long ComputeHash(string str, int start, int end, int base, int mod)
{
	long long hash = 0;
	for (int i = start; i < end; i++)
	{
		int exp = end - 1 - i;
		// hash += (curr_char * base^exp)mod (prime)
		long long res = (str[i] * exp_pow(base, exp, mod)) % mod;
		cout << "res: (" << (int)str[i] << " * " << base << "^" << exp << ")mod" << mod << "= " << res << endl;
		hash += res;
		cout << hash << endl;
	}
	hash = hash % mod;
	return hash;
}


long long int RollHash(long long int hash, string str, int start_char_index,int new_char_index, int base, int exp, int mod)
{
	// remove the 1st char in the window (big endian):
	// hash - (1st_char * base^strlen-1)mod (m): 
	long long int char_to_remove = str[start_char_index] * exp_pow(base, exp, mod) % mod;
	long long int h = hash - char_to_remove;
	long long int hash_rem_chars = (((hash - char_to_remove)%mod) + mod)%mod;
	// new_hash = (hash_rem_last * base)mod (m) + (new_charmod *base^strlen-1 (m)):
	long long int rem_chars_shifted_left = (hash_rem_chars * base) % mod;
	long long int new_hash = rem_chars_shifted_left + (str[new_char_index]%mod);

	return new_hash;
}

// Generate a prime number that is relativly prime 
//	to the base which must also be a prime:
int GeneratePrime(int base)
{


	return 0;
}


// True modulo operation which is needed to properly handles negative numbers:
int modulo(int a, int b) 
{ 
	return (a % b + b) % b; 
}



int main() {

	// using the multiplicatin method:
	// hash using big endian where largest magnitude is the leading/first char byte:
	// have an iterator i keeping track of the 1st char in a window and increment it by 1 as the window slides:
	// when the window slides do xi+1 = xi -(str[i] * base^str_len-1)
	
	int base = 257; // is a prime number bigger than the number of elements in the ASCII Table. 
	int mod = 773; // is relativly prime to the base.

	string whole_str = "testalsdkfja;licecream is goodsjtest;laksjfd";
	string sub_str = "icecream is good";
	long long int sub_hash = ComputeHash(sub_str, 0, sub_str.size(), base, mod);
	int sub_str_size = sub_str.size();
	int whole_str_size = whole_str.size();
	int first_char_in_window_index = 0;
	int new_incomming_char_index = sub_str_size;
	long long int hash = ComputeHash(whole_str, 0, sub_str_size, base, mod);

	
	int exp = sub_str_size - 1;
	
	while (new_incomming_char_index < whole_str_size)
	{
		if (sub_hash == hash)
		{
			cout << "FOUND IT !!!" << endl;
			cout << "1st char is " << whole_str[first_char_in_window_index] << endl;
			cout << "last char is " << whole_str[new_incomming_char_index-1] << endl;
			break;
		}
			
		hash = RollHash(hash, whole_str, first_char_in_window_index, new_incomming_char_index, base, exp, mod);
		first_char_in_window_index += 1;
		new_incomming_char_index += 1;
	}


	return 0;
}
