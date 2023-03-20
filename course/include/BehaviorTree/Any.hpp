/**
 * @file any.hpp
 * @author Gonzalo Cervetti (g.cervetti@ekumenlabs.com)
 * @brief Custom implementation of std::any
 * @version 0.1
 * @date 2020-06-10
 *
 * @copyright Copyright (c) 2020
 *
 */

#pragma once

// Standard include
#include <iostream>
#include <memory>
#include <string>
#include <typeindex>
#include <typeinfo>

namespace behavior_tree {

class Any {
   public:
    // templated constructor
    template <typename T>
    Any(const T &obj) {
        set(obj);
    }

    // Default constructor
    Any() = default;

    // default destructor
    ~Any() = default;
    /**
     * @brief Populates the object with the new value.
     *        The holder can inherit from PlaceHolder and hence,
     *        store any type of value.
     * @param value value to store
     */
    template <typename T>
    void set(const T &value) {
        content_type_ = std::make_shared<Holder<T>>(value);
        place_holder_type_ = std::make_shared<std::type_index>(typeid(value));
    }

    /**
     * @brief Returns the object and cast it into standard values.
     *        If you try to cast to a value different that the one set in
     *        set method, I will throw a runtime Exception.
     * @return T
     */
    template <typename T>
    T get() {
        if (!content_type_) {
            throw std::runtime_error("Cannot read without setting a value before.");
        }
        if (*place_holder_type_ != typeid(T)) {
            throw std::runtime_error("Casting object to a different type than the original.");
        }
        return static_cast<Holder<T> &>(*content_type_).value_;
    }

   private:
    struct PlaceHolder {
        virtual ~PlaceHolder() = default;
    };
    template <typename T>
    struct Holder : public PlaceHolder {
        template <typename U>
        explicit Holder(U &&value) {
            value_ = value;
        }

        T value_;
    };

    std::shared_ptr<PlaceHolder> content_type_;
    std::shared_ptr<std::type_index> place_holder_type_;
};

}  // namespace behavior_tree