#include "result.h"

bool result_ = true;

bool result() { return result_; }

void andResult(bool latestResultFactor) { result_ = result_ & latestResultFactor; }