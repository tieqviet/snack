#include "spis.h"
#include "stokenizer.h"
#include "spreprocessor.h"

std::ifstream stream;

snack::spis spiss;
snack::tokenizer spist;
snack::spreprocessor sproc;

bool process_spec = false;

void parse_argument(char* arg) {
	if (*arg != '-') {
		std::cout << "Invalid option: " << arg << std::endl;
	}

	if ((strcmp(arg, "-e") == 0) && !process_spec) {
		sproc.process();

		process_spec = true;

		std::cout << sproc.get_stream().data() << std::endl;
		return;
	}
}

int main(int argc, char** argv) {

	if ((argv[argc - 1] == "") || (*argv[argc - 1] == '-')) {
		std::cout << "File not specified. Try again!!" << std::endl;
	}

	stream.open(argv[argc - 1]);
	
	if (stream.fail()) {
		std::cout << "File has bad bit or does not exists. Try again :)" << std::endl;
		return 0;
	}


	spiss = snack::spis(&stream, std::string(argv[argc - 1]));
	spist = snack::tokenizer(spiss);

	sproc = snack::spreprocessor(spist);

	for (u32 i = 1; i < argc - 1; i++) {
		parse_argument(argv[i]);
	}

	if (!process_spec) {
		sproc.process();
	}

	spiss.set_pos(5);

	return 1;
}