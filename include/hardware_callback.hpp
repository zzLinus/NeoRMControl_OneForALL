//
// Created by rhine on 3/11/24.
//
#pragma once

#include "functional"
#include "map"

namespace Hardware {

    /** callback base define **/
    template<typename ...Args>
    class CallbackBase;

    template<typename Head, typename ...Args>
    class CallbackBase<Head, Args...> : public CallbackBase<Args...> {
    public:
        std::function<void(Head)> callback_fun;
    };

    template<typename Head>
    class CallbackBase<Head> {
    public:
        std::function<void(Head)> callback_fun;
    };

    /** callback define **/
    template<typename Head, typename ...Tail>
    CallbackBase<Head, Tail...> ListTypeFun(CallbackBase<Head, Tail...> &list);

    /** callback define **/
    template<typename ...Args>
    class Callback;


    template<typename Head, typename Sec, typename ...Args>
    class Callback<Head, Sec, Args...> {
    public:
        template<typename T>
        void register_callback(const std::function<void(const T &)> &fun) {
            ((decltype(ListTypeFun<T>(callback_set)) &)callback_set).callback_fun = fun;
        }

    protected:
        template<typename T>
        void callback(const T &val) {
            auto & fun = ((decltype(ListTypeFun<T>(callback_set)) &)callback_set).callback_fun;
            if(fun) {
                fun(val);
            }
        }
    private:
        CallbackBase<Head, Sec, Args...> callback_set;
    };

    template<typename Head>
    class Callback<Head> {
    public:
        template<class T>
        void register_callback(const std::function<void(const T &)> &fun) {
            callback_fun = fun;
        }
        void register_callback(const std::function<void(const Head &)> &fun) {
            callback_fun = fun;
        }

    protected:
        void callback(const Head &val) {
            if(callback_fun) {
                callback_fun(val);
            }
        }

    private:
        std::function<void(const Head &)> callback_fun;
    };

    /** callback key define**/
    template<typename Key, typename ...Args>
    class Callback_key {
    public:
        void callback_key(const Key &key, const Args &... args) {
            if(callback_map.count(key)) {
                callback_map[key](args...);
            }
        }

        void register_callback_key(const Key &key, const std::function<void(const Args &...)> &fun) {
            callback_map[key] = fun;
        }

    private:
        std::map<Key, std::function<void(const Args &...)>> callback_map;
    };

}  // namespace Hardware
