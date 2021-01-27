#include "gtest/gtest.h"
#include "../../include/concurrent_queue.h"
#include <algorithm>
#include <iostream>

TEST(TestConcurrentQueue, SizeAndClear) {
    ConcurrentQueue<int> queue{};
    const int n = 10;

    auto producer = [&queue, n]() {
        for (int i = 1; i <= n; ++i) {
            queue.push(i);
        }
    };

    std::thread producer_thread = std::thread{producer};

    producer_thread.join();

    ASSERT_EQ(queue.size(), n);
    ASSERT_FALSE(queue.empty());

    std::cout << "Queue is not empty.\n";

    queue.clear();

    ASSERT_EQ(queue.size(), 0);
    ASSERT_TRUE(queue.empty());

    std::cout << "Queue is empty now.\n";
}

TEST(TestConcurrentQueue, SumTryPop) {
    ConcurrentQueue<int> queue{};
    const int n = 10;
    const int expected_sum = n * (n + 1) / 2;
    int sum = 0;

    auto producer = [&queue, n]() {
        for (int i = 1; i <= n; ++i) {
            queue.push(i);
        }
    };

    auto consumer = [&queue, &sum, n]() {
        int val = 0;

        for (int j = 1; j <= n; ++j) {
            bool res = queue.try_pop(val);
            if (res) {
                sum += val;
            } else {
                std::cerr << "\t-- failed to read " << j << "th value\n";
            }
        }
    };

    std::thread producer_thread = std::thread{producer};
    // give the producer thread a lead
    std::this_thread::sleep_for(std::chrono::milliseconds{5});
    std::thread consumer_thread = std::thread{consumer};

    producer_thread.join();
    consumer_thread.join();

    ASSERT_EQ(sum, expected_sum);

    std::cout << "Sum of numbers read between [1," << n << "] is " << sum << ".\n";

    ASSERT_TRUE(queue.empty());
}

TEST(TestConcurrentQueue, SumWaitAndPop1) {
    ConcurrentQueue<int> queue{};
    const std::chrono::milliseconds delay{50};
    const int n = 10;
    const int expected_sum = n * (n + 1) / 2;
    int sum = 0;

    auto producer = [&queue, &delay, n]() {
        for (int i = 1; i <= n; ++i) {
            queue.push(i);
            std::this_thread::sleep_for(i * delay);
        }
    };

    auto consumer = [&queue, &sum, n]() {
        int val = 0;

        for (int j = 1; j <= n; ++j) {
            queue.wait_and_pop(val);
            sum += val;
        }
    };

    std::thread producer_thread = std::thread{producer};
    std::thread consumer_thread = std::thread{consumer};

    producer_thread.join();
    consumer_thread.join();

    ASSERT_EQ(sum, expected_sum);

    std::cout << "Sum of numbers read between [1," << n << "] is " << sum << ".\n";

    ASSERT_TRUE(queue.empty());
}

TEST(TestConcurrentQueue, SumWaitAndPop2) {
    ConcurrentQueue<int> queue{};
    const std::array<std::chrono::milliseconds, 3> delays{std::chrono::milliseconds{100},
                                                          std::chrono::milliseconds{200},
                                                          std::chrono::milliseconds{300}};
    const int n = 10;
    int sum = 0;
    int expected_sum = n * (n + 1) / 2;
    int k = 0;
    std::vector<std::thread> producer_threads{};

    for (int i = 0; i < delays.size(); ++i) {
        auto producer = [&queue, &delays, n, i]() {
            for (int j = 1; j <= n; ++j) {
                queue.push(j);
                std::this_thread::sleep_for(delays[i]);
            }
        };

        producer_threads.push_back(std::thread{producer});
    }

    expected_sum *= delays.size();

    auto consumer = [&queue, &sum, n, expected_sum]() {
        int val = 0;

        // consume fully
        while (sum < expected_sum) {
            queue.wait_and_pop(val);
            sum += val;
        }
    };

    std::thread consumer_thread = std::thread{consumer};

    for (int i = 0; i < delays.size(); ++i) {
        producer_threads[i].join();
    }

    consumer_thread.join();

    ASSERT_EQ(sum, expected_sum);

    std::cout << "Sum of numbers read between [1," << n << "] * " << delays.size() << " is " << sum << ".\n";

    ASSERT_TRUE(queue.empty());
}

