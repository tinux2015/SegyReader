#include"SegyReader.h"
using namespace std;
int main(void)
{
	SegyReader segy("test_ieee.sgy");
	cout << "Sample_sum = " << segy.getSampleSum() << endl;
	segy.outputTD2Txt("test_ieee_1_6", 1,6);
	return 0;
}