#include <tchar.h>
#include <iostream>
#include "FST.h"
#include "Error.h"
#include "In.h"
#include "Log.h"

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "rus");
	Log::LOG log = Log::INITLOG;
	/*std::cout << "---- тест In::getin	---" << std::endl << std::endl;*/
	try
	{
		Parm::PARM parm = Parm::getparm(argc, argv); // для записи значений входных параметров(in, out и log) в структуру PARM
		log = Log::getlog(parm.log);
		Log::WriteLine(log, L"Тест:", L" без ошибок ", L"");
		Log::WriteLog(log);
		Log::WriteParm(log, parm);
		In::IN in = In::getin(parm); // для ввода и проверки информации из файла
		Log::WriteIn(log, in);
		Log::Close(log);
		/*std::cout << in.text;*/
		
	}
	catch (Error::ERROR e)
	{
		Log::WriteError(log, e);
		/*std::cout << "Ошибка " << e.id << ": " << e.message << std::endl << std::endl;
		std::cout << "строка " << e.inext.line << " позиция " << e.inext.col << std::endl << std::endl;*/
	}
}
