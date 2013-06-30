#ifndef DISPATCHER_HPP
#define DISPATCHER_HPP

#include <map>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>
#include <boost/non_type.hpp>
#include <boost/mpl/aux_/yes_no.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/has_xxx.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/utility/enable_if.hpp>

namespace Utility
{
namespace Detail
{

BOOST_MPL_HAS_XXX_TRAIT_DEF(tag)

template<typename T0, typename T1> class has_check
{
    template<typename T> static boost::mpl::aux::yes_tag check(boost::non_type<T1, &T::check>*);
    template<typename T> static boost::mpl::aux::no_tag check(...);

public:
    static const bool value = sizeof(check<T0>(0)) == sizeof(boost::mpl::aux::yes_tag);
};

template<typename TRawEvent, typename TKey, typename TExecuteEvent> class DefaultDispatchType
{
public:
    typedef std::map<TKey, TExecuteEvent> type;

    template<typename T> static inline void init(T&) { }
    template<typename T> static inline bool check(T& p_entries, const TKey& p_key) { return p_entries.find(p_key) != p_entries.end(); }
    template<typename T> static inline T create(const TRawEvent& p_event) { return T(*p_event); }
    template<typename T> static inline TKey getId(const T& p_event) { return *p_event; }
    template<typename T> struct GetId { enum { value = T::value }; };
    template<typename T> struct GetType { typedef typename T::type type; };
};

} // namespace Detail

template
<
    typename TEventId = int,
    typename TRawEvent = boost::shared_ptr<const int>,
    template<typename, typename, typename> class TDispatchType = Detail::DefaultDispatchType
>
class Dispatcher
{
    class IDispatchTable
    {
    public:
        virtual bool dispatch(const TRawEvent&) = 0;
    };

    template<typename Handler, typename Events> class DispatchTable : public IDispatchTable
    {
        typedef bool (*ExecuteEvent)(Handler&, const TRawEvent&);
        typedef TDispatchType<TRawEvent, TEventId, ExecuteEvent> DispatchType;
        typedef typename DispatchType::type EntriesType;

        template<bool> class IsIntegral { };
        template<bool = Detail::has_check<DispatchType, bool (*)(EntriesType&, const TEventId&)>::value> class HasCheck { };

    public:
        explicit DispatchTable(Handler& p_handler)
            : handler(p_handler)
        {
            DispatchType::init(entries);
            fillEntries<Events>();
        }

        bool inline dispatch(const TRawEvent& p_rawEvent)
        {
            return dispatch(DispatchType::getId(p_rawEvent), p_rawEvent, HasCheck<>());
        }

    private:
        template<typename Seq> inline void fillEntries(typename boost::enable_if< boost::mpl::empty<Seq> >::type* = 0) { }
        template<typename Seq> inline void fillEntries(typename boost::disable_if< boost::mpl::empty<Seq> >::type* = 0)
        {
            typedef typename boost::mpl::front<Seq>::type Event;
            setEntry<Event>(IsIntegral<Detail::has_tag<Event>::value>());
            fillEntries<typename boost::mpl::pop_front<Seq>::type>();
        }

        template<typename Event> inline void setEntry(const IsIntegral<false>&)
        {
            entries[DispatchType::template GetId<Event>::value] = &DispatchTable::template dispatchImpl<Event>;
        }

        template<typename Event> inline void setEntry(const IsIntegral<true>&)
        {
            entries[Event::value] = &DispatchTable::template dispatchImpl<typename DispatchType::template GetType<Event>::type>;
        }

        inline bool dispatch(const TEventId& p_id, const TRawEvent& p_rawEvent, const HasCheck<false>&)
        {
            return entries[p_id](handler, p_rawEvent);
        }

        inline bool dispatch(const TEventId& p_id, const TRawEvent& p_rawEvent, const HasCheck<true>&)
        {
            if (!DispatchType::template check(entries, p_id))
            {
                return false;
            }

            return entries[p_id](handler, p_rawEvent);
        }

        template<typename Event> static inline bool dispatchImpl(Handler& p_handler, const TRawEvent& p_rawEvent)
        {
            p_handler.handleImpl(DispatchType::template create<Event>(p_rawEvent));
            return true;
        }

        Handler& handler;
        EntriesType entries;
    };

    typedef boost::shared_ptr<IDispatchTable> IDispatchTableType;

public:
    /**
     * register handler for whose dispatching will be made
     * example of generic handler code:
     * @code
     * class Handler
     * {
     * public:
     *     template<typename Event> void handleImpl(const Event& p_event)
     *     {
     *         fsm.processEvent(p_event);
     *     }
     * };
     * @endcode
     * @param p_handler handler
     */
    template<typename Events, typename Handler> void registerHandler(Handler& p_handler, typename boost::enable_if< boost::mpl::is_sequence<Events> >::type* = 0)
    {
        typedef DispatchTable<Handler, Events> Dispatcher;
        m_dispatchers.push_back(boost::shared_ptr<Dispatcher>(new Dispatcher(p_handler)));
    }

    template<typename Events, typename Handler> void registerHandler(Handler& p_handler, typename boost::disable_if< boost::mpl::is_sequence<Events> >::type* = 0)
    {
        typedef DispatchTable<Handler, boost::mpl::vector1<Events> > Dispatcher;
        m_dispatchers.push_back(boost::shared_ptr<Dispatcher>(new Dispatcher(p_handler)));
    }

    /**
     * call handleImpl for downcast type in appropriate handler corresponding to id given in p_rawEvent
     * @param p_rawEvent event to be dispatched
     * @return true if any of registered handlers accept the event, false otherwise
     */
    bool dispatch(const TRawEvent& p_rawEvent)
    {
        bool l_dispatched = false;

        BOOST_FOREACH(const IDispatchTableType& l_dispatcher, m_dispatchers)
        {
            l_dispatched = l_dispatcher->dispatch(p_rawEvent) || l_dispatched;
        }

        return l_dispatched;
    }

    std::vector<IDispatchTableType> m_dispatchers;
};

} // namespace Utility

#endif

