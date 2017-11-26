set (SRCS 
		main.cpp
		sconst.cpp
		sparser.cpp
		spp.cpp
		spis.cpp
		sutils.cpp
		sttype.cpp
		ststruct.cpp
		stexp.cpp)

set (HEADERS
		serror.h
		sconst.h
		sparser.h
		spp.h
		stokenizer.h
		sallocator.h
		spis.h
		stype.h
		sast.h
		sutils.h
		scontext.h)

add_executable(snack ${SRCS} ${HEADERS})	