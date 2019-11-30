#pragma once

#include <string>
#include <type_traits>
#include <utility>

namespace usw
{
    template <class SeriObjType>
    class UniSeriObjWrapper;

    template <class SeriObjType, class ValType>
    void toSeriObj(UniSeriObjWrapper<SeriObjType>& seriObjWrapper, const ValType&);

    template <class SeriObjType, class ValType>
    void fromSeriObj(const UniSeriObjWrapper<SeriObjType>& seriObjWrapper, ValType&);

    template <class SeriObjType>
    class UniSeriObjWrapper
    {
    public:
        using SeriConstRefObjType = typename SeriObjType::ConstRefObjType;

        explicit UniSeriObjWrapper()
        {
        }

        template <class ValType>
        void set(const std::string& name, const ValType& val)
        {
            set(name, val, 0);
        }

        template <class ValType>
        void fill(const std::string& name, ValType& val) const
        {
            fill(name, val, 0);
        }

        void deserializeFromString(const std::string& str)
        {
            return seriObj.deserializeFromString(str);
        }

        std::string serializeToString()
        {
            return seriObj.serializeToString();
        }

    private:
        explicit UniSeriObjWrapper(SeriObjType refSeriObj) : seriObj{refSeriObj}
        {
            static_assert(std::is_same<SeriObjType, SeriConstRefObjType>::value,
                          "This constructor should only be used with a reference.");
        }

        template <class ValType>
        auto set(const std::string& name, const ValType& val, int)
            -> decltype(std::declval<UniSeriObjWrapper<SeriObjType>>().seriObj.set(name, val), void())
        {
            seriObj.set(name, val);
        }

        template <class ValType>
        void set(const std::string& name, const ValType& val, long)
        {
            using usw::toSeriObj;

            UniSeriObjWrapper<SeriObjType> newSeriObjWrapper;
            toSeriObj(newSeriObjWrapper, val);
            seriObj.set(name, std::move(newSeriObjWrapper.seriObj));
        }

        template <class ValType>
        auto fill(const std::string& name, ValType& val, int) const
            -> decltype(std::declval<UniSeriObjWrapper<SeriObjType>>().seriObj.fill(name, val), void())
        {
            seriObj.fill(name, val);
        }

        template <class ValType>
        void fill(const std::string& name, ValType& val, long) const
        {
            using usw::fromSeriObj;

            UniSeriObjWrapper<SeriConstRefObjType> refSeriObjWrapper{seriObj.getSubObj(name)};
            fromSeriObj(refSeriObjWrapper, val);
        }

    private:
        SeriObjType seriObj;

    private:
        template <class OtherSeriObjType>
        friend class UniSeriObjWrapper;
    };
} // namespace usw