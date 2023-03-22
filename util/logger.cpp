#include <util/logger.h>
using namespace lh::util;

#include <time.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <stdexcept>

const char* Logger::__level_s[LOG_COUNT] = {
	"DEBUG",
	"INFO",
	"WARN",
	"ERROR",
	"FATAL"
};

Logger* Logger::__instance = nullptr;

Logger::Logger() : __max_m(0), __len(0), __level(LOG_DEBUG) {}

Logger::~Logger() {
	close();
}

Logger* Logger::instance() {
	if (__instance == nullptr)
		__instance = new Logger();

	return __instance;
}

void Logger::open(const string& filename) {
	__filename = filename;
	// 打开方式std::ios:app 如果没有文件，生成空文件；如果有文件，在文件尾追加
	__fout.open(filename.c_str(), std::ios::app);
	if (__fout.fail()) {
		throw std::logic_error("open log file failed: " + filename);
	}
	// 移动写指针到文件末尾
	// seekp(int offset, int mode)  指向从文件mode位置开始向后的 offset 字节处。offset 等于 0 即代表文件开头
	__fout.seekp(0, std::ios::end);
	// 获取文件长度
	__len = __fout.tellp();
}

void Logger::close() {
	__fout.close();
}

void Logger::log(Level level, const char* file, int line, const char* format, ...) {
	// 日志同一时刻只能由一个线程写入
	AutoLock lock(&__mutex);
	// 只有log等级大于当前等级才log
	if (__level > level) {
		return;
	}

	if (__fout.fail()) {
		return;
	}
	// time_t 这种类型就是用来存储从1970年到现在经过了多少秒
	time_t ticks = time(NULL);
	// struct tm* 获得日历时间
	struct tm* ptm = localtime(&ticks);
	char timestamp[32];
	memset(timestamp, 0, sizeof timestamp);
	// 转换时间格式为：%Y-%m-%d %H:%M:%S
	strftime(timestamp, sizeof timestamp, "%Y-%m-%d %H:%M:%S", ptm);

	int len = 0;
	const char* fmt = "%s %s %s:%d";
	// 将timestamp, level_[level], file, line以fmt的格式写入一行
	len = snprintf(NULL, 0, fmt, timestamp, __level_s[level], file, line);
	if (len > 0) {
		// 打印写入内容日志
		char* buffer = new char[len + 1];
		snprintf(buffer, len + 1, fmt, timestamp, __level_s[level], file, line);
		buffer[len] = 0;
		__fout << buffer;
		delete[] buffer;
		__len += len;
	}

	// va_list 获取可变参数
	va_list arg_ptr;
	va_start(arg_ptr, format); // 递归获取
	len = vsnprintf(NULL, 0, format, arg_ptr);
	va_end(arg_ptr);
	if (len > 0) {
		char* content = new char[len + 1];
		va_start(arg_ptr, format);
		vsnprintf(content, len + 1, format, arg_ptr);
		va_end(arg_ptr);
		content[len] = 0;
		__fout << content;
		delete[] content;
		__len += len;
	}

	__fout << "\n";
	__fout.flush();

	if (__max_m > 0 && __len >= __max_m) {
		rotate();
	}
}

void Logger::max(int bytes) {
	__max_m = bytes;
}

void Logger::level(int level) {
	__level = level;
}

void Logger::rotate() {
	// 超出文件最大长度，重命名该文件后，开启一个新文件
	close();
	time_t ticks = time(NULL);
	struct tm* ptm = localtime(&ticks);
	char timestamp[32];
	memset(timestamp, 0, sizeof(timestamp));
	strftime(timestamp, sizeof(timestamp), ".%Y-%m-%d_%H-%M-%S", ptm);
	string filename = __filename + timestamp;
	if (rename(__filename.c_str(), filename.c_str()) != 0)
	{
		throw std::logic_error("rename log file failed: " + string(strerror(errno)));
	}
	open(__filename);
}

