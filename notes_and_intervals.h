#include"fundamental_type_concepts.h"
#include<assert.h>

namespace ma
{

    template<typename t>
    concept interval = requires (t const& a)
    {
        {a*a} -> std::convertible_to<t>;
        {a/a} -> std::convertible_to<t>;
        {a.inverted()} -> std::convertible_to<t>;
    };

    template<typename t>
    concept note = requires (t const& a)
    {
        {a/a} -> interval;
        {a*(a/a)} -> std::convertible_to<t>;
        {a/(a/a)} -> std::convertible_to<t>;
    };

    //interval between note names, e.g. a third, a fifth, a second, not something like a major third or a minor seventh.
    //simply the number of diatonic steps required to get from one note to another
    template<interval_count_num interval_length_t>
    struct basic_interval
    {
        interval_length_t length;

        constexpr explicit basic_interval(interval_length_t const& a) : length(a){}
        
        constexpr explicit operator interval_length_t() const
        {
            return length;
        }

        constexpr basic_interval operator*(basic_interval const& rhs) const
        {
            return basic_interval{length+rhs.length};
        }

        constexpr basic_interval operator/(basic_interval const& rhs) const
        {
            return basic_interval{length+(-rhs.length)};
        }

        constexpr basic_interval inverted() const
        {
            return basic_interval{-length};
        }

    };


    template<interval_count_num interval_length_t, accidental_count_num accidental_count_t, note_letter_holder<accidental_count_t,interval_length_t> letter_t>
    struct note_letter
    {
        letter_t letter;
        
        constexpr explicit note_letter(letter_t const& a) : letter(a) {}

        constexpr explicit operator letter_t() const
        {
            return letter;
        }

        basic_interval<interval_length_t> operator/(note_letter const& rhs) const
        {
            return letter.diatonic_distance_above(rhs);
        }

        note_letter operator*(basic_interval<interval_length_t> const& rhs) const
        {
            return letter.shifted_up_by(rhs);
        }

        note_letter operator/(basic_interval<interval_length_t> const& rhs) const
        {
            return letter.shifted_up_by(rhs.inverted());
        }
    };

    template<accidental_count_num accidental_count_t>
    struct accidental
    {
        accidental_count_t accidental_count;
        
        constexpr explicit accidental(accidental_count_t const& a) : accidental_count(a) {}

        constexpr explicit operator accidental_count_t() const
        {
            return accidental_count;
        }

    };

    //an interval in its mathematically purest form.
    //indicates diatonic distance as well as the quality of the interval (ie major, minor, diminished, double diminished, augmented, etc)
    //does not indicate the actual numeric ratio of this interval
    template<interval_count_num interval_length_t, accidental_count_num accidental_count_t>
    struct pure_interval
    {
        basic_interval<interval_length_t> diatonic_distance;
        accidental_count_t accidental_distance;


        constexpr pure_interval(basic_interval<interval_length_t> const& a, accidental<accidental_count_t> const& b)
        : diatonic_distance(a), accidental_distance(accidental_count_t(b)) {}

        constexpr pure_interval operator*(pure_interval const& rhs) const
        {
            return pure_interval(diatonic_distance*rhs.diatonic_distance,accidental_distance+rhs.accidental_distance);
        }
        constexpr pure_interval operator/(pure_interval const& rhs) const 
        {
            return pure_interval(diatonic_distance/rhs.diatonic_distance,accidental_distance+(-rhs.accidental_distance));
        }

        constexpr pure_interval inverted() const
        {
            return pure_interval(diatonic_distance.inverted(),-accidental_distance);
        }

    private:
        constexpr explicit pure_interval(basic_interval<interval_length_t> const& a, accidental_count_t const& b)
        : diatonic_distance(a), accidental_distance(b) {}
    };

    //a note in its mathematically purest form.
    //indicates diatonic position as well as its accidental.
    template<
        interval_count_num interval_length_t, accidental_count_num accidental_count_t, 
        note_letter_holder<accidental_count_t,interval_length_t> letter_t>
    struct note_name
    {
        letter_t letter;
        accidental_count_t accidental_count;

        constexpr note_name(note_letter<interval_length_t,accidental_count_t,letter_t> const& a, accidental<accidental_count_t> const& b) 
        : letter(letter_t(a)), accidental_count(accidental_count_t(b)) {}

        constexpr pure_interval<interval_length_t,accidental_count_t> operator/(note_name const& rhs) const
        {
            return pure_interval(basic_interval{letter.diatonic_distance_above(rhs.letter)},accidental{letter.accidental_difference_from(rhs.letter)+(accidental_count+(-rhs.accidental_count))});
        }

        constexpr note_name operator*(pure_interval<interval_length_t,accidental_count_t> const& rhs) const
        {
            letter_t new_letter = letter.shifted_up_by(rhs.diatonic_distance.length);
            return note_name(new_letter,accidental_count+rhs.accidental_distance+(letter.accidental_difference_from(new_letter)));
        }

        constexpr note_name operator/(pure_interval<interval_length_t,accidental_count_t> const& rhs) const
        {
            return this->operator*(rhs.inverted());
        }

