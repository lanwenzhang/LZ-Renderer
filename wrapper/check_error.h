#pragma once

#ifdef DEBUG
#define GL_CALL(func) \
    do {              \
        func;         \
        lzgl::wrapper::checkError(); \
    } while (0)
#else
#define GL_CALL(func) \
    do {              \
        func;         \
    } while (0)
#endif

namespace lzgl::wrapper {
	void checkError();
}

