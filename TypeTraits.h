//
// Created by JQK on 2016/5/24.
//

#ifndef TINYSTL_TYPETRAITS_H
#define TINYSTL_TYPETRAITS_H

namespace MySTL{
    namespace zstd{
		// 类型萃取主要用于在编译期就能够分配对应的函数
		// 不同的函数用于显著提高不同情况下的效率
        struct _true_type {};
        struct _false_type {};

        /*
         * 萃取传入的 T 类型的类型特性
         */
        template <class T>
        struct _type_traits{
            typedef _false_type		has_trivial_default_constructor;
            typedef _false_type		has_trivial_copy_constructor;
            typedef _false_type		has_trivial_assignment_operator;
            typedef _false_type		has_trivial_destructor;
            typedef _false_type		is_POD_type;
        };

        template <>
        struct _type_traits<bool>{
            typedef _true_type		has_trivial_default_constructor;
            typedef _true_type		has_trivial_copy_constructor;
            typedef _true_type		has_trivial_assignment_operator;
            typedef _true_type		has_trivial_destructor;
            typedef _true_type		is_POD_type;
        };
        template<>
        struct _type_traits<char>
        {
        	typedef _true_type		has_trivial_default_constructor;
        	typedef _true_type		has_trivial_copy_constructor;
        	typedef _true_type		has_trivial_assignment_operator;
        	typedef _true_type		has_trivial_destructor;
        	typedef _true_type		is_POD_type;
        };
    	template<>
    	struct _type_traits<unsigned char>
    	{
    		typedef _true_type		has_trivial_default_constructor;
    		typedef _true_type		has_trivial_copy_constructor;
    		typedef _true_type		has_trivial_assignment_operator;
    		typedef _true_type		has_trivial_destructor;
    		typedef _true_type		is_POD_type;
    	};
		template<>
		struct _type_traits<signed char>
		{
			typedef _true_type		has_trivial_default_constructor;
			typedef _true_type		has_trivial_copy_constructor;
			typedef _true_type		has_trivial_assignment_operator;
			typedef _true_type		has_trivial_destructor;
			typedef _true_type		is_POD_type;
		};
		template<>
		struct _type_traits<wchar_t>
		{
			typedef _true_type		has_trivial_default_constructor;
			typedef _true_type		has_trivial_copy_constructor;
			typedef _true_type		has_trivial_assignment_operator;
			typedef _true_type		has_trivial_destructor;
			typedef _true_type		is_POD_type;
		};
		template<>
		struct _type_traits<short>
		{
			typedef _true_type		has_trivial_default_constructor;
			typedef _true_type		has_trivial_copy_constructor;
			typedef _true_type		has_trivial_assignment_operator;
			typedef _true_type		has_trivial_destructor;
			typedef _true_type		is_POD_type;
		};
		template<>
		struct _type_traits<unsigned short>
		{
			typedef _true_type		has_trivial_default_constructor;
			typedef _true_type		has_trivial_copy_constructor;
			typedef _true_type		has_trivial_assignment_operator;
			typedef _true_type		has_trivial_destructor;
			typedef _true_type		is_POD_type;
		};
		template<>
		struct _type_traits<int>
		{
			typedef _true_type		has_trivial_default_constructor;
			typedef _true_type		has_trivial_copy_constructor;
			typedef _true_type		has_trivial_assignment_operator;
			typedef _true_type		has_trivial_destructor;
			typedef _true_type		is_POD_type;
		};
		template<>
		struct _type_traits<unsigned int>
		{
			typedef _true_type		has_trivial_default_constructor;
			typedef _true_type		has_trivial_copy_constructor;
			typedef _true_type		has_trivial_assignment_operator;
			typedef _true_type		has_trivial_destructor;
			typedef _true_type		is_POD_type;
		};
		template<>
		struct _type_traits<long>
		{
			typedef _true_type		has_trivial_default_constructor;
			typedef _true_type		has_trivial_copy_constructor;
			typedef _true_type		has_trivial_assignment_operator;
			typedef _true_type		has_trivial_destructor;
			typedef _true_type		is_POD_type;
		};
		template<>
		struct _type_traits<unsigned long>
		{
			typedef _true_type		has_trivial_default_constructor;
			typedef _true_type		has_trivial_copy_constructor;
			typedef _true_type		has_trivial_assignment_operator;
			typedef _true_type		has_trivial_destructor;
			typedef _true_type		is_POD_type;
		};
		template<>
		struct _type_traits<long long>
		{
			typedef _true_type		has_trivial_default_constructor;
			typedef _true_type		has_trivial_copy_constructor;
			typedef _true_type		has_trivial_assignment_operator;
			typedef _true_type		has_trivial_destructor;
			typedef _true_type		is_POD_type;
		};
		template<>
		struct _type_traits<unsigned long long>
		{
			typedef _true_type		has_trivial_default_constructor;
			typedef _true_type		has_trivial_copy_constructor;
			typedef _true_type		has_trivial_assignment_operator;
			typedef _true_type		has_trivial_destructor;
			typedef _true_type		is_POD_type;
		};
		template<>
		struct _type_traits<float>
		{
			typedef _true_type		has_trivial_default_constructor;
			typedef _true_type		has_trivial_copy_constructor;
			typedef _true_type		has_trivial_assignment_operator;
			typedef _true_type		has_trivial_destructor;
			typedef _true_type		is_POD_type;
		};
		template<>
		struct _type_traits<double>
		{
			typedef _true_type		has_trivial_default_constructor;
			typedef _true_type		has_trivial_copy_constructor;
			typedef _true_type		has_trivial_assignment_operator;
			typedef _true_type		has_trivial_destructor;
			typedef _true_type		is_POD_type;
		};
		template<>
		struct _type_traits<long double>
		{
			typedef _true_type		has_trivial_default_constructor;
			typedef _true_type		has_trivial_copy_constructor;
			typedef _true_type		has_trivial_assignment_operator;
			typedef _true_type		has_trivial_destructor;
			typedef _true_type		is_POD_type;
		};

