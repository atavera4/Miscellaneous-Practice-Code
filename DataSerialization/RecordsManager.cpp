
#include "stdafx.h"

// _________________Start of Static Member Variables___________________________________________________________

// value type encoding for csv columns:  1 == int; 2 == double; 3 == char; 4 == string:
const string RecordsManager::quote_format = "442211";
const string RecordsManager::trade_format = "4423";
// b/c trade might be missing the condition:
const string RecordsManager::trade_format_missing_condition = "442";
const string RecordsManager::signal_format = "4421";

// 1 delim char + 6 chars for time + 4 chars for symbol + double + double + int + int:
const int RecordsManager::quote_binary_size = 1 + 7 + 4 + sizeof(double) + sizeof(double) + sizeof(int) + sizeof(int);
// 1 delim char + 6 chars for time + 4 chars for symbol + double + char:
const int RecordsManager::trade_binary_size = 1 + 7 + 4 + sizeof(double) + 1;
// 1 delim char + 6 chars for time + 4 chars for symbol + double + int:
const int RecordsManager::signal_binary_size = 1 + 7 + 4 + sizeof(double) + sizeof(int);


// record type encoding for determining type of record in binary:
const string RecordsManager::quote_bin_identifier = "##quote##";
const string RecordsManager::trade_bin_identifier = "##trade##";
const string RecordsManager::signal_bin_identifier = "##signal#";


const int RecordsManager::time_string_size = 23;
const int RecordsManager::symbol_string_size = 4;
const int RecordsManager::record_type_size = 9;
int RecordsManager::invalid_row_formats_detected = 0;

// file managment:
std::unordered_map<string, fstream*> RecordsManager::bin_file_map;
std::vector<string> RecordsManager::bin_file_write_tracker;
std::unordered_map<string, char*> RecordsManager::loaded_bin_mem_blocks;
std::vector<char*> RecordsManager::loaded_bin_mem_blocks_start;


// ________________START OF FUNCTION DEFINITIONS_________________________________________________________________

void RecordsManager::CloseAllFileStreams()
{
	for (auto it = RecordsManager::bin_file_map.begin(); it != RecordsManager::bin_file_map.end(); ++it)
	{
		it->second->close();
		delete it->second;
	}
	return;
}

void RecordsManager::CleanUpBinMemBlocks()
{
	int size = RecordsManager::loaded_bin_mem_blocks_start.size();
	for (int i = 0; i < size; i++)
	{
		delete[] RecordsManager::loaded_bin_mem_blocks_start[i];
	}
	return;
}

fstream* RecordsManager::GetBinaryFileStreamBySymbol(string symbol)
{
	// if no file matching symbol in map then add it:
	if (RecordsManager::bin_file_map.find(symbol) == RecordsManager::bin_file_map.end())
	{
		fstream* file = new fstream(symbol, std::ios::out | std::ios::binary);
		RecordsManager::bin_file_map[symbol] = file;
	}
	// Update bin file write order tracker:
	RecordsManager::bin_file_write_tracker.push_back(symbol);
	return RecordsManager::bin_file_map.find(symbol)->second;
}

// 1 == int; 2 == double; 3 == char; 4 == string:
char RecordsManager::DetermineValueType(string& value)
{
	// 1st determine if it is a number at all:
	if (value.size() == 1)
	{
		if (isdigit(value[0])) { return '1'; }
		else { return '3'; }
	}

	bool dot_seen = false;
	for (int i = 0; i < value.size(); i++)
	{
		if (value[i] == '.' && !dot_seen) { dot_seen = true; }
		else if (value[i] == '.' && dot_seen) { return '4'; }
		else if (value[i] == ':') { return '4'; }
		else if (!isdigit(value[i])) { return '4'; }
		// no value should be negative:
		else if (value[i] == '-') { return '4'; }
	}

	if (dot_seen) { return '2'; }
	else { return '1'; }
}

