#pragma once

#include <string>
#include <type_traits>

#include "nlohmannjson.hpp"

namespace usw
{
    template <class NlohmannJsonObjType = nlohmann::json>
    class NlohmannJsonSeriObj
    {
    public:
        using ConstRefObjType = NlohmannJsonSeriObj<const NlohmannJsonObjType&>;

        explicit NlohmannJsonSeriObj()
        {
        }

        template <class ValType, std::enable_if_t<std::is_arithmetic<ValType>::value, int> = 0>
        void set(const std::string& name, ValType val)
        {
            obj[name] = val;
        }

        void set(const std::string& name, std::string val)
        {
            obj[name] = std::move(val);
        }

        void set(const std::string& name, NlohmannJsonSeriObj val)
        {
            obj[name] = std::move(val.obj);
        }

        template <class ValType, std::enable_if_t<std::is_arithmetic<ValType>::value, int> = 0>
        void fill(const std::string& name, ValType& val) const
        {
            val = obj[name].template get<ValType>();
        }

        void fill(const std::string& name, std::string& val) const
        {
            val = obj[name].template get<std::string>();
        }

        ConstRefObjType getSubObj(const std::string& name) const
        {
            return ConstRefObjType{obj[name]};
        }

        void deserializeFromString(const std::string& str)
        {
            obj = nlohmann::json::parse(str);
        }

        std::string serializeToString()
        {
            return obj.dump();
        }

    private:
        explicit NlohmannJsonSeriObj(NlohmannJsonObjType newObj) : obj{newObj}
        {
            static_assert(std::is_reference<NlohmannJsonObjType>::value,
                          "This constructor should only be used with a reference.");
        }

    private:
        NlohmannJsonObjType obj;

    private:
        template <class OtherNlohmannJsonObjType>
        friend class NlohmannJsonSeriObj;
    };
} // namespace usw
