struct Player : state_machine<Player, Empty> { };

struct Empty : simple_state<Empty, Player>
{
    typedef boost::mpl::vector
    <
        transition<OpenClose, Open>
    >
    reactions;
};

struct Open : simple_state<Open, Player>
{
    typedef boost::mpl::vector
    <
        transition<OpenClose, Empty>
    >
    reactions;
};

Player l_fsm;

