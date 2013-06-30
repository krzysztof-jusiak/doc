#include <boost/shared_ptr.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/statechart/transition.hpp>
#include <boost/statechart/event.hpp>
#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/simple_state.hpp>

using namespace boost::statechart;

struct OpenClose : event<OpenClose> { };

class Empty;
class Open;

struct Player : state_machine<Player, Empty> { };

struct Empty : simple_state<Empty, Player>
{
    typedef boost::mpl::vector1
    <
        transition<OpenClose, Open>
    >
    reactions;
};

struct Open : simple_state<Open, Player>
{
    typedef boost::mpl::vector1
    <
        transition<OpenClose, Empty>
    >
    reactions;
};

int main()
{
    //given
    Player l_fsm;
    l_fsm.initiate();
    boost::shared_ptr<OpenClose> l_event(new OpenClose);

    //when
    for (unsigned i=0; i < EVENT_CALLS; ++i)
    {
        l_fsm.process_event(*l_event);
    }

    return 0;
}

