#include "StateMachine.hpp"
#include "NoAction.hpp"
#include "TransitionTo.hpp"
#include "Might.hpp"
#include "DefaultAction.hpp"
#include "OnEvent.hpp"
#include "Will.hpp"

using namespace fsm;

namespace example
{
	struct OpenEvent {};
	struct CloseEvent {};

	struct LockEvent
	{
		explicit LockEvent( int new_key_ ) : new_key( new_key_ ) {}
		int new_key;
	};

	struct UnlockEvent
	{
		explicit UnlockEvent( int key_ ) : key( key_ ) {}
		int key;
	};

	struct ClosedState;
	struct OpenState;
	class LockedState;
	struct VoidState;

	struct ClosedState
		: public Will<
		DefaultAction<NoAction>,
		OnEvent<LockEvent, TransitionTo<LockedState>>,
		OnEvent<OpenEvent, TransitionTo<OpenState>>
		>
	{
	};

	struct OpenState
		: public Will<
		DefaultAction<NoAction>,
		OnEvent<CloseEvent, TransitionTo<ClosedState>>
		>
	{
	};

	class LockedState
		: public DefaultAction<NoAction>
	{
	public:
		using DefaultAction<NoAction>::HandleEvent;

		explicit LockedState( int key_ )
			: key( key_ )
		{}

		NoAction OnEnter( const LockEvent& e )
		{
			key = e.new_key;
			return NoAction{};
		}

		Might<TransitionTo<ClosedState>> HandleEvent( const UnlockEvent& e )
		{
			if (e.key == key)
				return TransitionTo<ClosedState>{};

			return NoAction{};
		}

	private:
		int key;
	};


	struct VoidState
		: public Will<DefaultAction<NoAction>>
	{

	};

	using Door = Machine<
		States<ClosedState, OpenState, LockedState>,
		Events<OpenEvent, CloseEvent, LockEvent, UnlockEvent>>;
	
	void DoorExample()
	{
		Door door = Door( ClosedState{}, OpenState{}, LockedState{0} );

		door.Handle( LockEvent{ 123 } );
		door.Handle( LockEvent{ 123 } );
		door.Handle( UnlockEvent{ 123 } );

		auto& closed = door.GetState<ClosedState>(); (void)closed;
		//auto& void_state = door.GetState<VoidState>(); (void)void_state;

		auto top_state = door.GetActiveState();

		//auto d2 = Door( door );
		bool x = door.IsInState<LockedState>(); (void)x;
		//door.Handle( 123 ); // wont compile because of unknown type
	}

	/*struct X
	{
		void Do( int ) {}
	};

	static_assert(std::is_invocable_r<void, decltype(&X::Do), X&, int>::value, "Thing!");
	static_assert(std::is_invocable<decltype(&X::Do), void>::value, "Thing!");*/
}
