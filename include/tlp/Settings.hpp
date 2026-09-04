#pragma once
#include <Geode/Geode.hpp>

#include <concepts>
#include <functional>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

namespace tlp::settings::_impl {

template <class Callback, class T>
concept settings_callback =
    std::invocable<Callback, T> && std::same_as<std::invoke_result_t<Callback, T>, void>;

}

namespace tlp::settings {

using namespace geode::prelude;

template <class T, _impl::settings_callback<T> Callback>
void listener(std::string_view key, Callback&& callback) {
    std::invoke(callback, Mod::get()->getSettingValue<T>(key));
    listenForSettingChanges<T>(std::string(key), std::move(callback));
};

} // namespace tlp::settings