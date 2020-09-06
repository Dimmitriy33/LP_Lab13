#pragma once
#include <stddef.h>
#include "Parm.h"

namespace FST
{
	struct RELATION // ребро:символ -> вершина графа переходов КА
	{
		char symbol; // символ перехода
		short nnode; // номер смежной вершины
		RELATION(
			char c = 0x00,
			short ns = NULL
			);
	};

	struct NODE // вершина графов переходов
	{
		short n_relation; // количество инциндентных ребер
		RELATION* relations; // инциндентные ребра
		NODE();
		NODE(
			short n, // количество инциндетных ребер
			RELATION rel, ... // список ребер
		);
	};

	struct FST // недетерминированный конечный автомат
	{
		const char* string; // цепочка(строка, завершается 0x00)
		short position; // текущая позиция в цепочке
		short nstates; // количество состояний автомата
		NODE* nodes; // граф переходов: [0] - начальное состояние, [nstates-1] - конечное
		short* rstates; // возможные состояние автомата на данной позиции
		FST(
			const char* s, // цепочка
			short ns, // количество состояний автомата
			NODE N, ... // список состояний (граф переходов)
		);
	};
	bool step(FST& fst, short*& rstates); // шаг автомата

	bool execute( // выполнить распознавание цепочки
		FST& fst // недетерминированный конечный автомат
	);

	void NewLine(const char* str, int line, Parm::PARM parm);
	void read(Parm::PARM parm);

}
