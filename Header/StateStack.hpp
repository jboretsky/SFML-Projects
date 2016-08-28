#ifndef STATESTACK
#define STATESTACK

#include "./State.hpp"
#include "./StateIdentifiers.hpp"
#include "./ResourceIdentifiers.hpp"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>

#include <vector>
#include <utility>
#include <functional>
#include <map>

namespace sf {
	class Event;
	class RenderWindow;
}

class StateStack: private sf::NonCopyable {
	public:
		enum Action {
			Push,
			Pop,
			Clear,
		};
	public:
		explicit StateStack(State::Context context);
		template <typename T>
		void registerState(States::ID stateID);

		// these are simply called in application and passed on to all states
		void update(sf::Time dt);
		void draw();
		void handleEvent(const sf::Event& event);

		// these are used to push PendingChange structures to our mPendingList
		void pushState(States::ID stateID);
		void popState();
		void clearStates();

		bool isEmpty() const;

	private:
		// these are used to actually push states onto our stateStack.
		// applyPendingChanges will look at our pendingList and take any change from it
		// If it is a push change, then it will call createState(stateID)
		// this in turn calls the mFactory function for that ID, which returns the new state
		// and pushes that to the stateStack!
		State::Ptr createState(States::ID stateID);
		void applyPendingChanges();

	private:
		struct PendingChange {
			explicit PendingChange(Action action, States::ID stateID = States::None);
			Action action;
			States::ID stateID;
		};

	private:
		std::vector<State::Ptr> mStack;
		std::vector<PendingChange> mPendingList;

		// the statestack is constructed with a State::Context structure, 
		// it can give the context to any state
		State::Context mContext;
		std::map<States::ID, std::function<State::Ptr()>> mFactories;

};

// we register each state in application.cpp
// example use: mStateStack.registerState<TitleState>(States::Title)
// now when we push Stated::Title to the statestack, it creates a new instance of title
template <typename T>
void StateStack::registerState(States::ID stateID) {
	mFactories[stateID] = [this] () {
		return State::Ptr(new T(*this, mContext));
	};
}
#endif