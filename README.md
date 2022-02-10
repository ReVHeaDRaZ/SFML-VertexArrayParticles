# SFML-VertexArrayParticles
A VertexArray particle system using SFML with selectable types, emitters, steering behaviours, textures and blend modes.
Quad particles (have textures and can be different sizes) or Point particles (are single pixel points, but double the amount can be onscreen) can be selected from the main menu.

# Controls

|  |  |
| ------------- | ------------- |
| <b>LMB-</b>           | Emit Particles  |
| <b>MMB or P-</b>      | Change Emitter type  |
| <b>RMB-</b>           | Apply Steering behaviour  |
| <b>UP and DOWN-</b>   | Amount of Particles to spawn per frame  |
| <b>LEFT and RIGHT-</b> | Change Wind amount/direction |
| <b>O-</b>             | Spawn an Obstacle at mouse position |
| <b>B-</b>             | Change Steering behaviour |
| <b>T-</b>             | Change Texture (quads only) |
| <b>M-</b>             | Change Blending Mode (Add color/Add Alpha)  |
| <b>HOME and END-</b>  | Change Max Force that can be applied to particles |
| <b>PAGEUP and PAGEDOWN-</b> | Change the Max Speed of the particles |




# Install instructions
## Windows
Just download the '.zip' file from releases and unzip wherever you like, then run 'RaZParticles.exe'
The program will run at a default resolution of 1920x1080 Full-Screen, but other resolutions can be specified from the command prompt using the option '-r WIDTH HEIGHT'
For Example- 'RaZParticles.exe -r 1024 768'

## Other
For other platforms you will need to complile the source yourself.  It was written in VSCode setup with https://github.com/andrew-r-king/sfml-vscode-boilerplate for seperate debug and production release building.  GitBash, SFML and MinGW32 need to be installed in default root directories, unless you want to edit the paths in .vscode.
