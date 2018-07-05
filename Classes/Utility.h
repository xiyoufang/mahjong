//
// Created by farmer on 2018/7/5.
// 工具类

#ifndef COCOSTUDIO_MAHJONG_UTILITY_H
#define COCOSTUDIO_MAHJONG_UTILITY_H

namespace utility {

    template<typename T>
    inline std::string toString(T p) {
        std::ostringstream stream;
        stream << p;
        return stream.str();
    }

    template<typename T1, typename T2>
    inline std::string toString(T1 p1, T2 p2) {
        std::ostringstream stream;
        stream << p1 << p2;
        return stream.str();
    }

    template<typename T1, typename T2, typename T3>
    inline std::string toString(T1 p1, T2 p2, T3 p3) {
        std::ostringstream stream;
        stream << p1 << p2 << p3;
        return stream.str();
    }

    template<typename T1, typename T2, typename T3, typename T4>
    inline std::string toString(T1 p1, T2 p2, T3 p3, T4 p4) {
        std::ostringstream stream;
        stream << p1 << p2 << p3 << p4;
        return stream.str();
    }

    template<typename T1, typename T2, typename T3, typename T4, typename T5>
    inline std::string toString(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5) {
        std::ostringstream stream;
        stream << p1 << p2 << p3 << p4 << p5;
        return stream.str();
    }

    template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
    inline std::string toString(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6) {
        std::ostringstream stream;
        stream << p1 << p2 << p3 << p4 << p5 << p6;
        return stream.str();
    }

    template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
    inline std::string toString(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7) {
        std::ostringstream stream;
        stream << p1 << p2 << p3 << p4 << p5 << p6 << p7;
        return stream.str();
    }

    template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
    inline std::string toString(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8) {
        std::ostringstream stream;
        stream << p1 << p2 << p3 << p4 << p5 << p6 << p7 << p8;
        return stream.str();
    }

    template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
    inline std::string toString(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9) {
        std::ostringstream stream;
        stream << p1 << p2 << p3 << p4 << p5 << p6 << p7 << p8 << p9;
        return stream.str();
    }

    template<>
    inline std::string toString<bool>(bool _value) {
        return _value ? "true" : "false";
    }

    template<typename T>
    inline T parseValue( const std::string& _value )
    {
        std::istringstream stream(_value);
        T result;
        stream >> result;
        if (stream.fail())
            return T();
        else
        {
            int item = stream.get();
            while (item != -1)
            {
                if (item != ' ' && item != '\t')
                    return T();
                item = stream.get();
            }
        }
        return result;
    }

    template<>
    inline bool parseValue(const std::string& _value)
    {
        if (_value == "True" || _value == "true" || _value == "1")
            return true;
        return false;
    }

    template<>
    inline char parseValue(const std::string& _value)
    {
        return (char)parseValue<short>(_value);
    }

    template<>
    inline unsigned char parseValue(const std::string& _value)
    {
        return (unsigned char)parseValue<unsigned short>(_value);
    }


    inline short parseShort(const std::string& _value)
    {
        return parseValue<short>(_value);
    }

    inline unsigned short parseUShort(const std::string& _value)
    {
        return parseValue<unsigned short>(_value);
    }

    inline int parseInt(const std::string& _value)
    {
        return parseValue<int>(_value);
    }

    inline unsigned int parseUInt(const std::string& _value)
    {
        return parseValue<unsigned int>(_value);
    }

    inline size_t parseSizeT(const std::string& _value)
    {
        return parseValue<size_t>(_value);
    }

    inline float parseFloat(const std::string& _value)
    {
        return parseValue<float>(_value);
    }

    inline double parseDouble(const std::string& _value)
    {
        return parseValue<double>(_value);
    }

    inline bool parseBool(const std::string& _value)
    {
        return parseValue<bool>(_value);
    }

    inline char parseChar(const std::string& _value)
    {
        return parseValue<char>(_value);
    }

    inline unsigned char parseUChar(const std::string& _value)
    {
        return parseValue<unsigned char>(_value);
    }
}

#endif //COCOSTUDIO_MAHJONG_UTILITY_H
