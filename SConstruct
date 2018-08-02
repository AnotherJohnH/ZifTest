
source  = ['Source/scrape.cpp']
binary  = 'scrape'
app     = 'ZifTest'
version = '0.0.0'

# Get a build environment
env,libs = SConscript('Platform/build.scons', ['app', 'version'])

# Project specific build config
debug = ARGUMENTS.get('debug', 0)
if int(debug) == 0:
   env.Append(CCFLAGS = ['-O3', '-DNDEBUG'])
else:
   env.Append(CCFLAGS = ['-O0', '-g'])

# Builders
exe = env.Program(binary, source)
Depends(exe, libs)

env.Tar(app+'_'+env['target']+'_'+env['machine']+'_'+version+'.tgz',
        [exe, env['platform_files'], 'LICENSE'])

