#include <boost/mpl/vector/vector50.hpp>
#include "Common/IfAaSysCom.hpp"
#include "Common/MsgTraits.hpp"
#include "JumpTable/Dispatcher.hpp"
#include "CommonTest.hpp"

#if defined(RAW_TABLE)

    template<typename TRawEvent, typename TKey, typename TExecuteEvent>
    class CPlaneDispatchType
    {
    public:
        static const unsigned msgSize = 1000;
        typedef TExecuteEvent type[msgSize];

        template<typename T> static void init(T& p_entries)
        {
            for (unsigned i=0; i < msgSize; ++i)
            {
                p_entries[i] = &CPlaneDispatchType::defaultDispatchImpl;
            }
        }

        template<typename T> static inline bool defaultDispatchImpl(T&, const TRawEvent&) { return false; }
        template<typename T> static inline const T& create(const TRawEvent& p_event) { return *static_cast<const T*>(Common::AaSysComMsgGetPayload(p_event.get())) ; }
        template<typename T> static inline TKey getId(const T& p_event) { return Common::AaSysComMsgGetId(p_event.get()) - 1; }
        template<typename T> struct GetId { enum { value = Common::MsgTraits<T>::value - 1}; };
    };

#elif defined(STD_MAP)

# include <map>

    template<typename TRawEvent, typename TKey, typename TExecuteEvent>
    class CPlaneDispatchType
    {
    public:
        typedef std::map<TKey, TExecuteEvent> type;

        template<typename T> static inline void init(T&) { };
        template<typename T> static inline bool check(T& p_entries, const TKey& p_key) { return p_entries.find(p_key) != p_entries.end(); }
        template<typename T> static inline const T& create(const TRawEvent& p_event) { return *static_cast<const T*>(Common::AaSysComMsgGetPayload(p_event.get())); }
        template<typename T> static inline TKey getId(const T& p_event) { return Common::AaSysComMsgGetId(p_event.get()); }
        template<typename T> struct GetId { enum { value = Common::MsgTraits<T>::value }; };
    };

#elif defined(BOOST_UNORDERED_MAP)

# include <boost/unordered_map.hpp>

    template<typename TRawEvent, typename TKey, typename TExecuteEvent>
    class CPlaneDispatchType
    {
    public:
        typedef boost::unordered_map<TKey, TExecuteEvent> type;

        template<typename T> static inline void init(T&) { };
        template<typename T> static inline bool check(T& p_entries, const TKey& p_key) { return p_entries.find(p_key) != p_entries.end(); }
        template<typename T> static inline const T& create(const TRawEvent& p_event) { return *static_cast<const T*>(Common::AaSysComMsgGetPayload(p_event.get())); }
        template<typename T> static inline TKey getId(const T& p_event) { return Common::AaSysComMsgGetId(p_event.get()); }
        template<typename T> struct GetId { enum { value = Common::MsgTraits<T>::value }; };
    };

#else
# error CPlaneDispatchType not defined
#endif

int main()
{
    //given
    Handler l_handler;
    Utility::Dispatcher<Common::TAaSysComMsgId, boost::shared_ptr<const void>, CPlaneDispatchType> l_dispatcher;
    l_dispatcher.registerHandler<BOOST_PP_CAT(boost::mpl::vector, BOOST_PP_SEQ_SIZE(EXPECTED_EVENTS))<MPL(EXPECTED_EVENTS)> >(l_handler);

    //when
    test(l_dispatcher);

    return 0;
}

