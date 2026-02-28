#pragma once

#include <algorithm>
#include <charconv>
#include <expected>

using ErrorMessage = snox::ErrorMessage;

inline std::expected<std::string, ErrorMessage>
naturalToBinary(unsigned int n)
{
    if (n == 0)
    {
        return "0";
    }

    if (n == 1)
    {
        return "1";
    }

    if (n <= 0)
    {
        return std::unexpected<ErrorMessage>({
            "naturalToBinary error: invalid natural number." " "
            "Only 0 and positive integers are allowed"
        });
    }

    std::string remainders;

    while (n > 0)
    {
        remainders += std::to_string(n % 2);
        n = n / 2;
    }

    std::ranges::reverse(remainders);

    return remainders;
}

inline std::expected<std::string, ErrorMessage>
naturalToBinary(unsigned int n, const unsigned int bits)
{
    if (bits == 0)
    {
        return std::unexpected<ErrorMessage>(
            {"naturalToBinary error: bit width must be at least 1"});
    }

    if (bits < std::numeric_limits<unsigned int>::digits)
    {
        if (const unsigned int maxValue = (1u << bits) - 1u; n > maxValue)
        {
            return std::unexpected<ErrorMessage>({
                "naturalToBinary error: number does not fit in the requested bit width"
            });
        }
    }

    std::string result;
    if (n == 0)
    {
        for (int i = 0; i < bits; i++)
        {
            result += "0";
        }
        return result;
    }
    if (n == 1)
    {
        for (int i = 0; i < bits - 1; i++)
        {
            result += "0";
        }
        result += "1";
        return result;
    }

    if (n <= 0)
    {
        return std::unexpected<ErrorMessage>({
            "naturalToBinary error: invalid natural number." " "
            "Only 0 and positive integers are allowed"
        });
    }
    std::string remainders;

    while (n > 0)
    {
        remainders += std::to_string(n % 2);
        n = n / 2;
    }

    while (remainders.length() < bits)
    {
        remainders += "0";
    }

    std::ranges::reverse(remainders);

    return remainders;
}

inline std::expected<unsigned int, ErrorMessage>
binaryToNatural(const std::string& binary, const unsigned int bits)
{
    if (bits != binary.length())
    {
        return std::unexpected<ErrorMessage>{
            "binaryToNatural error: input length must be equal to bits. bits: " +
            std::to_string(bits)};
    }

    unsigned int decimal = 0;

    for (unsigned int i = 0, k = binary.length() - 1; i < binary.length(); i++, k--)
    {
        if (binary[i] == '0')
        {
            continue;
        }
        if (binary[i] == '1')
        {
            decimal += static_cast<unsigned int>(std::pow(2, k));
            continue;
        }

        const auto c = binary[i];
        const std::string msg1 = "string contains invalid characters: ";
        std::string msg2 = " at position " + std::to_string(i);
        const std::string msg3 = ". Only 1's and 0's are allowed";

        return std::unexpected<ErrorMessage>(
            {"binaryToNatural error: " + msg1 + "\"" + c + "\"" += msg2 += msg3});
    }

    return decimal;
}

inline std::expected<std::string, ErrorMessage>
charToBinary(const unsigned char c, unsigned int bits)
{
    if (c > 127)
    {
        return std::unexpected<ErrorMessage>({"charToBinary error: Not ASCII (must be 0..127)"});
    }
    const int i = c;

    return naturalToBinary(i);
}

inline std::expected<char, ErrorMessage>
binaryToChar(const std::string& binary, const unsigned int bits)
{
    if (bits > 7 || bits < 7)
        return std::unexpected<ErrorMessage>({
            "binaryToChar error: Binary representing ASCII must be exactly 7 bits, current bits: " + std::to_string(bits)
        });

    auto i = binaryToNatural(binary, bits);
    if (i)
    {
        return static_cast<char>(i.value());
    }
    return i;
}

inline std::expected<char, ErrorMessage>
naturalToChar(const unsigned int n)
{
    if (n > 127)
    {
        return std::unexpected<ErrorMessage>({"naturalToChar error: Not ASCII (must be 0..127)"});
    }

    const unsigned int i = n;
    return static_cast<char>(i);
}

inline std::expected<unsigned int, ErrorMessage>
charToNatural(const unsigned char c)
{
    if (c > 127)
    {
        return std::unexpected<ErrorMessage>({"charToNatural error: Not ASCII (must be 0..127)"});
    }

    const int i = c;
    return i;
}

//Hex: 0 1 2 3 4 5 6 7 8 9 a b c d e f
inline std::expected<std::string, ErrorMessage>
naturalToHexadecimal(const unsigned int& input)
{
    unsigned int quotient = input;

    std::string result;
    while (quotient > 0)
    {
        constexpr auto base = 16;
        const unsigned int remainder = quotient % base;
        constexpr std::string_view hexDigits = "0123456789ABCDEF";
        result.push_back(hexDigits[remainder]);
        quotient = quotient / base;
    }
    std::ranges::reverse(result);
    return result;
};

inline std::expected<std::string, ErrorMessage>
binaryToHexadecimal(const std::string& input, const unsigned int bits)
{
    auto natural = binaryToNatural(input, bits);
    if (!natural)
    {
        return std::unexpected(natural.error());
    }
    return naturalToHexadecimal(natural.value());
};

inline std::expected<std::string, ErrorMessage>
charToHexadecimal(const char& input)
{
    auto natural = charToNatural(input);
    if (!natural)
    {
        return std::unexpected(natural.error());
    }
    return naturalToHexadecimal(natural.value());
};

inline std::expected<unsigned int, ErrorMessage>
hexadecimalToNatural(const std::string& input)
{
    std::vector<unsigned int> hexadecimals;
    for (const char& c : input)
    {
        if (c >= '0' && c <= '9')
            hexadecimals.push_back(c - '0');
        else if (c >= 'A' && c <= 'F')
            hexadecimals.push_back(c - 'A' + 10);
    }

    for (int i = 0; i < hexadecimals.size(); ++i)
    {
        std::reverse(hexadecimals.begin(), hexadecimals.end() - i);
    }

    unsigned r = 0;
    for (int i = static_cast<int>(input.length()) - 1; i >= 0; --i)
    {
        if (i != 0)
        {
            constexpr auto base = 16;
            r += hexadecimals[i] * base;
        }
        else
        {
            r += hexadecimals[i];
        }
    }

    return r;
}

inline std::expected<std::string, ErrorMessage>
hexadecimalToBinary(const std::string& input, const unsigned int bits)
{
    auto hexToNat = hexadecimalToNatural(input);
    if (!hexToNat)
    {
        return std::unexpected(hexToNat.error());
    }
    return naturalToBinary(hexToNat.value());
}

inline std::expected<char, ErrorMessage>
hexadecimalToChar(const std::string& input)
{
    auto hexToNat = hexadecimalToNatural(input);
    if (!hexToNat)
    {
        return std::unexpected(hexToNat.error());
    }
    return naturalToChar(hexToNat.value());
}

enum class enumAscii : unsigned char;

inline std::vector<enumAscii>
stringToEnumVector(const std::string& rawInput)
{
    std::vector<enumAscii> tokens;

    for (const u_int8_t c : rawInput)
    {
        tokens.push_back(enumAscii{c});
    }
    return tokens;
}

