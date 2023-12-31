#pragma once
#include <type_traits>

namespace xme {
template<typename Derived, typename Base>
concept CDerivedFrom = std::is_base_of_v<Base, Derived>
                       && std::is_convertible_v<const volatile Derived*, const volatile Base*>;
}
