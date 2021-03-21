
#include<concepts>

//interval between note names, e.g. a third, a fifth, a second, not something like a major third or a minor seventh.
//simply the number of diatonic steps required to get from one note to another
struct basic_interval
{
    using interval_length_t = int;

    interval_length_t length;

    explicit constexpr basic_interval(interval_length_t a) : length(a){}

    constexpr basic_interval operator*(basic_interval rhs) const
    {
        return basic_interval{length+rhs.length};
    }

    constexpr basic_interval operator/(basic_interval rhs) const
    {
        return basic_interval{length-rhs.length};
    }

    constexpr basic_interval inverted() const
    {
        return basic_interval{-length};
    }

};

//a number type to represent the number and direction of an accidental attached to a note in a given system.
//must be able to be added and subtracted with itself and be .inverted
template<typename t>
concept accidental_count_num = requires (t const& a)
{
    {a+a} -> std::same_as<t>;
    {-a} -> std::same_as<t>;
};

//a type that represents a note name, independent of accidentals.
//can be multiplied by a basic_interval to shift it up by that interval
template<typename t, typename accidental_t>
concept note_letter_holder = 
    accidental_count_num<accidental_t> 
    && requires (t const& a, basic_interval const& b)
{
    {a.diatonic_distance_above(a)} -> std::same_as<basic_interval>; //should preferably only return positive intervals
    {a.shifted_up_by(b)} -> std::same_as<t>; //naturally shifts down if b is a negative interval
	{a.accidental_difference_from(a)} -> std::convertible_to<accidental_t>;
};


//an interval in its mathematically purest form.
//indicates diatonic distance as well as the quality of the interval (ie major, minor, diminished, double diminished, augmented, etc)
//does not indicate the actual numeric ratio of this interval
template<accidental_count_num num_t>
struct pure_interval
{
	basic_interval diatonic_distance;
	num_t accidental_distance;

    pure_interval(basic_interval a, num_t b) : diatonic_distance(a), accidental_distance(b) {}

    pure_interval operator*(pure_interval const& rhs) const
    {
        return pure_interval(diatonic_distance*rhs.diatonic_distance,accidental_distance+rhs.accidental_distance)
    }
    pure_interval operator/(pure_interval const& rhs) const 
    {
        return pure_interval(diatonic_distance/rhs.diatonic_distance,accidental_distance+(-rhs.accidental_distance))
    }

    pure_interval inverted() const
    {
        return pure_interval(diatonic_distance.inverted(),-accidental_distance);
    }
};

//a note in its mathematically purest form.
//indicates diatonic position as well as its accidental.
template<accidental_count_num num_t, note_letter_holder<num_t> letter_t>
struct note_name
{
    letter_t letter;
    num_t accidental_count;

    note_name(letter_t a, num_t b) : letter(a), accidental_count(b) {}

    pure_interval<num_t> operator/(note_name const& rhs) const
    {
        return pure_interval<num_t>(letter.diatonic_distance_above(rhs.letter),letter.accidental_difference_from(rhs.letter)+(accidental_count+(-rhs.accidental_count)));
    }

    note_name operator*(pure_interval<num_t> const& rhs) const
    {
        letter_t new_letter = letter.shifted_up_by(rhs.diatonic_distance);
        return note_name(new_letter,accidental_count+rhs.accidental_distance+(letter.accidental_difference_from(new_letter)));
    }

    note_name operator/(pure_interval<num_t> const& rhs) const
    {
        pure_interval<num_t> inv_rhs = rhs.inverted();
        return this->operator*(inv_rhs);
    }

};

