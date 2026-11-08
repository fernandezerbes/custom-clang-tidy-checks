#include <clang-tidy/ClangTidyCheck.h>

namespace clang::tidy::bugprone {

class NamedDestructuringCheck : public clang::tidy::ClangTidyCheck {
public:
  NamedDestructuringCheck(StringRef CheckName, ClangTidyContext *Context);

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
  // void storeOptions(ClangTidyOptions::OptionMap &Options) override;
  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override;

private:
  enum class Mode {
    Undefined,

    Exact,
    Fuzzy,
  };
};

} // namespace clang::tidy::bugprone