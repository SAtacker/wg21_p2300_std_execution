/*
 * Copyright (c) Shreyas Atre
 *
 * Licensed under the Apache License Version 2.0 with LLVM Exceptions
 * (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 *
 *   https://llvm.org/LICENSE.txt
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <catch2/catch.hpp>
#include <execution.hpp>
#include <test_common/receivers.hpp>
#include <test_common/type_helpers.hpp>
#include <coroutine>

namespace ex = std::execution;

struct promise : ex::with_awaitable_senders<promise> {
  std::coroutine_handle<promise> get_return_object() {
    return {std::coroutine_handle<promise>::from_promise(*this)};
  }
  std::suspend_always initial_suspend() noexcept { return {}; }
  std::suspend_always final_suspend() noexcept { return {}; }
  void return_void() {}
  void unhandled_exception() {}
};

struct awaiter {
  bool await_ready() { return false; }
  void await_suspend(std::coroutine_handle<promise>) {}
  void await_resume() {}
};

struct oper {
  oper() = default;
  oper(oper&&) = delete;
  friend void tag_invoke(ex::start_t, oper&) noexcept {}
};

struct my_sender0 {
  using completion_signatures = ex::completion_signatures< //
      ex::set_value_t(),                                   //
      ex::set_error_t(std::exception_ptr),                 //
      ex::set_stopped_t()>;

  friend oper tag_invoke(ex::connect_t, my_sender0, empty_recv::recv0&& r) { return {}; }
};

TEST_CASE("get_awaiter returns awaitable", "[concepts][coroutine_utils]") {
  auto res = ex::__get_awaiter(awaiter{}, nullptr);
  REQUIRE(ex::__awaiter<decltype(res)>);
  REQUIRE(ex::__awaitable<decltype(res)>);
}