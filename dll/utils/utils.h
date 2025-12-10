#pragma once



#define IN_RANGE(v , min_ , max_) ( (v) >= (min_) && (v)<= (max_))


template<typename T>
struct ValueReturn {
    T * pointer{};
    T origin{};
    ValueReturn() = default;
    ValueReturn(T * pointer , T newValue ) {
        origin = *pointer;
        *pointer = newValue;
    }

    ~ValueReturn() {
        if (pointer)
            *pointer = origin;
    }
};