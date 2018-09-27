#pragma once
#include "stdafx.h"
using namespace std;

// ____________________________STATIC CLASS responsible for managing the input and output csv/binary files and the record objects in Record.h_____________________________//
class RecordsManager {
public:

	// file stream and memory managment:
	static void CloseAllFileStreams();
	static void CleanUpBinMemBlocks();

	// Determines the value type of the values in the csv file columns:
	static char DetermineValueType(string& value);

	// Csv to Binary conversion functions - read from csv and immediatley write to binary without storing any data to memory:
	static void TransferRecordsFromCsvIntoBinaryFiles(const char* input_file);
	static void WriteRecordToBinaryFile(vector<string>& row_elements);
	static fstream* GetBinaryFileStreamBySymbol(string symbol);

	// Binary to Csv conversion functions - read from binary and immediatly write to csv without storing:
	static void TransferRecordsFromBinaryIntoCsv(const char* csv_file_name);
	static char* WriteQuoteToCsv(fstream& csv_file, char* memblock);
	static char* WriteTradeToCsv(fstream& csv_file, char* memblock);
	static char* WriteSignalToCsv(fstream& csv_file, char* memblock);
	static char* DetermineNextRecordType(char* memblock, string& record_type);

private:
	// value type encoding for determining type of record in csv
	//						1 == int; 2 == double; 3 == char; 4 == string:
	static const string quote_format;
	static const string trade_format;
	static const string trade_format_missing_condition;
	static const string signal_format;

	// record type encoding for determining type of record in binary:
	static const string quote_bin_identifier;
	static const string trade_bin_identifier;
	static const string signal_bin_identifier;
	
	// size in bytes of the different record class types:
	static const int quote_binary_size;
	static const int trade_binary_size;
	static const int signal_binary_size;

	// size in bytes of time, symbol, and record type delim string values:
	static const int time_string_size;
	static const int symbol_string_size;
	static const int record_type_size;

	// Keep track of unexpected record formats parsed from csv:
	static int invalid_row_formats_detected;

	// map of open fstream bin files according to symbol:
	static std::unordered_map<string, fstream*> bin_file_map;

	// list used to keep track of the order in which the binary files were written to:
	// since the final csv result must match the initial one this will be used to 
	// re-create the csv from the binary files:
	static std::vector<string> bin_file_write_tracker;

	// map of memory block loaded from binary files:
	static std::unordered_map<string, char*> loaded_bin_mem_blocks;

	// map of memory block start pointers b/c memblocks will be incremented therefore
	// this is needed for properly deleting the memblocks read from binary files:
	static std::vector<char*> loaded_bin_mem_blocks_start;

	// make constructor private b/c this is a static class:
	RecordsManager() {};
};

