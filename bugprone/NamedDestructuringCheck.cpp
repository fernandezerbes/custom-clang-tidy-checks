#include "NamedDestructuringCheck.h"
#include <clang-tidy/ClangTidyDiagnosticConsumer.h>
#include <clang/AST/DeclBase.h>
#include <clang/AST/DeclCXX.h>
#include <clang/AST/Type.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/Support/Error.h>
#include <ranges>
#include <rapidfuzz/fuzz.hpp>
#include <string>

namespace clang::tidy::bugprone {

static constexpr double FUZZY_SCORE_CUTOFF = 90.0;

NamedDestructuringCheck::NamedDestructuringCheck(StringRef CheckName,
                                                 ClangTidyContext *Context)
    : ClangTidyCheck(CheckName, Context), Mode(getComparisonMode()) {}

void NamedDestructuringCheck::registerMatchers(
    ast_matchers::MatchFinder *Finder) {

  using namespace clang::ast_matchers;
  Finder->addMatcher(decompositionDecl().bind("decomposition-decl"), this);
}

void NamedDestructuringCheck::check(
    const ast_matchers::MatchFinder::MatchResult &Result) {
  const auto *DecompDecl =
      Result.Nodes.getNodeAs<DecompositionDecl>("decomposition-decl");
  if (!DecompDecl) {
    return;
  }

  const auto *RecordDecl = DecompDecl->getType()->getAsRecordDecl();
  if (!RecordDecl || !(RecordDecl->isClass() || RecordDecl->isStruct())) {
    return;
  }

  // FIXME: handle parameter packs once C++26 is released.
  // See https://en.cppreference.com/cpp/language/structured_binding.
  for (const auto &[Binding, Field] :
       std::ranges::zip_view(DecompDecl->bindings(), RecordDecl->fields())) {
    const auto BindingName = Binding->getNameAsString();
    const auto FieldName = Field->getNameAsString();

    if (Mode == ComparisonMode::Exact && BindingName != FieldName) {
      diag(Binding->getLocation(), "Variable '%0' binds to a member '%1'")
          << BindingName << FieldName
          << FixItHint::CreateReplacement(Binding->getSourceRange(),
                                          Field->getName());
    } else if (Mode == ComparisonMode::Fuzzy) {
      // TODO: Fine tune fuzzer. Add a cutoff option if necessary.
      const auto score = rapidfuzz::fuzz::partial_ratio(BindingName, FieldName);
      if (score < FUZZY_SCORE_CUTOFF) {
        diag(Binding->getLocation(),
             "Variable '%0' binds to a member '%1' with a fuzz ratio %2 < %3")
            << BindingName << FieldName << std::to_string(score)
            << std::to_string(FUZZY_SCORE_CUTOFF)
            << FixItHint::CreateReplacement(Binding->getSourceRange(),
                                            Field->getName());
      }
    }
  }
}

void NamedDestructuringCheck::storeOptions(ClangTidyOptions::OptionMap &Opts) {
  Options.store(Opts, "ComparisonMode",
                Mode == ComparisonMode::Exact ? "Exact" : "Fuzzy");
}

bool NamedDestructuringCheck::isLanguageVersionSupported(
    const LangOptions &LangOpts) const {
  return LangOpts.CPlusPlus;
}

NamedDestructuringCheck::ComparisonMode
NamedDestructuringCheck::getComparisonMode() const {
  const auto ModeStr = Options.get("ComparisonMode");
  if (!ModeStr || *ModeStr == "Exact") {
    return ComparisonMode::Exact;
  }
  if (*ModeStr == "Fuzzy") {
    return ComparisonMode::Fuzzy;
  }
  configurationDiag("Invalid ComparisonMode '%0'") << ModeStr->str();
  return ComparisonMode::Undefined;
}

} // namespace clang::tidy::bugprone