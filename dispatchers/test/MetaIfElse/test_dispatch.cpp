#include <boost/mpl/vector/vector50.hpp>
#include "Common/IfAaSysCom.hpp"
#include "Common/MsgTraits.hpp"
#include "MetaIfElse/Dispatcher.hpp"
#include "CommonTest.hpp"

template<typename TRawEvent, typename TKey>
class CPlaneDispatchType
{
public:
    template<typename T> static inline const T& create(const TRawEvent& p_event) { return *static_cast<const T*>(Common::AaSysComMsgGetPayload(p_event.get())) ; }
    template<typename T> static inline TKey getId(const T& p_event) { return Common::AaSysComMsgGetId(p_event.get()) - 1; }
    template<typename T> struct GetId { enum { value = Common::MsgTraits<T>::value - 1}; };
};

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

