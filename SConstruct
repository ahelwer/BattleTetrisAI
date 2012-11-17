env = Environment()
env.MergeFlags('-pthread')

env.Append(CPPPATH = ['#'])
#env.MergeFlags(['-g', '-O0', '-Wall', '-Wextra'])
env.MergeFlags(['-O2', '-DNDEBUG', '-Wall', '-Wextra'])

model_cpp = env.Glob('#/model/source/*.cpp')
model_obj = env.Object(model_cpp)

core_cpp = env.Glob('#/core/source/*.cpp')
core_obj = env.Object(core_cpp);

trainer_cpp = env.Glob('#/trainer/source/*.cpp')
trainer_obj = env.Object(trainer_cpp)

prog_main = ['main/main.cpp']
test_main = ['main/test_main.cpp']
trainer_main = ['main/trainer_main.cpp']

trainer_env = env.Clone()
trainer_env.MergeFlags(['-fopenmp'])
trainer_env.Append(CPPDEFINES = ['PARALLEL'])
trainer_env.Program(target = 'runTrainer', source = core_obj+trainer_obj+model_obj+trainer_main, LIBS = ['gomp'])

