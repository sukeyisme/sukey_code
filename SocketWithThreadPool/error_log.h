#pragma once

#include <string>
#include <iostream>
using namespace std;

#ifndef ERROR_TRY
#define ERROR_TRY try\
	{
#endif

#ifndef ERROR_UNKNOW
#define ERROR_UNKNOW(ERROR_CLASS, ERROR_METHOD) \
	}\
	catch (...)\
	{\
		ReportError(ERROR_CLASS, ERROR_METHOD);\
	}
#endif

#ifndef ERROR_UNKNOW_RETURN
#define ERROR_UNKNOW_RETURN(ERROR_CLASS, ERROR_METHOD, ERROR_RETURN) \
		return ERROR_RETURN;\
	}\
	catch (...)\
	{\
		ReportError(ERROR_CLASS, ERROR_METHOD);\
		return ERROR_RETURN;\
	}
#endif

#ifndef ERROR_UNKNOW_POINT
#define ERROR_UNKNOW_POINT(ERROR_CLASS, ERROR_METHOD, ERROR_POINT) \
	}\
	catch (...)\
	{\
		ReportError(ERROR_CLASS, ERROR_METHOD);\
	}
#endif

#ifndef ERROR_UNKNOW_POINT
#define ERROR_UNKNOW_POINT(ERROR_CLASS, ERROR_METHOD, POINT_NAME, ERROR_POINT) \
	}\
	catch (...)\
	{\
		ReportError(ERROR_CLASS, ERROR_METHOD, POINT_NAME, ERROR_POIN);\
	}
#endif

#ifndef ERROR_UNKNOW_POINT_RETURN
#define ERROR_UNKNOW_POINT_RETURN(ERROR_CLASS, ERROR_METHOD, POINT_NAME, ERROR_POINT, ERROR_RETURN) \
		return ERROR_RETURN;\
	}\
	catch (...)\
	{\
		ReportError(ERROR_CLASS, ERROR_METHOD, POINT_NAME, ERROR_POIN);\
		return ERROR_RETURN;\
	}
#endif



class CErrorLog
{
public:
	CErrorLog();
	~CErrorLog();
	
protected:
	static void	ReportError(const string &sClass, const string &sMethod);
	static void	ReportError(const string &sClass, const string &sMethod, const string &sPoint, void *pPoint);
};


