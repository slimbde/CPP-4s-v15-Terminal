#pragma once
#include "Factories.h"

using namespace System;
using namespace System::Collections::Generic;

namespace Terminal
{
	public ref struct TerminalInfo
	{
		String^ UnhandledInfo;
		List<String^>^ GateInfo;
		int elapsed;

		TerminalInfo() { GateInfo = gcnew List<String^>(); }
	};




	///////////////////////////////////////////////////// PASSENGER
	public ref class GatesTerminal
	{
		Queue<IHandleable^>^ people;
		List<IGate^>^ gates;
		int elapsedTime;
		Dictionary<String^, IGate^>^ gatesDone;
		int extraSync;

	public:
		GatesTerminal(PassengerFactory^ pFactory, GatesFactory^ gFactory)
		{
			people = pFactory->Create(50);
			gates = gFactory->Create(4);
			gatesDone = gcnew Dictionary<String^, IGate^>();
		}


		TerminalInfo^ Sync()
		{
			++elapsedTime;

			auto info = gcnew TerminalInfo();
			info->elapsed = elapsedTime;
			info->UnhandledInfo = String::Concat("Не обслуженных: ", people->Count);

			for each (IGate^ gate in gates)
			{
				info->GateInfo->Add(String::Concat(gate->ToString(), ": ", gate->GetLength(), " чел.   обслужено: ", gate->GetHandledNumber()));
				routeNewPassenger();

				try { gate->Sync(); }
				catch (Exception^)
				{
					if (!gatesDone->ContainsKey(gate->ToString()))
						gatesDone->Add(gate->ToString(), gate);

					if (gatesDone->Count == gates->Count && extraSync++ > 0)
						throw gcnew Exception("Регистрация завершена");
				}
			}

			return info;
		}



	private:
		void routeNewPassenger()
		{
			if (people->Count > 0)
			{
				auto passgr = people->Dequeue();

				// picking out the gate for the passenger
				int aptGateIndex = -1;
				int currentMax = int::MaxValue;
				for (int i = gates->Count - 1; i >= 0; --i)
					if (gates[i]->GetLength() <= currentMax)
					{
						currentMax = gates[i]->GetLength();
						aptGateIndex = i;
					}

				IGate^ gate = gates[aptGateIndex];

				if (gate->GetLength() < gate->MaxPeople)
					gate->Enqueue(passgr);
				else
					people->Enqueue(passgr);
			}
		}
	};





}