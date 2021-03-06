#include "task.hh"

static_assert(sizeof(td::container::task) == td::system::l1_cacheline_size, "task exceeds cacheline size");
static_assert(std::is_trivial_v<td::container::task>, "task is not trivial");
