#include "In.h"
#include "Error.h"
#include "FST.h"
#include <fstream>
#include <iostream>

namespace In
{
	IN getin(Parm::PARM parm)
	{
		std::ofstream fout;
		fout.open(parm.out);
		std::ifstream file(parm.in);
		if (!file.is_open())
			throw ERROR_THROW(110);
		if (!fout.is_open())
			throw ERROR_THROW(110);

		IN input;
		input.size = input.lines = input.ignor = 0;
		input.text = new unsigned char[IN_MAX_LEN_TEXT];

		//char table[] = IN_CODE_TABLE;

		int l, posstr = 0;
		std::string str;
		do
		{
			l = file.get(); 
			if (l != IN_CODE_DELIMITER && l != EOF)
			{
				if (l == IN_CODE_ENDL)
				{
					posstr = 0;
					input.lines++;
					const char* c = str.c_str();
					if(str != "")
						FST::NewLine(c, input.lines, parm);
					str = "";
				}
				else {
					str += l;
				}
			}
			else {
				const char* c = str.c_str();
				if (str != "")
					FST::NewLine(c, input.lines, parm);
				str = "";
			}

			if (input.code[l] == IN::F)
			{
				throw ERROR_THROW_IN(111, input.lines, posstr);
			}
			else if (input.code[l] == IN::I)
			{
				input.ignor++;
				continue;
			}
			else if (input.code[l] == IN::T)
			{
				input.text[input.size] = (char)l;
				input.size++;
			}
			else
			{
				input.text[input.size] = input.code[l];
				input.size++;

				posstr++;
			}
		} while (l != EOF);

		input.text[input.size] = '\0';
		file.close();
		fout.close();
		return input;
	}
}
