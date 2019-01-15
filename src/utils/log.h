/*
 * Log.h
 *
 *  Created on: Jan 6, 2019
 *      Author: zhangpeng <polygon.cn@gmail.com>
 */

#ifndef LOG_H_
#define LOG_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>
#include <stdarg.h>
#include <fcntl.h>

#include <sys/time.h>

#include <string>

#define MAX_DATETIME_SIZE 32
#define MAX_FILE_PATH_SIZE 256

#define LOG_TIME_FORMAT "%04d%02d%02d-%02d:%02d:%02d.%03ld"
#define LOG_FILE_TIME_FORMAT "%04d%02d%02d-%02d_%02d_%02d.%03ld"

typedef enum _log_level
{
    LOG_LEVEL_ERROR     = 1,  //Error
    LOG_LEVEL_WARNING   = 2,  //Warning
    LOG_LEVEL_INFO      = 3,  //info
    LOG_LEVEL_TRACE     = 4,  //trace
    LOG_LEVEL_MAX
} log_level_t;

class MutexLock
{
private:
    pthread_mutex_t m_lock;
public:
    MutexLock(void) {pthread_mutex_init(&m_lock, NULL);}
    virtual ~MutexLock(void) {pthread_mutex_destroy(&m_lock);}
    inline void lock() {pthread_mutex_lock(&m_lock);}
    inline void unlock() {pthread_mutex_unlock(&m_lock);}
};

#define SET_LOG_NAME(filepath)              CLogFactory::get_instance()->set_log_path(filepath)
#define SET_LOG_LEVEL(level)                CLogFactory::get_instance()->set_log_level(level)
#define SET_LOG_SIZE(size)                  CLogFactory::get_instance()->set_log_size(size)

#define LOG_TRACE(format, ...)              CLogFactory::get_instance()->write_log(LOG_LEVEL_TRACE, __FILE__, __LINE__, format, ## __VA_ARGS__)
#define LOG_INFO(format, ...)               CLogFactory::get_instance()->write_log(LOG_LEVEL_INFO, __FILE__, __LINE__, format, ## __VA_ARGS__)
#define LOG_WARNING(format, ...)            CLogFactory::get_instance()->write_log(LOG_LEVEL_WARNING, __FILE__, __LINE__, format, ## __VA_ARGS__)
#define LOG_ERROR(format, ...)              CLogFactory::get_instance()->write_log(LOG_LEVEL_ERROR, __FILE__, __LINE__, format, ## __VA_ARGS__)

class CLog
{
public:
	CLog();
	virtual ~CLog();
public:
	inline void     set_log_size(uint32_t size) {m_log_size = size;}
	inline void     set_log_level(log_level_t level){m_log_level = level;}
	inline uint64_t get_thread_id(){return getpid();}
	inline int      set_log_path(const char * filepath);
public:
	void  write_log(log_level_t level, const char *file, int line, const char *format_str, ...);
	char* get_time(const char * format);
	int rename_logfile();
private:
	uint32_t       m_log_size;//MB
	std::string    m_log_filepath;
	log_level_t    m_log_level;
	MutexLock      m_lock;
	FILE*          m_file;
	char           m_datetime[MAX_DATETIME_SIZE];
};

inline int CLog::set_log_path(const char * path)
{
	m_log_filepath = path;
	if(!m_file)
	{
		m_file = fopen(m_log_filepath.c_str(), "wb");

		if(!m_file)
		{
			fprintf(stderr, "open file error\n");
			return -1;
		}
	}
	return 0;
}

class CLogFactory
{
private:
    CLogFactory();
    virtual ~CLogFactory();
public:
    static CLog* get_instance();
    static void  free_instance();

private:
    static MutexLock    m_lock;
    static CLog        *m_log;
};

#endif /* LOG_H_ */
