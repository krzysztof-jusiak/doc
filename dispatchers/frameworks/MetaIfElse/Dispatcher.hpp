#ifndef DISPATCHER_HPP
#define DISPATCHER_HPP

#include <map>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>
#include <boost/non_type.hpp>
#include <boost/mpl/aux_/yes_no.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/has_xxx.hpp>
#include <boost/utility/enable_if.hpp>

namespace Utility
{
namespace Detail
{

BOOST_MPL_HAS_XXX_TRAIT_DEF(tag)

template<typename TRawEvent, typename TKey> class DefaultDispatchType
{
public:
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
    template<typename, typename> class TDispatchType = Detail::DefaultDispatchType
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
        typedef TDispatchType<TRawEvent, TEventId> DispatchType;
        template<bool> class IsIntegral { };

    public:
        explicit DispatchTable(Handler& p_handler)
            : handler(p_handler)
        { }

        bool inline dispatch(const TRawEvent& p_rawEvent)
        {
            return dispatchImpl<Events>(DispatchType::getId(p_rawEvent), p_rawEvent);
        }

    private:
        template<typename Seq> inline bool dispatchImpl(const TEventId&, const TRawEvent&, typename boost::enable_if< boost::mpl::empty<Seq> >::type* = 0)
        {
            return false;
        }

        template<typename Seq> inline bool dispatchImpl(const TEventId& p_id, const TRawEvent& p_rawEvent, typename boost::disable_if< boost::mpl::empty<Seq> >::type* = 0)
        {
            typedef typename boost::mpl::front<Seq>::type Event;

            if (p_id == getId<Event>(IsIntegral<Detail::has_tag<Event>::value>()))
            {
                handler.handleImpl(DispatchType::template create<Event>(p_rawEvent));
                return true;
            }

            return dispatchImpl<typename boost::mpl::pop_front<Seq>::type>(p_id, p_rawEvent);
        }

        template<typename Event> inline TEventId getId(const IsIntegral<false>&)
        {
            return DispatchType::template GetId<Event>::value;
        }

        template<typename Event> inline TEventId getId(const IsIntegral<true>&)
        {
            return DispatchType::template GetId<DispatchType::template GetType<Event>::type>::value;
        }

        Handler& handler;
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
    template<typename Events, typename Handler> void registerHandler(Handler& p_handler)
    {
        typedef DispatchTable<Handler, Events> Dispatcher;
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


