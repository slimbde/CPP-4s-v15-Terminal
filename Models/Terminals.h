#pragma once
#include "Factories.h"
#include "PatternInterfaces.h"

using namespace System;
using namespace System::Collections::Generic;


#define PEOPLE_TO_CREATE 100
#define GATES_TO_CREATE 5


namespace Terminal
{
	///////////////////////////////////////////////////// TerminalInfo
	public ref struct TerminalInfo
	{
		String^ UnhandledInfo;
		List<GateInfo^>^ GateInfos;
		int elapsed;
		Dictionary<int, double>^ AvgHandlingTime;

		TerminalInfo() { GateInfos = gcnew List<GateInfo^>(); }
	};





	///////////////////////////////////////////////////// GatesTerminal
	public ref class GatesTerminal : IPublisher
	{
		PassengerFactory^ pFactory;
		Queue<IHandleable^>^ people;
		List<ISubscriber^>^ subscribers;
		int elapsedTime;

		List<ISubscriber^>^ idleSubscribers;
		int extraSync;							// the var allowes extra cycle when everyone is handled
		TerminalInfo^ summary;

	public:
		GatesTerminal(PassengerFactory^ pFactory, GatesFactory^ gFactory)
		{
			this->pFactory = pFactory;
			people = gcnew Queue<IHandleable^>();
			subscribers = gFactory->Create(GATES_TO_CREATE);

			for each (ISubscriber^ sub in subscribers)
				sub->Subscribe(this);
		}


		virtual Unsubscriber^ RegisterSubscriber(ISubscriber^ sub)
		{
			if (!subscribers->Contains(sub))
				subscribers->Add(sub);

			return gcnew Unsubscriber(subscribers, sub);
		}
		virtual TerminalInfo^ Sync()
		{
			++elapsedTime;

			auto info = gcnew TerminalInfo();
			info->elapsed = elapsedTime;
			info->UnhandledInfo = String::Concat("Не обслуженных: ", people->Count);

			for each (ISubscriber^ sub in subscribers)
				info->GateInfos->Add(sub->Sync());

			try
			{
				routeNewPassenger();
				return info;
			}
			catch (Exception^ ex)
			{
				summary = info;
				throw ex;
			}
		}
		virtual TerminalInfo^ GetSummary()
		{
			summary->AvgHandlingTime = gcnew Dictionary<int, double>();

			for each (GateInfo^ info in summary->GateInfos)
				summary->AvgHandlingTime->Add(info->Id, info->handledNum > 0 ? (double)info->totalHandledTime / info->handledNum : 0);

			return summary;
		}
		virtual int Increase()
		{
			extraSync = 0;
			idleSubscribers = gcnew List<ISubscriber^>();

			auto people = pFactory->Create(PEOPLE_TO_CREATE);
			for each (IHandleable^ one in people)
				this->people->Enqueue(one);

			return PEOPLE_TO_CREATE;
		}


	private:
		void routeNewPassenger()
		{
			if (people->Count > 0)
			{
				// picking out the gate for the passenger
				IHandleable^ passgr = people->Dequeue();

				int aptGateIndex = -1;
				int currentMax = int::MaxValue;
				for (int i = subscribers->Count - 1; i >= 0; --i)
					if (subscribers[i]->Length <= currentMax)
					{
						currentMax = subscribers[i]->Length;
						aptGateIndex = i;
					}

				subscribers[aptGateIndex]->Enqueue(passgr);
			}
			else
			{
				for (int i = 0; i < subscribers->Count; ++i)
				{
					auto sub = subscribers[i];
					if (sub->Length == 0 && !idleSubscribers->Contains(sub))
						idleSubscribers->Add(sub);
				}

				if (idleSubscribers->Count == subscribers->Count)
					++extraSync;

				if (extraSync > 2)
					throw gcnew Exception("Регистрация закончена");
			}
		}
	};

}