void RecordsManager::WriteRecordToBinaryFile(vector<string>& row_elements)
{
	string format = "";
	int size = row_elements.size();
	for (int i = 0; i < size; i++)
	{
		format += RecordsManager::DetermineValueType(row_elements[i]);
	}
	if (format == RecordsManager::quote_format)
	{
		auto start_time = std::chrono::high_resolution_clock::now();
		Quote quote(row_elements[0], row_elements[1], stod(row_elements[2]), stod(row_elements[3]), stoi(row_elements[4]), stoi(row_elements[5]));
		vector<uint8_t> serial_bytes;
		quote.Serialize(serial_bytes);
		fstream* bin_file = RecordsManager::GetBinaryFileStreamBySymbol(quote.symbol);
		for (int i = 0; i < serial_bytes.size(); i++)
		{
			bin_file->write((char*)&serial_bytes[i], sizeof(char));
		}
		auto end_time = std::chrono::high_resolution_clock::now();
		PerformanceStats::UpdateAverageQuoteCsvToBinTime(start_time, end_time);
	}
	else if (format == RecordsManager::trade_format)
	{
		auto start_time = std::chrono::high_resolution_clock::now();
		Trade trade(row_elements[0], row_elements[1], stod(row_elements[2]), row_elements[3]);
		vector<uint8_t> serial_bytes;
		trade.Serialize(serial_bytes);
		fstream* bin_file = RecordsManager::GetBinaryFileStreamBySymbol(trade.symbol);
		for (int i = 0; i < serial_bytes.size(); i++)
		{
			bin_file->write((char*)&serial_bytes[i], sizeof(char));
		}
		auto end_time = std::chrono::high_resolution_clock::now();
		PerformanceStats::UpdateAverageTradeCsvToBinTime(start_time, end_time);
	}
	else if (format == RecordsManager::trade_format_missing_condition)
	{
		auto start_time = std::chrono::high_resolution_clock::now();
		Trade trade(row_elements[0], row_elements[1], stod(row_elements[2]));
		vector<uint8_t> serial_bytes;
		trade.Serialize(serial_bytes);
		fstream* bin_file = RecordsManager::GetBinaryFileStreamBySymbol(trade.symbol);
		for (int i = 0; i < serial_bytes.size(); i++)
		{
			bin_file->write((char*)&serial_bytes[i], sizeof(char));
		}
		auto end_time = std::chrono::high_resolution_clock::now();
		PerformanceStats::UpdateAverageTradeCsvToBinTime(start_time, end_time);
	}
	else if (format == RecordsManager::signal_format)
	{
		auto start_time = std::chrono::high_resolution_clock::now();
		Signal signal(row_elements[0], row_elements[1], stod(row_elements[2]), stoi(row_elements[3]));
		vector<uint8_t> serial_bytes;
		signal.Serialize(serial_bytes);
		fstream* bin_file = RecordsManager::GetBinaryFileStreamBySymbol(signal.symbol);
		for (int i = 0; i < serial_bytes.size(); i++)
		{
			bin_file->write((char*)&serial_bytes[i], sizeof(char));
		}
		auto end_time = std::chrono::high_resolution_clock::now();
		PerformanceStats::UpdateAverageSignalCsvToBinTime(start_time, end_time);
	}
	else
	{
		//cerr << "Format in csv does not match with any of the expected ..." << endl;
		//cerr << "Format encoding: 1->int ; 2->double; 3->char; 4->string: " << endl;
		//cerr << "Invalid Format detected: " << format << endl;
		RecordsManager::invalid_row_formats_detected += 1;
	}
	return;
}

void RecordsManager::TransferRecordsFromCsvIntoBinaryFiles(const char* input_file)
{
	ifstream csv_file(input_file);
	string csv_row;
	int test = 0;
	while (csv_file.good())
	{
		getline(csv_file, csv_row, '\n');
		stringstream s(csv_row);
		vector<string> row_elements;
		while (s.good())
		{
			string elem;
			getline(s, elem, ',');
			if (elem != "")
			{
				row_elements.push_back(elem);
			}
		}
		RecordsManager::WriteRecordToBinaryFile(row_elements);
	}

	csv_file.close();
	RecordsManager::CloseAllFileStreams();
	cout << "Total invalid formats detected and ignored: " << RecordsManager::invalid_row_formats_detected << endl;
	cout << "These were records in csv whose values did not match any of the expected formats for Quote, Trade, or Signal" << endl;
	return;
}

