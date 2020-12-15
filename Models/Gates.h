#pragma once
#include "Passengers.h"
#include "PatternInterfaces.h"

using namespace System;
using namespace System::Collections::Generic;


namespace Terminal
{
	public ref struct GateInfo
	{
		String^ data;
		int handledNum;
		int totalHandledTime;
		int Id;

		virtual  String^ ToString() override { return data; }
	};




	///////////////////////////////////////////////// GATE
	public ref class Gate : ISubscriber
	{
		Unsubscriber^ unsubscriber;

		int id;
		Queue<IHandleable^>^ queue;
		Queue<IHandleable^>^ handled;
		int totalHandledTime;
		int backCounter;

	public:
		virtual property int Length { virtual int get() { return queue->Count; } virtual void set(int value) {}}

		Gate(int id)
		{
			queue = gcnew Queue<IHandleable^>();
			handled = gcnew Queue<IHandleable^>();
			this->id = id;
			backCounter = 0;
		}


		virtual void Enqueue(IHandleable^ one)
		{
			queue->Enqueue(one);
		}
		virtual void Subscribe(IPublisher^ pub)
		{
			if (pub != nullptr)
				unsubscriber = pub->RegisterSubscriber(this);
		}
		virtual void Unsubscribe() { unsubscriber->Unsubscribe(); }
		virtual GateInfo^ Sync()
		{
			auto gi = gcnew GateInfo();
			gi->data = String::Concat(this->ToString(), ": ", queue->Count, " чел.   обслужено: ", handled->Count);
			gi->handledNum = handled->Count;
			gi->Id = id;

			if (backCounter == 0)
			{
				if (queue->Count > 0)
				{
					auto passgr = queue->Dequeue();
					handled->Enqueue(passgr);
					backCounter = passgr->GetHandlingTime();
					
					totalHandledTime += passgr->GetHandlingTime();
				}
			}
			else --backCounter;

			gi->totalHandledTime = totalHandledTime;

			return gi;
		}

		virtual String^ ToString() override { return String::Concat("Окно ", id); }
	};

}
