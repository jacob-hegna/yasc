#ifndef __YASC_AST_NUMBER_FWD_
#define __YASC_AST_NUMBER_FWD_

#include <complex>
#include <utility>

namespace yasc {
    template<typename T>
    using fraction_t = std::pair<T, T>;

    template<typename T>
    class Number;

    using Complex  = Number<std::complex<double>>;
    using Real     = Number<double>;
    using Rational = Number<fraction_t<int>>;
    using Integer  = Number<int>;
}


#endif
