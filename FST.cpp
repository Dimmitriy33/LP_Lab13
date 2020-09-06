#include "FST.h"
#include "Parm.h"
#include "Error.h"
#include "In.h"
#include <iostream>
#include <fstream>
using namespace std;
using namespace Error;

namespace FST
{
	// онструктор структуры RELATION
	RELATION::RELATION(char c, short nn)
	{
		symbol = c;
		nnode = nn;
	}

	// онструктор структуры NODE
	NODE::NODE() // по умолчанию
	{
		n_relation = 0;
		RELATION* relations = NULL;
	}

	NODE::NODE(short n, RELATION rel, ...) // с параметрами
	{
		n_relation = n;
		RELATION* p = &rel;
		relations = new RELATION[n];
		for (short i = 0; i < n; i++)
			relations[i] = p[i];
	}

	int col = 0;

	// онструктор структуры FST
	FST::FST(const char* s, short ns, NODE n, ...)
	{
		string = s;
		nstates = ns;
		nodes = new NODE[ns];
		NODE* p = &n;
		for (int k = 0; k < ns; k++)
			nodes[k] = p[k];
		rstates = new short[nstates];
		memset(rstates, 0xff, sizeof(short) * nstates); // заполн€ет nstates байтов блока пам€ти, через указатель rstates.
		rstates[0] = 0;
		position = -1;
	}

	bool step(FST& fst, short*& rstates) // один шаг автомата
	{
		bool rc = false;
		std::swap(rstates, fst.rstates); // смена массивов
		for (short i = 0; i < fst.nstates; i++)
		{
			if (rstates[i] == fst.position)
				for (short j = 0; j < fst.nodes[i].n_relation; j++)
				{
					if (fst.nodes[i].relations[j].symbol == fst.string[fst.position]) {
						fst.rstates[fst.nodes[i].relations[j].nnode] = fst.position + 1;
						rc = true;
					};		
				};
		};
		return rc;
	};

	bool execute(FST& fst)
	{
		short* rstates = new short[fst.nstates];
		memset(rstates, 0xff, sizeof(short) * fst.nstates);
		short lstring = strlen(fst.string);
		bool rc = true;
		for (short i = 0; i < lstring && rc; i++)
		{
			col = i;
			fst.position++; // продвинули позицию
			rc = step(fst, rstates); // один шаг автомата
		}
		delete[] rstates;
		return (rc ? (fst.rstates[fst.nstates - 1] == lstring) : rc);
	}

	void out(FST& fst, int i, Parm::PARM parm)
	{
		fstream fout;

		fout.open(parm.out, ios::app);//ios::app - дописыввать в конец файла
		if (!fout.is_open())
			throw ERROR_THROW(110);
		if (execute(fst))
			fout << "÷епочка " << fst.string << " распознана " << endl;
		else 
			fout << "÷епочка " << fst.string << " не распознана, " << i << " строка " << col + 1 << " позици€ " << endl;
		fout.close();
	}

	void NewLine(const char* str, int line, Parm::PARM parm)
	{
		FST fst(
			str,
			8,
			NODE(1, RELATION('a', 1)),
			NODE(3, RELATION('b', 1), RELATION('c', 2), RELATION('d', 2)),
			NODE(1, RELATION('b', 3)),
			NODE(2, RELATION('b', 3), RELATION('f', 4)),
			NODE(4, RELATION('b', 5), RELATION('c', 2), RELATION('d', 2), RELATION('e', 6)),
			NODE(2, RELATION('b', 5), RELATION('e', 6)),
			NODE(1, RELATION('f', 7)),
			NODE()
		);
		out(fst, line, parm);
	}

}