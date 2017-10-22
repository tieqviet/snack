#include "stexp.h"

namespace snack {
	smem::smem() : data(0), dfunc(0) {};

	template <typename T>
	smem::smem(T* data, node_destroy_func dfunc) : data(data),
		dfunc(dfunc) {};

	void smem::destroy() {
		dfunc(data);
	}

	stexp::stexp(int type): type(type) {

	}
}