		template<class T>
		struct _type_traits<T*>
		{
			typedef _true_type		has_trivial_default_constructor;
			typedef _true_type		has_trivial_copy_constructor;
			typedef _true_type		has_trivial_assignment_operator;
			typedef _true_type		has_trivial_destructor;
			typedef _true_type		is_POD_type;
		};
		template<class T>
		struct _type_traits<const T*>
		{
			typedef _true_type		has_trivial_default_constructor;
			typedef _true_type		has_trivial_copy_constructor;
			typedef _true_type		has_trivial_assignment_operator;
			typedef _true_type		has_trivial_destructor;
			typedef _true_type		is_POD_type;
		};
		template<>
		struct _type_traits<char*>
		{
			typedef _true_type		has_trivial_default_constructor;
			typedef _true_type		has_trivial_copy_constructor;
			typedef _true_type		has_trivial_assignment_operator;
			typedef _true_type		has_trivial_destructor;
			typedef _true_type		is_POD_type;
		};
		template<>
		struct _type_traits<unsigned char*>
		{
			typedef _true_type		has_trivial_default_constructor;
			typedef _true_type		has_trivial_copy_constructor;
			typedef _true_type		has_trivial_assignment_operator;
			typedef _true_type		has_trivial_destructor;
			typedef _true_type		is_POD_type;
		};
		template<>
		struct _type_traits<signed char*>
		{
			typedef _true_type		has_trivial_default_constructor;
			typedef _true_type		has_trivial_copy_constructor;
			typedef _true_type		has_trivial_assignment_operator;
			typedef _true_type		has_trivial_destructor;
			typedef _true_type		is_POD_type;
		};
		template<>
		struct _type_traits<const char*>
		{
			typedef _true_type		has_trivial_default_constructor;
			typedef _true_type		has_trivial_copy_constructor;
			typedef _true_type		has_trivial_assignment_operator;
			typedef _true_type		has_trivial_destructor;
			typedef _true_type		is_POD_type;
		};
		template<>
		struct _type_traits<const unsigned char*>
		{
			typedef _true_type		has_trivial_default_constructor;
			typedef _true_type		has_trivial_copy_constructor;
			typedef _true_type		has_trivial_assignment_operator;
			typedef _true_type		has_trivial_destructor;
			typedef _true_type		is_POD_type;
		};
		template<>
		struct _type_traits<const signed char*>
		{
			typedef _true_type		has_trivial_default_constructor;
			typedef _true_type		has_trivial_copy_constructor;
			typedef _true_type		has_trivial_assignment_operator;
			typedef _true_type		has_trivial_destructor;
			typedef _true_type		is_POD_type;
		};
    }
}


#endif //TINYSTL_TYPETRAITS_H
