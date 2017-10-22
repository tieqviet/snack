#include "spis.h"
#include "stokenizer.h"
#include "spreprocessor.h"

int main() {
	std::ifstream stream("script2.sn");
	
	if (stream.fail()) {
		std::cout << "FAILED!!" << std::endl;
	}

	snack::spis spiss(&stream, "script2.sn");
	snack::tokenizer spist(spiss);

	snack::spreprocessor sproc(spist);
	bool res = sproc.process();
	
	std::ofstream ostream("script2_process.sn");
	
	ostream << sproc.get_stream().data();


	/*
	snack::token tok = spist.read_next();
	snack::token tok2 = spist.read_next();
	snack::token tok3 = spist.read_next();
	snack::token tok4 = spist.read_next();
	snack::token tok5 = spist.read_next();
	snack::token tok6 = spist.read_next();

	snack::token tok1 = spist.read_next();
	snack::token tok21 = spist.read_next();
	snack::token tok31 = spist.read_next();
	snack::token tok41 = spist.read_next();
	snack::token tok51 = spist.read_next();
	snack::token tok61 = spist.read_next();
	*/

	return 0;
}