#include <boost/msm/back/favor_compile_time.hpp>
#include <boost/mpl/vector/vector50.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/msm/back/state_machine.hpp>
#include <boost/msm/front/state_machine_def.hpp>
#include <boost/msm/front/functor_row.hpp>

using namespace boost::msm::front;

struct e1 { };
struct e2 { };
struct e3 { };
struct e4 { };
struct e5 { };
struct e6 { };

class Player_ : public state_machine_def<Player_>
{
    struct State1 : public state<> { };
    struct State2 : public state<> { };
    struct State3 : public state<> { };
    struct State4 : public state<> { };
    struct State5 : public state<> { };
    struct State6 : public state<> { };

public:
    typedef State1 initial_state;

    struct transition_table : boost::mpl::vector36
    <
        Row <State1, e1, State1, none, none>,
        Row <State1, e2, State2, none, none>,
        Row <State1, e3, State3, none, none>,
        Row <State1, e4, State4, none, none>,
        Row <State1, e5, State5, none, none>,
        Row <State1, e6, State6, none, none>,

        Row <State2, e1, State1, none, none>,
        Row <State2, e2, State2, none, none>,
        Row <State2, e3, State3, none, none>,
        Row <State2, e4, State4, none, none>,
        Row <State2, e5, State5, none, none>,
        Row <State2, e6, State6, none, none>,

        Row <State3, e1, State1, none, none>,
        Row <State3, e2, State2, none, none>,
        Row <State3, e3, State3, none, none>,
        Row <State3, e4, State4, none, none>,
        Row <State3, e5, State5, none, none>,
        Row <State3, e6, State6, none, none>,

        Row <State4, e1, State1, none, none>,
        Row <State4, e2, State2, none, none>,
        Row <State4, e3, State3, none, none>,
        Row <State4, e4, State4, none, none>,
        Row <State4, e5, State5, none, none>,
        Row <State4, e6, State6, none, none>,

        Row <State5, e1, State1, none, none>,
        Row <State5, e2, State2, none, none>,
        Row <State5, e3, State3, none, none>,
        Row <State5, e4, State4, none, none>,
        Row <State5, e5, State5, none, none>,
        Row <State5, e6, State6, none, none>,

        Row <State6, e1, State1, none, none>,
        Row <State6, e2, State2, none, none>,
        Row <State6, e3, State3, none, none>,
        Row <State6, e4, State4, none, none>,
        Row <State6, e5, State5, none, none>,
        Row <State6, e6, State6, none, none>
    >
    { };
};

typedef boost::msm::back::state_machine<Player_, boost::msm::back::TEST_TYPE> Player;

int main()
{
    //given
    Player l_fsm;
    boost::shared_ptr<e1> l_e1(new e1);
    boost::shared_ptr<e2> l_e2(new e2);
    boost::shared_ptr<e3> l_e3(new e3);
    boost::shared_ptr<e4> l_e4(new e4);
    boost::shared_ptr<e5> l_e5(new e5);
    boost::shared_ptr<e6> l_e6(new e6);

    //when
    for (unsigned i=0; i < EVENT_CALLS; ++i)
    {
        switch(i % 6)
        {
            case 0: l_fsm.process_event(*l_e1); break;
            case 1: l_fsm.process_event(*l_e2); break;
            case 2: l_fsm.process_event(*l_e3); break;
            case 3: l_fsm.process_event(*l_e4); break;
            case 4: l_fsm.process_event(*l_e5); break;
            case 5: l_fsm.process_event(*l_e6); break;
        }
    }

    return 0;
}

