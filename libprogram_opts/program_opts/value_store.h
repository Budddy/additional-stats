//
//  Copyright (c) Benjamin Kaufmann 2010
//
//  This is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version. 
// 
//  This file is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program. If not, see <http://www.gnu.org/licenses/>.
//
//
// NOTE: ProgramOptions is inspired by Boost.Program_options
//       see: www.boost.org/libs/program_options
//
#ifndef PROGRAM_OPTIONS_VALUE_STORE_H_INCLUDED
#define PROGRAM_OPTIONS_VALUE_STORE_H_INCLUDED

#include <typeinfo>
#include <new>

namespace ProgramOptions {
    namespace detail {
        typedef void (*vcall_type)(const void *in, void **out);
        typedef vcall_type vtable_type[4];
        typedef vtable_type *vptr_type;
// workaround: some compilers don't support
// out-of-class definition of member templates.
        template<class T>
        inline vptr_type vtable(const T * = 0);
        template<class T>
        inline vptr_type base_vtable(const T * = 0);
    }

//! A type that can hold any kind of value type
    class ValueStore {
    public:
        //! creates an empty object
        ValueStore();
        //! copies the value in other
        ValueStore(const ValueStore &other);
        //! stores a copy of obj
        template<class T>
        ValueStore(const T &obj)
                : vptr_(detail::vtable(static_cast<const T *>(0))), value_(0) {
            clone(&obj, &value_);
        }
        //! releases any stored value
        ~ValueStore();
        //! stores a copy of other releasing any previous value
        ValueStore &operator=(ValueStore other);
        //! stores a copy of obj releasing any previous value
        template<class T>
        ValueStore &operator=(const T &obj) {
            ValueStore(obj).swap(*this);
            return *this;
        }
        //! stores obj in this and takes over ownership of obj
        /*!
         * \pre delete obj is valid
         */
        template<class T>
        ValueStore &assimilate(T *obj) {
            clear();
            vptr_ = detail::base_vtable(static_cast<const T *>(0));
            value_ = obj;
            return *this;
        }
        //! swaps this with other
        void swap(ValueStore &other);
        //! Returns true if holder does not contain a value.
        bool empty() const { return vptr_ == 0; }
        //! Returns the type of the stored value.
        const std::type_info &
                type() const;
        //! destroys and releases any stored value
        void clear();
        //! surrenders any stored value without destroying it
        void surrender();

        void *extract_raw() const {
            return !empty()
                   ? extract(const_cast<void **>(&value_))
                   : 0;
        }
    private:
        enum {
            call_extract = 0, vcall_clone = 1, vcall_destroy = 2, vcall_typeid = 3
        };
        typedef detail::vptr_type vptr_type;
        void clone(const void *obj, void **out) const;
        void *extract(void **o) const;
        vptr_type vptr_;
        void *value_;
    };

    struct bad_value_cast : std::bad_cast {
        const char *what() const throw() {
            return "value_cast: invalid conversion on ValueStore";
        }
    };

//! Extracts a typed value from a ValueStore.
/*!
 * \throw bad_value_cast if value is not of type T
 */
    template<class T>
    const T &value_cast(const ValueStore &v, const T * = 0) {
        if (v.type() == typeid(T)) {
            return *static_cast<const T *>(const_cast<const void *>(v.extract_raw()));
        }
        throw bad_value_cast();
    }
//! Returns 0 on error
    template<class T>
    const T *value_cast(const ValueStore *v, const T * = 0) {
        if (v->type() == typeid(T)) {
            return static_cast<const T *>(const_cast<const void *>(v->extract_raw()));
        }
        return 0;
    }
/*!
 * \overload
 */
    template<class T>
    T &value_cast(ValueStore &v, const T *p = 0) {
        return const_cast<T &>(value_cast(const_cast<const ValueStore &>(v), p));
    }
    template<class T>
    T *value_cast(ValueStore *v, const T *p = 0) {
        return const_cast<T *>(value_cast(const_cast<const ValueStore *>(v), p));
    }

//! Extracts a typed value from a ValueStore *without* checking if the type matches
    template<class T>
    const T *unsafe_value_cast(const ValueStore *v, const T * = 0) {
        return static_cast<const T *>(const_cast<const void *>(v->extract_raw()));
    }
/*!
 * \overload
 */
    template<class T>
    T *unsafe_value_cast(ValueStore *v, const T *p = 0) {
        return const_cast<T *>(unsafe_value_cast(const_cast<const ValueStore *>(v), p));
    }

#include "detail/value_store.h"
}
#endif