TEST(TestConcurrentQueue, SumWaitAndPop3) {
    ConcurrentQueue<int> queue{};
    const std::array<std::chrono::milliseconds, 3> delays{std::chrono::milliseconds{100},
                                                          std::chrono::milliseconds{200},
                                                          std::chrono::milliseconds{300}};
    const int n = 10;
    const int expected_sum = 100;
    int sum = 0;
    int k = 0;
    std::vector<std::thread> producer_threads{};
    bool loop = true;

    for (int i = 0; i < delays.size(); ++i) {
        auto producer = [&queue, &delays, i](bool& loop) {
            int s = 0;
            while (loop) {
                ++s;
                queue.push(1);
                std::this_thread::sleep_for(delays[i]);
            }

            std::cout << "\t++ detached thread with delay " << delays[i].count()
                      << "ms pushed " << s << " times\n";
        };

        producer_threads.push_back(std::thread{producer, std::ref(loop)});
    }

    auto consumer = [&queue, &sum, n, expected_sum]() {
        int val = 0;

        // consume fully
        while (sum < expected_sum) {
            queue.wait_and_pop(val);
            sum += val;
        }
    };

    std::thread consumer_thread = std::thread{consumer};

    for (int i = 0; i < delays.size(); ++i) {
        producer_threads[i].detach();
    }

    consumer_thread.join();

    ASSERT_EQ(sum, expected_sum);

    loop = false;

    // wait for IO
    std::this_thread::sleep_for(std::chrono::milliseconds{500});

    std::cout << "Three detached threads pushed in total " << sum << " times.\n";

    ASSERT_TRUE(queue.empty());
}

TEST(TestConcurrentQueue, SumWaitAndPopWhile) {
    ConcurrentQueue<int> queue{};
    const std::chrono::milliseconds timeout{20};
    const std::chrono::milliseconds check{5};
    const std::chrono::milliseconds delay{5};
    const int n = 10;
    const int expected_sum = n * (n + 1) / 2;
    int sum = 0;

    auto producer = [&queue, &delay, n]() {
        for (int i = 1; i <= n; ++i) {
            queue.push(i);
            std::this_thread::sleep_for(delay);
        }
    };

    auto consumer = [&queue, &timeout, &check, &sum, n]() {
        int val = 0;

        for (int j = 1; j <= n; ++j) {
            bool res = queue.wait_and_pop_while(val, timeout, check);
            if (res) {
                sum += val;
            } else {
                std::cerr << "\t-- failed to read " << j << "th value\n";
            }
        }
    };

    std::thread producer_thread = std::thread{producer};
    std::thread consumer_thread = std::thread{consumer};

    producer_thread.join();
    consumer_thread.join();

    ASSERT_EQ(sum, expected_sum);

    std::cout << "Sum of numbers read between [1," << n << "] is " << sum << ".\n";

    ASSERT_TRUE(queue.empty());
}

TEST(TestConcurrentQueue, SumWaitAndPopWhileWithTimeout1) {
    ConcurrentQueue<int> queue{};
    const std::chrono::milliseconds timeout{200};
    const std::chrono::milliseconds check{5};
    const std::chrono::milliseconds delay{50};
    const int n = 10;
    const int expected_sum = n * (n + 1) / 2;
    int sum = 0;

    auto producer = [&queue, &delay, n]() {
        for (int i = 1; i <= n; ++i) {
            queue.push(i);
            std::this_thread::sleep_for(i * delay); // increasing delay
        }
    };

    auto consumer = [&queue, &timeout, &check, &sum, n]() {
        int val = 0;

        for (int j = 1; j <= n; ++j) {
            bool res = queue.wait_and_pop_while(val, timeout, check);
            if (res) {
                sum += val;
            } else {
                std::cerr << "\t-- failed to read value at " << j << "th attempt\n";
            }
        }
    };

    std::thread producer_thread = std::thread{producer};
    std::thread consumer_thread = std::thread{consumer};

    producer_thread.join();
    consumer_thread.join();

    ASSERT_NE(sum, expected_sum);

    std::cout << "Sum of numbers read between [1," << n << "] is " << sum << ", not " << expected_sum << ".\n";

    ASSERT_FALSE(queue.empty());

    std::size_t q_size = queue.size();
    std::cout << "Size of the queue is " << q_size << ".\n";

    ASSERT_TRUE(q_size > 0);
}

