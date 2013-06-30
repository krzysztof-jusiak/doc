#include "SwitchCase/Dispatcher.hpp"
#include "CommonTest.hpp"

DISPATCH_TABLE(Test, GIVEN_EVENTS);

int main()
{
    //given
    Handler l_handler;
    Utility::Dispatcher<> l_dispatcher;
    l_dispatcher.registerHandler<Test>(l_handler);

    //when
    test(l_dispatcher);

    return 0;
}

