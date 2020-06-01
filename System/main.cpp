#include <AirConditionHost.h>

int main(int argc, char *argv[])
{
	AirConditionHost host(argc, argv);
	host.PowerOn();
	return 0;
}
