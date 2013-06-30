class e1 { };
class e2 { };

namespace
{

class Entity
{
    class Player : public QFsm::Front::Fsm<Player>
    {
        struct State1 { };
        struct State2 { };

    public:
        typedef boost::mpl::vector
        <
            Transition < State1, e1, State2 >,
            Transition < State2, e2, State1 >
        >
        TransitionTable;
    };

    typedef boost::shared_ptr< Front::Fsm<Player> > Fsm;

public:
    template<typename Dispatcher> explicit Entity(Dispatcher& p_dispatcher)
    {
        p_dispatcher.registerHandler<Fsm::value_type::Events>(*this);
    }

    template<typename Event> inline void handleImpl(const Event& p_event)
    {
        m_fsms[p_event.id]->processEvent(p_event);
    }

private:
    std::map<unsigned, Fsm> m_fsms;
};

} //namespace

class Dispatch
{
public:
    Dispatch()
        : m_entity(*this)
    { }

    void handleMsg(boost::shared_ptr<const void> p_event)
    {
        m_dispatcher.dispatch(p_event);
    }

private:
    Entity m_entity;
    Utility::Dispatcher<> m_dispatcher;
};

