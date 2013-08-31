#include "error_log.h"

CErrorLog::CErrorLog()
{
}

CErrorLog::~CErrorLog()
{
}

void CErrorLog::ReportError(const string &sClass, const string &sMethod)
{
	cout << "Error class name is " << sClass << "\r\n"
		<< "Error Method is " << sMethod << endl;
}

void CErrorLog::ReportError(const string &sClass, const string &sMethod, const string &sPoint, void *pPoint)
{
	cout << "Error class name is " << sClass << "\r\n"
		<< "Error Method is " << sMethod << "\r\n"
		<< "Error point is " << sPoint << ":" << pPoint << endl;
}