void RecordsManager::TransferRecordsFromBinaryIntoCsv(const char* csv_file_name)
{

	// open the csv file here: 
	fstream csv_file;
	csv_file = fstream(csv_file_name, std::ios::out);
	if (!csv_file.is_open())
	{
		cerr << "Error: " << csv_file_name << "could not be opened. exiting ..." << endl;
		exit(0);
	}

	// iterate through the list of writes recorded in the bin_write_tracker :
	// this will ensure that all entries written to the new csv are written in the same order 
	// in which they were read in from the original csv file:
	vector<string>& bin_write_tracker = RecordsManager::bin_file_write_tracker;
	int size = bin_write_tracker.size();
	for (int i = 0; i < size; i++)
	{
		string &file_name = bin_file_write_tracker[i];
		if (RecordsManager::loaded_bin_mem_blocks.find(file_name) == RecordsManager::loaded_bin_mem_blocks.end())
		{
			fstream bin_file(file_name, ios::in | ios::binary | ios::ate);
			streampos file_size = bin_file.tellg();
			if (bin_file.is_open())
			{
				char* memblock = new char[file_size];
				bin_file.seekg(0, ios::beg);
				bin_file.read(memblock, file_size);
				RecordsManager::loaded_bin_mem_blocks[file_name] = memblock;
				RecordsManager::loaded_bin_mem_blocks_start.push_back(memblock);
			}
			else
			{
				cerr << "Error could not open/locate binary file with name: " << file_name << endl;
			}
		}
		// now that we are sure the binary file we are looking for has been loaded into memory:
		// read and increment the memblock and look for the record start sequence of bytes:
		char* memblock = RecordsManager::loaded_bin_mem_blocks[file_name];
		string record_type;
		memblock = RecordsManager::DetermineNextRecordType(memblock, record_type);
		
		if (record_type == RecordsManager::quote_bin_identifier)
		{
			auto start_time = std::chrono::high_resolution_clock::now();
			memblock = RecordsManager::WriteQuoteToCsv(csv_file, memblock);
			auto end_time = std::chrono::high_resolution_clock::now();
			PerformanceStats::UpdateAverageQuoteBinToCsvTime(start_time, end_time);
		}
		else if (record_type == RecordsManager::trade_bin_identifier)
		{
			auto start_time = std::chrono::high_resolution_clock::now();
			memblock = RecordsManager::WriteTradeToCsv(csv_file, memblock);
			auto end_time = std::chrono::high_resolution_clock::now();
			PerformanceStats::UpdateAverageTradeBinToCsvTime(start_time, end_time);
		}
		else if (record_type == RecordsManager::signal_bin_identifier)
		{
			auto start_time = std::chrono::high_resolution_clock::now();
			memblock = RecordsManager::WriteSignalToCsv(csv_file, memblock);
			auto end_time = std::chrono::high_resolution_clock::now();
			PerformanceStats::UpdateAverageSignalBinToCsvTime(start_time, end_time);
		}
		else
		{
			cerr << "Error: record type identifier in binary file not recognized: " << record_type << endl;
			exit(0);
		}
		RecordsManager::loaded_bin_mem_blocks[file_name] = memblock;
	}
	RecordsManager::CleanUpBinMemBlocks();
	return;
}

char* RecordsManager::DetermineNextRecordType(char* memblock, string& record_type)
{
	for (int i = 0; i < RecordsManager::record_type_size; i++)
	{
		record_type += *memblock;
		memblock++;
	}
	return memblock;
}

