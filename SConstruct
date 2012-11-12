env = Environment()
env.Append(LIBS = ['zmq', 'json'])
env.Append(CPPPATH = ['#'])
dirs = ['#/control/source/', '#/model/source/', '#/server/source/', '#/core/source/']
cpp = []
for dir in dirs:
	cpp += env.Glob(dir + '*.cpp')
#env.MergeFlags(['-g', '-O0'])
env.Program(target = 'a.out', source = cpp)

