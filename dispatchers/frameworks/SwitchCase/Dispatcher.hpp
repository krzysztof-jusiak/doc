#ifndef DISPATCHER_HPP
#define DISPATCHER_HPP

#include <vector>
#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/comparison/greater.hpp>
#include "Common/IfAaSysCom.hpp"
#include "Common/MsgTypes.hpp"

#define HANDLE_MSG(id)\
    case id: return dispatchImpl<Common::Msg##id>(p_event);

#define HANDLE(r, data, elem)\
    HANDLE_MSG(r)

#define SWITCH(SEQ)\
    BOOST_PP_SEQ_FOR_EACH(HANDLE, _, SEQ)

#define DISPATCH_TABLE(NAME, SEQ)\
    template<typename Handler, typename TRawEvent = boost::shared_ptr<const void> > class NAME : public IDispatchTable<TRawEvent>\
    {\
    public:\
        explicit NAME(Handler& p_handler)\
            : handler(p_handler)\
        { }\
    \
        bool dispatch(const TRawEvent& p_event)\
        {\
            switch (Common::AaSysComMsgGetId(p_event.get()))\
            {\
                SWITCH(SEQ)\
                default: return false;\
            }\
    \
            return false;\
        }\
    \
    private:\
        template<typename Event> inline bool dispatchImpl(const TRawEvent& p_rawEvent)\
        {\
            handler.handleImpl(*static_cast<const Event*>(Common::AaSysComMsgGetPayload(p_rawEvent.get())));\
            return true;\
        }\
    \
        Handler& handler;\
    }

template<typename TRawEvent> class IDispatchTable
{
public:
    virtual bool dispatch(const TRawEvent&) = 0;
};

namespace Utility
{

template<typename TEventId = Common::TAaSysComMsgId, typename TRawEvent = boost::shared_ptr<const void> >
class Dispatcher
{
    typedef boost::shared_ptr< IDispatchTable<TRawEvent> > IDispatchTableType;

public:
    template<template<typename, typename> class DispatchTable, typename Handler> void registerHandler(Handler& p_handler)
    {
        typedef DispatchTable<Handler, TRawEvent> Dispatcher;
        m_dispatchers.push_back(boost::shared_ptr<Dispatcher>(new Dispatcher(p_handler)));
    }

    bool dispatch(const boost::shared_ptr<void>& p_rawEvent)
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

