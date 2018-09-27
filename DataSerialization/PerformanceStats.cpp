#include "stdafx.h"


double PerformanceStats::whole_test_time;
double PerformanceStats::whole_csv_to_binary_time;
double PerformanceStats::whole_bin_to_csv_time;


int PerformanceStats::total_records_from_csv_to_bin = 0;
double PerformanceStats::average_record_csv_to_bin;
int PerformanceStats::total_records_bin_to_csv = 0;
double PerformanceStats::average_record_bin_to_csv;

int PerformanceStats::total_quotes_from_csv_to_bin = 0;
double PerformanceStats::average_quote_csv_to_bin;
int PerformanceStats::total_quotes_bin_to_csv = 0;
double PerformanceStats::average_quote_bin_to_csv;

int PerformanceStats::total_trades_csv_to_bin = 0;
double PerformanceStats::average_trade_csv_to_bin;
int PerformanceStats::total_trades_bin_to_csv = 0;
double PerformanceStats::average_trade_bin_to_csv;

int PerformanceStats::total_signals_csv_to_bin = 0;
double PerformanceStats::average_signal_csv_to_bin;
int PerformanceStats::total_signals_bin_to_csv = 0;
double PerformanceStats::average_signal_bin_to_csv;


void PerformanceStats::PrintPerformanceStatistics()
{
	cout << endl << endl;
	cout << "Performance Statistics: " << endl;
	cout << "time in micro-seconds ..." << endl << endl;
	cout << "	Total run time = " << PerformanceStats::GetTotalRunTime() << endl;
	cout << "	Total CSV TO BIN file time: " << PerformanceStats::GetTotalCsvToBinTime() << endl;
	cout << "	Total BIN back to CSV file time: " << PerformanceStats::GetTotalBinToCsvTime() << endl << endl;
	cout << "	Average Record csv to binary format conversion time: " << PerformanceStats::GetAverageRecordCsvTobinTime() << endl;
	cout << "	Average Record binary to csv format conversion time: " << PerformanceStats::GetAverageRecordBinToCsvTime() << endl << endl;
	cout << "	Average Quote csv to binary format conversion time: " << PerformanceStats::GetAverageQuoteCsvToBinTime() << endl;
	cout << "	Average Quote binary to csv format conversion time: " << PerformanceStats::GetAverageQuoteBinToCsvTime() << endl << endl;
	cout << "	Average Trade csv to binary format conversion time: " << PerformanceStats::GetAverageTradeCsvToBinTime() << endl;
	cout << "	Average Trade binary to csv format conversion time: " << PerformanceStats::GetAverageTradeBinToCsvTime() << endl << endl;
	cout << "	Average Signal csv to binary format conversion time: " << PerformanceStats::GetAverageSignalCsvToBinTime() << endl;
	cout << "	Average Signal binary to csv format conversion time: " << PerformanceStats::GetAverageSignalBinToCsvTime() << endl << endl;
	return;
}



double PerformanceStats::GetTotalRunTime()
{
	return PerformanceStats::whole_test_time;
}

double PerformanceStats::GetTotalCsvToBinTime()
{
	return PerformanceStats::whole_csv_to_binary_time;
}

double PerformanceStats::GetTotalBinToCsvTime()
{
	return PerformanceStats::whole_bin_to_csv_time;
}

double PerformanceStats::GetAverageRecordCsvTobinTime()
{
	return PerformanceStats::average_record_csv_to_bin;
}

double PerformanceStats::GetAverageRecordBinToCsvTime()
{
	return PerformanceStats::average_record_bin_to_csv;
}

double PerformanceStats::GetAverageQuoteCsvToBinTime()
{
	return PerformanceStats::average_quote_csv_to_bin;
}

double PerformanceStats::GetAverageQuoteBinToCsvTime()
{
	return PerformanceStats::average_quote_bin_to_csv;
}

double PerformanceStats::GetAverageTradeCsvToBinTime()
{
	return PerformanceStats::average_trade_csv_to_bin;
}

double PerformanceStats::GetAverageTradeBinToCsvTime()
{
	return PerformanceStats::average_trade_bin_to_csv;
}

