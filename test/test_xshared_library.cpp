#include <test.hpp>
#include <xplugin/xshared_library.hpp>

#include <filesystem>
namespace fs = std::filesystem;

TEST_CASE("test_xshared_library")
{
    fs::path libdir("testlib");
    fs::path libpath = libdir / "testlib_01.xplugin";
    xp::xshared_library lib(libpath);

    using f42_type = int (*)();

    auto f42 = lib.find_symbol<f42_type>("f42");
    CHECK(f42);
    CHECK_EQ(f42(), 42);
}
