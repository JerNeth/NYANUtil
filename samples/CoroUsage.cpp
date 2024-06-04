#include <string>
#include <string_view>
#include <coroutine>
#include <iostream>

import NYANCoro;



struct RetType {
	struct promise_type {
		std::string testIn;
		std::string testOut;

		void unhandled_exception() noexcept {
			std::cout << "unhandled exception\n";
		}
		RetType get_return_object() noexcept
		{
			std::cout << "Coro creation\n";
			return RetType{ this };
		}
		std::suspend_always initial_suspend() noexcept
		{
			std::cout << "initial_suspend\n";
			return {};
		}

		std::suspend_always yield_value(std::string test) noexcept
		{
			std::cout << "yield_value\n";
			testOut = std::move(test);
			return {};
		}

		std::suspend_always yield_value(int a) noexcept
		{
			std::cout << "yield_value (int)\n";
			testOut = std::to_string(a) + "\n";
			return {};
		}

		auto await_transform(std::string test) noexcept
		{
			struct awaiter {
				promise_type& pt;
				bool await_ready() const noexcept {
					std::cout << "await_ready\n";
					return false;
				}
				std::string await_resume() const noexcept {
					std::cout << "await_resume\n";
					return std::move(pt.testIn);
				}
				void await_suspend(std::coroutine_handle<>) const noexcept {
					std::cout << "await_suspend\n";
				}
			};
			std::cout << "await_transform: " << test;
			return awaiter{ *this };
		}
		void return_value(std::string msg) noexcept {
			std::cout << "return_value\n";
			testOut = std::move(msg);
		}

		//void return_void() noexcept 
		//{
		//	std::cout << "return_void\n";
		//}

		std::suspend_always final_suspend() noexcept {
			std::cout << "final_suspend\n";
			return {};
		}

	};
	using CoroHandle = std::coroutine_handle<promise_type>;
	CoroHandle mCoroHdl{};
	explicit RetType(promise_type* p) noexcept : mCoroHdl{ CoroHandle::from_promise(*p) } {

	}
	RetType(RetType&& rhs) noexcept :
		mCoroHdl{std::exchange(rhs.mCoroHdl, nullptr)}
	{

	}
	~RetType() noexcept 
	{
		if (mCoroHdl)
			mCoroHdl.destroy();
	}
	std::string test_out() noexcept
	{
		if (not mCoroHdl.done())
			mCoroHdl.resume();
		return std::move(mCoroHdl.promise().testOut);
	}
	void test_in(std::string msg)  noexcept
	{
		mCoroHdl.promise().testIn = std::move(msg);
		if (not mCoroHdl.done())
			mCoroHdl.resume();
	}

};

RetType testFun() noexcept
{
	co_yield "Yield value\n";
	co_yield 5;

	std::cout << co_await "in coro await\n";

	throw "fun";

	co_return "Return value\n";
}

int main()
{
	std::cout << "before test_fun\n";
	auto test = testFun();
	std::cout << "after test_fun\n";

	std::cout << test.test_out();
	std::cout << test.test_out();

	test.test_in("await\n");

	std::cout << test.test_out();

	return 0;
}

//before test_fun					before test_fun
//	Coro creation						Coro creation
//		initial_suspend						initial_suspend
//		yield_value							after test_fun
//after test_fun							yield_value
//	await_transform : in coro await				Yield value
//	await_ready							await_transform: in coro await
//	await_suspend						await_ready
//Yield value						await_suspend
//	await_resume						await_resume
//	await								await
//return_value						return_value
//final_suspend						final_suspend
//Return value						Return value
