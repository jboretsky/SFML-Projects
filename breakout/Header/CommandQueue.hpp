#ifndef COMMANDQUEUE
#define COMMANDQUEUE

#include "./Command.hpp"

#include <queue>

class CommandQueue {
	public:
		void push(const Command& command);
		Command pop();
		bool isEmpty() const;

	private:
		std::queue<Command> mQueue;
};

#endif