#include "inline_scheduler.hpp"
#include <iostream>
#include <typeinfo>

int main() {
  using example::inline_scheduler;
  struct inline_scheduler s{};
  auto k = std::execution::get_forward_progress_guarantee(std::ref(s));
  std::cout<< typeid(k).name()<<"\n";
  static_assert(k==std::execution::forward_progress_guarantee::weakly_parallel);
}
