env = Environment()

env.Append(CPPPATH = ['#'])
env.MergeFlags(['-g', '-O0', '-Wall', '-Wextra'])
#env.MergeFlags(['-O2', '-DNDEBUG', '-Wall', '-Wextra'])

control_cpp = env.Glob('#/control/source/*.cpp')
model_cpp = env.Glob('#/model/source/*.cpp')
server_cpp = env.Glob('#/server/source/*.cpp')
test_cpp = env.Glob('#/test/source/*.cpp')
core_cpp = env.Glob('#/core/source/*.cpp')
trainer_cpp = env.Glob('#/trainer/source/*.cpp')
prog_main = ['main/main.cpp']
test_main = ['main/test_main.cpp']
trainer_main = ['main/trainer_main.cpp']

prog_env = env.Clone();
prog_env.Append(LIBS = ['zmq', 'json'])
prog_env.Program(target = 'runTetris', source = control_cpp+model_cpp+server_cpp+core_cpp+prog_main)

test_env = env.Clone()
test_env.Append(LIBS = ['zmq', 'json', 'cppunit'])
test_env.Program(target = 'runTests', source = control_cpp+model_cpp+server_cpp+test_cpp+core_cpp+trainer_cpp+test_main)

trainer_env = env.Clone()
trainer_env.Program(target = 'runTrainer', source = core_cpp+trainer_cpp+model_cpp+trainer_main)