TEST(TestConcurrentQueue, SumWaitAndPopWhileWithTimeout2) {
    ConcurrentQueue<int> queue{};
    const std::chrono::milliseconds timeout{200};
    const std::chrono::milliseconds check{5};
    const std::chrono::milliseconds delay{50};
    const int n = 10;
    const int expected_sum = n * (n + 1) / 2;
    int sum = 0;
    int j = 0;

    auto producer = [&queue, &delay, n]() {
        for (int i = 1; i <= n; ++i) {
            queue.push(i);
            std::this_thread::sleep_for(i * delay); // increasing delay
        }
    };

    auto consumer = [&queue, &timeout, &check, &sum, &j, expected_sum]() {
        int val = 0;

        // consume fully
        while (sum < expected_sum) {
            ++j;
            bool res = queue.wait_and_pop_while(val, timeout, check);
            if (res) {
                sum += val;
            } else {
                std::cerr << "\t-- failed to read value at " << j << "th attempt\n";
            }
        }
    };

    std::thread producer_thread = std::thread{producer};
    std::thread consumer_thread = std::thread{consumer};

    producer_thread.join();
    consumer_thread.join();

    std::cout << "Read in " << j << " attempts.\n";

    ASSERT_EQ(sum, expected_sum);

    std::cout << "Sum of numbers read between [1," << n << "] is " << sum << ".\n";

    ASSERT_TRUE(queue.empty());
}

TEST(TestConcurrentQueue, SumWaitAndPopWhileWithTimeout3) {
    ConcurrentQueue<int> queue{};
    const std::chrono::milliseconds timeout{300};
    const std::chrono::milliseconds check{10};
    const std::array<std::chrono::milliseconds, 3> delays{std::chrono::milliseconds{100},
                                                          std::chrono::milliseconds{200},
                                                          std::chrono::milliseconds{900}};
    const int n = 10;
    int sum = 0;
    int expected_sum = n * (n + 1) / 2;
    int k = 0;
    std::vector<std::thread> producer_threads{};

    for (int i = 0; i < delays.size(); ++i) {
        auto producer = [&queue, &delays, n, i]() {
            for (int j = 1; j <= n; ++j) {
                queue.push(j);
                std::this_thread::sleep_for(delays[i]);
            }
        };

        producer_threads.push_back(std::thread{producer});
    }

    expected_sum *= delays.size();

    auto consumer = [&queue, &timeout, &check, &sum, &k, expected_sum]() {
        int val = 0;

        // consume fully
        while (sum < expected_sum) {
            ++k;
            bool res = queue.wait_and_pop_while(val, timeout, check);
            if (res) {
                sum += val;
            } else {
                std::cerr << "\t-- failed to read value at " << k << "th attempt\n";
            }
        }
    };

    std::thread consumer_thread = std::thread{consumer};

    for (int i = 0; i < delays.size(); ++i) {
        producer_threads[i].join();
    }

    consumer_thread.join();

    std::cout << "Read in " << k << " attempts.\n";

    ASSERT_EQ(sum, expected_sum);

    std::cout << "Sum of numbers read between [1," << n << "] * " << delays.size() << " is " << sum << ".\n";

    ASSERT_TRUE(queue.empty());
}

TEST(TestConcurrentQueue, SumWaitAndPopWhileWithTimeout4) {
    ConcurrentQueue<int> queue{};
    const std::chrono::milliseconds timeout{150};
    const std::chrono::milliseconds check{10};
    const std::array<std::chrono::milliseconds, 3> delays{std::chrono::milliseconds{100},
                                                          std::chrono::milliseconds{200},
                                                          std::chrono::milliseconds{500}};
    const int n = 10;
    const int expected_sum = 100;
    int sum = 0;
    int k = 0;
    std::vector<std::thread> producer_threads{};
    bool loop = true;

    for (int i = 0; i < delays.size(); ++i) {
        auto producer = [&queue, &delays, i](bool& loop) {
            int s = 0;
            while (loop) {
                ++s;
                queue.push(1);
                std::this_thread::sleep_for(delays[i]);
            }

            std::cout << "\t++ detached thread with delay " << delays[i].count()
                      << "ms pushed " << s << " times\n";
        };

        producer_threads.push_back(std::thread{producer, std::ref(loop)});
    }

    auto consumer = [&queue, &timeout, &check, &sum, &k, expected_sum]() {
        int val = 0;

        // consume fully
        while (sum < expected_sum) {
            ++k;
            bool res = queue.wait_and_pop_while(val, timeout, check);
            if (res) {
                sum += val;
            } else {
                std::cerr << "\t-- failed to read value at " << k << "th attempt\n";
            }
        }
    };

    std::thread consumer_thread = std::thread{consumer};

    for (int i = 0; i < delays.size(); ++i) {
        producer_threads[i].detach();
    }

    consumer_thread.join();

    ASSERT_EQ(sum, expected_sum);

    loop = false;

    auto max_delay = std::max_element(delays.begin(), delays.end());
    // wait for IO and timeouts
    std::this_thread::sleep_for(*max_delay + timeout + check);

    std::cout << "Three detached threads pushed in total " << sum << " times.\n";

    ASSERT_TRUE(queue.empty());
}