char* RecordsManager::WriteQuoteToCsv(fstream& csv_file, char* memblock)
{
	vector<char*> time_bytes;
	for (int i = 0; i < RecordsManager::time_string_size; i++)
	{
		time_bytes.push_back(memblock);
		memblock++;
	}
	vector<char*> symbol_bytes;
	for (int i = 0; i < RecordsManager::symbol_string_size; i++)
	{
		symbol_bytes.push_back(memblock);
		memblock++;
	}
	vector<char*> bid_bytes;
	for (int i = 0; i < sizeof(double); i++)
	{
		bid_bytes.push_back(memblock);
		memblock++;
	}
	vector<char*> ask_bytes;
	for (int i = 0; i < sizeof(double); i++)
	{
		ask_bytes.push_back(memblock);
		memblock++;
	}
	vector<char*> bsize_bytes;
	for (int i = 0; i < sizeof(int); i++)
	{
		bsize_bytes.push_back(memblock);
		memblock++;
	}
	vector<char*> asize_bytes;
	for (int i = 0; i < sizeof(int); i++)
	{
		asize_bytes.push_back(memblock);
		memblock++;
	}

	// insert raw binary into copy constructor for Quote:
	Quote q(time_bytes, symbol_bytes, 
		bid_bytes, ask_bytes, bsize_bytes, asize_bytes);

	// write quote to csv:
	csv_file << q.time << ',' << q.symbol << ',' << to_string(q.bid) << ',' << to_string(q.ask) << ',' << to_string(q.bsize) << ',' << to_string(q.asize) << ',' << endl;
	time_bytes.clear();
	symbol_bytes.clear();
	bid_bytes.clear();
	ask_bytes.clear();
	bsize_bytes.clear();
	asize_bytes.clear();
	return memblock;
}

char* RecordsManager::WriteTradeToCsv(fstream& csv_file, char* memblock)
{
	vector<char*> time_bytes;
	for (int i = 0; i < RecordsManager::time_string_size; i++)
	{
		time_bytes.push_back(memblock);
		memblock++;
	}
	vector<char*> symbol_bytes;
	for (int i = 0; i < RecordsManager::symbol_string_size; i++)
	{
		symbol_bytes.push_back(memblock);
		memblock++;
	}
	vector<char*> price_bytes;
	for (int i = 0; i < sizeof(double); i++)
	{
		price_bytes.push_back(memblock);
		memblock++;
	}
	vector<char*> condition_byte;
	for (int i = 0; i < 1; i++)
	{
		condition_byte.push_back(memblock);
		memblock++;
	}
		
	// insert raw binary into copy constructor for Quote:
	Trade t(time_bytes, symbol_bytes,price_bytes, condition_byte);

	// write quote to csv:
	csv_file << t.time << ',' << t.symbol << ',' << to_string(t.price) << ',' << to_string(t.condition) << ',' << endl;
	return memblock;
}

char* RecordsManager::WriteSignalToCsv(fstream& csv_file, char* memblock)
{
	vector<char*> time_bytes;
	for (int i = 0; i < RecordsManager::time_string_size; i++)
	{
		time_bytes.push_back(memblock);
		memblock++;
	}
	vector<char*> symbol_bytes;
	for (int i = 0; i < RecordsManager::symbol_string_size; i++)
	{
		symbol_bytes.push_back(memblock);
		memblock++;
	}
	vector<char*> price_bytes;
	for (int i = 0; i < sizeof(double); i++)
	{
		price_bytes.push_back(memblock);
		memblock++;
	}
	vector<char*> code_bytes;
	for (int i = 0; i < sizeof(int); i++)
	{
		code_bytes.push_back(memblock);
		memblock++;
	}


	// insert raw binary into copy constructor for Quote:
	Signal s(time_bytes, symbol_bytes, price_bytes, code_bytes);

	// write quote to csv:
	csv_file << s.time << ',' << s.symbol << ',' << to_string(s.price) << ',' << to_string(s.code) << ',' << endl;

	return memblock;
}


