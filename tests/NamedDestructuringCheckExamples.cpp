#include <cstdlib>
#include <string>

struct FooStruct {
  int abcdef;
  float ghijkl;
  std::string mnopqr;
};

struct FooClass {
public:
  int abcdef;
  float ghijkl;
  std::string mnopqr;
};

int main() {
  {
    // OK
    const auto [abcdef, ghijkl, mnopqr] = FooStruct{};
  }
  {
    // OK
    const auto [abcdef, ghijkl, mnopqr] = FooClass{};
  }
  {
    // Parameters swapped, NOT OK
    const auto [abcdef, mnopqr, ghijkl] = FooStruct{};
  }
  {
    // Parameters swapped, NOT OK
    const auto [abcdef, mnopqr, ghijkl] = FooClass{};
  }
  {
    // Entirely new name, NOT OK
    const auto [stuvwx, ghijkl, mnopqr] = FooStruct{};
  }
  {
    // Entirely new name, NOT OK
    const auto [stuvwx, ghijkl, mnopqr] = FooClass{};
  }
  {
    // Partial match, NOT OK in Exact ComparisonMode, OK in Fuzzy ComparisonMode
    const auto [abcdef, ghijkl, moq] = FooStruct{};
  }
  {
    // Partial match, NOT OK in Exact ComparisonMode, OK in Fuzzy ComparisonMode
    const auto [abcdef, ghijkl, moq] = FooClass{};
  }
  {
    // Partial match below threshold, NOT OK in Exact ComparisonMode, NOT OK in
    // Fuzzy ComparisonMode
    const auto [azzzzz, ghijkl, mnopqr] = FooStruct{};
  }
  {
    // Partial match below threshold, NOT OK in Exact ComparisonMode, NOT OK in
    // Fuzzy ComparisonMode
    const auto [azzzzz, ghijkl, mnopqr] = FooClass{};
  }
  {
    // Real case of variables swap, taken from
    // https://github.com/isocpp/CppCoreGuidelines/issues/1655
    // Return type is:
    //   typedef struct
    // {
    //   int quot;			/* Quotient.  */
    //   int rem;			/* Remainder.  */
    // } div_t;
    //
    // NOT OK in Exact ComparisonMode, NOT OK in Fuzzy ComparisonMode
    auto [remainder, quotient] = std::div(123456, 73);
  }
  return 0;
}