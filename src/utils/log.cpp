/*
 * log.cpp
 *
 *  Created on: Jan 6, 2019
 *      Author: zhangpeng <polygon.cn@gmail.com>
 */


#include "../utils/log.h"

static const char* s_level_str[] = \
{"ERROR", "WARNING", "INFO", "TRACE"};

CLog::CLog():
m_log_size(512),
m_log_filepath(""),
m_log_level(LOG_LEVEL_TRACE),
m_lock(),
m_file(NULL),
m_datetime()
{
}

CLog::~CLog()
{
	if(m_file)
	{
		fclose(m_file);
		m_file = NULL;
	}
}

void CLog::write_log(log_level_t level, const char *file, int line, \
		const char * format_str, ...)
{
	if(m_log_level < level) return;

	m_lock.lock();
	assert(NULL != m_file);

	if(ftell(m_file) > m_log_size * 1024 * 1024)
	{
		if(rename_logfile())
		{
			m_lock.unlock();
			return;
		}

		m_file = fopen(m_log_filepath.c_str(), "wb");
		if (NULL == m_file)
		{
			fprintf(stderr, "Open log file(%s) fail!\n", m_log_filepath.c_str());
			m_lock.unlock();
			return;
		}
	}

    int write_len = fprintf(m_file, "[%s-%lu][%s][%d][%s]", \
    		get_time(LOG_TIME_FORMAT), \
    		get_thread_id(), file, line, s_level_str[level-1]);

    va_list p_list;
    va_start(p_list, format_str);
    write_len += vfprintf(m_file, format_str, p_list);
    va_end(p_list);
    write_len += fprintf(m_file, "\n");
    fflush(m_file);
    m_lock.unlock();
}

int CLog::rename_logfile()
{
	if(m_file)
	{
		fclose(m_file);
		m_file = NULL;
	}

	char new_path[MAX_FILE_PATH_SIZE];
	snprintf(new_path, MAX_FILE_PATH_SIZE, "%s-%s", m_log_filepath.c_str(), \
			get_time(LOG_FILE_TIME_FORMAT));

	if(rename(m_log_filepath.c_str(), new_path))
	{
		fprintf(stderr, "rename file error : %s\n", new_path);
		return -1;
	}

	return 0;
}

char * CLog::get_time(const char * format)
{
	time_t t = time(NULL);
	struct tm  tms;
	localtime_r(&t, &tms);

	struct timeval tv = {0};
	gettimeofday(&tv, 0);
	uint64_t milisec = tv.tv_usec/1000;

	snprintf(m_datetime, MAX_DATETIME_SIZE, format, \
	            (1900+tms.tm_year), (1+tms.tm_mon), tms.tm_mday, \
				tms.tm_hour, tms.tm_min, tms.tm_sec, milisec);
	return m_datetime;
}

MutexLock   CLogFactory::m_lock;
CLog       *CLogFactory::m_log = NULL;

CLog * CLogFactory::get_instance()
{
	CLog *plog = NULL;
	m_lock.lock();
	if(!m_log)
	{
		m_log = new CLog();
	}
	plog = m_log;
	m_lock.unlock();
	return plog;
}

void CLogFactory::free_instance()
{
	m_lock.lock();
	if(m_log)
	{
		delete m_log;
		m_log = NULL;
	}
	m_lock.unlock();
}
