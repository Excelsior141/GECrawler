#include "Logger.hpp"

Logger* Logger::mpLogger = nullptr;

void Logger::Write(const char* pFormat, ...)
{
	va_list argp;
	va_start(argp, pFormat);
	getInstance()->write(pFormat, argp);
	va_end(argp);
}

Logger::Logger()
{
	fopen_s(&mpFile, "Log.txt", "w");
}

Logger::~Logger()
{
	if (mpFile)
	{
		fclose(mpFile);
		mpFile = nullptr;
	}
}

Logger* Logger::getInstance()
{
	if (!mpLogger)
		mpLogger = new Logger();

	return mpLogger;
}

void Logger::write(const char* pFormat, va_list& argp)
{
	memset(mBuffer, 0, sizeof(mBuffer));

	vsprintf_s(mBuffer, pFormat, argp);

	std::cout << mBuffer << std::endl;

	if (mpFile)
		fprintf_s(mpFile, "%s\n", mBuffer);
}