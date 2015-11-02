#ifndef ARABICA_VECTOR_EXPTEN
#define ARABICA_VECTOR_EXPTEN

namespace arabica
{
    template<class L, class R>
    class add
    {
        private:

            const L& l;
            const R& r;

        public:

            add(const L& lhs, const R& rhs)
                : l(lhs), r(rhs)
            {
                ;
            }

            double operator[](int i) const 
            {
                return l[i] + r[i];
            }
    };

    template<class L, class R>
    class sub
    {
        private:

            const L& l;
            const R& r;

        public:

            sub(const L& lhs, const R& rhs)
                : l(lhs), r(rhs)
            {
                ;
            }

            double operator[](int i) const
            {
                return l[i] - r[i];
            }
    };

}//end namespace arabica

#endif //ARABICA_VECTOR_EXPTEN
