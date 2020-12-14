#pragma once
using namespace System;

namespace Terminal
{

	public interface class IHandleable
	{
		int GetHandlingTime();
	};



	///////////////////////////////////////////////////// PASSENGER
	public ref class Passenger : IHandleable
	{
		int handlingTime;

	public:
		Passenger(int time) { handlingTime = time; }

		virtual int GetHandlingTime() { return handlingTime; }
	};

}