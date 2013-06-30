#include <boost/shared_ptr.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/statechart/transition.hpp>
#include <boost/statechart/event.hpp>
#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/simple_state.hpp>

using namespace boost::statechart;

struct e1 : event<e1> { };
struct e2 : event<e2> { };
struct e3 : event<e3> { };
struct e4 : event<e4> { };
struct e5 : event<e5> { };
struct e6 : event<e6> { };

class State1;
class State2;
class State3;
class State4;
class State5;
class State6;

struct Player : state_machine<Player, State1> { };

struct State1 : simple_state<State1, Player>
{
    typedef boost::mpl::vector6
    <
        transition<e1, State1>,
        transition<e2, State2>,
        transition<e3, State3>,
        transition<e4, State4>,
        transition<e5, State5>,
        transition<e6, State6>
    >
    reactions;
};

struct State2 : simple_state<State2, Player>
{
    typedef boost::mpl::vector6
    <
        transition<e1, State1>,
        transition<e2, State2>,
        transition<e3, State3>,
        transition<e4, State4>,
        transition<e5, State5>,
        transition<e6, State6>
    >
    reactions;
};

struct State3 : simple_state<State3, Player>
{
    typedef boost::mpl::vector6
    <
        transition<e1, State1>,
        transition<e2, State2>,
        transition<e3, State3>,
        transition<e4, State4>,
        transition<e5, State5>,
        transition<e6, State6>
    >
    reactions;
};

struct State4 : simple_state<State4, Player>
{
    typedef boost::mpl::vector6
    <
        transition<e1, State1>,
        transition<e2, State2>,
        transition<e3, State3>,
        transition<e4, State4>,
        transition<e5, State5>,
        transition<e6, State6>
    >
    reactions;
};

struct State5 : simple_state<State5, Player>
{
    typedef boost::mpl::vector6
    <
        transition<e1, State1>,
        transition<e2, State2>,
        transition<e3, State3>,
        transition<e4, State4>,
        transition<e5, State5>,
        transition<e6, State6>
    >
    reactions;
};

struct State6 : simple_state<State6, Player>
{
    typedef boost::mpl::vector6
    <
        transition<e1, State1>,
        transition<e2, State2>,
        transition<e3, State3>,
        transition<e4, State4>,
        transition<e5, State5>,
        transition<e6, State6>
    >
    reactions;
};

int main()
{
    //given
    Player l_fsm;
    l_fsm.initiate();

    boost::shared_ptr<event_base> events[6] =
    {
        boost::shared_ptr<event_base>(new e1),
        boost::shared_ptr<event_base>(new e2),
        boost::shared_ptr<event_base>(new e3),
        boost::shared_ptr<event_base>(new e4),
        boost::shared_ptr<event_base>(new e5),
        boost::shared_ptr<event_base>(new e6)
    };

    //when
    for (unsigned i=0; i < EVENT_CALLS; ++i)
    {
        l_fsm.process_event(*events[i % 6]);
    }

    return 0;
}

