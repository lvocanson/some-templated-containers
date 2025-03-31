#include <iostream>
#include <chrono>
#include <vector>
#include <string_view>
#include <algorithm>
#include <format>

template <typename T>
concept callable = requires(T t) { t(); };

struct time_result
{
    double time;
    std::string_view unit;

    explicit time_result(std::chrono::nanoseconds ns)
    {
        auto t = ns.count();
        if (t >= 1e9) { time = t / 1e9; unit = "s"; }
        else if (t >= 1e6) { time = t / 1e6; unit = "ms"; }
        else if (t >= 1e3) { time = t / 1e3; unit = "us"; }
        else { time = double(t); unit = "ns"; }
    }
};

template <callable F>
void benchmark(std::string_view name, F func, size_t iterations)
{
    using namespace std;
    using namespace chrono;

    auto start = high_resolution_clock::now();
    for (size_t i = 0; i < iterations; ++i) func();
    auto end = high_resolution_clock::now();

    auto total_time = duration_cast<nanoseconds>(end - start);
    auto avg_time = total_time / iterations;

    auto total = time_result(total_time);
    auto avg = time_result(avg_time);

    cout << format("{:<20} {:>13} {} {:>13} {}\n", name, total.time, total.unit, avg.time, avg.unit);
}

template <typename... Fns>
void compare_benchmarks(size_t iterations, std::pair<std::string_view, Fns>... functions)
{
    using namespace std;
    using namespace chrono;

    vector<pair<string, nanoseconds>> results;

    (results.emplace_back(functions.first, [&]
    {
        auto start = high_resolution_clock::now();
        for (size_t i = 0; i < iterations; ++i) functions.second();
        auto end = high_resolution_clock::now();
        return duration_cast<nanoseconds>(end - start);
    }()), ...);

    constexpr size_t col_size = 13;
    size_t title_size = col_size;

    sort(results.begin(), results.end(), [&](const auto& a, const auto& b)
    {
        title_size = max(a.first.size(), title_size);
        return a.second < b.second;
    });

    auto fastest_time = results.front().second;
    auto fastest_avg = fastest_time / iterations;

    cout << left << setw(title_size) << "Function"
        << right << setw(col_size) << "Total Time"
        << right << setw(col_size) << "Avg Time"
        << right << setw(col_size) << "Efficiency"
        << "\n";
    cout << string(title_size + 3 * col_size, '-') << "\n";

    for (const auto& [name, time] : results)
    {
        auto total = time_result(time);
        auto average = time_result(time / iterations);
        double efficiency = 100. * fastest_time.count() / time.count();

        cout << left << setw(title_size) << name
            << right << setw(col_size) << std::format("{} {}", total.time, total.unit)
            << right << setw(col_size) << std::format("{} {}", average.time, average.unit)
            << right << setw(col_size - 2) << efficiency << " %"
            << "\n";
    }
}
