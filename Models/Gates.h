#pragma once
#include "Passengers.h"

using namespace System;
using namespace System::Collections::Generic;


namespace Terminal
{

	public interface class IGate
	{
		property int MaxPeople;

		void Enqueue(IHandleable^ one);
		int GetLength();
		int GetHandledNumber();
		void Sync();
	};



	///////////////////////////////////////////////// GATE
	public ref class Gate : IGate
	{
		int id;
		Queue<IHandleable^>^ queue;
		Queue<IHandleable^>^ handled;
		int backCounter;
		int maxPeople = 5;

	public:
		virtual property int MaxPeople { virtual int get() { return maxPeople; } virtual void set(int value) {}}

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
		virtual int GetLength() { return queue->Count; }
		virtual int GetHandledNumber() { return handled->Count; }
		virtual void Sync()
		{
			if (backCounter == 0)
			{
				if (queue->Count > 0)
				{
					auto passgr = queue->Dequeue();
					handled->Enqueue(passgr);
					backCounter = passgr->GetHandlingTime();
				}
				else
					throw gcnew Exception("Люди закончились");
			}
			else --backCounter;
		}

		virtual String^ ToString() override { return String::Concat("Окно ", id); }
	};

}
