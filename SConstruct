env = Environment()
env.Append(LIBS = ['zmq', 'json'])
env.MergeFlags(['-g', '-O0'])
env.Program(target = 'a.out', source = env.Glob('*.cpp'))

