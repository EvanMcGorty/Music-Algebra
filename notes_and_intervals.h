#include"fundamental_type_concepts.h"

template<typename t>
concept interval = requires (t const& a)
{
    {a*a} -> std::same_as<t>;
    {a/a} -> std::same_as<t>;
    {a.inverted()} -> std::same_as<t>;
};

template<typename t>
concept note = requires (t const& a)
{
    {a/a} -> interval;
    {a*(a/a)} -> std::same_as<t>;
    {a/(a/a)} -> std::same_as<t>;
};

//interval between note names, e.g. a third, a fifth, a second, not something like a major third or a minor seventh.
//simply the number of diatonic steps required to get from one note to another
template<interval_count_num interval_length_t>
struct basic_interval
{

    interval_length_t length;

    constexpr explicit basic_interval(interval_length_t a) : length(a){}

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


template<interval_count_num interval_length_t, accidental_count_num accidental_count_t, note_letter_holder<accidental_count_t,basic_interval<interval_length_t>> letter_t>
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

    constexpr explicit operator accidental_count_t()
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

    constexpr explicit pure_interval(basic_interval<interval_length_t> const& a, accidental_count_t const& b)
    : diatonic_distance(a), accidental_distance(b) {}

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
};

//a note in its mathematically purest form.
//indicates diatonic position as well as its accidental.
template<interval_count_num interval_length_t, accidental_count_num accidental_count_t, note_letter_holder<accidental_count_t,basic_interval<interval_length_t>> letter_t>
struct note_name
{
    letter_t letter;
    accidental_count_t accidental_count;

    constexpr explicit note_name(letter_t const& a, accidental_count_t const& b) 
    : letter(a), accidental_count(b) {}

    constexpr note_name(note_letter<interval_length_t,accidental_count_t,letter_t> const& a, accidental<accidental_count_t> const& b) 
    : letter(letter_t(a)), accidental_count(accidental_count_t(b)) {}

    constexpr pure_interval<interval_length_t,accidental_count_t> operator/(note_name const& rhs) const
    {
        return pure_interval(letter.diatonic_distance_above(rhs.letter),letter.accidental_difference_from(rhs.letter)+(accidental_count+(-rhs.accidental_count)));
    }

    constexpr note_name operator*(pure_interval<interval_length_t,accidental_count_t> const& rhs) const
    {
        letter_t new_letter = letter.shifted_up_by(rhs.diatonic_distance);
        return note_name(new_letter,accidental_count+rhs.accidental_distance+(letter.accidental_difference_from(new_letter)));
    }

    constexpr note_name operator/(pure_interval<interval_length_t,accidental_count_t> const& rhs) const
    {
        return this->operator*(rhs.inverted());
    }
};
