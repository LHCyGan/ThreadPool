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
	// �򿪷�ʽstd::ios:app ���û���ļ������ɿ��ļ���������ļ������ļ�β׷��
	__fout.open(filename.c_str(), std::ios::app);
	if (__fout.fail()) {
		throw std::logic_error("open log file failed: " + filename);
	}
	// �ƶ�дָ�뵽�ļ�ĩβ
	// seekp(int offset, int mode)  ָ����ļ�modeλ�ÿ�ʼ���� offset �ֽڴ���offset ���� 0 �������ļ���ͷ
	__fout.seekp(0, std::ios::end);
	// ��ȡ�ļ�����
	__len = __fout.tellp();
}

void Logger::close() {
	__fout.close();
}

void Logger::log(Level level, const char* file, int line, const char* format, ...) {
	// ��־ͬһʱ��ֻ����һ���߳�д��
	AutoLock lock(&__mutex);
	// ֻ��log�ȼ����ڵ�ǰ�ȼ���log
	if (__level > level) {
		return;
	}

	if (__fout.fail()) {
		return;
	}
	// time_t �������;��������洢��1970�굽���ھ����˶�����
	time_t ticks = time(NULL);
	// struct tm* �������ʱ��
	struct tm* ptm = localtime(&ticks);
	char timestamp[32];
	memset(timestamp, 0, sizeof timestamp);
	// ת��ʱ���ʽΪ��%Y-%m-%d %H:%M:%S
	strftime(timestamp, sizeof timestamp, "%Y-%m-%d %H:%M:%S", ptm);

	int len = 0;
	const char* fmt = "%s %s %s:%d";
	// ��timestamp, level_[level], file, line��fmt�ĸ�ʽд��һ��
	len = snprintf(NULL, 0, fmt, timestamp, __level_s[level], file, line);
	if (len > 0) {
		// ��ӡд��������־
		char* buffer = new char[len + 1];
		snprintf(buffer, len + 1, fmt, timestamp, __level_s[level], file, line);
		buffer[len] = 0;
		__fout << buffer;
		delete[] buffer;
		__len += len;
	}

	// va_list ��ȡ�ɱ����
	va_list arg_ptr;
	va_start(arg_ptr, format); // �ݹ��ȡ
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
	// �����ļ���󳤶ȣ����������ļ��󣬿���һ�����ļ�
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

