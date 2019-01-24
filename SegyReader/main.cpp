#include"SegyReader.h"
using namespace std;
int main1(void)
{
	SegyReader segy("test.sgy");
	cout << "Sample_sum = " << segy.getSampleSum() << endl;
	segy.outputTD2Txt("test_1_6", 1,6);
	return 0;
}