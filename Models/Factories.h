#pragma once
#include "Passengers.h"
#include "Gates.h"

using namespace System;
using namespace System::Collections::Generic;

namespace Terminal
{

	///////////////////////////////////////////////////// PASSENGER FACTORY
	public ref class PassengerFactory
	{
		Random^ rnd;

	public:
		PassengerFactory() { rnd = gcnew Random(); }

		Queue<IHandleable^>^ Create(int number)
		{
			auto result = gcnew Queue<IHandleable^>();

			for (int i = 0; i < number; ++i)
				result->Enqueue(gcnew Passenger(rnd->Next(1, 8)));

			return result;
		}
	};





	///////////////////////////////////////////////////// GATES FACTORY
	public ref class GatesFactory
	{

	public:
		List<ISubscriber^>^ Create(int number)
		{
			auto result = gcnew List<ISubscriber^>();

			for (int i = 0; i < number; ++i)
				result->Add(gcnew Gate(i + 1));

			return result;
		}
	};

}