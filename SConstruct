envUnitTests  =  Environment()

libsPath  =  "/usr/local/lib/boost_cpp/boost_work/build-dir/boost/bin.v2/libs/test/build/gcc-7/release/link-static/threading-multi/visibility-hidden/"
libs  =  "boost_unit_test_framework"

envUnitTests.Append( CPPPATH = ['/usr/local/lib/boost_cpp/boost_1_74_0/', 'include/'] )
envUnitTests.Append( SCONS_CXX_STANDARD="c++11" )
envUnitTests.Append( CPPFLAGS = [ '-g', '-std=c++11', '-Wall', '-Wextra', '-Werror', '--pedantic-errors', '-fprofile-arcs', '-ftest-coverage' ] )
envUnitTests.Append( LINKFLAGS = [ '-fprofile-arcs' ] )

envUnitTests.Program( target = 'unit_tests/boostTest', source = [ Glob( 'unit_tests/src/*.cpp' ) ], LIBS = libs, LIBPATH = libsPath )


envBenchmark  =  Environment()

envBenchmark.Append( CPPPATH = ['include/'] )
envBenchmark.Append( SCONS_CXX_STANDARD="c++11" )

envBenchmark.Append( CPPFLAGS = [ '-std=c++11', '-Wall', '-Wextra', '-Werror', '--pedantic-errors' ] )

envBenchmark.Program( target = 'benchmark/benchmark', source = [ Glob( 'benchmark/src/*.cpp' ) ] )


envExamples  =  Environment()
envExamples.Append( CPPPATH = [ 'include/' ] )
envExamples.Append( SCONS_CXX_STANDARD= "c++11" )
envExamples.Append( CPPFLAGS = [ '-std=c++11', '-Wall', '-Wextra', '-Werror', '--pedantic-errors' ] )

envExamples.Program( target = 'examples/example', source = 'examples/src/example1.cpp' )

