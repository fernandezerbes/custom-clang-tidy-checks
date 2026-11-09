#include <clang-tidy/ClangTidyCheck.h>
#include <llvm/ADT/StringRef.h>

namespace clang::tidy::bugprone {

// TODO: Explain what this does.
class NamedDestructuringCheck : public clang::tidy::ClangTidyCheck {
public:
  NamedDestructuringCheck(StringRef CheckName, ClangTidyContext *Context);

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
  void storeOptions(ClangTidyOptions::OptionMap &Options) override;
  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override;

private:
  enum class ComparisonMode {
    Undefined,

    Exact,
    Fuzzy,
  };

  ComparisonMode getComparisonMode() const;

  const ComparisonMode Mode;
};

} // namespace clang::tidy::bugprone