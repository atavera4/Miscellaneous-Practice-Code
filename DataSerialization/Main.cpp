

#include "stdafx.h"

int main(int argc,char* argv[])
{
	string in_file = "q.csv";
	string out_file = "NEW_Q.csv";
	if (argc == 2)
	{
		in_file = argv[1];
	}
	else if (argc == 3)
	{
		in_file = argv[1];
		out_file = argv[2];
	}
	else if (argc > 3)
	{
		cout << "Error: too many arguments passed. Max args are: 2" << endl;
		cout << "input_csv_file, output_csv_file" << endl;
	}
	auto start_time = std::chrono::high_resolution_clock::now();
	RecordsManager::TransferRecordsFromCsvIntoBinaryFiles(in_file.c_str());
	auto end_csv_to_bin_time = std::chrono::high_resolution_clock::now();
	RecordsManager::TransferRecordsFromBinaryIntoCsv(out_file.c_str());
	auto end_time = std::chrono::high_resolution_clock::now();
	PerformanceStats::UpdateTotalRunTime(start_time, end_time);
	PerformanceStats::UpdateTotalCsvToBinTime(start_time, end_csv_to_bin_time);
	PerformanceStats::UpdateTotalBinToCsvTime(end_csv_to_bin_time, end_time);
	PerformanceStats::PrintPerformanceStatistics();
	
	return 0;
}