double PerformanceStats::GetAverageSignalCsvToBinTime()
{
	return PerformanceStats::average_signal_csv_to_bin;
}

double PerformanceStats::GetAverageSignalBinToCsvTime()
{
	return PerformanceStats::average_signal_bin_to_csv;
}


void PerformanceStats::UpdateTotalRunTime(std::chrono::time_point<std::chrono::high_resolution_clock> start,
	std::chrono::time_point<std::chrono::high_resolution_clock> end)
{
	std::chrono::duration<double> dur = end - start;
	auto micro_secs = std::chrono::duration_cast<std::chrono::microseconds>(dur).count();
	PerformanceStats::whole_test_time = micro_secs;
}

void PerformanceStats::UpdateTotalCsvToBinTime(std::chrono::time_point<std::chrono::high_resolution_clock> start,
	std::chrono::time_point<std::chrono::high_resolution_clock> end)
{
	std::chrono::duration<double> dur = end - start;
	auto micro_secs = std::chrono::duration_cast<std::chrono::microseconds>(dur).count();
	PerformanceStats::whole_csv_to_binary_time = micro_secs;
}

void PerformanceStats::UpdateTotalBinToCsvTime(std::chrono::time_point<std::chrono::high_resolution_clock> start,
	std::chrono::time_point<std::chrono::high_resolution_clock> end)
{
	std::chrono::duration<double> dur = end - start;
	auto micro_secs = std::chrono::duration_cast<std::chrono::microseconds>(dur).count();
	PerformanceStats::whole_bin_to_csv_time = micro_secs;
}


void PerformanceStats::UpdateAverageRecordCsvToBinTime(std::chrono::time_point<std::chrono::high_resolution_clock> start,
	std::chrono::time_point<std::chrono::high_resolution_clock> end)
{
	if (PerformanceStats::total_records_from_csv_to_bin == 0)
	{
		std::chrono::duration<double> dur = end - start;
		auto micro_secs = std::chrono::duration_cast<std::chrono::microseconds>(dur).count();
		PerformanceStats::average_record_csv_to_bin = micro_secs;
		PerformanceStats::total_records_from_csv_to_bin += 1;
	}
	else
	{
		std::chrono::duration<double> dur = end - start;
		auto micro_secs = std::chrono::duration_cast<std::chrono::microseconds>(dur).count();
		PerformanceStats::average_record_csv_to_bin = (PerformanceStats::average_record_csv_to_bin * PerformanceStats::total_records_from_csv_to_bin);
		PerformanceStats::average_record_csv_to_bin = PerformanceStats::average_record_csv_to_bin + micro_secs;
		PerformanceStats::total_records_from_csv_to_bin += 1;
		PerformanceStats::average_record_csv_to_bin = PerformanceStats::average_record_csv_to_bin / PerformanceStats::total_records_from_csv_to_bin;
	}
	return;
}
void PerformanceStats::UpdateAverageRecordBinToCsvTime(std::chrono::time_point<std::chrono::high_resolution_clock> start,
	std::chrono::time_point<std::chrono::high_resolution_clock> end)
{
	if (PerformanceStats::total_records_bin_to_csv == 0)
	{
		std::chrono::duration<double> dur = end - start;
		auto micro_secs = std::chrono::duration_cast<std::chrono::microseconds>(dur).count();
		PerformanceStats::average_record_bin_to_csv = micro_secs;
		PerformanceStats::total_records_bin_to_csv += 1;
	}
	else
	{
		std::chrono::duration<double> dur = end - start;
		auto micro_secs = std::chrono::duration_cast<std::chrono::microseconds>(dur).count();
		PerformanceStats::average_record_bin_to_csv = (PerformanceStats::average_record_bin_to_csv * PerformanceStats::total_records_bin_to_csv);
		PerformanceStats::average_record_bin_to_csv = PerformanceStats::average_record_bin_to_csv + micro_secs;
		PerformanceStats::total_records_bin_to_csv += 1;
		PerformanceStats::average_record_bin_to_csv = PerformanceStats::average_record_bin_to_csv / PerformanceStats::total_records_bin_to_csv;
	}
	return;
}



