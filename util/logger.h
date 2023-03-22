#pragma once

#include <string>
#include <fstream>

using std::string;
using std::ofstream;

#include <thread/auto_lock.h>

using namespace lh::thread;

namespace lh {
namespace util {

#define log_debug(format, ...)\
	Logger::instance() -> log(Logger::LOG_DEBUG, __FILE__, __LINE__, format, ##__VA__ARGS__)

#define log_info(format, ...)\
	Logger::instance() -> log(Logger::LOG_INFO, __FILE__, __LINE__, format, ##__VA__ARGS__)

#define log_warn(format, ...)\
	Logger::instance() -> log(Logger::LOG_WARN, __FILE__, __LINE__, format, ##__VA__ARGS__)

#define log_error(format, ...)\
	Logger::instance() -> log(Logger::LOG_ERROR, __FILE__, __LINE__, format, ##__VA__ARGS__)

#define log_fatal(format, ...)\
	Logger::instance() -> log(Logger::LOG_FATAL, __FILE__, __LINE__, format, ##__VA__ARGS__)

	class Logger {
	public:
		enum Level {
			LOG_DEBUG = 0,
			LOG_INFO,
			LOG_WARN,
			LOG_ERROR,
			LOG_FATAL,
			LOG_COUNT
		};

		static Logger* instance();
		void open(const string& filename);
		void close();
		void log(Level level, const char* file, int line, const char* format, ...);
		void max(int bytes);
		void level(int level);

	private:
		Logger();
		~Logger();
		void rotate();

	private:
		string __filename;
		ofstream __fout;
		int __max_m; // 文件最大长度
		int __len; // 当前长度
		int __level; // 当前日志等级
		Mutex __mutex;
		static const char* __level_s[LOG_COUNT];
		// 单例模式
		static Logger* __instance;

	};

}
}


