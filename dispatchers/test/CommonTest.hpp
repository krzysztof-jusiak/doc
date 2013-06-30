#ifndef TEST_COMMON_HPP
#define TEST_COMMON_HPP

#include <vector>
#include <boost/assign.hpp>
#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/facilities/empty.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/arithmetic/add.hpp>
#include <boost/preprocessor/comparison/less.hpp>

#define ELEM(r, data, elem) BOOST_PP_CAT(Common::Msg, elem) BOOST_PP_COMMA_IF(BOOST_PP_LESS(r, data))
#define MPL(SEQ) BOOST_PP_SEQ_FOR_EACH(ELEM, BOOST_PP_ADD(BOOST_PP_SEQ_SIZE(SEQ), 1), SEQ)

int global = 0;

class Handler
{
public:
    template<typename Event> void handleImpl(const Event&)
    {
        global++; //to avoid excessive optimization
    }
};

template<typename T> void test(T& p_dispatcher)
{
    std::vector<int> ids = boost::assign::list_of GIVEN_EVENTS;
    boost::shared_ptr<void> events[BOOST_PP_SEQ_SIZE(GIVEN_EVENTS)];

    int i = 0;
    BOOST_FOREACH(const int& id, ids)
    {
        events[i++].reset(new int(id));
    }

    for (unsigned i=0; i < EVENT_CALLS; ++i)
    {
        p_dispatcher.dispatch(events[i % BOOST_PP_SEQ_SIZE(GIVEN_EVENTS)]);
    }
}

#endif

