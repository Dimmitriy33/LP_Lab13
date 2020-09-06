
#define _CRT_SECURE_NO_WARNINGS

#include "Log.h"

namespace Log
{
	LOG getlog(wchar_t logfile[]) //создание и открытие потокового вывода
	{
		LOG f;
		f.stream = new std::ofstream;
		f.stream->open(logfile);
		if (!f.stream->is_open())
			throw ERROR_THROW(112);
		wcscpy_s(f.logfile, logfile);
		return f;
	}

	void WriteLine(LOG log, char* c, ...) // конкатенирует элементы строки и выводит в протокол
	{
		va_list arg;
		va_start(arg, c);
		char* sym = c;
		std::string str;
		while (sym != "")
		{
			str += sym;
			sym = va_arg(arg, char*);
		}
		va_end(arg);
		*log.stream << str << "\n";
		return;
	}

	/*void WriteLine(LOG log, wchar_t* c, ...)
	{
		wchar_t str[255], *sym = c;
		va_list arg;
		va_start(arg, c);
		while (sym != L"")
		{
			for (int i = 0; i < wcslen(sym); i++)
				str[i] = arg[i];
			sym = va_arg(arg, wchar_t*);
			*log.stream << str;
		}
		va_end(arg);
		*log.stream << "\n";
		return;
	}*/

	void WriteLine(LOG log, const wchar_t* c, ...) {
		char convertedBuffer[255];
		const wchar_t** args = &c;

		for (int i = 0; args[i] != L""; ++i) {
			wcstombs_s(nullptr, convertedBuffer, args[i], sizeof(convertedBuffer));
			*log.stream << convertedBuffer;
		}
		*log.stream << std::endl;
	}

	void WriteLog(LOG log) // выводит заголовок протокола
	{
		tm DateNow;
		time_t TimeNow = time(0);
		localtime_s(&DateNow, &TimeNow);
		*log.stream << "----- Протокол ----- Дата: ";
		//tm_mday - день месяца, tm_mon - номер месяца(отсчет с 0, аоэтому +1), .tm_year - номер года, начиная с 1900
		*log.stream << DateNow.tm_mday << "." << DateNow.tm_mon + 1<< "." << 1900 + DateNow.tm_year; // с коррекцией
		*log.stream << " " << std::setw(2) << DateNow.tm_hour;
		*log.stream << ":" << std::setw(2) << DateNow.tm_min;
		*log.stream << ":" << std::setw(2) << DateNow.tm_sec;
		*log.stream << " ------\n";
		return;
	}

	void WriteParm(LOG log, Parm::PARM parm) //выводит информацию о входных параметрах в протокол
	{
		char str[255];
		*log.stream << "----- Параметры -----\n";
		//wcstombs -- Преобразует последовательность широких символов в соответствующую последовательность многобайтовых символов.
		wcstombs(str, parm.in, wcslen(parm.in)); // куда, что, количество
		str[wcslen(parm.in)] = '\0';
		*log.stream << "-in: ";
		log.stream->write(str, strlen(str)) << '\n';

		wcstombs(str, parm.out, wcslen(parm.out)); // куда, что, количество
		str[wcslen(parm.out)] = '\0';
		*log.stream << "-out: ";
		log.stream->write(str, strlen(str)) << '\n';

		wcstombs(str, parm.log, wcslen(parm.log)); // куда, что, количество
		str[wcslen(parm.log)] = '\0';
		str[wcslen(parm.log) + 1] = '\n';
		*log.stream << "-log: ";
		log.stream->write(str, strlen(str));
		return;
	}

	void WriteIn(LOG log, In::IN in) // вывод информации о входных данных
	{
		*log.stream << std::endl << "----- Исходные данные -----";
		*log.stream << "\nВсего символов:      " << in.size;
		*log.stream << "\nПроигнорированно:    " << in.ignor;
		*log.stream << "\nВсего строк:         " << in.lines;
		return;
	}

	void WriteError(LOG log, Error::ERROR err) // вывод ошибок
	{
		std::cout << "Ошибка " << err.id << ": " << err.message << ", строка " << err.inext.line << ", позиция " << err.inext.col << "\n\n";
	}

	void Close(LOG log) // хыкратие входного потока
	{
		log.stream->close();
		delete log.stream;
		return;
	}
}