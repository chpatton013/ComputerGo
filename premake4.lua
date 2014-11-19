solution('cgo')
language('C++')

defines({})
includedirs({
   'external/boost/libs/**/include',
   'external/Catch/single_include',
   'include',
})
buildoptions({
   '--pipe',
   '--std=c++1y',
   '-Wno-unused-result'
})
libdirs({'lib'})

configurations('debug', 'profile', 'release')

configuration('debug')
defines({'DEBUG', 'NPROFILE', 'NRELEASE'})
flags({'ExtraWarnings', 'Optimize', 'Symbols'})

configuration('profile')
defines({'NDEBUG', 'PROFILE', 'NRELEASE'})
flags({'OptimizeSpeed', 'Symbols'})

configuration('release')
defines({'NDEBUG', 'NPROFILE', 'RELEASE'})
flags({'OptimizeSpeed'})

project('cgo_base')
kind('SharedLib')
targetdir('lib')
files({'src/base/**.cpp', 'include/cgo/base/**.hpp', 'include/cgo/base/**.inl'})

project('cgo_stdin')
kind('SharedLib')
targetdir('lib')
files({'src/stdin/**.cpp', 'include/cgo/stdin/**.hpp', 'include/cgo/stdin/**.inl'})
links({'cgo_base'})

project('cgo_random')
kind('SharedLib')
targetdir('lib')
files({'src/random/**.cpp', 'include/cgo/random/**.hpp', 'include/cgo/random/**.inl'})
links({'cgo_base'})

-- project('cgo_minimax')
-- kind('SharedLib')
-- targetdir('lib')
-- files({'src/minimax/**.cpp', 'include/cgo/minimax/**.hpp', 'include/cgo/minimax/**.inl'})
-- links({'cgo_base'})

project('cgo_montecarlo')
kind('SharedLib')
targetdir('lib')
files({'src/montecarlo/**.cpp', 'include/cgo/montecarlo/**.hpp', 'include/cgo/montecarlo/**.inl'})
links({'cgo_base'})

project('cgo_driver')
kind('ConsoleApp')
targetdir('bin')
files({'src/driver/**.cpp', 'include/cgo/driver/**.hpp', 'include/cgo/driver/**.inl'})
links({
   'cgo_base',
   'cgo_stdin',
   'cgo_random',
   -- 'cgo_minimax',
   'cgo_montecarlo'
})

project('cgo_test')
kind('ConsoleApp')
targetdir('bin')
files({'tests/**.cpp', 'tests/**.hpp', 'tests/**.inl'})
links({
   'cgo_base',
   -- 'cgo_stdin',
   -- 'cgo_random',
   -- 'cgo_minimax',
   -- 'cgo_montecarlo'
})
