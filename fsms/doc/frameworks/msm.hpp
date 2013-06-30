class Player_ : public state_machine_def<Player_>
{
    struct Empty : public state<> { };
    struct Open : public state<> { };

public:
    typedef Empty initial_state;

    struct transition_table : boost::mpl::vector
    <
        Row <Empty, OpenClose, Open,  none, none >,
        Row <Open,  OpenClose, Empty, none, none >
    >
    { };
};

typedef boost::msm::back::state_machine<Player_> Player;

Player l_fsm;

