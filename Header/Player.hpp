#ifndef PLAYER
#define PLAYER

#include "./Command.hpp"

#include <SFML/Window/Event.hpp>

#include <map>

class CommandQueue;

class Player {
	public:
		enum Action {
			MoveLeft,
			MoveRight,
			ActionCount,
		};

	public:
		Player();
		void handleEvent(const sf::Event& event, CommandQueue& commands);
		void handleRealtimeInput(CommandQueue& commands);
		void assignKey(Action action, sf::Keyboard::Key key);
		sf::Keyboard::Key getAssignedKey(Action action) const;
		int getLives() const;

	private:
		void initializeActions();
		static bool isRealtimeAction(Action action);

	private:
		// actions are just numbers
		// so a key is mapped to a number, which is mapped to a command
		std::map<sf::Keyboard::Key, Action> mKeyBinding;
		std::map<Action, Command> mActionBindings;
		int mLives;
};

#endif