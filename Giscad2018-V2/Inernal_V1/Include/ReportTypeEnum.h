#pragma once

#define MAX_LOGTYPES 5

enum REPORTTYPE
{
    REPORT_INVALID = -1,
	REPORT_HTML = 1,
	REPORT_TXT = 2,
	REPORT_MTEXT = 3
};

enum LOGTYPE
{

    LOG_INVALID = -1,
	LOG_HEADER = 1,
	LOG_GIS = 2,
    LOG_CAD = 3,
    LOG_OTHER = 4,
    LOG_WARNINGS = 5,
	LOG_INFO = 6,
};