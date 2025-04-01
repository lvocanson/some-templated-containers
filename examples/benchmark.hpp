#include <algorithm>
#include <chrono>
#include <format>
#include <iostream>
#include <string_view>
#include <vector>

template <typename T>
concept callable_size_param = requires(T t, size_t i) { t(i); };

template <typename T>
concept callable_no_param = requires(T t) { t(); };

template <typename T>
concept callable = callable_no_param<T> || callable_size_param<T>;

class benchmark
{
public:

	/**
	 * Execute a callable object a specific number of times and returns the time it took.
	 *
	 * @param c The callable object: function pointer, functor, lambda...
	 * @param iterations The number of times to execute the callable. It defines how many times `c` will be invoked.
	 * @return The time it took to complete `iterations` executions of `c`, in nanoseconds.
	 */
	static std::chrono::nanoseconds execute(callable auto c, size_t iterations)
	{
		auto start = std::chrono::steady_clock::now();
		for (size_t i = 0; i < iterations; ++i)
		{
			if constexpr (callable_size_param<decltype(c)>) c(i);
			else c();
		}
		auto end = std::chrono::steady_clock::now();
		return end - start;
	}

	/**
	 * Execute a callable object repeatedly until a specified time limit is reached and returns the number of executions.
	 *
	 * @param c The callable object: function pointer, functor, lambda...
	 * @param time_limit The maximum amount of time allowed for executions of `c`. The function will stop executing once this time limit is reached.
	 * @return The number of executions of `c` that were performed before the `time_limit` was reached.
	 */
	static size_t execute(callable auto c, std::chrono::nanoseconds time_limit)
	{
		auto start = std::chrono::steady_clock::now();
		for (size_t i = 0; i < std::numeric_limits<size_t>::max(); ++i)
		{
			if constexpr (callable_size_param<decltype(c)>) c(i);
			else c();
			auto now = std::chrono::steady_clock::now();
			if (now - start >= time_limit) return i;
		}
		return std::numeric_limits<size_t>::max();
	}

public:

	/**
	 * A structure that stores the benchmark results for a callable.
	 *
	 * @param name The name of the callable function.
	 * @param time The total time taken for the benchmark in nanoseconds.
	 * @param iterations The number of iterations (or executions) performed.
	 */
	struct result
	{
		std::string_view name;
		std::chrono::nanoseconds time;
		size_t iterations;
	};

	/**
	 * Constructor that initializes the benchmark with a specific number of iterations.
	 *
	 * @param iterations The number of times the callable should be executed in the benchmark.
	 * @throws std::invalid_argument if the number of iterations is zero.
	 */
	benchmark(size_t iterations = 1)
		: iterations_(iterations)
	{
		if (iterations == 0)
			throw std::invalid_argument("Benchmark must have at least one iteration");
	}

	/**
	 * Constructor that initializes the benchmark with a specific time limit.
	 *
	 * @param time_limit The time limit for executing the callable function.
	 */
	benchmark(std::chrono::nanoseconds time_limit)
		: time_limit_(time_limit)
	{
	}

	/**
	 * Adds a callable to the benchmark and runs the specified benchmark using either
	 * the number of iterations or the time limit. Results are stored internally.
	 *
	 * @param name The name of the callable function for identification.
	 * @param c The callable object: function pointer, functor, lambda...
	 * @return A reference to the current benchmark object.
	 */
	benchmark& add(std::string_view name, callable auto&& c)
	{
		if (iterations_)
		{
			auto time = execute(c, iterations_);
			results_.emplace_back(name, time, iterations_);
		}
		else
		{
			auto iterations = execute(c, time_limit_);
			results_.emplace_back(name, time_limit_, iterations);
		}

		return *this;
	}

	/**
	 * Prints the results of the benchmark to an output stream.
	 * The results are sorted based on the time or the number of iterations, depending on the benchmark type.
	 *
	 * @param output The output stream to print the results to.
	 * @param col_width The width for each column of the result table.
	 * @return A reference to the current benchmark object.
	 */
	benchmark& print_results(std::ostream& output = std::cout, size_t col_width = 13)
	{
		using namespace std;
		size_t title_width = col_width;

		sort(results_.begin(), results_.end(), [&](const result& a, const result& b)
		{
			title_width = max({a.name.size(), b.name.size(), title_width});
			return (iterations_)
				? a.time < b.time
				: a.iterations > b.iterations;
		});

		output << format("{0:<{4}}{1:>{5}}{2:>{5}}{3:>{5}}\n", "Function", (iterations_ ? "Total Time" : "Iterations"), "Avg Time", "Efficiency", title_width, col_width)
			<< string(title_width + 3 * col_width, '-') << '\n';

		for (auto& [name, time, iterations] : results_)
		{
			using nano = std::chrono::nanoseconds;
			nano average = iterations ? nano(time / iterations) : nano();
			auto formatted_time = [&](nano time) -> string
			{
				auto ns = time.count();
				if (ns >= 1e9) return format("{:>{}.2f} s ", ns / 1e9, col_width - 3);
				else if (ns >= 1e6) return format("{:>{}.2f} ms", ns / 1e6, col_width - 3);
				else if (ns >= 1e3) return format("{:>{}.2f} us", ns / 1e3, col_width - 3);
				else return format("{:>{}} ns", ns, col_width - 3);
			};

			output << format("{:<{}}", name, title_width);
			if (iterations_)
			{
				auto efficiency = 100. * results_.front().time.count() / time.count();
				output << formatted_time(time) << formatted_time(average) << format("{:>{}.3} %\n", efficiency, col_width - 2);
			}
			else
			{
				auto efficiency = 100. * iterations / results_.front().iterations;
				output << format("{:>{}}", iterations, col_width) << formatted_time(average) << format("{:>{}.3} %\n", efficiency, col_width - 2);
			}
		}

		return *this;
	}

	/**
	 * Gets the list of benchmark results stored in the object.
	 * The results may not be sorted.
	 *
	 * @return A constant reference to the vector of benchmark results.
	 */
	const std::vector<result>& get_results() const { return results_; }

private:

	size_t iterations_{};
	std::chrono::nanoseconds time_limit_{};
	std::vector<result> results_;
};
