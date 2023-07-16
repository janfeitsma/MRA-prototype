#ifndef _INCLUDED_TUNING_PARAM_HPP
#define _INCLUDED_TUNING_PARAM_HPP

#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>


class TunableParameter
{
public:
    std::string name;
    google::protobuf::FieldDescriptor::Type type; // TODO custom enum instead of protobuf dependency
    int minValue;
    int maxValue;
    int defaultValue;
    int value;

    TunableParameter(const std::string& n, google::protobuf::FieldDescriptor::Type t, int min, int max, int def)
        : name(n), type(t), minValue(min), maxValue(max), defaultValue(def), value(def) {}
    virtual ~TunableParameter() {}

    void setValue(int v) { value = v; }

    virtual std::ostream& printValue(std::ostream& os) const = 0;
    friend std::ostream& operator<<(std::ostream& os, const TunableParameter& param) {
        os << "Parameter: " << param.name << ", Value: ";
        param.printValue(os);
        return os;
    }


    /*template <typename T>
    T getValue()
    {
        return T();
    }*/
    //virtual operator int() const { return 0; };
    //virtual operator float() const { return 0.0; };
};

class TunableFloatParameter : public TunableParameter
{
public:
    TunableFloatParameter(const std::string& n, int min, int max, float def)
        : TunableParameter(n, google::protobuf::FieldDescriptor::TYPE_FLOAT, min, max, static_cast<int>(def)) {}

    std::ostream& printValue(std::ostream& os) const override {
        return os << value;
    }

    operator float() const { return static_cast<float>(value); }
};

class TunableIntParameter : public TunableParameter
{
public:
    TunableIntParameter(const std::string& n, int min, int max, int def)
        : TunableParameter(n, google::protobuf::FieldDescriptor::TYPE_INT32, min, max, def) {}

    std::ostream& printValue(std::ostream& os) const override {
        return os << value;
    }

    operator int() const { return value; }
};

#endif // _INCLUDED_TUNING_PARAM_HPP