void PerformanceStats::UpdateAverageQuoteCsvToBinTime(std::chrono::time_point<std::chrono::high_resolution_clock> start, 
	std::chrono::time_point<std::chrono::high_resolution_clock> end)
{
	PerformanceStats::UpdateAverageRecordCsvToBinTime(start, end);
	if (PerformanceStats::total_quotes_from_csv_to_bin == 0)
	{
		std::chrono::duration<double> dur = end - start;
		auto micro_secs = std::chrono::duration_cast<std::chrono::microseconds>(dur).count();
		PerformanceStats::average_quote_csv_to_bin = micro_secs;
		PerformanceStats::total_quotes_from_csv_to_bin += 1;
	}
	else
	{
		std::chrono::duration<double> dur = end - start;
		auto micro_secs = std::chrono::duration_cast<std::chrono::microseconds>(dur).count();
		PerformanceStats::average_quote_csv_to_bin = (PerformanceStats::average_quote_csv_to_bin * PerformanceStats::total_quotes_from_csv_to_bin);
		PerformanceStats::average_quote_csv_to_bin = PerformanceStats::average_quote_csv_to_bin + micro_secs;
		PerformanceStats::total_quotes_from_csv_to_bin += 1;
		PerformanceStats::average_quote_csv_to_bin = PerformanceStats::average_quote_csv_to_bin / PerformanceStats::total_quotes_from_csv_to_bin;
	}
	return;
}
void PerformanceStats::UpdateAverageQuoteBinToCsvTime(std::chrono::time_point<std::chrono::high_resolution_clock> start, 
	std::chrono::time_point<std::chrono::high_resolution_clock> end)
{
	PerformanceStats::UpdateAverageRecordBinToCsvTime(start, end);
	if (PerformanceStats::total_quotes_bin_to_csv == 0)
	{
		std::chrono::duration<double> dur = end - start;
		auto micro_secs = std::chrono::duration_cast<std::chrono::microseconds>(dur).count();
		PerformanceStats::average_quote_bin_to_csv = micro_secs;
		PerformanceStats::total_quotes_bin_to_csv += 1;
	}
	else
	{
		std::chrono::duration<double> dur = end - start;
		auto micro_secs = std::chrono::duration_cast<std::chrono::microseconds>(dur).count();
		PerformanceStats::average_quote_bin_to_csv = (PerformanceStats::average_quote_bin_to_csv * PerformanceStats::total_quotes_bin_to_csv);
		PerformanceStats::average_quote_bin_to_csv = PerformanceStats::average_quote_bin_to_csv + micro_secs;
		PerformanceStats::total_quotes_bin_to_csv += 1;
		PerformanceStats::average_quote_bin_to_csv = PerformanceStats::average_quote_bin_to_csv / PerformanceStats::total_quotes_bin_to_csv;
	}
	return;

}




