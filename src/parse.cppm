export module util.parse;

import std;
import util.types;

export namespace util::parse {

template <typename T>
inline vec<T>
parse_whitespace_numbers (sv s)
{
    vec<T>      out;
    const char* p = s.data();
    const char* e = s.data() + s.size();

    while (p < e)
    {
        while (p < e && std::isspace(static_cast<unsigned char>(*p)))
            ++p;
        if (p >= e)
            break;

        T value{};
        auto [q, ec] = std::from_chars(p, e, value);
        if (ec != std::errc{})
            break;
        p = q;

        out.push_back(value);
    }

    return out;
}

template <typename T>
inline vec<pair<T, T>>
parse_whitespace_pairs (sv s)
{
    vec<pair<T, T>> out;
    const char*     p = s.data();
    const char*     e = s.data() + s.size();

    while (p < e)
    {
        while (p < e && std::isspace(static_cast<unsigned char>(*p)))
            ++p;
        if (p >= e)
            break;

        T a{}, b{};
        auto [q1, ec1] = std::from_chars(p, e, a);
        if (ec1 != std::errc{})
            break;
        p = q1;

        while (p < e && std::isspace(static_cast<unsigned char>(*p)))
            ++p;
        if (p >= e)
            break;

        auto [q2, ec2] = std::from_chars(p, e, b);
        if (ec2 != std::errc{})
            break;
        p = q2;

        out.emplace_back(a, b);
    }

    return out;
}

}
