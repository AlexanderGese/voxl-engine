# voxl

A voxel game engine written in C with OpenGL — chunked terrain streaming, flood-fill lighting, fluid simulation, and a small entity system.

## controls

- WASD — walk
- space — jump
- ctrl — sprint
- F — toggle fly
- 1..9 — hotbar
- scroll — cycle hotbar
- LMB — break
- RMB — place
- F3 — debug overlay
- esc — release cursor

## build

needs: glfw3, glad (in third_party/), stb_image.h (in third_party/)

    make
    ./voxl

## layout

```
src/
├── main.c
├── config.h
├── math/    vec3 mat4 noise
├── util/    log file darray
├── input/   glfw callbacks
├── render/  gl texture camera mesh mesher renderer skybox
├── world/   block chunk world worldgen lighting save
├── player/  player physics raycast inventory
└── ui/      hud debug
shaders/
assets/
third_party/  glad, stb_image.h
```

## TODO

- greedy meshing (tried once, broke, rolled back)
- incremental lighting updates (currently relights whole chunk)
- text rendering for hud numbers
- biomes?
- sound

— alex
