#include "stdafx.h"
#include "LLogger.h"


using namespace std;



LLogger::LLogger(std::ostream & stream)
	:_output_stream(stream),
	_enable_info(false),_enable_err(true){}

LLogger::~LLogger()
{
}

LLogger::PrintThread LLogger::Info()
{
	return PrintThread(_mutex_stream,_output_stream,"[Info]",false);
}

LLogger::PrintThread LLogger::Err()
{
	return PrintThread(_mutex_stream, _output_stream, "[Error]");
}

void LLogger::Redirect(std::ostream& output_stream)
{
	_output_stream.rdbuf(output_stream.rdbuf());
}

