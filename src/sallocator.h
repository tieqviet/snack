#pragma once

#include "sttype.h"

namespace snack {

	namespace itf {

		template <typename T>
		class sallocator_interface {
		public:
			using pointer = T*;

			virtual sallocator_interface() = 0;

			virtual pointer alloc(u32 p_num, u32 p_size) = 0;
			virtual void	free(pointer p_pointer) = 0;
		};

	}
	
	namespace memory {
		template <typename T>
		class sdefault_allocator : public snack::itf::sallocator_interface<T> {
			using pointer = T*;
			
			u32 allocated_mem;
			u32 allocated_instance;
			
		public:
			sdefault_allocator() : allocated_mem(0), allocated_instance(0) {};

			pointer alloc(u32 p_num, u32 p_size) {
				pointer t_pointee = reinterpret_cast<pointer>(malloc(p_num * sizeof(T));

				allocated_mem += p_num * sizeof(T);
				allocated_instance += p_num;

				return t_pointee;
			};

			void  free(T* p_pointer) {
				free(reinterpret_cast<void*>(p_pointer));

				allocated_instance -= 1;
				allocated_mem -= sizeof(T);
			}
		};

	}
}