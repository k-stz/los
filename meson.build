

project('los', 'cpp')

source_files = files([
  'main.cpp',
  'src/level.cpp',
  'src/player.cpp',
  'src/world.cpp',
  'src/entity.cpp'
])

sdl_dep = dependency('sdl2')
sdl_ttf_dep = dependency('SDL2_ttf')
sdl_image_dep = dependency('SDL2_image')

executable(
  'los',
  source_files,
  dependencies : [sdl_dep, sdl_ttf_dep, sdl_image_dep],
)
