#include <string>
#include <iostream>
#include <cmath>
#include <vector>
using namespace std;


// for reverse iteration
template <typename T>
struct reversion_wrapper { T& iterable; };

template <typename T>
auto begin(reversion_wrapper<T> w) { return rbegin(w.iterable); }

template <typename T>
auto end(reversion_wrapper<T> w) { return rend(w.iterable); }

template <typename T>
auto reverse(T&& iterable) { return reversion_wrapper<T> { iterable }; }


vector<long> prime_sieve(long max_prime)
{
	vector<bool> is_prime(max_prime, true);

	long sqrt_max_prime = sqrt(max_prime);
	for (long to_test = 2; to_test <= sqrt_max_prime; ++to_test)
	{
		if (is_prime[to_test])
		{
			long next = to_test * 2;
			while (next <= max_prime)
			{
				is_prime[next] = false;
				next += to_test;
			}
		}
	}

	vector<long> primes;
	for (long i = 2; i <= max_prime; ++i)
	{
		if (is_prime[i])
		{
			primes.push_back(i);
		}
	}
	return primes;
}

vector<long> prime_sieve_sub(long max_prime, long begin, long end, const vector<long> prev_primes)
{
	vector<bool> is_prime(end - begin, true);

	for (long prev_prime: prev_primes)
	{
		long next = prev_prime * 2;
		while (next <= end)
		{
			is_prime[next - begin] = false;
			next += prev_prime;
		}
	}

	for (long to_test = begin; to_test <= end; ++to_test)
	{
		if (is_prime[to_test - begin])
		{
			long next = to_test * 2;
			while (next <= end)
			{
				is_prime[next - begin] = false;
				next += to_test;
			}
		}
	}

	vector<long> primes;
	for (long i = begin; i <= end; ++i)
	{
		if (is_prime[i - begin])
		{
			primes.push_back(i);
		}
	}
	return primes;
}

vector<long> prime_sieve_paged(long max_prime)
{
	const long page_size = 16384;
	//const long page_size = 16;
	long sqrt_max_prime = sqrt(max_prime);
	long begin = 2;
	long end = begin + page_size;
	vector<long> primes;
	while (begin < sqrt_max_prime)
	{
		vector<long> new_primes = prime_sieve_sub(max_prime, 2, sqrt_max_prime, primes);
		primes.insert(primes.end(), new_primes.begin(), new_primes.end());
		begin = end;
		end = min(sqrt_max_prime, end + page_size);
	}
	return primes;
}


long largest_prime_factor(long n)
{
	if (n <= 3) {
		return n;
	}

	long sqrtn = sqrt(n);
	vector<long> primes = prime_sieve_paged((n + 1) / 2);
	//for (long prime: primes)
	//{
		//cout << prime << ',';
	//}
	cout << primes.size() << " primes" << endl;

	for (long prime : reverse(primes)) 
	{
		if (n % prime == 0) 
		{
			return prime;
		}
	}

	return 1;
}

int main(int argc, char** argv)
{
	long n = stol(argv[1]);

	cout << largest_prime_factor(n) << endl;
}
