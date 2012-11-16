env = Environment()

env.Append(CPPPATH = ['#'])
env.MergeFlags(['-g', '-O0', '-Wall', '-Wextra'])
#env.MergeFlags(['-O2', '-DNDEBUG', '-Wall', '-Wextra'])

control_cpp = env.Glob('#/control/source/*.cpp')
control_obj = env.Object(control_cpp, LIBS = ['zmq', 'json'])

server_cpp = env.Glob('#/server/source/*.cpp')
server_obj = env.Object(server_cpp, LIBS = ['zmq', 'json'])

test_cpp = env.Glob('#/test/source/*.cpp')
test_obj = env.Object(test_cpp, LIBS = ['zmq', 'json', 'cppunit'])

model_cpp = env.Glob('#/model/source/*.cpp')
model_obj = env.Object(model_cpp)

core_cpp = env.Glob('#/core/source/*.cpp')
core_obj = env.Object(core_cpp);

trainer_cpp = env.Glob('#/trainer/source/*.cpp')
trainer_obj = env.Object(trainer_cpp)

prog_main = ['main/main.cpp']
test_main = ['main/test_main.cpp']
trainer_main = ['main/trainer_main.cpp']

prog_env = env.Clone();
prog_env.Append(LIBS = ['zmq', 'json'])
prog_env.Program(target = 'runTetris', source = control_obj+model_obj+server_obj+core_obj+prog_main)

test_env = env.Clone()
test_env.Append(LIBS = ['zmq', 'json', 'cppunit'])
test_env.Program(target = 'runTests', source = control_obj+model_obj+server_obj+test_obj+core_obj+trainer_obj+test_main)

trainer_env = env.Clone()
trainer_env.MergeFlags(['-fopenmp'])
trainer_env.Program(target = 'runTrainer', source = core_obj+trainer_obj+model_obj+trainer_main, LIBS = ['gomp'])

