#include "renderinfo.h"
#include "timer.h"

ORION_NAMESPACE_BEGIN

void ProcessReporter::outputProgress()
{

	float spendTime = timer.getElaspedTime() / 1000.0f;
	static float lastSpendTime = spendTime, intervalTime = -1.0f;
	if (spendTime - lastSpendTime > intervalTime) {
		lock.lock();
		lastSpendTime = spendTime;
		if (spendTime < 10 && intervalTime != 0.2f)
			intervalTime = 0.2f;
		else if (spendTime < 20 && intervalTime != 0.4f)
			intervalTime = 0.4f;
		else if (spendTime < 30 && intervalTime != 0.8f)
			intervalTime = 0.8f;
		else
			intervalTime = 5.0f;

		float needTime = spendTime * totalWork / workDone - spendTime;
		// output progress
		int plussesPrinted = 0;

		// Initialize progress string
		const int bufLen = 128;
		std::unique_ptr<char[]> buf(new char[bufLen]);
		snprintf(buf.get(), bufLen, "\r%s: [", "Rendering");
		char *curSpace = buf.get() + strlen(buf.get());
		char *s = curSpace;
		for (int i = 0; i < 50; ++i)
			*s++ = ' ';
		*s++ = ']';
		*s++ = ' ';

		Float percentDone = Float(workDone) / Float(totalWork);
		int plussesNeeded = (int)std::round(50 * percentDone);
		while (plussesPrinted < plussesNeeded) {
			*curSpace++ = '+';
			++plussesPrinted;
		}

		snprintf(s, 20, " (%.1fs|%.1fs)  ", spendTime, needTime);
		fputs(buf.get(), stdout);
		fflush(stdout);
		lock.unlock();
	}
}

ORION_NAMESPACE_END
