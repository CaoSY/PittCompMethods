#include <iostream>
#include <limits>
#include <cstdint>
#include <bitset>
#include <iomanip>
#include <string>
using namespace std;

class FLOAT
{
private:
    static const uint32_t sign_width = 1;
    static const uint32_t exponent_width = 8;
    static const uint32_t fraction_width = 23;
    static const uint32_t float_width = sign_width + exponent_width + fraction_width;

    static const uint32_t sign_mask = uint32_t(1) << (exponent_width + fraction_width);
    static const uint32_t exponent_mask = ((uint32_t(1) << exponent_width) - 1) << fraction_width;
    static const uint32_t fraction_mask = (uint32_t(1) << fraction_width) - 1;

public:
    typedef bitset<sign_width> FloatSign;
    typedef bitset<exponent_width> FloatExponent;
    typedef bitset<fraction_width> FloatFraction;
    typedef bitset<float_width> FloatBit;

    static uint32_t const uint32_cast(const float num_flt)
    {
        return *(uint32_t *)&num_flt;
    }

    static FloatBit const bit_cast(const float num_flt)
    {
        return FloatBit(uint32_cast(num_flt));
    }

    static FloatSign const sign(const float num_flt)
    {
        return FloatSign((uint32_cast(num_flt) & sign_mask) == sign_mask);
    }

    static FloatExponent const exponent(const float num_flt)
    {
        return FloatExponent((uint32_cast(num_flt) & exponent_mask) >> fraction_width);
    }

    static FloatFraction const fraction(const float num_flt)
    {
        return FloatFraction(uint32_cast(num_flt) & fraction_mask);
    }
};

class DOUBLE
{
private:
    static const uint64_t sign_width = 1;
    static const uint64_t exponent_width = 11;
    static const uint64_t fraction_width = 52;
    static const uint64_t double_width = sign_width + exponent_width + fraction_width;

    static const uint64_t sign_mask = uint64_t(1) << (exponent_width + fraction_width);
    static const uint64_t exponent_mask = ((uint64_t(1) << exponent_width) - 1) << fraction_width;
    static const uint64_t fraction_mask = (uint64_t(1) << fraction_width) - 1;

public:
    typedef bitset<sign_width> DoubleSign;
    typedef bitset<exponent_width> DoubleExponent;
    typedef bitset<fraction_width> DoubleFraction;
    typedef bitset<double_width> DoubleBit;

    static uint64_t const uint64_cast(const double num_dbl)
    {
        return *(uint64_t *)&num_dbl;
    }

    static DoubleBit const bit_cast(const double num_dbl)
    {
        return DoubleBit(uint64_cast(num_dbl));
    }

    static DoubleSign const sign(const double num_dbl)
    {
        return DoubleSign((uint64_cast(num_dbl) & sign_mask) == sign_mask);
    }

    static DoubleExponent const exponent(const double num_dbl)
    {
        return DoubleExponent((uint64_cast(num_dbl) & exponent_mask) >> fraction_width);
    }

    static DoubleFraction const fraction(const double num_dbl)
    {
        return DoubleFraction(uint64_cast(num_dbl) & fraction_mask);
    }
};


int main(int argc, char **argv)
{
    float float_max = numeric_limits<float>::max();
    float float_eps = numeric_limits<float>::epsilon();
    float float_min = numeric_limits<float>::min();

    double double_max = numeric_limits<double>::max();
    double double_eps = numeric_limits<double>::epsilon();
    double double_min = numeric_limits<double>::min();

    const int text_width = 20;
    const int base_ten_width = 15;
    const int base_two_width = 50;
    const int sign_bit_width = 10;
    const int exponent_bit_width = 20;
    const int fraction_bit_width = 55;

    cout << setw(text_width) << " ";
    cout << setw(base_ten_width) << left << "base10";
    cout << setw(sign_bit_width) << left << "sign bit";
    cout << setw(exponent_bit_width) << left << "exponent bit";
    cout << setw(fraction_bit_width) << left << "fraction bit";
    cout << endl;

    cout << setw(text_width) << left << "max of float";
    cout << setw(base_ten_width) << left << float_max;
    cout << setw(sign_bit_width) << left << FLOAT::sign(float_max);
    FLOAT::FloatExponent float_max_exponent = FLOAT::exponent(float_max);
    cout << setw(exponent_bit_width) << left << float_max_exponent.to_string()
            + " (" + to_string(float_max_exponent.to_ulong()) + ")";
    cout << setw(fraction_bit_width) << left << FLOAT::fraction(float_max);
    cout << endl;

    cout << setw(text_width) << left << "max of double";
    cout << setw(base_ten_width) << left << double_max;
    cout << setw(sign_bit_width) << left << DOUBLE::sign(double_max);
    DOUBLE::DoubleExponent double_max_exponent = DOUBLE::exponent(double_max);
    cout << setw(exponent_bit_width) << left << double_max_exponent.to_string()
             + " (" + to_string(double_max_exponent.to_ulong()) + ")";
    cout << setw(fraction_bit_width) << left << DOUBLE::fraction(double_max);
    cout << endl;

    cout << setw(text_width) << left << "min of float";
    cout << setw(base_ten_width) << left << float_min;
    cout << setw(sign_bit_width) << left << FLOAT::sign(float_min);
    FLOAT::FloatExponent float_min_exponent = FLOAT::exponent(float_min);
    cout << setw(exponent_bit_width) << left << float_min_exponent.to_string()
            + " (" + to_string(float_min_exponent.to_ulong()) + ")";
    cout << setw(fraction_bit_width) << left << FLOAT::fraction(float_min);
    cout << endl;

    cout << setw(text_width) << left << "min of double";
    cout << setw(base_ten_width) << left << double_min;
    cout << setw(sign_bit_width) << left << DOUBLE::sign(double_min);
    DOUBLE::DoubleExponent double_min_exponent = DOUBLE::exponent(double_min);
    cout << setw(exponent_bit_width) << left << double_min_exponent.to_string()
            + " (" + to_string(double_min_exponent.to_ulong()) + ")";
    cout << setw(fraction_bit_width) << left << DOUBLE::fraction(double_min);
    cout << endl;

    cout << setw(text_width) << left << "epsilon of float";
    cout << setw(base_ten_width) << left << float_eps;
    cout << setw(sign_bit_width) << left << FLOAT::sign(float_eps);
    FLOAT::FloatExponent float_eps_exponent = FLOAT::exponent(float_eps);
    cout << setw(exponent_bit_width) << left << float_eps_exponent.to_string()
            + " (" + to_string(float_eps_exponent.to_ulong()) + ")";
    cout << setw(fraction_bit_width) << left << FLOAT::fraction(float_eps);
    cout << endl;

    cout << setw(text_width) << left << "epsilon of double";
    cout << setw(base_ten_width) << left << double_eps;
    cout << setw(sign_bit_width) << left << DOUBLE::sign(double_eps);
    DOUBLE::DoubleExponent double_eps_exponent = DOUBLE::exponent(double_eps);
    cout << setw(exponent_bit_width) << left << double_eps_exponent.to_string()
            + " (" + to_string(double_eps_exponent.to_ulong()) + ")";
    cout << setw(fraction_bit_width) << left << DOUBLE::fraction(double_eps);
    cout << endl;


    return 0;
}