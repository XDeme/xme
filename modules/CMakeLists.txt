add_library(xme_modules)

target_sources(xme_modules PUBLIC
    FILE_SET xme_modules TYPE CXX_MODULES FILES
    xme.cppm
    container/container.cppm
    core/functional/functional.cppm
    core/iterators/iterators.cppm
    math/math.cppm
    core/utility/utility.cppm)

# Modules currently only builds with libc++, libstdc++ is broken
target_link_options(xme_modules PRIVATE -lc++)
target_compile_options(xme_modules PRIVATE -stdlib=libc++)
target_link_libraries(xme_modules PUBLIC xme)
target_compile_features(xme_modules PRIVATE cxx_std_20)
set_target_properties(xme_modules PROPERTIES LINKER_LANGUAGE CXX)