    private:
        constexpr explicit note_name(letter_t const& a, accidental_count_t const& b) 
        : letter(a), accidental_count(b) {}
    };



    template<exact_distance_num exact_distance_t>
    struct ratio_interval
    {

        exact_distance_t distance;

        constexpr explicit ratio_interval(exact_distance_t const& a) : distance(a){}

        constexpr explicit operator exact_distance_t() const
        {
            return distance;
        }

        constexpr ratio_interval operator*(ratio_interval const& rhs) const
        {
            return ratio_interval{distance+rhs.distance};
        }

        constexpr ratio_interval operator/(ratio_interval const& rhs) const
        {
            return ratio_interval{distance+(-rhs.distance)};
        }

        constexpr ratio_interval inverted() const
        {
            return ratio_interval{-distance};
        }

    };


    template<interval first_t, interval second_t>
    struct multiple_interval
    {
        first_t first;
        second_t second;

        constexpr multiple_interval(first_t a,second_t b) : first(a), second(b) {}

        constexpr multiple_interval operator*(multiple_interval const& rhs) const
        {
            return {first*rhs.first,second*rhs.second};
        }
        
        constexpr multiple_interval operator/(multiple_interval const& rhs) const
        {
            return {first/rhs.first,second/rhs.second};
        }

        constexpr multiple_interval inverted() const
        {
            return {first.inverted(),second.inverted()};
        }
    };


    template<interval_count_num interval_length_t, accidental_count_num accidental_count_t, exact_distance_num exact_distance_t, exact_distance_num...exact_distance_ts >
    struct exact_interval
        : public multiple_interval<
            ratio_interval<exact_distance_t>,
            exact_interval<interval_length_t,accidental_count_t,exact_distance_ts...>
        >
    {

        constexpr exact_interval(pure_interval<interval_length_t,accidental_count_t> const& a, ratio_interval<exact_distance_t> const& b, ratio_interval<exact_distance_ts> const&...rest)
        : multiple_interval<ratio_interval<exact_distance_t>,exact_interval<interval_length_t,accidental_count_t,exact_distance_ts...>>
        (b,exact_interval<interval_length_t,accidental_count_t,exact_distance_ts...>{a,rest...}) {}

        constexpr exact_interval(multiple_interval<ratio_interval<exact_distance_t>,exact_interval<interval_length_t,accidental_count_t,exact_distance_ts...>> const& a)
        : multiple_interval<ratio_interval<exact_distance_t>,exact_interval<interval_length_t,accidental_count_t,exact_distance_ts...>>
        (a.first,a.second) {}

        constexpr exact_interval<interval_length_t,accidental_count_t,exact_distance_ts...> next() const
        {
            return
            multiple_interval<ratio_interval<exact_distance_t>,exact_interval<interval_length_t,accidental_count_t,exact_distance_ts...>>
            ::second;
        }

        template<size_t index>
        constexpr interval auto index_ratio() const
        {
            if constexpr (index==0)
            {
                return 
                multiple_interval<ratio_interval<exact_distance_t>,exact_interval<interval_length_t,accidental_count_t,exact_distance_ts...>>
                ::first;
            }
            else
            {
                return 
                multiple_interval<ratio_interval<exact_distance_t>,exact_interval<interval_length_t,accidental_count_t,exact_distance_ts...>>
                ::second.template index_ratio<index-1>();
            }
        }
    };

    template<interval_count_num interval_length_t, accidental_count_num accidental_count_t, exact_distance_num exact_distance_t>
    struct exact_interval<interval_length_t,accidental_count_t,exact_distance_t>
        : public multiple_interval<ratio_interval<exact_distance_t>,pure_interval<interval_length_t,accidental_count_t>>
    {

        constexpr exact_interval(pure_interval<interval_length_t,accidental_count_t> const& a, ratio_interval<exact_distance_t> const& b)
        : multiple_interval<ratio_interval<exact_distance_t>,pure_interval<interval_length_t,accidental_count_t>>
        (b,a) {}

        constexpr exact_interval(multiple_interval<ratio_interval<exact_distance_t>,pure_interval<interval_length_t,accidental_count_t>> const& a)
        : multiple_interval<ratio_interval<exact_distance_t>,pure_interval<interval_length_t,accidental_count_t>>
        (a.first,a.second) {}

        constexpr pure_interval<interval_length_t,accidental_count_t> pure_distance() const
        {
            return 
            multiple_interval<ratio_interval<exact_distance_t>,pure_interval<interval_length_t,accidental_count_t>>
            ::second;
        }

        template<size_t index>
        constexpr ratio_interval<exact_distance_t> index_ratio() const
        {
            static_assert(index==0,"exact_interval indexed for a ratio interval too deep");
            return
            multiple_interval<ratio_interval<exact_distance_t>,pure_interval<interval_length_t,accidental_count_t>>
            ::first;
        }
    };


    template<
        interval_count_num interval_length_t, accidental_count_num accidental_count_t,
        note_letter_holder<accidental_count_t,interval_length_t> letter_t,
        exact_distance_num exact_distance_t, exact_frequency_num<exact_distance_t> exact_frequency_t>
    struct exact_note
    {
        note_name<interval_length_t,accidental_count_t,letter_t> name;

        
        
    };

}