#include "NamedDestructuringCheck.h"
#include <clang-tidy/ClangTidyModule.h>
#include <clang-tidy/ClangTidyModuleRegistry.h>

namespace clang::tidy::bugprone {

class BugProneCustomModule : public clang::tidy::ClangTidyModule {
public:
  void addCheckFactories(ClangTidyCheckFactories &CheckFactories) override {
    CheckFactories.registerCheck<NamedDestructuringCheck>(
        "bugprone-named-destructuring-check");
  }
};

static ClangTidyModuleRegistry::Add<BugProneCustomModule>
    X("brugprone-custom-module",
      "Add custom checks for bugprone code constructs.");

// This anchor is used to force the linker to link in the generated object file
// and thus register the BugproneModule.
// NOLINTNEXTLINE(misc-use-internal-linkage)
volatile int BugproneCustomModuleAnchorSource = 0;

} // namespace clang::tidy::bugprone