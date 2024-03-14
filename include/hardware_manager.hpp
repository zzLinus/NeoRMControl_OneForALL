#pragma once

#include "hardware_list_base.hpp"
namespace Hardware
{
    /** hardware manager **/
    template<typename ...List>
    class Hardware_manager {
       private:
        using BaseType = ListBase::ListBase<0, List...>;
        BaseType hardware;

        template<size_t idx>
        using ListType = ListBase::ListElement<idx, BaseType>::ListType;

       public:
        template<typename ...Args>
        explicit Hardware_manager(Args &&... args) : hardware(args...) {}

        template<size_t idx, typename ...Args>
        auto get(Args &&... args) {
            return ((ListType<idx> &) hardware).val;
        }

        template<size_t idx, typename ...Args>
        void send(Args &&... args) {
            ((ListType<idx> &) hardware).val->send(args...);
        }

        /** callback **/
        template<size_t idx, typename Type>
        void register_callback(const std::function<void(const Type &)> &fun) {
            ((ListType<idx> &) hardware).val->template register_callback<Type>(fun);
        }

        /** callback one element **/
        template<size_t idx, typename T>
        void register_callback(const T &fun) {
            ((ListType<idx> &) hardware).val->register_callback(fun);
        }

        /** callback key **/
        template<size_t idx, typename Key, typename ...Args>
        void register_callback(const Key &key, Args &&... args) {
            ((ListType<idx> &) hardware).val->register_callback_key(key, args...);
        }

        void join() {
            ListBase::join(hardware);
        }
    };
}  // namespace Hardware
