#pragma once

#include "hardware_list_base.hpp"
namespace Hardware
{
    /** hardware manager **/
    template<typename ...List>
    class Hardware_manager {
       private:
        ListBase::List_base<0, List...> hardware;
       public:
        template<typename ...Args>
        explicit Hardware_manager(Args &&... args) : hardware(args...) {}

        template<size_t idx, typename ...Args>
        auto get(Args &&... args) {
            return ((decltype(ListTypeFun<idx>(hardware)) &) hardware).val->get(hardware, args...);
        }

        template<size_t idx, typename ...Args>
        void send(Args &&... args) {
            ((decltype(ListTypeFun<idx>(hardware)) &) hardware).val->send(args...);
        }

        /** callback **/
        template<size_t idx, typename Type>
        void register_callback(const std::function<void(const Type &)> &fun) {
            ((decltype(ListTypeFun<idx>(hardware)) &) hardware).val->template register_callback<Type>(fun);
        }

        /** callback one element **/
        template<size_t idx, typename T>
        void register_callback(const T &fun) {
            ((decltype(ListTypeFun<idx>(hardware)) &) hardware).val->register_callback(fun);
        }

        /** callback key **/
        template<size_t idx, typename Key, typename ...Args>
        void register_callback(const Key &key, Args &&... args) {
            ((decltype(ListTypeFun<idx>(hardware)) &) hardware).val->register_callback_key(key, args...);
        }

        void join() {
            ListBase::join(hardware);
        }
    };
}  // namespace Hardware
