#pragma once

#include "thread"
#include "memory"
#include "iostream"

namespace ListBase {
    /** list base define **/
    template<size_t idx, typename ...List>
    class ListBase;

    template<size_t idx, typename Head, typename ...Tail>
    class ListBase<idx, Head, Tail...> :
        public ListBase<idx + 1, Tail...> {
       public:
        using Inherited = ListBase<idx + 1, Tail...>;
        using Type = Head;

        Head *val = nullptr;
        std::shared_ptr<std::thread> thread;

        template<typename ...Args>
        explicit constexpr ListBase(Head &head, Args &&... tail)
            : Inherited(tail...) {
            val = &head;
            thread = std::make_shared<std::thread>(&Head::task, &head);
        }

        template<typename ...Args>
        explicit constexpr ListBase(Head *head, Args &&... tail)
            : Inherited(tail...) {
            val = head;
            if(head != nullptr) {
                thread = std::make_shared<std::thread>(&Head::task, head);
            }
            else {
                std::cerr << "the " << idx + 1 << "th hardware is null!" << std::endl;
            }
        }
    };

    template<size_t idx, typename Head>
    class ListBase<idx, Head> {
       public:
        using Inherited = ListBase<idx + 1>;
        using Type = Head;

        Head *val = nullptr;
        std::shared_ptr<std::thread> thread;

        explicit constexpr ListBase(Head &head) {
            val = &head;
            thread = std::make_shared<std::thread>(&Head::task, &head);
        }

        explicit constexpr ListBase(Head *head) {
            val = head;
            if(head != nullptr) {
                thread = std::make_shared<std::thread>(&Head::task, head);
            }
            else {
                std::cerr << "the " << idx + 1 << "th hardware is null!" << std::endl;
            }
        }
    };
    template<size_t idx, typename Head, typename ...Tail>
    ListBase<idx, Head, Tail...> ListTypeFun(ListBase<idx, Head, Tail...> &list);

    /** join define**/
    template<size_t idx, typename Head, typename Sec, typename ...Tail>
    void join(ListBase<idx, Head, Sec, Tail...> &list) {
        join<idx + 1, Sec, Tail...>(list);
        if(list.thread) {
            list.thread->join();
        }
    }

    template<size_t idx, typename Head>
    void join(ListBase<idx, Head> &list) {
        if(list.thread) {
            list.thread->join();
        }
    }

    /** callback element define **/
    template<size_t idx, typename T>
    struct ListElement;

    template<size_t idx, size_t idx_t, typename Head, typename ...Args>
    struct ListElement<idx, ListBase<idx_t, Head, Args...>> : ListElement<idx, ListBase<idx_t + 1, Args...>> {
        using Inherited = ListElement<idx, ListBase<idx_t + 1, Args...>>;
        using Type = typename Inherited::Type;
        using ListType = typename Inherited::ListType;
    };

    template<size_t idx, typename Head, typename ...Args>
    struct ListElement<idx, ListBase<idx, Head, Args...>> {
        using Type = Head;
        using ListType = ListBase<idx, Head, Args...>;
    };
}
