#pragma once

#include "stdafx.h"

//__________________FOR KEEPING TRACK OF TIME SPENT ON THE CRITICAL PORTIONS OF CODE______________________________________________//
class PerformanceStats {
public:
	static double  whole_test_time;
	static double  whole_csv_to_binary_time;
	static double  whole_bin_to_csv_time;

	static int total_records_from_csv_to_bin;
	static double  average_record_csv_to_bin;
	static int total_records_bin_to_csv;
	static double  average_record_bin_to_csv;

	static int total_quotes_from_csv_to_bin;
	static double  average_quote_csv_to_bin;
	static int total_quotes_bin_to_csv;
	static double  average_quote_bin_to_csv;

	static int total_trades_csv_to_bin;
	static double  average_trade_csv_to_bin;
	static int total_trades_bin_to_csv;
	static double  average_trade_bin_to_csv;

	static int total_signals_csv_to_bin;
	static double  average_signal_csv_to_bin;
	static int total_signals_bin_to_csv;
	static double  average_signal_bin_to_csv;

	
	static void PrintPerformanceStatistics();


	static void UpdateTotalCsvToBinTime(std::chrono::time_point<std::chrono::high_resolution_clock> start,
		std::chrono::time_point<std::chrono::high_resolution_clock> end);


	static void UpdateTotalBinToCsvTime(std::chrono::time_point<std::chrono::high_resolution_clock> start,
		std::chrono::time_point<std::chrono::high_resolution_clock> end);

	static std::chrono::duration<double> total_time; // = whole_test_end - whole_test_start;
	static double GetTotalRunTime();
	static double GetTotalCsvToBinTime();
	static double GetTotalBinToCsvTime();
	static double GetAverageRecordCsvTobinTime();
	static double GetAverageRecordBinToCsvTime();
	static double GetAverageQuoteCsvToBinTime();
	static double GetAverageQuoteBinToCsvTime();
	static double GetAverageTradeCsvToBinTime();
	static double GetAverageTradeBinToCsvTime();
	static double GetAverageSignalCsvToBinTime();
	static double GetAverageSignalBinToCsvTime();

	static void UpdateTotalRunTime(std::chrono::time_point<std::chrono::high_resolution_clock> start,
		std::chrono::time_point<std::chrono::high_resolution_clock> end);

	static void UpdateAverageRecordCsvToBinTime(std::chrono::time_point<std::chrono::high_resolution_clock> start,
		std::chrono::time_point<std::chrono::high_resolution_clock> end);

	static void UpdateAverageRecordBinToCsvTime(std::chrono::time_point<std::chrono::high_resolution_clock> start,
		std::chrono::time_point<std::chrono::high_resolution_clock> end);

	static void UpdateAverageQuoteCsvToBinTime(std::chrono::time_point<std::chrono::high_resolution_clock> start, 
		std::chrono::time_point<std::chrono::high_resolution_clock> end);

	static void UpdateAverageQuoteBinToCsvTime(std::chrono::time_point<std::chrono::high_resolution_clock> start,
		std::chrono::time_point<std::chrono::high_resolution_clock> end);
	
	static void UpdateAverageTradeCsvToBinTime(std::chrono::time_point<std::chrono::high_resolution_clock> start,
		std::chrono::time_point<std::chrono::high_resolution_clock> end);
	
	static void UpdateAverageTradeBinToCsvTime(std::chrono::time_point<std::chrono::high_resolution_clock> start,
		std::chrono::time_point<std::chrono::high_resolution_clock> end);

	static void UpdateAverageSignalCsvToBinTime(std::chrono::time_point<std::chrono::high_resolution_clock> start,
		std::chrono::time_point<std::chrono::high_resolution_clock> end);

	static void UpdateAverageSignalBinToCsvTime(std::chrono::time_point<std::chrono::high_resolution_clock> start,
		std::chrono::time_point<std::chrono::high_resolution_clock> end);
	
private:
	PerformanceStats() {}
};



