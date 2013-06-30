#include <boost/shared_ptr.hpp>

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

struct OpenClose { };
struct Empty { };
struct Open { };

class Player : public Front::Fsm
{
    struct Empty { };
    struct Open { };

public:
    typedef Empty InitialState;

    typedef boost::mpl::vector2
    <
        Transition <Empty, OpenClose, Open>,
        Transition <Open,  OpenClose, Empty>
    >
    TransitionTable;
};

int main()
{
    //given
    Front::Default::Fsm<Player, Back::TEST_TYPE> l_fsm;
    boost::shared_ptr<OpenClose> l_event(new OpenClose);

    //when
    for (unsigned i=0; i < EVENT_CALLS; ++i)
    {
        l_fsm.processEvent(*l_event);
    }

    return 0;
}

