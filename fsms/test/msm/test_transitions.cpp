#include <boost/msm/back/favor_compile_time.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/msm/back/state_machine.hpp>
#include <boost/msm/front/state_machine_def.hpp>
#include <boost/msm/front/functor_row.hpp>

using namespace boost::msm::front;

struct OpenClose { };

class Player_ : public state_machine_def<Player_>
{
    struct Empty : public state<> { };
    struct Open : public state<> { };

public:
    typedef Empty initial_state;

    struct transition_table : boost::mpl::vector2
    <
        Row<Empty, OpenClose, Open, none, none>,
        Row<Open, OpenClose, Empty, none, none>
    >
    { };
};

typedef boost::msm::back::state_machine<Player_, boost::msm::back::TEST_TYPE> Player;

int main()
{
    //given
    Player l_fsm;
    boost::shared_ptr<OpenClose> l_event(new OpenClose);

    //when
    for (unsigned i=0; i < EVENT_CALLS; ++i)
    {
        l_fsm.process_event(*l_event);
    }

    return 0;
}

