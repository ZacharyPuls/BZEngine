
#include "targetver.h"

#include "BZApp.h"

int main()
{
	auto bzApp = new BZApp("Test");
	bzApp->Start();
	delete bzApp;
	return 0;
}
