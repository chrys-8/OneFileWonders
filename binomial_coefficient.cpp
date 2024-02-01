typedef unsigned long long uint64

constexpr uint64 binomialCoefficient(uint64 n, uint64 k) {
  if (k > n) return 0;
  uint64 factorial_k = 1;
  for (uint64 i = 1; i <= k; ++i)
    factorial_k *= i;

  uint64 factorial_quotient = 1;
  for (uint64 i = n - k + 1; i <= n; ++i)
    factorial_quotient *= i;

  return factorial_quotient / factorial_k;
}
