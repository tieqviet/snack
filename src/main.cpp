#include "spis.h"
#include "stokenizer.h"
#include "spp.h"
#include "stype.h"

std::ifstream stream;
std::ofstream ostream;

std::vector<const char*> args_parsed;

snack::spis spiss;
snack::tokenizer spist;
snack::spp sppi;
//snack::spreprocessor sproc;

bool process_spec = false;

int main(int argc,char** argv) {
	std::cout << "SNACK COMPLIER" << std::endl;
	std::cout << "(C) 2017 - present Trong Thu Do" << std::endl;

	if ((argv[argc - 1] == "") || (*argv[argc - 1] == '-') || (argc == 1)) {
		std::cout << "File not specified. Try again!!" << std::endl;
		return 0;
	}

	stream.open(argv[argc - 1]);

	if (stream.fail() || stream.bad()) {
		std::cout << "File has bad bit or does not exists. Try again :)" << std::endl;
		return 0;
	}

	spiss = snack::spis(&stream, std::string(argv[argc - 1]));
	spist = snack::tokenizer(spiss);

	spist.optional_tokens_set_visibility(true);

	sppi.lexer = &spist;

	snack::token tok = spist.read_next();
	sppi._dir_par(tok);
	tok = spist.read_next();
	sppi._dir_par(tok);
	tok = spist.read_next();
	tok = sppi._expd(tok);
	sppi._dir_par(tok);
}

/*
void parse_argument(const char* arg) {
	if ((strcmp(arg, "-e") == 0)) {
		
		if (!process_spec) {
			sproc.process();

			process_spec = true;

		}
		
		std::cout << sproc.get_stream().data() << std::endl;
		return;
	}
	else if ((strcmp(arg, "-pe") == 0)) {
		if (!process_spec) {
			sproc.process();

			process_spec = true;
		}
	}
	else if ((strcmp(arg, "-h") == 0)) {
		std::cout << "-e: Do processor work and print processed work" << std::endl;
	}
	else
		if (args_parsed.size() > 0) {
			if (strcmp(args_parsed[args_parsed.size() - 1], "-pe") == 0) {
				ostream = std::ofstream(arg);
				std::string str = sproc.get_stream().data();
				const char* buf = str.data();

				ostream.write(buf, strlen(buf));
			}
			else {
				std::cout << "Invalid argument: " << arg << std::endl;
			}
		}
		
	args_parsed.push_back(arg);
}

int main(int argc, char** argv) {
	std::cout << "SNACK COMPLIER" << std::endl;
	std::cout << "(C) 2017 - present Trong Thu Do" << std::endl;

	if ((argv[argc - 1] == "") || (*argv[argc - 1] == '-') || (argc == 1)) {
		std::cout << "File not specified. Try again!!" << std::endl;
		return 0;
	}

	stream.open(argv[argc - 1]);
	
	if (stream.fail() || stream.bad()) {
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

	//std::getchar();

	return 1;
}*/