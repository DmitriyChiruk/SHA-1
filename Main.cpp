#include <iostream>
#include <boost/compute/detail/sha1.hpp>

#include "SHA1.h"

using namespace std;
using namespace boost::uuids::detail;


void PrintSha1(sha1& Sha1) 
{
	unsigned int Hash[5];
	Sha1.get_digest(Hash);
	
	cout << "0x";
	for (size_t i = 0; i < sizeof(Hash) / sizeof(Hash[0]); ++i) {
		cout << hex << Hash[i];
	}
	cout << dec << std::endl;
}


int main()
{
	vector<string> Dict;
	Dict.push_back("Hello world!");
	Dict.push_back("hello from a very long text, but it's not so long to be honest)))");
	Dict.push_back("hello from a very long text, but it's not so long to be honest))) So that's why let's add some more unnecessary words like bla bla bla ...");
	Dict.push_back(string("To be, or not to be, that is the question:" )+ 
						string("Whether 'tis nobler in the mind to suffer") +
						string("The slings and arrows of outrageous fortune,") +
						string("Or to take arms against a sea of troubles") +
						string("And by opposing end them.To die—to sleep,") +
						string("No more; and by a sleep to say we end") +
						string("The heart - ache and the thousand natural shocks") +
						string("That flesh is heir to : 'tis a consummation") +
						string("Devoutly to be wish'd. To die, to sleep;") +
						string("To sleep, perchance to dream—ay, there's the rub:") +
						string("For in that sleep of death what dreams may come,") +
						string("When we have shuffled off this mortal coil,") +
						string("Must give us pause—there's the respect") +
						string("That makes calamity of so long life.") +
						string("For who would bear the whips and scorns of time,") +
						string("Th'oppressor's wrong, the proud man's contumely,") +
						string("The pangs of dispriz'd love, the law's delay,") +
						string("The insolence of office, and the spurns") +
						string("That patient merit of th'unworthy takes,") +
						string("When he himself might his quietus make") +
						string("With a bare bodkin ? Who would fardels bear,") +
						string("To grunt and sweat under a weary life,") +
						string("But that the dread of something after death,") +
						string("The undiscovere'd country, from whose bourn") +
						string("No traveller returns, puzzles the will"));

	/*
	vector<bool> input(32);
	input[2] = 1;
	input[10] = 1;
	input[13] = 1;
	input[18] = 1;
	input[21] = 1;
	input[22] = 1;
	input[24] = 1;
	input[26] = 1;
	input[27] = 1;
	input[31] = 1;

	cout << VecToUInt32(input);

	*/
	
	// Test SHA1:
	for (auto sentence : Dict)
	{
		vector<bool> Test = StrToVec(sentence);
		vector<bool> TestHash = GetHash(Test);

		// PrintVector(Test);
		// PrintHexVector(Test);
		// cout << endl << endl;


		// cout << endl << endl;
		// PrintVector(TestHash);
		// cout << Test.size() << " bits" << endl;
		cout << "Ours:    ";
		PrintHexVector(TestHash);

		sha1 Answer;
		Answer.process_bytes(sentence.data(), sentence.size());
		cout << "Correct: ";
		PrintSha1(Answer);

		cout << endl;
	}

	return 0;
}