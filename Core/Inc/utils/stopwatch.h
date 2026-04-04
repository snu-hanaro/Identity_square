#include "common.h"

class stopwatch_millis_t {
private:
    bool isset;
    uint32_t start_millis;
public:
    stopwatch_millis_t() : isset(false), start_millis(0) {}

    // 스톱워치 시작
    void set();
    // 스톱워치 비활성화
    void unset();
    bool is_set() const;
    uint32_t elapsed() const;
};
