set (SRCS 
		main.cpp
		sconst.cpp
		sparser.cpp
		spreprocessor.cpp
		stokenizer.cpp
		spis.cpp
		sutils.cpp
		sttype.cpp
		ststruct.cpp
		stexp.cpp)

set (HEADERS
		serror.h
		sconst.h
		sparser.h
		spreprocessor.h
		stokenizer.h
		spis.h
		sutils.h
		sttype.h
		ststruct.h
		stexp.h
		stype.h
		stvar.h
		stfunc.h
		stbin.h
		stbuiltin.h
		stcall.h)

add_executable(snack ${SRCS} ${HEADERS})	