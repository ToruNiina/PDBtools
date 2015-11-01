#ifndef ARABICA_REALVEC_H
#define ARABICA_REALVEC_H
#include <array>
#include <iostream>
#include <iomanip>
// #include "ExpTenplate.hpp"

namespace arabica
{
    class Realvec
    {
        public:

            std::array<double, 3> values;

        public:

            Realvec()
                : values({0e0, 0e0, 0e0})
            {
                ;
            }

            Realvec(double _x, double _y, double _z)
                : values({_x, _y, _z})
            {
                ;
            }

            Realvec(const Realvec& v)
                : values(v.values)
            {
                ;
            }

            ~Realvec(){}

            double operator[](int i)
            {
                return values[i];
            }

    };

    std::ostream& operator<<(std::ostream& os, const Realvec& rv)
    {
        os << std::setprecision(16) << rv.values[0] << " ";
        os << std::setprecision(16) << rv.values[1] << " ";
        os << std::setprecision(16) << rv.values[2];
        return os;
    }

}
#endif //ARABICA_REALVEC_H
