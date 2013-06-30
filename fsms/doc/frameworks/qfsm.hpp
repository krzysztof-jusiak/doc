//Default front end
class Player : public QFsm::Front::Fsm<Player>
{
    struct Empty { };
    struct Open { };

public:
    typedef Empty InitialState;

    typedef boost::mpl::vector
    <
        Transition < Empty, OpenClose, Open  >,
        Transition < Open,  OpenClose, Empty >
    >
    TransitionTable;
};

QFsm::Front::Default::Fsm<Player> l_fsm;

//Default front end in place
QFsm::Front::Fsm
<
    boost::mpl::vector
    <
        Transition < Empty, OpenClose, Open  >,
        Transition < Open,  OpenClose, Empty >
    >,
    Empty
>
l_fsm;

//Fusion front end
BOOST_AUTO(l_transitionTable,
    (
        QFsm::Fusion::TransitionTable<Empty>()
            .transition(Empty(), Event<OpenClose>(), Open())
            .transition(Open(), Event<OpenClose>(), Empty())
    )
);

QFsm::Front::Fusion::Fsm<BOOST_TYPEOF(l_transitionTable)> l_fsm(l_transitionTable);

