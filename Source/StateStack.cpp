#include "../Header/StateStack.hpp"

#include <cassert>

// important to note: this is constructed from application, which passes context to it
// so mContext is initialized and can give all the states a context!
StateStack::StateStack(State::Context context)
: mStack()
, mPendingList()
, mContext(context)
, mFactories() {}

State::Ptr StateStack::createState(States::ID stateID) {
	auto found = mFactories.find(stateID);
	assert(found != mFactories.end());
	
	// return the result of calling the lambda function
	// which is a pointer to a new state
	return found->second();
}

void StateStack::update(sf::Time dt) {
	for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr) {
		if (!(*itr)->update(dt))
			break;
	}

	applyPendingChanges();
}

void StateStack::draw() {
	for (auto &state : mStack) {
		state->draw();
	}
}

// go through each active state from end to beginning (highest to lowest)
// handle event is called on each state. but if the state wants, it can
// disallow the event to pass down through to other states (by returning false)
void StateStack::handleEvent(const sf::Event& event) {
	for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr) {
		if (!(*itr)->handleEvent(event))
			break;
	}

	applyPendingChanges();
}

void StateStack::pushState(States::ID stateID) {
	mPendingList.push_back(PendingChange(Push, stateID));
}

void StateStack::popState() {
	mPendingList.push_back(PendingChange(Pop));
}

void StateStack::clearStates() {
	mPendingList.push_back(PendingChange(Clear));
}

bool StateStack::isEmpty() const {
	return mStack.empty();
}

// check if pending list has any actions, if so, execute them
void StateStack::applyPendingChanges() {
	for(auto change : mPendingList) {
		switch (change.action) {
			case Push:
				mStack.push_back(createState(change.stateID));
				break;
			case Pop:
				mStack.pop_back();
				break;
			case Clear:
				mStack.clear();
				break;
		}
	}

	mPendingList.clear();
}

StateStack::PendingChange::PendingChange(Action action, States::ID stateID)
: action(action)
, stateID(stateID) {}
