
#include <cstdint>
#include <fstream>
#include <vector>


extern "C" {


#define CVP(X) CoVeRaGePrOfIlInG_ ## X


extern uint64_t CVP(nblocks);

extern uint64_t CVP(nfuncs);


extern struct{
	uint64_t fId;
	bool visited;
} CVP(blockInfo)[];


extern char* CVP(funcStr)[];


// arguments are supplied during instrumentation
void
CVP(count) (uint64_t blockid)
{
	CVP(blockInfo)[blockid].visited = true;
}


void
CVP(print) (void)
{
	std::vector<std::pair<double,double> > coverage(CVP(nfuncs), {0, 0});
	for (uint64_t i = 0; i < CVP(nblocks); i++)
	{
		uint64_t f = CVP(blockInfo)[i].fId;
		if (CVP(blockInfo)[i].visited)
		{
			coverage[f].first++;
		}
		coverage[f].second++;
	}
	std::ofstream out("coverage-profile.txt");
	for (uint64_t i = 0; i < CVP(nfuncs); i++)
	{
		double percent_coverage = 100 * coverage[i].first / coverage[i].second;
		out << CVP(funcStr)[i] << ": " << percent_coverage << "%\n";
	}
}


struct CVP(printer)
{
	~CVP(printer) (void)
	{
		// rely on sanitizer to kill this, then we print
		CVP(print)();
	}

private:
	friend void CVP(start) (void);
	CVP(printer) (void) {}
};


void
CVP(start) (void)
{
	static CVP(printer) inst;
}


}

