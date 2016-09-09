#ifndef STATE
#define STATE

#include "./StateIdentifiers.hpp"
#include "./ResourceIdentifiers.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

#include <memory>

namespace sf {
	class RenderWindow;
}

class StateStack;
class Player;

class State {
	public:
		typedef std::unique_ptr<State> Ptr;

		// each State gets a context from mStateStack, the state is constructed
		// by the mFactories map in stateStack on the fly.
		// interesting because the stateStack is constructed using this structure,
		// but then passes it back to each state.
		struct Context {
			Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts, Player& player);

			sf::RenderWindow* window;
			TextureHolder* textures;
			FontHolder* fonts;
			Player* player;
		};

	public:
		State(StateStack& stack, Context context);
		virtual ~State();

		// each state must implement its own draw, update and handleEvent functions
		// ex. gameState has pretty complicated logic for these because it is the most complex state!
		// = 0 makes the entire class abstract, which means a state cannot be instantiated without a concrete class
		virtual void draw() = 0;
		virtual bool update(sf::Time dt) = 0;
		virtual bool handleEvent(const sf::Event& event) = 0;

	protected:
		// each state can requeststackpushes, pops, or clears
		// this just forwards the call to the stateStack. We need them
		// because mStack is private
		void requestStackPush(States::ID stateID);
		void requestStackPop();
		void requestStateClear();

		// returns the context, we need this because mContext is private
		// we can use the context to get the window, or textures, etc.
		Context getContext() const;

	private:
		StateStack* mStack;
		Context mContext;
};

#endif