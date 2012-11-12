env = Environment()

env.Append(LIBS = ['zmq', 'json'])
env.Append(CPPPATH = ['#'])
#env.MergeFlags(['-g', '-O0'])

dirs = ['#/control/source/', '#/model/source/', '#/server/source/', '#/core/source/']
cpp = []
for dir in dirs:
	cpp += env.Glob(dir + '*.cpp')
prog_main = ['main.cpp']
prog_objects = env.Object(source = cpp)
env.Program(target = 'a.out', source = prog_objects + prog_main)

testEnv = env.Clone()
testEnv.Append(LIBS = ['cppunit'])
test_dirs = ['#/test/source/']
test_cpp = []
for dir in test_dirs:
    test_cpp += env.Glob(dir + '*.cpp')
test_objects = env.Object(test_cpp)
testEnv.Program(target = 'runTests', source = test_objects + prog_objects)