void PerformanceStats::UpdateAverageTradeCsvToBinTime(std::chrono::time_point<std::chrono::high_resolution_clock> start, 
	std::chrono::time_point<std::chrono::high_resolution_clock> end)
{
	PerformanceStats::UpdateAverageRecordCsvToBinTime(start, end);
	if (PerformanceStats::total_trades_csv_to_bin == 0)
	{
		std::chrono::duration<double> dur = end - start;
		auto micro_secs = std::chrono::duration_cast<std::chrono::microseconds>(dur).count();
		PerformanceStats::average_trade_csv_to_bin = micro_secs;
		PerformanceStats::total_trades_csv_to_bin += 1;
	}
	else
	{
		std::chrono::duration<double> dur = end - start;
		auto micro_secs = std::chrono::duration_cast<std::chrono::microseconds>(dur).count();
		PerformanceStats::average_trade_csv_to_bin = (PerformanceStats::average_trade_csv_to_bin * PerformanceStats::total_trades_csv_to_bin);
		PerformanceStats::average_trade_csv_to_bin = PerformanceStats::average_trade_csv_to_bin + micro_secs;
		PerformanceStats::total_trades_csv_to_bin += 1;
		PerformanceStats::average_trade_csv_to_bin = PerformanceStats::average_trade_csv_to_bin / PerformanceStats::total_trades_csv_to_bin;
	}
	return;
}
void PerformanceStats::UpdateAverageTradeBinToCsvTime(std::chrono::time_point<std::chrono::high_resolution_clock> start, 
	std::chrono::time_point<std::chrono::high_resolution_clock> end)
{
	PerformanceStats::UpdateAverageRecordBinToCsvTime(start, end);
	if (PerformanceStats::total_trades_bin_to_csv == 0)
	{
		std::chrono::duration<double> dur = end - start;
		auto micro_secs = std::chrono::duration_cast<std::chrono::microseconds>(dur).count();
		PerformanceStats::average_trade_bin_to_csv = micro_secs;
		PerformanceStats::total_trades_bin_to_csv += 1;
	}
	else
	{
		std::chrono::duration<double> dur = end - start;
		auto micro_secs = std::chrono::duration_cast<std::chrono::microseconds>(dur).count();
		PerformanceStats::average_trade_bin_to_csv = (PerformanceStats::average_trade_bin_to_csv * PerformanceStats::total_trades_bin_to_csv);
		PerformanceStats::average_trade_bin_to_csv = PerformanceStats::average_trade_bin_to_csv + micro_secs;
		PerformanceStats::total_trades_bin_to_csv += 1;
		PerformanceStats::average_trade_bin_to_csv = PerformanceStats::average_trade_bin_to_csv / PerformanceStats::total_trades_bin_to_csv;
	}
	return;
}



void PerformanceStats::UpdateAverageSignalCsvToBinTime(std::chrono::time_point<std::chrono::high_resolution_clock> start, 
	std::chrono::time_point<std::chrono::high_resolution_clock> end)
{
	PerformanceStats::UpdateAverageRecordCsvToBinTime(start, end);
	if (PerformanceStats::total_signals_csv_to_bin == 0)
	{
		std::chrono::duration<double> dur = end - start;
		auto micro_secs = std::chrono::duration_cast<std::chrono::microseconds>(dur).count();
		PerformanceStats::average_signal_csv_to_bin = micro_secs;
		PerformanceStats::total_signals_csv_to_bin += 1;
	}
	else
	{
		std::chrono::duration<double> dur = end - start;
		auto micro_secs = std::chrono::duration_cast<std::chrono::microseconds>(dur).count();
		PerformanceStats::average_signal_csv_to_bin = (PerformanceStats::average_signal_csv_to_bin * PerformanceStats::total_signals_csv_to_bin);
		PerformanceStats::average_signal_csv_to_bin = PerformanceStats::average_signal_csv_to_bin + micro_secs;
		PerformanceStats::total_signals_csv_to_bin += 1;
		PerformanceStats::average_signal_csv_to_bin = PerformanceStats::average_signal_csv_to_bin / PerformanceStats::total_signals_csv_to_bin;
	}
	return;
}
void PerformanceStats::UpdateAverageSignalBinToCsvTime(std::chrono::time_point<std::chrono::high_resolution_clock> start, 
	std::chrono::time_point<std::chrono::high_resolution_clock> end)
{
	PerformanceStats::UpdateAverageRecordBinToCsvTime(start, end);
	if (PerformanceStats::total_signals_bin_to_csv == 0)
	{
		std::chrono::duration<double> dur = end - start;
		auto micro_secs = std::chrono::duration_cast<std::chrono::microseconds>(dur).count();
		PerformanceStats::average_signal_bin_to_csv = micro_secs;
		PerformanceStats::total_signals_bin_to_csv += 1;
	}
	else
	{
		std::chrono::duration<double> dur = end - start;
		auto micro_secs = std::chrono::duration_cast<std::chrono::microseconds>(dur).count();
		PerformanceStats::average_signal_bin_to_csv = (PerformanceStats::average_signal_bin_to_csv * PerformanceStats::total_signals_bin_to_csv);
		PerformanceStats::average_signal_bin_to_csv = PerformanceStats::average_signal_bin_to_csv + micro_secs;
		PerformanceStats::total_signals_bin_to_csv += 1;
		PerformanceStats::average_signal_bin_to_csv = PerformanceStats::average_signal_bin_to_csv / PerformanceStats::total_signals_bin_to_csv;
	}
	return;
}