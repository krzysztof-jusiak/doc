#include <boost/shared_ptr.hpp>
#include <boost/mpl/vector/vector50.hpp>

#ifdef UML
# include "QFsm/Extensions/Uml.hpp"
#endif

#ifdef NON_UML
# include "QFsm/Extensions/NonUml.hpp"
#endif

#ifdef ALL
# include "QFsm/Extensions/Uml.hpp"
# include "QFsm/Extensions/NonUml.hpp"
#endif

#include "QFsm/Front/Default/Fsm.hpp"
#include "QFsm/Front/Operations/Transition.hpp"

using namespace QFsm;
using namespace QFsm::Front::Operations;

class e1 { };
class e2 { };
class e3 { };
class e4 { };
class e5 { };
class e6 { };

class Player : public Front::Fsm
{
    class State1 { };
    class State2 { };
    class State3 { };
    class State4 { };
    class State5 { };
    class State6 { };

public:
    typedef State1 InitialState;

    typedef boost::mpl::vector36
    <
        Transition<State1, e1, State1>,
        Transition<State1, e2, State2>,
        Transition<State1, e3, State3>,
        Transition<State1, e4, State4>,
        Transition<State1, e5, State5>,
        Transition<State1, e6, State6>,

        Transition<State2, e1, State1>,
        Transition<State2, e2, State2>,
        Transition<State2, e3, State3>,
        Transition<State2, e4, State4>,
        Transition<State2, e5, State5>,
        Transition<State2, e6, State6>,

        Transition<State3, e1, State1>,
        Transition<State3, e2, State2>,
        Transition<State3, e3, State3>,
        Transition<State3, e4, State4>,
        Transition<State3, e5, State5>,
        Transition<State3, e6, State6>,

        Transition<State4, e1, State1>,
        Transition<State4, e2, State2>,
        Transition<State4, e3, State3>,
        Transition<State4, e4, State4>,
        Transition<State4, e5, State5>,
        Transition<State4, e6, State6>,

        Transition<State5, e1, State1>,
        Transition<State5, e2, State2>,
        Transition<State5, e3, State3>,
        Transition<State5, e4, State4>,
        Transition<State5, e5, State5>,
        Transition<State5, e6, State6>,

        Transition<State6, e1, State1>,
        Transition<State6, e2, State2>,
        Transition<State6, e3, State3>,
        Transition<State6, e4, State4>,
        Transition<State6, e5, State5>,
        Transition<State6, e6, State6>
    >
    TransitionTable;
};

int main()
{
    //given
    Front::Default::Fsm<Player, Back::TEST_TYPE> l_fsm;
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
            case 0: l_fsm.processEvent(*l_e1); break;
            case 1: l_fsm.processEvent(*l_e2); break;
            case 2: l_fsm.processEvent(*l_e3); break;
            case 3: l_fsm.processEvent(*l_e4); break;
            case 4: l_fsm.processEvent(*l_e5); break;
            case 5: l_fsm.processEvent(*l_e6); break;
        }
    }

    return 0;
}

