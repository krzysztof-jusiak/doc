class SuperState : public QFsm::Front::Fsm<SuperState>
{
    struct State1 { };
    struct State2 { };
    struct State3 { };

    class Fsm : public QFsm::Front::Fsm<Fsm>
    {
    public:
        typedef boost::mpl::vector
        <
            //         < InitState,  Event,     TargetTstate,   Action,     Guard   >
            //------------------------------------------------------------------------
            Transition < State1,     e1,        State2,         None,       isTrue >,
            Transition < State2,     e2,        State1,         None,       None   >
        >
        TransitionTable;
    };

public:
    typedef boost::mpl::vector
    <
        //         < InitState,  Event,      TargetTstate,   Action,     Guard   >
        //------------------------------------------------------------------------
        Transition < Fsm,        e1,         None,           action1,    isFalse >,
        Transition < Fsm,        e3,         State3,         action2,    None    >,
        Transition < State3,     e4,         Terminate,      None,       None    >
    >
    TransitionTable;
};

