#pragma once

#include "thread"
#include "memory"

namespace ListBase {
    /** list base define **/
    template<size_t idx, typename ...List>
    class List_base;

    template<size_t idx, typename Head, typename ...Tail>
    class List_base<idx, Head, Tail...> :
        public List_base<idx + 1, Tail...> {
       public:
        using Inherited = List_base<idx + 1, Tail...>;
        using Type = Head;

        Head *val;
        std::shared_ptr<std::thread> thread;

        explicit constexpr List_base(Head &head, Tail &... tail)
            : Inherited(tail...) {
            val = &head;
            thread = std::make_shared<std::thread>(&Head::task, &head);
        }
    };

    template<size_t idx, typename Head>
    class List_base<idx, Head> {
       public:
        using Inherited = List_base<idx + 1>;
        using Type = Head;

        Head *val;
        std::shared_ptr<std::thread> thread;

        explicit constexpr List_base(Head &head) {
            val = &head;
            thread = std::make_shared<std::thread>(&Head::task, &head);
        }
    };
    template<size_t idx, typename Head, typename ...Tail>
    List_base<idx, Head, Tail...> ListTypeFun(List_base<idx, Head, Tail...> &list);

    /** join define**/
    template<size_t idx, typename Head, typename Sec, typename ...Tail>
    void join(List_base<idx, Head, Sec, Tail...> &list) {
        join<idx + 1, Sec, Tail...>(list);
        list.thread->join();
    }

    template<size_t idx, typename Head>
    void join(List_base<idx, Head> &list) {
        list.thread->join();
    }
}
