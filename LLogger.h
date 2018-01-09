#pragma once

#include <iostream>
#include <sstream>
#include <fstream>
#include <mutex>
#include <map>

namespace lu
{
	class LLogger
	{
		class PrintThread : public std::ostringstream
		{
		public:
			PrintThread(std::mutex& mutex, std::ostream& stream, std::string prefix = "", bool verbose = true)
				:_mutexPrint(mutex), _output_stream(stream), _prefix(prefix), _verbose(verbose) {}

			PrintThread(const PrintThread& p)
				:_mutexPrint(p._mutexPrint), _output_stream(p._output_stream),
				_prefix(p._prefix), _verbose(p._verbose) {}

			~PrintThread()
			{
				if (str().size() != 0 && _verbose)
				{
					std::lock_guard<std::mutex> guard(_mutexPrint);
					_output_stream << _prefix << str();
					if (*str().rbegin() != '\n')
					{
						_output_stream << '\n';
					}
					_output_stream.flush();
				}
			}

		protected:
			std::mutex& _mutexPrint;
			std::ostream& _output_stream;
			std::string _prefix;
			bool _verbose;
		};

	public:
		LLogger(std::ostream& stream = std::cerr);

		~LLogger();

		// thread safe
		PrintThread Info();
		PrintThread Err();
		PrintThread Debug();
		PrintThread Log(std::string title);

		void EnableLog(std::string title, bool enable);
		void EnableInfo(bool enable) { _enable_info = enable; }
		void EnableErr(bool enable) { _enable_err = enable; }
		void EnableDebug(bool enable) { _enable_debug = enable; }
		void DisableAllExceptOriginal(bool disable = true) { _enable = !disable; }

		// useless
		void AddLog(std::string str, bool enable = true);
		void RemoveLog(std::string str);
		void RemoveAllLog();

		// not thread safe


		void RedirectToFile(std::string filename);
		void RedirecBack();
	protected:
		void Redirect(std::ostream& output_stream);

		PrintThread Output(std::string title,bool enable);
		std::mutex _mutex_stream;
		std::ostream& _output_stream;

		bool _enable_info;
		bool _enable_err;
		bool _enable_debug;
		bool _enable;

		std::mutex _mutex_map;
		std::map<std::string, bool> _enable_map;

		std::ofstream _output_file;
		std::streambuf* _old_buf;
	};

	extern LLogger lerr;
	extern LLogger lout;
	extern LLogger llog;
}


