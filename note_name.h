#include"fundamental_type_concepts.h"

//interval between note names, e.g. a third, a fifth, a second, not something like a major third or a minor seventh.
//simply the number of diatonic steps required to get from one note to another
template<interval_count_num interval_length_t>
struct basic_interval
{

    interval_length_t length;

    explicit constexpr basic_interval(interval_length_t a) : length(a){}

    constexpr basic_interval operator*(basic_interval rhs) const
    {
        return basic_interval{length+rhs.length};
    }

    constexpr basic_interval operator/(basic_interval rhs) const
    {
        return basic_interval{length+(-rhs.length)};
    }

    constexpr basic_interval inverted() const
    {
        return basic_interval{-length};
    }

};
//an interval in its mathematically purest form.
//indicates diatonic distance as well as the quality of the interval (ie major, minor, diminished, double diminished, augmented, etc)
//does not indicate the actual numeric ratio of this interval
template<accidental_count_num num_t, interval_count_num interval_length_t>
struct pure_interval
{
	basic_interval<interval_length_t> diatonic_distance;
	num_t accidental_distance;

    pure_interval(basic_interval<interval_length_t> a, num_t b) : diatonic_distance(a), accidental_distance(b) {}

    pure_interval operator*(pure_interval const& rhs) const
    {
        return pure_interval(diatonic_distance*rhs.diatonic_distance,accidental_distance+rhs.accidental_distance);
    }
    pure_interval operator/(pure_interval const& rhs) const 
    {
        return pure_interval(diatonic_distance/rhs.diatonic_distance,accidental_distance+(-rhs.accidental_distance));
    }

    pure_interval inverted() const
    {
        return pure_interval(diatonic_distance.inverted(),-accidental_distance);
    }
};

//a note in its mathematically purest form.
//indicates diatonic position as well as its accidental.
template<accidental_count_num num_t, interval_count_num interval_length_t, note_letter_holder<num_t,interval_length_t> letter_t>
struct note_name
{
    letter_t letter;
    num_t accidental_count;

    note_name(letter_t a, num_t b) : letter(a), accidental_count(b) {}

    pure_interval<num_t,interval_length_t> operator/(note_name const& rhs) const
    {
        return pure_interval(letter.diatonic_distance_above(rhs.letter),letter.accidental_difference_from(rhs.letter)+(accidental_count+(-rhs.accidental_count)));
    }

    note_name operator*(pure_interval<num_t,interval_length_t> const& rhs) const
    {
        letter_t new_letter = letter.shifted_up_by(rhs.diatonic_distance);
        return note_name(new_letter,accidental_count+rhs.accidental_distance+(letter.accidental_difference_from(new_letter)));
    }

    note_name operator/(pure_interval<num_t,interval_length_t> const& rhs) const
    {
        pure_interval inv_rhs = rhs.inverted();
        return this->operator*(inv_rhs);
    }

};
