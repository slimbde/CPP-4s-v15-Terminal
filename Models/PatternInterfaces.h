#pragma once
using namespace System;
using namespace System::Collections::Generic;

namespace Terminal
{
	// declarations necessary to make it work
	interface class IPublisher;
	ref struct GateInfo;
	ref struct TerminalInfo;



	///////////////////////////////////////////////////// ISubscriber
	public interface class ISubscriber
	{
		property int Length;

		void Enqueue(IHandleable^ one);
		void Subscribe(IPublisher^ pub);
		void Unsubscribe();
		GateInfo^ Sync();
	};



	///////////////////////////////////////////////////// Unsubscriber
	public ref class Unsubscriber
	{
		ISubscriber^ subscriber;
		List<ISubscriber^>^ subscribers;

	public:
		Unsubscriber(List<ISubscriber^>^ subscribers, ISubscriber^ subscriber)
		{
			this->subscriber = subscriber;
			this->subscribers = subscribers;
		}

		void Unsubscribe()
		{
			if (subscriber != nullptr && subscribers->Contains(subscriber))
				subscribers->Remove(subscriber);
		}
	};



	///////////////////////////////////////////////////// IPublisher
	public interface class IPublisher
	{
		Unsubscriber^ RegisterSubscriber(ISubscriber^ sub);
		TerminalInfo^ Sync();
		TerminalInfo^ GetSummary();
		int Increase();
	};

}