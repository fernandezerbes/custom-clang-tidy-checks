#include "NamedDestructuringCheck.h"
#include <clang-tidy/ClangTidyDiagnosticConsumer.h>
#include <clang/AST/DeclBase.h>
#include <clang/AST/DeclCXX.h>
#include <clang/AST/Type.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <format>
#include <llvm/ADT/StringRef.h>
#include <ranges>

namespace clang::tidy::bugprone {

NamedDestructuringCheck::NamedDestructuringCheck(StringRef CheckName,
                                                 ClangTidyContext *Context)
    : ClangTidyCheck(CheckName, Context) {
  // TODO: Handle options
}

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

  for (const auto &[Binding, Field] :
       std::ranges::zip_view(DecompDecl->bindings(), RecordDecl->fields())) {
    const auto BindingName = Binding->getNameAsString();
    const auto FieldName = Field->getNameAsString();
    if (BindingName != FieldName) {
      diag(Binding->getLocation(),
           std::format("Variable '{}' binds to a member of different name '{}'",
                       BindingName, FieldName));
    }
  }
}

// void StructureBindingEqualAdjacentTypes::storeOptions(
//     ClangTidyOptions::OptionMap &Options) {}

bool NamedDestructuringCheck::isLanguageVersionSupported(
    const LangOptions &LangOpts) const {
  // Structured bindings were introduced in C++17
  // https://en.cppreference.com/cpp/language/structured_binding
  // return LangOpts.LangStd < LangStandard::Kind::lang_c17;
  return true;
}

} // namespace clang::tidy::bugprone