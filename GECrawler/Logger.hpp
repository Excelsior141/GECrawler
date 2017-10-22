#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <iostream>

class Logger
{
public:

	static void Write(const char* pFormat, ...);

private:

	Logger();
	~Logger();

	static Logger* getInstance();
	void write(const char* pFormat, va_list& argp);

private:

	static Logger* mpLogger;

	char mBuffer[4096];
	FILE* mpFile;
};