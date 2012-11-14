env = Environment()

env.Append(CPPPATH = ['#'])
env.MergeFlags(['-g', '-O0', '-Wall', '-Wextra'])

prog_main = ['main/main.cpp']
prog_env = env.Clone();
prog_env.Append(LIBS = ['zmq', 'json'])
prog_dirs = ['#/control/source/', '#/model/source/', '#/server/source/', '#/core/source/']
prog_cpp = []
for dir in prog_dirs:
    prog_cpp += prog_env.Glob(dir + '*.cpp')
prog_objects = prog_env.Object(source = prog_cpp)
prog_env.Program(target = 'runTetris', source = prog_objects + prog_main)

test_main = ['main/test_main.cpp']
test_env = env.Clone()
test_env.Append(LIBS = ['zmq', 'json', 'cppunit'])
test_dirs = ['#/test/source/']
test_cpp = []
for dir in test_dirs:
    test_cpp += test_env.Glob(dir + '*.cpp')
test_objects = test_env.Object(test_cpp)
test_env.Program(target = 'runTests', source = prog_objects + test_objects + test_main)

trainer_main = ['main/trainer_main.cpp']
trainer_env = env.Clone()
trainer_dirs = ['#/trainer/source/']
trainer_cpp = []
for dir in trainer_dirs:
    trainer_cpp += trainer_env.Glob(dir + '*.cpp')
trainer_objects = trainer_env.Object(trainer_cpp)
test_env.Program(target = 'runTrainer', source = prog_objects + trainer_objects + trainer_main)

