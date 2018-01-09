#include "stdafx.h"
#include "LLogger.h"
#include <ctime>


using namespace std;

namespace lu
{
#if !defined PROJECT_NAME
#define PROJECT_NAME ""
#endif

	LLogger lerr(std::cerr);
	LLogger lout(std::cout);
	ofstream logfile("record/"+string(PROJECT_NAME) + ".log", ios::app | ios::out);
	LLogger llog(logfile);

	LLogger::LLogger(std::ostream & stream)
		:_output_stream(stream), _enable_info(false),
		_enable_debug(false), _enable_err(true),
		_old_buf(nullptr),_enable(true)
	{}

	LLogger::~LLogger()
	{
		if (_output_file.is_open())
		{
			_output_file.close();
		}
	}

	LLogger::PrintThread LLogger::Info()
	{
		return Output("Info", _enable_info);
	}

	LLogger::PrintThread LLogger::Err()
	{
		return Output("Err", _enable_err);
	}

	lu::LLogger::PrintThread LLogger::Debug()
	{
		return Output("Debug", _enable_debug);
	}

	lu::LLogger::PrintThread LLogger::Log(std::string title)
	{
		bool enable = true;

		{
			lock_guard<mutex> lock(_mutex_map);

			auto iter = _enable_map.find(title);
			if (iter != _enable_map.end())
			{
				enable = iter->second;
			}
			_enable_map[title] = true;
		}

		return Output(title, enable);
	}

	void LLogger::EnableLog(std::string title, bool enable)
	{
		lock_guard<mutex> lock(_mutex_map);
		_enable_map[title] = enable;
	}

	void LLogger::AddLog(std::string str, bool enable /*= true*/)
	{
		lock_guard<mutex> lock(_mutex_map);
		_enable_map.insert(make_pair(str,enable));
	}

	void LLogger::RemoveLog(std::string str)
	{
		lock_guard<mutex> lock(_mutex_map);
		_enable_map.erase(str);
	}

	void LLogger::RemoveAllLog()
	{
		lock_guard<mutex> lock(_mutex_map);
		_enable_map.clear();
	}

	void LLogger::RedirectToFile(std::string filename)
	{
		if (_output_file.is_open())
		{
			_output_file.close();
		}
		if (filename.find('.') == std::string::npos)
		{
			filename += ".log";
		}
		_output_file.open(filename, ios::app | ios::out);

		Redirect(_output_file);
	}

	void LLogger::RedirecBack()
	{
		if (_old_buf)
		{
			_output_stream.rdbuf(_old_buf);
			_old_buf = nullptr;
		}
	}

	void LLogger::Redirect(std::ostream& output_stream)
	{
		if (_old_buf)
		{
			_output_stream.rdbuf(output_stream.rdbuf());
		}
		else
		{
			_old_buf = _output_stream.rdbuf(output_stream.rdbuf());
		}
	}

	LLogger::PrintThread LLogger::Output(std::string title,bool enable)
	{
		time_t t = time(0);
		tm now;
		localtime_s(&now,&t);

		char buffer[50];
		int count = sprintf_s(buffer, 50, "[%04d-%02d-%02d %02d:%02d:%02d],",
			now.tm_year + 1900, now.tm_mon + 1, now.tm_mday, now.tm_hour, now.tm_min, now.tm_sec);

		string str_time(buffer);
		return PrintThread(_mutex_stream, _output_stream, "[" + title + "]," + str_time, enable && _enable);
	}

}

